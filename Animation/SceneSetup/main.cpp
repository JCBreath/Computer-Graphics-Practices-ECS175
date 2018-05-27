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
    int x, y, z;
} Point;

void init() {
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
    GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat Light_Model_Ambient[] = {0.2, 0.2, 0.2, 1.0};

    glClearColor(0.0, 0.0, 0.0, 0.0);
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

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);
    glPushMatrix();
    glutSolidSphere(1.0, 20, 16);
    glRotatef(20.0, 0.0, 1.0, 0.0);
    glTranslatef(2.0, 0.0, 0.0);
    glutSolidTeapot(0.5);
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);
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
    
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("BEZIER SPLINE");
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(callback_keyboard);
    glutMouseFunc(callback_mouse);

    glutMainLoop();  // use to stay running

    return 0;
}