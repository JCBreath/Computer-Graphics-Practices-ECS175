#include <GL/glut.h> // graphic library utility toolkit
#include <cstdio>
#include <cmath>

#define WINDOW_WIDTH	900
#define WINDOW_HEIGHT	900
#define BUFFER_WIDTH	900
#define BUFFER_HEIGHT	900

typedef struct RGB {
	GLfloat r, g, b;
} RGB;

typedef struct Point {
	int x, y;
} Point;

Point CtrlPts[100];
bool CP_sel[100];
int CP_count = 0;

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

void fillScreen() {
	int x, y;
	for (y=0;y<BUFFER_HEIGHT;y++) {
		for(x = 0; x < BUFFER_WIDTH; x++) {
			setPixel(x, y, 0, 0, 0);
		}
	}
}

void drawCtrlPt(int x, int y, bool sel) {
	if(!sel) {
		glRecti(x, y, x + 5, y + 5);
	}
}

void drawBez() {
	Point bezPts[10];
	glBegin(GL_POINTS);
	
	for(int i = 0; i < 10; i++){
		float t = float(i) / 10;
		bezPts[i].x = (1-t) * (1-t) * CtrlPts[0].x + 2*t*(1-t)*CtrlPts[1].x + t*t*CtrlPts[2].x;
		bezPts[i].y = (1-t) * (1-t) * CtrlPts[0].y + 2*t*(1-t)*CtrlPts[1].y + t*t*CtrlPts[2].y;
		glVertex2f(float(bezPts[i].x), float(bezPts[i].y));
	}
	glEnd();
}

void update() {
	// commented: not using command line
	//printf("update.\n");
	//fflush(0);
	// Upper
	//lineBres(150, 100, 180, 160);
	//setPixel(450, 450);
	//setPixel(451, 450);
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	for(int i = 0; i < CP_count; i++) {
		glVertex2f(float(CtrlPts[i].x), float(CtrlPts[i].y));
		printf("%d,%d\n", CtrlPts[i].x, CtrlPts[i].y);
	}

	//glVertex2f(float(x - 450)/900, float(y - 450)/900);
	glEnd();
	if(CP_count == 3) {
		drawBez();
	}
	//glDrawPixels(BUFFER_WIDTH, BUFFER_HEIGHT, GL_RGB, GL_FLOAT, pixelBuffer);
	glFlush();
	//glutPostRedisplay(); // explicitly set flag (use for animation)
}

void callback_keyboard(unsigned char key, int x, int y) {
	//printf("%c", key);
	//if(key == 'c') {
	y = WINDOW_HEIGHT - y;
	printf("%d,%d\n", x, y);
	//drawCtrlPt(x, y, false);

	if(key == 'c') {
		CtrlPts[CP_count].x = x;
		CtrlPts[CP_count].y = y;
		CP_count++;
	}

	//glClear(GL_COLOR_BUFFER_BIT);
	/*glPointSize(5.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	glVertex2f(float(x - 450)/900, float(y - 450)/900);
	glEnd();
	glFlush();*/


	update();
	//}
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("BEZIER SPLINE");
	//fillScreen();
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, 900.0, 0, 900.0, -300, 300.0);
	glutDisplayFunc(update);
	glutKeyboardFunc(callback_keyboard);

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