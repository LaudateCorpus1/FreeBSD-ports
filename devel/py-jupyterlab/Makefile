PORTNAME=	jupyterlab
PORTVERSION=	3.4.8
CATEGORIES=	devel python
MASTER_SITES=	CHEESESHOP
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	sunpoet@FreeBSD.org
COMMENT=	JupyterLab server extension
WWW=		https://jupyter.org/

LICENSE=	BSD3CLAUSE
LICENSE_FILE=	${WRKSRC}/LICENSE

BUILD_DEPENDS=	${PYTHON_PKGNAMEPREFIX}jupyter-packaging>=0:devel/py-jupyter-packaging@${PY_FLAVOR}
RUN_DEPENDS=	${PYTHON_PKGNAMEPREFIX}ipython>=0:devel/ipython@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}Jinja2>=2.1:devel/py-Jinja2@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}jupyter-core>=0:devel/py-jupyter-core@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}jupyter-server>=1.16<2:devel/py-jupyter-server@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}jupyterlab-server>=2.10<3:devel/py-jupyterlab-server@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}nbclassic>=0:devel/py-nbclassic@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}notebook>=0<7:www/py-notebook@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}packaging>=0:devel/py-packaging@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}tomli>=0:textproc/py-tomli@${PY_FLAVOR} \
		${PYTHON_PKGNAMEPREFIX}tornado>=6.1.0:www/py-tornado@${PY_FLAVOR}

USES=		python:3.7+
USE_PYTHON=	autoplist concurrent distutils

NO_ARCH=	yes

.include <bsd.port.mk>
