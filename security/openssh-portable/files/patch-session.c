--- session.c.orig	Wed Jun 26 15:51:06 2002
+++ session.c	Mon Jul  1 21:33:04 2002
@@ -64,6 +64,13 @@
 #define is_winnt       (GetVersion() < 0x80000000)
 #endif
 
+#ifdef __FreeBSD__
+#include <libutil.h>
+#include <syslog.h>
+#include <time.h>
+#define _PATH_CHPASS "/usr/bin/passwd"
+#endif /* __FreeBSD__ */
+
 /* func */
 
 Session *session_new(void);
@@ -474,6 +481,13 @@
 		log_init(__progname, options.log_level, options.log_facility, log_stderr);
 
 		/*
+		 * Using login and executing a specific "command" are mutually
+		 * exclusive, so turn off use_login if there's a command.
+		 */
+		if (command != NULL)
+			options.use_login = 0;
+
+		/*
 		 * Create a new session and process group since the 4.4BSD
 		 * setlogin() affects the entire process group.
 		 */
@@ -572,6 +586,9 @@
 {
 	int fdout, ptyfd, ttyfd, ptymaster;
 	pid_t pid;
+#if defined(USE_PAM)
+	const char *shorttty;
+#endif
 
 	if (s == NULL)
 		fatal("do_exec_pty: no session");
@@ -579,7 +596,16 @@
 	ttyfd = s->ttyfd;
 
 #if defined(USE_PAM)
-	do_pam_session(s->pw->pw_name, s->tty);
+	/* check if we have a pathname in the ttyname */
+	shorttty = rindex( s->tty, '/' );
+	if (shorttty != NULL ) {
+		/* use only the short filename to check */
+		shorttty ++;
+	} else {
+		/* nothing found, use the whole name found */
+		shorttty = s->tty;
+	}
+	do_pam_session(s->pw->pw_name, shorttty);
 	do_pam_setcred(1);
 #endif
 
@@ -588,6 +614,14 @@
 
 		/* Child.  Reinitialize the log because the pid has changed. */
 		log_init(__progname, options.log_level, options.log_facility, log_stderr);
+
+		/*
+		 * Using login and executing a specific "command" are mutually
+		 * exclusive, so turn off use_login if there's a command.
+		 */
+		if (command != NULL)
+			options.use_login = 0;
+
 		/* Close the master side of the pseudo tty. */
 		close(ptyfd);
 
@@ -714,6 +748,18 @@
 	struct sockaddr_storage from;
 	struct passwd * pw = s->pw;
 	pid_t pid = getpid();
+#ifdef HAVE_LOGIN_CAP
+	FILE *f;
+	char buf[256];
+	char *fname;
+	const char *shorttty;
+#endif /* HAVE_LOGIN_CAP */
+#ifdef __FreeBSD__
+#define DEFAULT_WARN  (2L * 7L * 86400L)  /* Two weeks */
+	char *newcommand;
+	struct timeval tv;
+	time_t warntime = DEFAULT_WARN;
+#endif /* __FreeBSD__ */
 
 	/*
 	 * Get IP address of client. If the connection is not a socket, let
@@ -747,6 +793,72 @@
 	}
 #endif
 
+#ifdef __FreeBSD__
+	if (pw->pw_change || pw->pw_expire)
+		(void)gettimeofday(&tv, NULL);
+#ifdef HAVE_LOGIN_CAP
+	warntime = login_getcaptime(lc, "warnpassword",
+				    DEFAULT_WARN, DEFAULT_WARN);
+#endif /* HAVE_LOGIN_CAP */
+	/*
+	 * If the password change time is set and has passed, give the
+	 * user a password expiry notice and chance to change it.
+	 */
+	if (pw->pw_change != 0) {
+		if (tv.tv_sec >= pw->pw_change) {
+			(void)printf(
+			    "Sorry -- your password has expired.\n");
+			log("%s Password expired - forcing change",
+			    pw->pw_name);
+			if (newcommand != NULL)
+				xfree(newcommand);
+			newcommand = xstrdup(_PATH_CHPASS);
+		} else if (pw->pw_change - tv.tv_sec < warntime &&
+			   !check_quietlogin(s, command))
+			(void)printf(
+			    "Warning: your password expires on %s",
+			     ctime(&pw->pw_change));
+	}
+
+#ifndef USE_PAM
+	if (pw->pw_expire) {
+		if (tv.tv_sec >= pw->pw_expire) {
+			(void)printf(
+			    "Sorry -- your account has expired.\n");
+			log(
+	   "LOGIN %.200s REFUSED (EXPIRED) FROM %.200s ON TTY %.200s",
+				pw->pw_name, get_remote_name_or_ip(utmp_len,
+				options.verify_reverse_mapping), s->tty);
+			exit(254);
+		} else if (pw->pw_expire - tv.tv_sec < warntime &&
+			   !check_quietlogin(s, command))
+			(void)printf(
+			    "Warning: your account expires on %s",
+			     ctime(&pw->pw_expire));
+	}
+#endif /* !USE_PAM */
+#endif /* __FreeBSD__ */
+
+#ifdef HAVE_LOGIN_CAP
+	/* check if we have a pathname in the ttyname */
+	shorttty = rindex( s->tty, '/' );
+	if (shorttty != NULL ) {
+		/* use only the short filename to check */
+		shorttty ++;
+	} else {
+		/* nothing found, use the whole name found */
+		shorttty = s->tty;
+	}
+	if (!auth_ttyok(lc, shorttty)) {
+		(void)printf("Permission denied.\n");
+		log(
+	       "LOGIN %.200s REFUSED (TTY) FROM %.200s ON TTY %.200s",
+		    pw->pw_name, get_remote_name_or_ip(utmp_len,
+			options.verify_reverse_mapping), s->tty);
+		exit(254);
+	}
+#endif /* HAVE_LOGIN_CAP */
+
 	if (check_quietlogin(s, command))
 		return;
 
@@ -759,7 +871,17 @@
 		printf("%s\n", aixloginmsg);
 #endif /* WITH_AIXAUTHENTICATE */
 
-	if (options.print_lastlog && s->last_login_time != 0) {
+	/*
+	 * If the user has logged in before, display the time of last
+	 * login. However, don't display anything extra if a command
+	 * has been specified (so that ssh can be used to execute
+	 * commands on a remote machine without users knowing they
+	 * are going to another machine). Login(1) will do this for
+	 * us as well, so check if login(1) is used
+	 */
+	if (command == NULL && options.print_lastlog &&
+	    s->last_login_time != 0 &&
+	    !options.use_login) {
 		time_string = ctime(&s->last_login_time);
 		if (strchr(time_string, '\n'))
 			*strchr(time_string, '\n') = 0;
@@ -770,7 +892,30 @@
 			    s->hostname);
 	}
 
-	do_motd();
+#ifdef HAVE_LOGIN_CAP
+	if (command == NULL &&
+	    !options.use_login) {
+		fname = login_getcapstr(lc, "copyright", NULL, NULL);
+		if (fname != NULL && (f = fopen(fname, "r")) != NULL) {
+			while (fgets(buf, sizeof(buf), f) != NULL)
+				fputs(buf, stdout);
+			fclose(f);
+		} else
+			(void)printf("%s\n\t%s %s\n",
+	"Copyright (c) 1980, 1983, 1986, 1988, 1990, 1991, 1993, 1994",
+	    "The Regents of the University of California. ",
+	    "All rights reserved.");
+	}
+#endif /* HAVE_LOGIN_CAP */
+
+	/*
+	 * Print /etc/motd unless a command was specified or printing
+	 * it was disabled in server options or login(1) will be
+	 * used.  Note that some machines appear to print it in
+	 * /etc/profile or similar.
+	 */
+	if (command == NULL && !options.use_login)
+		do_motd();
 }
 
 /*
@@ -786,9 +931,9 @@
 #ifdef HAVE_LOGIN_CAP
 		f = fopen(login_getcapstr(lc, "welcome", "/etc/motd",
 		    "/etc/motd"), "r");
-#else
+#else /* !HAVE_LOGIN_CAP */
 		f = fopen("/etc/motd", "r");
