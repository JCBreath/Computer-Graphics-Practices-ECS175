ARG = -lglut -lGL -lGLU

all: scanline share_edge

scanline:
	g++ scanline.cpp $(ARG) -o scanline.out

share_edge:
	g++ share_edge.cpp $(ARG) -o share_edge.out

clean:
	rm -f *.out
