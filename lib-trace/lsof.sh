#!/bin/bash

# Use strace on lsof(1).
# Inspect what lsof does to show all the open files.
# Then, create a shell script version of lsof to do the same.

for p in /proc/[1-9]*;
do
	if [[ $(ls ${p}/fd|wc -l) = '0' ]];
	then
		continue;
	fi;
	echo ${p}:
	ls -l ${p}/fd | sed 1d | awk '{print $NF}' | sort | uniq
done

