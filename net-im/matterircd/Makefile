PORTNAME=	matterircd
DISTVERSIONPREFIX=	v
DISTVERSION=	0.25.1
PORTREVISION=	5
CATEGORIES=	net-im irc

MAINTAINER=	norrland@nullbyte.se
COMMENT=	IRC bridge for Mattermost and Slack
WWW=		https://github.com/42wim/matterircd

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/LICENSE

ONLY_FOR_ARCHS=	aarch64 amd64 armv6 armv7 i386

USES=		go:modules
USE_GITHUB=	yes
GH_ACCOUNT=	42wim
USE_RC_SUBR=	matterircd

USERS=		matterircd
GROUPS=		matterircd

post-install:
	${INSTALL_DATA} ${WRKSRC}/matterircd.toml.example \
		${STAGEDIR}${PREFIX}/etc/matterircd.toml.sample

.include <bsd.port.mk>