-#endif
+#endif /* HAVE_LOGIN_CAP */
 		if (f) {
 			while (fgets(buf, sizeof(buf), f))
 				fputs(buf, stdout);
@@ -815,10 +960,10 @@
 #ifdef HAVE_LOGIN_CAP
 	if (login_getcapbool(lc, "hushlogin", 0) || stat(buf, &st) >= 0)
 		return 1;
-#else
+#else /* HAVE_LOGIN_CAP */
 	if (stat(buf, &st) >= 0)
 		return 1;
-#endif
+#endif /* HAVE_LOGIN_CAP */
 	return 0;
 }
 
@@ -938,6 +1083,10 @@
 	char buf[256];
 	u_int i, envsize;
 	char **env;
+#ifdef HAVE_LOGIN_CAP
+	extern char **environ;
+	char **senv, **var;
+#endif /* HAVE_LOGIN_CAP */
 	struct passwd *pw = s->pw;
 
 	/* Initialize the environment. */
@@ -945,6 +1094,9 @@
 	env = xmalloc(envsize * sizeof(char *));
 	env[0] = NULL;
 
+	/* Moved up to resove confict with gsssapi patches */
+	if (getenv("TZ"))
+		child_set_env(&env, &envsize, "TZ", getenv("TZ"));
 #ifdef HAVE_CYGWIN
 	/*
 	 * The Windows environment contains some setting which are
@@ -958,10 +1110,6 @@
 		child_set_env(&env, &envsize, "USER", pw->pw_name);
 		child_set_env(&env, &envsize, "LOGNAME", pw->pw_name);
 		child_set_env(&env, &envsize, "HOME", pw->pw_dir);
-#ifdef HAVE_LOGIN_CAP
-		(void) setusercontext(lc, pw, pw->pw_uid, LOGIN_SETPATH);
-		child_set_env(&env, &envsize, "PATH", getenv("PATH"));
-#else /* HAVE_LOGIN_CAP */
 # ifndef HAVE_CYGWIN
 		/*
 		 * There's no standard path on Windows. The path contains
@@ -976,7 +1124,6 @@
 		child_set_env(&env, &envsize, "PATH", _PATH_STDPATH);
 #  endif /* SUPERUSER_PATH */
 # endif /* HAVE_CYGWIN */
