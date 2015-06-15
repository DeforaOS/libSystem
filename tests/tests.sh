#!/bin/sh
#$Id$
#Copyright (c) 2011-2015 Pierre Pronchery <khorben@defora.org>
#This file is part of DeforaOS System libSystem
#This program is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, version 3 of the License.
#
#This program is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with this program.  If not, see <http://www.gnu.org/licenses/>.



#variables
[ -n "$OBJDIR" ] || OBJDIR="./"
PROGNAME="tests.sh"
#executables
DATE="date"


#functions
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
	"$OBJDIR$test" "$@") 2>&1
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
	echo "Usage: $PROGNAME [-c][-P prefix]" 1>&2
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
if [ $# -ne 1 ]; then
	_usage
	exit $?
fi
target="$1"

[ "$clean" -ne 0 ]			&& exit 0

$DATE > "$target"
FAILED=
echo "Performing tests:" 1>&2
_test "array"
_test "config"
_test "includes"
_test "string"
_test "variable"
echo "Expected failures:" 1>&2
_fail "python.sh"
if [ -n "$FAILED" ]; then
	echo "Failed tests:$FAILED" 1>&2
	exit 2
fi
echo "All tests completed" 1>&2
