#!/bin/bash
convert background.png -compose Atop 800x100.png -geometry 600x100+0+100! -composite -compose Atop logo.png -geometry 600x100+0+300 -composite  out.png
#
#convert %s -colorspace sRGB -depth 32 -alpha set -flip stream.bmp

#0x0A  date

