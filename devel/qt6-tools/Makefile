PORTNAME=	tools
DISTVERSION=	${QT6_VERSION}
PORTREVISION=	1
CATEGORIES=	devel
PKGNAMEPREFIX=	qt6-

PATCH_SITES=	https://github.com/qt/qttools/commit/
PATCHFILES=	65cb9a7c5444669d498aad2e2707b2a47b56f002.diff:-p1


MAINTAINER=	kde@FreeBSD.org
COMMENT=	Qt declarative framework for dynamic user interfaces
WWW=		https://qt-project.org

BUILD_DEPENDS=	${LOCALBASE}/include/vulkan/vulkan.h:graphics/vulkan-headers \
		llvm${LLVM_DEFAULT}>0:devel/llvm${LLVM_DEFAULT}
LIB_DEPENDS=	libvulkan.so:graphics/vulkan-loader \
		libLLVM.so:devel/llvm${LLVM_DEFAULT} \
		libdouble-conversion.so:devel/double-conversion \
		libexpat.so:textproc/expat2 \
		libfontconfig.so:x11-fonts/fontconfig \
		libfreetype.so:print/freetype2 \
		libgraphite2.so:graphics/graphite2 \
		libharfbuzz.so:print/harfbuzz \
		libicudata.so:devel/icu \
		libpcre.so:devel/pcre \
		libpcre2-16.so:devel/pcre2 \
		libpng16.so:graphics/png \
		libxkbcommon.so:x11/libxkbcommon \
		libzstd.so:archivers/zstd

USES=		cmake compiler:c++17-lang gettext-runtime gl gnome python:build xorg \
		qt-dist:6
USE_GL=		gl
USE_GNOME=	glib20
USE_QT=		base declarative
USE_XORG=	x11 xau xcb xdmcp
USE_LDCONFIG=	${PREFIX}/${QT_LIBDIR_REL}

CMAKE_ARGS+=	-DClang_DIR:PATH="${LOCALBASE}/llvm${LLVM_DEFAULT}/lib/cmake/clang" \
		-DLLVM_DIR:PATH="${LOCALBASE}/llvm${LLVM_DEFAULT}/lib/cmake/llvm"

post-install:
	# Install symlinks for user-facing tools
	${REINPLACE_CMD} -e "s#^#${STAGEDIR}#" -e "s# # ${STAGEDIR}#" \
		${WRKDIR}/.build/user_facing_tool_links.txt
	while read t; do \
		${RLN} $$t; \
	done <${WRKDIR}/.build/user_facing_tool_links.txt

.include <bsd.port.mk>
