#include <GL/glut.h> // graphic library utility toolkit
#include <cstdio>

#define WINDOW_WIDTH	300
#define WINDOW_HEIGHT	300
#define BUFFER_WIDTH	300
#define BUFFER_HEIGHT	300

typedef struct RGB {
	GLfloat r, g, b;
} RGB;

typedef struct Polygon {
	int x[20];
	int y[20];
	int v_num;
} Polygon;

typedef struct MyPoint {
	int x;
	int y;
} MyPoint;

//float pixelBuffer[BUFFER_WIDTH * BUFFER_HEIGHT * 3];

RGB pixelBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

void setPixel(int x, int y, float r, float g, float b) {
	pixelBuffer[BUFFER_WIDTH * y + x].r = r / 255;
	pixelBuffer[BUFFER_WIDTH * y + x].g = g / 255;
	pixelBuffer[BUFFER_WIDTH * y + x].b = b / 255;
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

void fillScreen(float r, float g, float b) {
  	for(int i = 0; i < 300; i++) {
  		for(int j = 0; j < 300; j++) {
    		setPixel(i, j, r, g, b);
  		}
	}
}

Polygon triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
	Polygon p;
	p.v_num = 3;
	p.x[0] = x1;
	p.x[1] = x2;
	p.x[2] = x3;
	p.y[0] = y1;
	p.y[1] = y2;
	p.y[2] = y3;
	return p;
}

Polygon quadrangle(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
	Polygon p;
	p.v_num = 4;
	p.x[0] = x1;
	p.x[1] = x2;
	p.x[2] = x3;
	p.x[3] = x4;
	p.y[0] = y1;
	p.y[1] = y2;
	p.y[2] = y3;
	p.y[3] = y4;
	return p;
}

Polygon head() {
	Polygon p;
	p.v_num = 8;
	p.x[0] = 0;
	p.x[1] = 75;
	p.x[2] = 75;
	p.x[3] = 100;
	p.x[4] = 200;
	p.x[5] = 225;
	p.x[6] = 225;
	p.x[7] = 300;
	p.y[0] = 0;
	p.y[1] = 40;
	p.y[2] = 250;
	p.y[3] = 200;
	p.y[4] = 200;
	p.y[5] = 250;
	p.y[6] = 40;
	p.y[7] = 0;
	return p;
}

Polygon left_eye() {
	return triangle(100, 150, 140, 137, 125, 130);
}

Polygon right_eye() {
	return triangle(200, 150, 160, 137, 175, 130);
}

Polygon face() {
	Polygon p;
	p.v_num = 7;
	p.x[0] = 100;
	p.x[1] = 100;
	p.x[2] = 150;
	p.x[3] = 200;
	p.x[4] = 200;
	p.x[5] = 190;
	p.x[6] = 110;
	p.y[0] = 60;
	p.y[1] = 120;
	p.y[2] = 100;
	p.y[3] = 120;
	p.y[4] = 60;
	p.y[5] = 50;
	p.y[6] = 50;
	return p;
}

int minY(Polygon p) {
	int min_y = p.y[0];

	for(int i = 0; i < p.v_num; i++) {
		if(p.y[i] < min_y) {
			min_y = p.y[i];
		}
	}

	return min_y;
}

int maxY(Polygon p) {
	int max_y = p.y[0];

	for(int i = 0; i < p.v_num; i++) {
		if(p.y[i] > max_y) {
			max_y = p.y[i];
		}
	}

	return max_y;
}

// sort (bubble sort)
void sort(int arr[], int n) {
	for(int i = 0;i < n;i++){
		for(int j = 0;j < n-i-1;j++) {  
			if(arr[j] > arr[j+1]) {
			int temp = arr[j];
			arr[j] = arr[j+1];
			arr[j+1] = temp;
			}
		}
	}
}

