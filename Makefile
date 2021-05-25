OBJ=image
SRC_CPP=$(shell find ./ -name "*.cpp")
SRC=$(shell find ./ -name "*.c")
cc=g++

prom=$(*.o)

$(OBJ):$(prom)
	$(cc) -o $(OBJ) $< -I ./freetype2 -I ./freetype2/freetype/ -I ./

%.o:%.cpp
	$(cc) -c $< -o $@ -I ./freetype2 -I ./freetype2/freetype/ -I ./

%.o:%.c
	gcc -c $< -o $@ -I ./freetype2 -I ./freetype2/freetype/ -I ./


clean:
	rm *.o $(OBJ)
