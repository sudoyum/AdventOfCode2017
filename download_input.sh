#!/bin/bash
# Use Export Cookies Firefox extension 

if [ -z "$1" ]; then
  echo "pass in day"
  exit
fi

if [ "$1" -gt 9 ]; then
    outfolder=Day$1
else
    outfolder=Day0$1
fi

wget --load-cookies=cookies-adventofcode-com.txt https://adventofcode.com/2017/day/$1/input -O $outfolder/input$1.txt
