PORTNAME=	octave-forge-oct2mat
PORTVERSION=	1.0.7
PORTREVISION=	27
CATEGORIES=	math
MASTER_SITES=	SF/octave/Octave%20Forge%20Packages/Old%20Monolithic%20Releases/R2009-06-07
DISTNAME=	${OCTAVE_PKGNAME}-${DISTVERSIONFULL}

MAINTAINER=	stephen@FreeBSD.org
COMMENT=	Octave-forge package ${OCTAVE_PKGNAME}
WWW=		http://octave.sourceforge.net/

LICENSE=	BSD3CLAUSE
LICENSE_FILE=	${WRKSRC}/COPYING

GNU_CONFIGURE=	yes

RUN_DEPENDS+=	${TARBALLS_DIR}/io.tar.gz:math/octave-forge-io

.include "${.CURDIR}/../../Mk/bsd.octave.mk"
.include <bsd.port.mk>
