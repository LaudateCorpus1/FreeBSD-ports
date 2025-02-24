PORTNAME=	ecflow
DISTVERSION=	5.9.0
CATEGORIES=	misc
MASTER_SITES=	https://confluence.ecmwf.int/download/attachments/8650755/
DISTNAME=	ecFlow-${DISTVERSION}-Source

MAINTAINER=	yuri@FreeBSD.org
COMMENT=	Workflow package that enables users to run a large number of programs
WWW=		https://confluence.ecmwf.int/display/ECFLOW/ecflow+home

LICENSE=	APACHE20
LICENSE_FILE=	${WRKSRC}/LICENSE

PY_DEPENDS=	${PYTHON_PKGNAMEPREFIX}boost-libs>0:devel/boost-python-libs@${PY_FLAVOR}
BUILD_DEPENDS=	bash:shells/bash \
		${PY_DEPENDS}
LIB_DEPENDS=	libboost_program_options.so:devel/boost-libs
RUN_DEPENDS=	${PY_DEPENDS}

USES=		cmake compiler:c++17-lang localbase perl5 pkgconfig python:3.6+ qt:5 shebangfix ssl
USE_QT=		charts core gui network svg widgets buildtools:build qmake:build

SHEBANG_GLOB=	*.sh *.in *.py

CMAKE_OFF=	ENABLE_STATIC_BOOST_LIBS ENABLE_TESTS ENABLE_ALL_TESTS

CXXFLAGS_i386=	-fPIC # attempt to work around compilation failure, see https://jira.ecmwf.int/browse/SUP-3688

CONFLICTS_BUILD=	python27 # build attempts to find python-2.7 and use it

.include <bsd.port.mk>
