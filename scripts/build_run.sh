#!/bin/sh
make -C build
if [ $? -eq 0 ]; then
    build/src/feud
else
    echo FAIL
fi