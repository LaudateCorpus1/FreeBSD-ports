--- chrome/browser/extensions/api/settings_private/prefs_util.cc.orig	2022-10-01 07:40:07 UTC
+++ chrome/browser/extensions/api/settings_private/prefs_util.cc
@@ -186,7 +186,7 @@ const PrefsUtil::TypedPrefMap& PrefsUtil::GetAllowlist
 
 // TODO(crbug.com/1052397): Revisit the macro expression once build flag switch
 // of lacros-chrome is complete.
-#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_CHROMEOS_LACROS)
+#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_CHROMEOS_LACROS) || BUILDFLAG(IS_BSD)
   (*s_allowlist)[::prefs::kUseCustomChromeFrame] =
       settings_api::PrefType::PREF_TYPE_BOOLEAN;
 #endif
@@ -200,7 +200,7 @@ const PrefsUtil::TypedPrefMap& PrefsUtil::GetAllowlist
       settings_api::PrefType::PREF_TYPE_NUMBER;
 // TODO(crbug.com/1052397): Revisit the macro expression once build flag switch
 // of lacros-chrome is complete.
-#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_CHROMEOS_LACROS)
+#if BUILDFLAG(IS_LINUX) || BUILDFLAG(IS_CHROMEOS_LACROS) || BUILDFLAG(IS_BSD)
   (*s_allowlist)[::prefs::kUsesSystemTheme] =
       settings_api::PrefType::PREF_TYPE_BOOLEAN;
 #endif
