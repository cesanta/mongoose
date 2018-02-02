#!/bin/bash -e

cd $(dirname $0)
./mkmgtest.cmd $PWD/mgtest.vcxproj
cd ..
cp win/Debug/mgtest.exe .

./mgtest.exe || {
    #hack to flush the logs
    echo failed
    exit 1
}
