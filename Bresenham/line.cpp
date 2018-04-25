#include <GL/glut.h> // graphic library utility toolkit
#include <cstdio>

#define WINDOW_WIDTH	300
#define WINDOW_HEIGHT	300
#define BUFFER_WIDTH	300
#define BUFFER_HEIGHT	300

typedef struct RGB {
	GLfloat r, g, b;
} RGB;

//float pixelBuffer[BUFFER_WIDTH * BUFFER_HEIGHT * 3];

RGB pixelBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

void setPixel(int x, int y, float r, float g, float b) {
	pixelBuffer[BUFFER_WIDTH * y + x].r = r;
	pixelBuffer[BUFFER_WIDTH * y + x].g = g;
	pixelBuffer[BUFFER_WIDTH * y + x].b = b;
}

void setPixel(int x, int y) {
	pixelBuffer[BUFFER_WIDTH * y + x].r = 1;
	pixelBuffer[BUFFER_WIDTH * y + x].g = 1;
	pixelBuffer[BUFFER_WIDTH * y + x].b = 1;
}

int fabs(int i) {
	if(i < 0)
		return  0 - i;
}

void lineBres(int x0, int y0, int xEnd, int yEnd) {
	int dx = fabs(x0 - xEnd),
		dy = fabs(y0 - yEnd);
	int p = 2 * dy - dx;
	int twoDy = 2 * dy,
		twoDyMinusDx = 2 * (dy - dx),
		twoDx = 2 * dx,
		twoDxMinusDy = 2 * (dx - dy);
	int x = x0, y = y0;

	// incrementing x
	if(dx >= dy) {
		if(x0 > xEnd) {
			x = xEnd;
			y = yEnd;
			xEnd = x0;
			yEnd = y0;
			x0 = x;
			y0 = y;
		}

		setPixel(x, y);
	
		for(x++ ; x < xEnd; x++) {
			if (p < 0)
				p += twoDy;
			else {
				if(y0 < yEnd)
					y++;
				else
					y--;
				p += twoDyMinusDx;
			}
			setPixel (x, y);
		}
	} else {
		if(y0 > yEnd) {
			x = xEnd;
			y = yEnd;
			xEnd = x0;
			yEnd = y0;
			x0 = x;
			y0 = y;
		}

		p = 2 * dx - dy;

		// TODO: make pixel for each column
	
		for(y++ ; y < yEnd; y++) {
			if (p < 0)
				p += twoDx;
			else {
				if(x0 < xEnd)
					x++;
				else
					x--;
				p += twoDxMinusDy;
			}
			setPixel (x, y);
		}
	}

	

	
}

void update() {
	// commented: not using command line
	//printf("update.\n");
	//fflush(0);
	// Upper
	lineBres(170, 166, 150, 150);
	lineBres(130, 166, 150, 150);
	lineBres(150, 166, 163, 161);
	lineBres(150, 166, 137, 161);
	lineBres(168, 150, 163, 161);
	lineBres(132, 150, 137, 161);
	lineBres(186, 150, 163, 161);
	lineBres(114, 150, 137, 161);
	lineBres(186, 150, 187, 161);
	lineBres(114, 150, 113, 161);
	lineBres(187, 161, 195, 150);
	lineBres(113, 161, 105, 150);
	lineBres(187, 161, 170, 166);
	lineBres(113, 161, 130, 166);
	// Middle
	lineBres(100, 150, 200, 150);
	lineBres(120, 166, 180, 166);
	lineBres(100, 150, 120, 166);
	lineBres(200, 150, 180, 166);
	// Bottom
	lineBres(100, 148, 200, 148);
	lineBres(100, 148, 100, 150);
	lineBres(200, 148, 200, 150);
	lineBres(150, 107, 200, 148);
	lineBres(150, 107, 100, 148);
	lineBres(132, 148, 150, 107);
	lineBres(168, 148, 150, 107);
	lineBres(156, 121, 150, 148);
	lineBres(144, 121, 150, 148);
	lineBres(156, 121, 186, 148);
	lineBres(144, 121, 114, 148);
	lineBres(162, 116, 186, 148);
	lineBres(138, 116, 114, 148);
	lineBres(162, 116, 195, 148);
	lineBres(138, 116, 105, 148);
	//lineBres(150, 100, 180, 160);
	glDrawPixels(BUFFER_WIDTH, BUFFER_HEIGHT, GL_RGB, GL_FLOAT, pixelBuffer);
	glFlush();
	//glutPostRedisplay(); // explicitly set flag (use for animation)
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutCreateWindow("DIAMOND");

	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	glutDisplayFunc(update);

	//while(1);
	glutMainLoop();  // use to stay running
	/*
	while(1) {
		if(keyboardEvent) {
			callbackKey
		}
		if(displayadjustment) {
			callbackdisplay
		}
	}
	*/

	return 0;
}

// in vim;
// :!g++ % -lglut -lGL && ./a.out