#!/bin/sh
#$Id$
#Copyright (c) 2016-2025 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS Desktop libDesktop
#Redistribution and use in source and binary forms, with or without
#modification, are permitted provided that the following conditions are met:
#
# * Redistributions of source code must retain the above copyright notice, this
#   list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above copyright notice,
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.
#
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
#FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
#DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
#SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
#OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
#OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



#variables
CONFIGSH="${0%/pkgconfig.sh}/../config.sh"
PKG_CONFIG_PATH="$OBJDIR../data:$PKG_CONFIG_PATH"
PKG_CONFIG_PATH="${PKG_CONFIG_PATH%:}"
PROGNAME="pkgconfig.sh"
#executables
ECHO="echo"
PKGCONFIG="pkg-config"
UNAME="uname"
[ "$($UNAME -s)" != "Darwin" ] || ECHO="/bin/echo"

[ -f "$CONFIGSH" ] && . "$CONFIGSH"


#functions
#pkgconfig
_pkgconfig()
{(
	ret=0

	_pkgconfig_do "EXISTS:" --exists "$PACKAGE"		|| return 2

	_pkgconfig_do "VERSION:" --modversion "$PACKAGE"	|| ret=3
	_pkgconfig_do "CFLAGS:	" --cflags "$PACKAGE"		|| ret=4
	_pkgconfig_do "LIBS:	" --libs "$PACKAGE"		|| ret=5
	_pkgconfig_do "PROVIDES:" --print-provides "$PACKAGE"	|| ret=6
	_pkgconfig_do "REQUIRES:" --print-requires "$PACKAGE"	|| ret=7
	return $ret
)}

_pkgconfig_do()
{
	caption="$1"
	options="$2"
	packages="$3"

	$ECHO -n "$caption"
	PKG_CONFIG_PATH="$PKG_CONFIG_PATH" $PKGCONFIG $options "$packages"
}


#usage
_usage()
{
	echo "Usage: $PROGNAME [-c] target..." 1>&2
	return 1
}


#main
clean=0
while getopts "cO:P:" name; do
	case "$name" in
		c)
			clean=1
			;;
		O)
			export "${OPTARG%%=*}"="${OPTARG#*=}"
			;;
		P)
			#XXX ignored for compatibility
			;;
		?)
			_usage
			exit $?
			;;
	esac
done
shift $((OPTIND - 1))
if [ $# -lt 1 ]; then
	_usage
	exit $?
fi

#clean
[ $clean -ne 0 ] && exit 0

exec 3>&1
ret=0
while [ $# -gt 0 ]; do
	target="$1"
	dirname="${target%/*}"
	shift

	if [ -n "$dirname" -a "$dirname" != "$target" ]; then
		$MKDIR -- "$dirname"				|| ret=$?
	fi
	_pkgconfig > "$target"					|| ret=$?
done
exit $ret
