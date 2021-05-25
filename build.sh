#convert -size 640x480 xc:"rgba(201,201,201,50)"  -pointsize 20 -fill blue -draw "text 10,70 'color-R'" -fill red -draw "text 560,70 '30%'"  -fill "rgb(255,255,0)" -stroke "rgb(0,0,0)" -strokewidth 2 -draw "roundrectangle 80,50 550,30 30,0"  -fill "rgb(255,0,0)" -draw "roundrectangle 80,50 50,30 30,0"  new.png 
#make
rm image
g++ image.cpp MIO_Freetype2.c -o image -I /usr/local/freetype/include/freetype2/ -I ./   -lfreetype  -L /usr/local/freetype/lib/
./image
#scp osd  root@192.168.10.40:/usr/Hseries/bin/Middleware/osd.bmp

