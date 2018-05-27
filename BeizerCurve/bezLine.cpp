#include <GL/glut.h> // graphic library utility toolkit
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

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
int sel_index = -1;
int CP_count = 0;
int smooth = 10;
bool displayHelp = true;
int displayMode = 0;

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
	Point bezPts[smooth + 1];
	int segCount = (CP_count - 1) / 3;

	glBegin(GL_LINE_STRIP);
	for(int s = 0; s < segCount; s++) {
		int beginPt = 3 * s;
		for(int i = 0; i < smooth + 1; i++){
			float t = float(i) / smooth;
			bezPts[i].x = (1-t) * (1-t) * (1-t) * CtrlPts[beginPt].x + 3*t*(1-t)*(1-t)*CtrlPts[beginPt+1].x + 3*t*t*(1-t)*CtrlPts[beginPt+2].x + t*t*t*CtrlPts[beginPt+3].x;
			bezPts[i].y = (1-t) * (1-t) * (1-t) * CtrlPts[beginPt].y + 3*t*(1-t)*(1-t)*CtrlPts[beginPt+1].y + 3*t*t*(1-t)*CtrlPts[beginPt+2].y + t*t*t*CtrlPts[beginPt+3].y;
			glVertex2f(float(bezPts[i].x), float(bezPts[i].y));
		}
	}
	glEnd();
}

void displayString(char* str , int x, int y) {
	glRasterPos2i(x, y);
	for(int i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, str[i]);
	}
}

void update() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(5.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	
	if(displayMode < 2) {
		glBegin(GL_POINTS);
		for(int i = 0; i < CP_count; i++) {
			if(i == sel_index)
				glColor3f(1.0f, 0.0f, 0.0f);
			glVertex2f(float(CtrlPts[i].x), float(CtrlPts[i].y));
			if(i == sel_index)
				glColor3f(0.0f, 1.0f, 0.0f);
		}
		glEnd();
	}
	
	if(displayMode < 1) {
		glBegin(GL_LINE_STRIP);
		for(int i = 0; i < CP_count; i++) {
			glVertex2f(float(CtrlPts[i].x), float(CtrlPts[i].y));
		}
		glEnd();
	}

	glColor3f(1.0f, 1.0f, 1.0f);
	if(CP_count >= 4) {
		drawBez();
	}
	glColor3f(0.0f, 1.0f, 0.0f);

	if(displayHelp) {
		glColor3f(0.0f, 1.0f, 1.0f);
		displayString("Help:", 700, 860);
		displayString("+/-: Smooth control", 700, 840);
		displayString("N: Add new control point", 700, 820);
		displayString("S: Save curve to file", 700, 800);
		displayString("L: Save curve from file", 700, 780);
		displayString("Left Mouse: Select point", 700, 760);
		displayString("Right Mouse: Move point", 700, 740);
		displayString("H: Hide help", 700, 720);
		displayString("V: Toggle view mode", 700, 700);
		displayString("R: Reset", 700, 680);
		displayString("Please type filename in terminal. Filename limit is 20 characters without space.", 20, 20);
		glColor3f(0.0f, 1.0f, 0.0f);
	}
	//glutBitmapCharacter(GLUT_BITMAP_8_BY_13, 'a');
	glFlush();
}

void saveBez() {
	char filename[20];
	cout<<"Save as:"<<endl;
	cin>>filename;

	ofstream outf;
	outf.open(filename);
	outf << CP_count << endl;
	for(int i = 0; i < CP_count; i++)
		outf << CtrlPts[i].x << " " << CtrlPts[i].y << endl;

	cout<<"Curve saved to "<<filename<<endl;
	outf.close();
}

void loadBez() {
	char filename[20];
	cout<<"Load file:"<<endl;
	cin>>filename;
	ifstream inf;
	inf.open(filename);
	if(inf) {
		inf >> CP_count;
		for(int i = 0; i < CP_count; i++)
			inf >> CtrlPts[i].x >> CtrlPts[i].y;
		inf.close();
		cout<<"Load curve successfully."<<endl;
	} else {
		cout<<"No such a file."<<endl;
	}
}

void callback_keyboard(unsigned char key, int x, int y) {
	y = WINDOW_HEIGHT - y;

	switch(key) {
		case 'n':
			if(CP_count == 1 || (CP_count - 1) % 3 != 0) {
				CtrlPts[CP_count].x = x;
				CtrlPts[CP_count].y = y;
			} else {
				CtrlPts[CP_count].x = 2 * CtrlPts[CP_count - 1].x - CtrlPts[CP_count - 2].x;
				CtrlPts[CP_count].y = 2 * CtrlPts[CP_count - 1].y - CtrlPts[CP_count - 2].y;
			}
			CP_count++;
			break;
		case '+':
			smooth++;
			break;
		case '-':
			if(smooth > 3)
				smooth--;
			break;
		case 's':
			saveBez();
			break;
		case 'l':
			sel_index = -1;
			loadBez();
			break;
		case 'h':
			if(displayHelp)
				displayHelp = false;
			else
				displayHelp = true;
			break;
		case 'v':
			if(displayMode < 2)
				displayMode++;
			else
				displayMode = 0;
			break;
		case 'r':
			CP_count = 0;
	}

	update();
	//}
}

void callback_mouse(int button, int state, int x, int y) {
	y = WINDOW_HEIGHT - y;
	switch(button) {
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_UP) {
				for(int i = 0; i < CP_count; i++) {
					if(abs(CtrlPts[i].x - x) < 5 && abs(CtrlPts[i].y - y) < 5) {
						sel_index = i;
						printf("Control Point (%d, %d) selected.\n", CtrlPts[i].x, CtrlPts[i].y);
						break;
					}
				}
			}
			break;
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_UP) {
				if(sel_index != -1) {
					printf("Control Point (%d, %d) changed to (%d, %d).\n", CtrlPts[sel_index].x, CtrlPts[sel_index].y, x, y);
					int dx = x - CtrlPts[sel_index].x;
					int dy = y - CtrlPts[sel_index].y;
					CtrlPts[sel_index].x = x;
					CtrlPts[sel_index].y = y;
					
					if((sel_index+1) % 3 == 0) {
						if(CP_count >= sel_index+3) {
							CtrlPts[sel_index + 2].x = 2 * CtrlPts[sel_index + 1].x - CtrlPts[sel_index].x;
							CtrlPts[sel_index + 2].y = 2 * CtrlPts[sel_index + 1].y - CtrlPts[sel_index].y;
						}
					} else if((sel_index+1) % 3 == 2 && sel_index != 1) {
						CtrlPts[sel_index - 2].x = 2 * CtrlPts[sel_index - 1].x - CtrlPts[sel_index].x;
						CtrlPts[sel_index - 2].y = 2 * CtrlPts[sel_index - 1].y - CtrlPts[sel_index].y;
					} else if((sel_index+1) % 3 == 1 && sel_index != 0) {
						
						if(CP_count > sel_index + 1) {
						CtrlPts[sel_index - 1].x += dx;
						CtrlPts[sel_index - 1].y += dy;
						CtrlPts[sel_index + 1].x += dx;
						CtrlPts[sel_index + 1].y += dy;
						}
					}
				}
			}
	}
	update();
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
	glutMouseFunc(callback_mouse);
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