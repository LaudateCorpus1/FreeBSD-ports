PORTNAME=	fheroes2
PORTVERSION=	0.9.20
CATEGORIES=	games

MAINTAINER=	pkubaj@FreeBSD.org
COMMENT=	Free implementation of Heroes of Might and Magic II engine
WWW=		https://ihhub.github.io/fheroes2/

LICENSE=	GPLv2
LICENSE_FILE=	${WRKSRC}/LICENSE

LIB_DEPENDS=	libpng.so:graphics/png
RUN_DEPENDS=	timidity:audio/timidity

USES=		cmake gettext-tools iconv localbase:ldflags pkgconfig sdl \
		shebangfix
USE_GITHUB=	yes
GH_ACCOUNT=	ihhub
USE_SDL=	image2 mixer2 sdl2 ttf2
SHEBANG_FILES=	script/demo/download_demo_version.sh

OPTIONS_DEFINE=	DOCS

.include <bsd.port.mk>
