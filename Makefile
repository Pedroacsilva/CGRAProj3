all: 12_1 proj2_anaglyph main textures proj3_ar


main: deecshader.o main.cpp cgraimage.o cgraobject.cpp
	g++ -o main cgraimage.o deecshader.o main.cpp -I./vendor/stb_image -I./vendor -I./vendor/imgui  -lglfw -lGLEW -lGL -Wall -lrt -lm -ldl -I/opt/local/include -lnetpbm

cgraimage.o:	cgraimage.h cgraimage.cpp
	g++ -c -g -I/opt/local/include -I/usr/include/netpbm cgraimage.cpp

textures: deecshader.o textures.cpp cgraimage.o cgraobject.cpp
	g++ -o textures	deecshader.o cgraimage.o textures.cpp	-I/opt/local/include -I./vendor/stb_image -I./vendor -I./vendor/imgui  -lglfw -lGLEW -lGL -Wall -lrt -lm -ldl -lnetpbm 

fbo_test: deecshader.o cgraobject.cpp fbo_test.cpp FrameBuffer.cpp RenderBuffer.cpp TextureBuffer.cpp
	g++ -o fbo_test	deecshader.o cgraimage.o fbo_test.cpp	-I/opt/local/include -I./vendor/stb_image -I./vendor -I./vendor/imgui  -lglfw -lGLEW -lGL -Wall -lrt -lm -ldl -lnetpbm 

12_1: deecshader.o 12_1.cpp cgraimage.o cgraobject.cpp
		g++ -o 12_1 cgraimage.o deecshader.o 12_1.cpp -I./vendor/stb_image -I./vendor -I./vendor/imgui  -lglfw -lGLEW -lGL -Wall -lrt -lm -ldl -I/opt/local/include -lnetpbm

proj2_anaglyph: deecshader.o proj2_anaglyph.cpp cgraimage.o cgraobject.cpp
	g++ -o proj2_anaglyph cgraimage.o deecshader.o proj2_anaglyph.cpp -I./vendor/stb_image -I./vendor -I./vendor/imgui  -lglfw -lGLEW -lGL -Wall -lrt -lm -ldl -I/opt/local/include -lnetpbm

proj3_ar: deecshader.o proj3_ar.cpp cgraimage.o cgraobject.cpp VideoTexture.cpp
	g++ -o proj3_ar cgraimage.o deecshader.o proj3_ar.cpp `pkg-config --cflags --libs opencv4` -I./vendor/stb_image -I./vendor -I./vendor/imgui  -lglfw -lGLEW -lGL -Wall -lrt -lm -ldl -I/opt/local/include -lnetpbm

deecshader.o:
	g++ -c -g deecshader.cpp

clean:
	rm -f 12_1 proj2_anaglyph textures fbo_test main
