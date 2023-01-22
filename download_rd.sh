#!/bin/bash
# Use Export Cookies Firefox extension 

if [ -z "$1" ]; then
  echo "pass in day"
  exit
fi


filename="input$i.html"
wget --load-cookies=cookies-adventofcode-com.txt https://adventofcode.com/2017/day/$1 -O "$filename" 
if [ "$1" -gt 9 ]; then
    outfile=Day$1/README.txt
else
    outfile=Day0$1/README.txt
fi
w3m -dump "$filename" | sed '/---/,$!d' > "$outfile"
rm "$filename"
