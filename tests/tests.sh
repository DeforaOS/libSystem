#!/bin/sh
#$Id$
#Copyright (c) 2011-2018 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS System libSystem
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
[ -n "$OBJDIR" ] || OBJDIR="./"
PROGNAME="tests.sh"
#executables
DATE="date"
PKGCONFIG="pkg-config"


#functions
#date
_date()
{
	if [ -n "$SOURCE_DATE_EPOCH" ]; then
		TZ=UTC $DATE -d "@$SOURCE_DATE_EPOCH" '+%a %b %d %T %Z %Y'
	else
		$DATE
	fi
}


#fail
_fail()
{
	_run "$@" >> "$target"
}


#run
_run()
{
	test="$1"
	sep=
	[ $# -eq 1 ] || sep=" "

	shift
	echo -n "$test:" 1>&2
	(echo
	echo "Testing: $test" "$@"
	testexe="./$test"
	[ -x "$OBJDIR$test" ] && testexe="$OBJDIR$test"
	LD_LIBRARY_PATH="$OBJDIR../src" "$testexe" "$@") 2>&1
	res=$?
	if [ $res -ne 0 ]; then
		echo "Test: $test$sep$@: FAIL (error $res)"
		echo " FAIL" 1>&2
	else
		echo "Test: $test$sep$@: PASS"
		echo " PASS" 1>&2
	fi
	return $res
}


#test
_test()
{
	_run "$@" >> "$target"
	res=$?
	[ $res -eq 0 ] || FAILED="$FAILED $test(error $res)"
}


#usage
_usage()
{
	echo "Usage: $PROGNAME [-c] target..." 1>&2
	return 1
}


#main
clean=0
while getopts "cP:" name; do
	case "$name" in
		c)
			clean=1
			;;
		P)
			#XXX ignored
			;;
		?)
			_usage
			exit $?
			;;
	esac
done
shift $((OPTIND - 1))
if [ $# -eq 0 ]; then
	_usage
	exit $?
fi

if [ -n "$PKG_CONFIG_SYSROOT_DIR" ]; then
	#XXX cross-compiling
	tests="pkgconfig.sh"
	failures=
else
	tests="array config error event includes pkgconfig.sh string variable"
	failures=
fi
if $PKGCONFIG --exists "python-2.7"; then
	tests="$tests python.sh"
else
	failures="$failures python.sh"
fi

while [ $# -ne 0 ]; do
	target="$1"
	shift

	[ "$clean" -eq 0 ]					|| break

	_date > "$target"
	FAILED=
	echo "Performing tests:" 1>&2
	for test in $tests; do
		_test "$test"
	done
	echo "Expected failures:" 1>&2
	for test in $failures; do
		_fail "$test"
	done
	if [ -n "$FAILED" ]; then
		echo "Failed tests:$FAILED" 1>&2
		exit 2
	fi
	echo "All tests completed" 1>&2
done
exit 0