void ScanFill(Polygon p, float r, float g, float b) {
	int miny = minY(p);
	int maxy = maxY(p);
	int x1, x2, y1, y2;

	// < instead of <= for space of another sharing bottom edge
	for(int y = miny; y < maxy; y++) {
		int v1, v2;
		int x;
		// Intersections
		int inter[10];
		int inter_count = 0;

		for(v1 = 0, v2 = p.v_num - 1; v1 < p.v_num; v2 = v1, v1++) {
			x1 = p.x[v1];
			y1 = p.y[v1];
			x2 = p.x[v2];
			y2 = p.y[v2];

			int y_high = (y1>y2)?y1:y2;
			int y_low = (y1<y2)?y1:y2;
            
            



			// horizontal
			if(y1 == y2) {
				// overlapping
				if(y == y1) {
					int maxx = (x1>x2)?x1:x2;
					int minx = (x1<x2)?x1:x2;

					for(x = minx; x < maxx; x++) {
						setPixel(x, y, r, g, b);
					}
				}
				continue;
			}

			// no intersection
			if(y > y_high || y < y_low)
				continue;

			x = x1 + (y - y1) * (x2 - x1) / (y2 - y1);
			
			//setPixel(x, y);
			
			// if it is a vertex
			if((x == x1) && (y == y1)) {
				
				int vv1, vv2;
				if(v1 == 0) {
					vv1 = p.v_num - 1;
					vv2 = 1;
				} else if (v1 == p.v_num - 1) {
					vv1 = v1 - 1;
					vv2 = 0;
				} else {
					vv1 = v1 - 1;
					vv2 = v1 + 1;
				}
				printf("%d,%d[%d]\n",x,y,inter_count);
				if((y - p.y[vv1]) * (y - p.y[vv2]) < 0) {
					inter[inter_count] = x;
					inter_count++;
				} else {
					inter[inter_count] = x;
					inter_count++;
					inter[inter_count] = x;
					inter_count++;
				}
			} else if((x == x2) && (y == y2)) {
				
				/*int vv1, vv2;
				if(v2 == 0) {
					vv1 = p.v_num - 1;
					vv2 = 1;
				} else if (v2 == p.v_num - 1) {
					vv1 = v2 - 1;
					vv2 = 0;
				} else {
					vv1 = v2 - 1;
					vv2 = v2 + 1;
				}

				if((y - p.y[vv1]) * (y - p.y[vv2]) < 0) {
					inter[inter_count] = x;
					inter_count++;
				} else {*/
					inter[inter_count] = x;
					inter_count++;
					inter[inter_count] = x;
					inter_count++;
				//}
			} else {
				inter[inter_count] = x;
				inter_count++;
			}
			

		}
		
		sort(inter, inter_count);

		// fill
		for(int i = 0; i < inter_count; i += 2) {
			int j = i + 1;
			

			// < instead of <= for space of another sharing right edge
			for(x = inter[i]; x < inter[j]; x++) {
				setPixel(x, y, r, g, b);
			}
		}

	}
}

void update() {
	// commented: not using command line
	//printf("update.\n");
	//fflush(0);
	fillScreen(100, 100, 100); // background grey
	Polygon p1 = triangle(200,200,150,150,150,250);
	Polygon p2 = triangle(100,200,150,150,150,250);
	Polygon p3 = triangle(100,100,150,150,200,100);
	Polygon p4 = triangle(100,100,150,50,200,100);

	ScanFill(head(), 0, 10, 20);
	ScanFill(left_eye(), 255, 255, 255);
	ScanFill(right_eye(), 255, 255, 255);
	ScanFill(face(), 243, 208, 177);

	
	

	glDrawPixels(BUFFER_WIDTH, BUFFER_HEIGHT, GL_RGB, GL_FLOAT, pixelBuffer);
	glFlush();
	//glutPostRedisplay(); // explicitly set flag (use for animation)
}

int main(int argc, char **argv) {
	glutInit(&argc, argv);

	glutCreateWindow("BATMAN");

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