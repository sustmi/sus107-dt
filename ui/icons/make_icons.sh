#!/bin/sh

echo "* Generating icons.cpp and icons.h"
./png2wx.pl -H icons.h -C icons.cpp *.png
mv icons.h icons.cpp ../../src/ui/