-#endif /* HAVE_LOGIN_CAP */
 
 		snprintf(buf, sizeof buf, "%.200s/%.50s",
 			 _PATH_MAILDIR, pw->pw_name);
@@ -984,9 +1131,21 @@
 
 		/* Normal systems set SHELL by default. */
 		child_set_env(&env, &envsize, "SHELL", shell);
+#ifdef HAVE_LOGIN_CAP
+		senv = environ;
+		environ = xmalloc(sizeof(char *));
+		*environ = NULL;
+		if (setusercontext(lc, pw, pw->pw_uid,
+			LOGIN_SETENV|LOGIN_SETPATH) < 0) {
+			perror("unable to set user context enviroment");
+		}
+		copy_environment(environ, &env, &envsize);
+		for (var = environ; *var != NULL; ++var)
+			xfree(*var);
+		xfree(environ);
+		environ = senv;
+#endif /* HAVE_LOGIN_CAP */
 	}
-	if (getenv("TZ"))
-		child_set_env(&env, &envsize, "TZ", getenv("TZ"));
 
 	/* Set custom environment options from RSA authentication. */
 	if (!options.use_login) {
@@ -1174,7 +1333,7 @@
 		setpgid(0, 0);
 #endif
 		if (setusercontext(lc, pw, pw->pw_uid,
-		    (LOGIN_SETALL & ~LOGIN_SETPATH)) < 0) {
+		    (LOGIN_SETALL & ~(LOGIN_SETENV|LOGIN_SETPATH))) < 0) {
 			perror("unable to set user context");
 			exit(1);
 		}
@@ -1325,7 +1484,7 @@
 	 * initgroups, because at least on Solaris 2.3 it leaves file
 	 * descriptors open.
 	 */
-	for (i = 3; i < 64; i++)
+	for (i = 3; i < getdtablesize(); i++)
 		close(i);
 
 	/*
@@ -1355,6 +1514,31 @@
 			exit(1);
 #endif
 	}
+
+#ifdef __FreeBSD__
+  	if (!options.use_login) {
+		/*
+		 * If the password change time is set and has passed, give the
+		 * user a password expiry notice and chance to change it.
+		 */
+		if (pw->pw_change != 0) {
+			struct timeval tv;
+
+			(void)gettimeofday(&tv, NULL);
+			if (tv.tv_sec >= pw->pw_change) {
+				(void)printf(
+				    "Sorry -- your password has expired.\n");
+				syslog(LOG_INFO,
+				    "%s Password expired - forcing change",
+				    pw->pw_name);
+				if (system("/usr/bin/passwd") != 0) {
+					perror("/usr/bin/passwd");
+					exit(1);
+				}
+			}
+		}
+	}
+#endif /* __FreeBSD__ */
 
 	if (!options.use_login)
 		do_rc_files(s, shell);
