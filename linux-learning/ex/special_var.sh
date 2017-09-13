#!/bin/bash

echo "============demo special variables ======"
echo "Script Name: $0"
function func {
    for var in $*
    do
        let i=i+1
        echo "The \$${i} argument is: ${var}"
    done
    echo "Total count of arguments: $#"
}
func We are argument

echo "============demo \$* and \$@ =============="
function func2 {
    echo "--- \"\$*\""
    for ARG in "$*"
    do
        echo $ARG
    done

    echo "--- \"\$@\""
    for ARG in "$@"
    do
        echo $ARG
    done
}
func2 We are argument

coredumper_options="$coredumper_options -o localdir=/var/crash"
echo $coredumper_options
