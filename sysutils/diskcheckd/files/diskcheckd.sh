#!/bin/sh

if ! PREFIX=$(expr $0 : "\(/.*\)/etc/rc\.d/$(basename $0)\$"); then
    echo "$0: Cannot determine the PREFIX" >&2
    exit 1
fi

case "$1" in
	start)
		[ -x ${PREFIX}/sbin/diskcheckd ] && ${PREFIX}/sbin/diskcheckd
		;;
	stop)
		/usr/bin/killall diskcheckd
		;;
	*)
		echo ""
		echo "Usage: `basename $0` { start | stop }"
		echo ""
		;;
esac
