#!/bin/sh
#$Id$
#Copyright (c) 2015-2019 Pierre Pronchery <khorben@defora.org>
#
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
CONFIGSH="${0%/platform.sh}/../config.sh"
DESTDIR=
LDSOCONF="/etc/ld.so.conf"
PREFIX="/usr/local"
PROGNAME="platform.sh"
SOEXT=".so"
#executables
UNAME="uname"
[ -f "$CONFIGSH" ] && . "$CONFIGSH"


#functions
#_libray_include
_library_include(){
	while read line; do
		#remove trailing comments
		line="${line%#*}"
		
		case "$line" in
			"include "*)
				files="${line#include }"
				for f in $files; do
					[ -f "$f" ] && _library_include "$f"
				done;
				;;
			*)
				#XXX breaks on whitespace
				[ -n "${line}" ] && path="$path:$line"
				;;
		esac
	done < $1

}

#platform_library
_platform_library()
{
	library="$1"
	libdir=$(_platform_variable "LIBDIR")
	path="/lib:/usr/lib:$libdir"

        if [ -f "$DESTDIR/etc/ld.so.conf" ]; then
		_library_include "$DESTDIR/etc/ld.so.conf"
	fi
	(IFS=:; for p in $path; do
		if [ -f "$DESTDIR$p/lib$library$SOEXT" ]; then
			echo "-l$library"
			return
		fi
	done)
}

#platform_variable
_platform_variable()
{
	variable="$1"

	case "$variable" in
		ARCH)
			if [ -n "$ARCH" ]; then
				echo "$ARCH"
				return 0
			fi
			ARCH=$($UNAME -m)
			case "$ARCH" in
				amd64|x86_64)
					echo "amd64"
					;;
				i[3456]86)
					echo "i386"
					;;
				*)
					echo "unknown"
					;;
			esac
			;;
		BINDIR)
			echo "$PREFIX/bin"
			;;
		DATADIR)
			echo "$PREFIX/share"
			;;
		LIBDIR)
			echo "$PREFIX/lib"
			;;
		MANDIR)
			if [ "$PREFIX" = "/usr/pkg" ]; then
				echo "$PREFIX/man"
			else
				echo "$PREFIX/share/man"
			fi
			;;
		PREFIX)
			echo "$PREFIX"
			;;
		SBINDIR)
			echo "$PREFIX/sbin"
			;;
		SYSCONFDIR)
			if [ "$PREFIX" = "/usr" ]; then
				echo "/etc"
			else
				echo "$PREFIX/etc"
			fi
			;;
	esac
}


#usage
_usage()
{
	echo "Usage: $PROGNAME -l library" 1>&2
	echo "       $PROGNAME -V variable" 1>&2
	return 1
}


#main
type=
variable=
while getopts "l:O:V:" name; do
	case "$name" in
		O)
			export "${OPTARG%%=*}"="${OPTARG#*=}"
			;;
		V)
			type="variable"
			variable="$OPTARG"
			;;
		l)
			type="library"
			library="$OPTARG"
			;;
		?)
			_usage
			exit $?
			;;
	esac
done
shift $((OPTIND - 1))
if [ $# -ne 0 ]; then
	_usage
	exit $?
fi

case "$type" in
	library)
		"_platform_$type" "$library"
		;;
	variable)
		"_platform_$type" "$variable"
		;;
	*)
		_usage
		exit $?
		;;
esac
