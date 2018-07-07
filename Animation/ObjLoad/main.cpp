#include <GL/glut.h> // graphic library utility toolkit
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>

using namespace std;

#define WINDOW_WIDTH    900
#define WINDOW_HEIGHT   900
#define BUFFER_WIDTH    900
#define BUFFER_HEIGHT   900

typedef struct RGB {
    GLfloat r, g, b;
} RGB;

typedef struct Point {
    double x, y, z;
} Point;

typedef struct Vector {
    double x, y, z;
} Vector;

double degree;
double color;
GLuint scene;
int frameCount;

void loadScene();

void init() {

    scene = glGenLists(1);
    glNewList(scene, GL_COMPILE);
    loadScene();
    glEndList();

    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat light_position2[] = {2.0, 1.0, 1.0, 0.0};
    GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat Light_Model_Ambient[] = {1.0, 1.0, 1.0, 1.0};

    glClearColor(0.4, 0.4, 0.4, 1.0);
    glShadeModel(GL_SMOOTH);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
}

Vector calcNormal(Point p1, Point p2, Point p3) {
    Vector v1, v2, n;
    v1.x = p1.x - p2.x;
    v1.y = p1.y - p2.y;
    v1.z = p1.z - p2.z;

    v2.x = p1.x - p3.x;
    v2.y = p1.y - p3.y;
    v2.z = p1.z - p3.z;

    n.x = v1.y * v2.z - v1.z * v2.y;
    n.y = v1.z * v2.x - v1.x * v2.z;
    n.z = v1.x * v2.y - v1.y * v2.x;

    double length = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);

    n.x = n.x / length;
    n.y = n.y / length;
    n.z = n.z / length;

    return n;
}

void loadScene() {
    ifstream inf;
    inf.open("surface.obj");
    if(inf) {
        printf("Loading scene...\n");
        Point surfacePts[1500];
        Vector normals[1500];
        int surfaceCount = 0;
        int normalCount = 0;
        int surfacePtCount = 0;
        while(inf.good()) {
            char line[100];
            inf.getline(line, 100);
            if(line[0] != '\0') {
                char* valType = strtok(line, " ");
                if(!strcmp(valType, "v")) {
                    surfacePts[surfacePtCount].x = strtod(strtok(NULL, " "), NULL);
                    surfacePts[surfacePtCount].y = strtod(strtok(NULL, " "), NULL);
                    surfacePts[surfacePtCount].z = strtod(strtok(NULL, " "), NULL);
                    surfacePtCount++;
                } else if(!strcmp(valType, "vn")) {
                    normals[normalCount].x = strtod(strtok(NULL, " "), NULL);
                    normals[normalCount].y = strtod(strtok(NULL, " "), NULL);
                    normals[normalCount].z = strtod(strtok(NULL, " "), NULL);
                    normalCount++;
                } else if (!strcmp(valType, "f")) {
                    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                    glBegin(GL_POLYGON);
                    Point p1, p2, p3, p4;
                    /*
                    int normalIndex;
                    char* n_str;*/
                    p1 = surfacePts[atoi(strtok(NULL, "/")) - 1];

                    //normalIndex = atoi();
                    //n_str = strtok(NULL, " ");
                    strtok(NULL, " ");
                    //normalIndex = atoi(n_str + 1);
                    p2 = surfacePts[atoi(strtok(NULL, "/")) - 1];

                    strtok(NULL, " ");
                    p3 = surfacePts[atoi(strtok(NULL, "/")) - 1];
                    
                    strtok(NULL, " ");
                    p4 = surfacePts[atoi(strtok(NULL, "/")) - 1];

                    Vector n1, n2;
                    double n[3];

                    

                    n1 = calcNormal(p1, p2, p3);
                    n2 = calcNormal(p3, p4, p1);
                    
                    n[0] = (n1.x + n2.x) / 2;
                    n[1] = (n1.y + n2.y) / 2;
                    n[2] = (n1.z + n2.z) / 2;
                    /*
                    n[0] = normals[normalIndex].x;
                    n[1] = normals[normalIndex].y;
                    n[2] = normals[normalIndex].z;
                    */

                    glNormal3dv(n);
                    glVertex3d(p1.x, p1.y, p1.z);
                    glVertex3d(p2.x, p2.y, p2.z);
                    glVertex3d(p3.x, p3.y, p3.z);
                    glVertex3d(p4.x, p4.y, p4.z);
                    glEnd();
                    surfaceCount++;
                }
            }
        }
        printf("Scene loading complete. (%d vertices, %d surfaces)\n", surfacePtCount, surfaceCount);
        //printf("%d %d %d\n", surfacePtCount, normalCount, surfaceCount);
    }
}

void loadCar() {

}

void display() {
    //printf("Rendering frame %d\n", frameCount++);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    
    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    degree = degree + 1.0;
    glRotatef(degree, 0.0, 1.0, 0.0);
    glTranslatef(0.0, 0.0, 0.0);
    glCallList(scene);
    //glTranslatef(0.0, 0.0, 3.0);
    //glutSolidSphere(1.0, 20, 16);
    
    
    //glutSolidTeapot(0.5);
    
    glPopMatrix();
    glutSwapBuffers();
    glFlush();
    display();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void callback_keyboard(unsigned char key, int x, int y) {
}

void callback_mouse(int button, int state, int x, int y) {
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("ANIMATION");
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(callback_keyboard);
    glutMouseFunc(callback_mouse);

    glutMainLoop();  // use to stay running

    return 0;
}