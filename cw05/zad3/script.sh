#!/bin/bash

if ! [[ -e temp ]]; then
    touch temp
else
    exit 1
fi


for ((i = 1; i < $#; i++)); do
    cat "${!i}" >> temp
    printf '\n' >> temp
done


if cmp -s temp ${!#}; then
    printf 'WORKS\n'
else
    printf 'DOESNT WORK\n'
fi


rm -f temp
