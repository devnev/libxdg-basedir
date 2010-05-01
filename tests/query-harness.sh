#!/bin/sh

if [ -z "$expected" ]; then
	echo "invalid test case, missing \$expected variable" >&2
fi
if [ -z "$arguments" ]; then
	echo "invalid test case, missing \$arguments variable" >&2
fi


output="`./testquery $arguments`"
test x"$expected" = x"$output"
result=$?
if [ $# -gt 0 ] && [ x"$1" = x"-d" ]; then
	echo "for $arguments"
	echo "expected:"
	echo "$expected"
	echo "got:"
	echo "$output"
	if [ "0" = "$result" ]; then
		echo "result: pass"
	else
		echo "result: fail ($result)"
	fi
fi
exit $result
