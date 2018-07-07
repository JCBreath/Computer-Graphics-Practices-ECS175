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
GLuint mount, grass, road, car1, water, wheel;
int frameCount;
int timeCount;
int sceneIndex = 0;
double fov = 73.0; // 80
double view_y = 250.0;
double wheel_rot_deg = 0.0;
double rot_ratio = 37.5;
double lapse_speed = 1.0;
double lapse_mult = 0.5;

// Diffuses
GLfloat mount_top_diffuse[] = { 1.0, 1.0, 1.0, 1.000000};
GLfloat mount_mid_diffuse[] = { 0.2901, 0.2745, 0.262, 1.000000};
GLfloat mount_mid2_diffuse[] = {0.411764706, 0.317647059, 0.28627451, 1.0};
GLfloat grass_top_diffuse[] = {0.525490196, 0.631372549, 0.22745098, 1.0};
GLfloat grass_bottom_diffuse[] = {0.592156863, 0.654901961, 0.235294118, 1.0};
GLfloat water_diffuse[] = {0.254901961, 0.592156863, 0.6, 0.5};
GLfloat road_diffuse[] = {0.543137255, 0.519607843, 0.519607843, 1.0};
GLfloat mount_bottom_diffuse[] = { 0.639215686, 0.411764706, 0.317647059, 1.000000};
GLfloat car_diffuse[] = {0.000000, 0.000000, 0.100000, 1.000000};
GLfloat wheel_diffuse[] = {0.00000, 0.00000, 0.00000, 1.000000};

// Ambient
GLfloat no_ambient[] = { 0.0, 0.0, 0.0, 1.0};
GLfloat mount_top_ambient[] = {0.2, 0.2, 0.3, 1.0};
GLfloat water_ambient[] = {0.154901961, 0.142156863, 0.2, 1.0};
GLfloat car_ambient[] = {0.050000, 0.050000, 0.050000, 1.000000};

// Specular
GLfloat no_specular[] = {0.05, 0.05, 0.1, 1.0};
GLfloat low_specular[] = {0.1, 0.1, 0.1, 1.0};
GLfloat water_specular[] = { 1.0, 1.0, 1.0, 1.0};
GLfloat car_specular[] = {0.774597, 0.774597, 0.974597, 1.000000};
GLfloat mount_top_specular[] = {0.774597, 0.74597, 0.974597, 1.000000};


// 

void loadScene(char* filename);

void init() {

    // mount color: 0.2901 0.2745 0.262
    

    mount = glGenLists(1);
    glNewList(mount, GL_COMPILE);
    
    loadScene("mount.obj");
    glEndList();

    grass = glGenLists(1);
    glNewList(grass, GL_COMPILE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, no_ambient);
    loadScene("grass.obj");
    glEndList();

    car1 = glGenLists(1);
    glNewList(car1, GL_COMPILE);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, car_ambient);
    loadScene("car1.obj");
    glEndList();

    road = glGenLists(1);
    glNewList(road, GL_COMPILE);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, road_ambient);
    loadScene("road.obj");
    glEndList();

    water = glGenLists(1);
    glNewList(water, GL_COMPILE);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, road_ambient);
    loadScene("water.obj");
    glEndList();

    wheel = glGenLists(1);
    glNewList(wheel, GL_COMPILE);
    //glMaterialfv(GL_FRONT, GL_AMBIENT, road_ambient);
    loadScene("wheel.obj");
    glEndList();

    //GLfloat mat_ambient[] = { 0.192250, 0.192250, 0.192250, 1.000000};
    //GLfloat mat_diffuse[] = { 0.507540, 0.507540, 0.507540, 1.000000};
    //GLfloat mat_specular[] = { 0.708273, 0.708273, 0.908273, 1.000000 };
    GLfloat mat_shininess[] = {10.0}; //材质RGBA镜面指数，数值在0～128范围内

 
 
    
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
/*
    GLfloat mat_specular[] = {0.0, 0.0, 1.0, 1.0};
    GLfloat mat_shininess[] = {50.0};
*/
    GLfloat light_position[] = {2.5, 1.0, 2.0, 0.0};
    GLfloat light_position2[] = {2.0, 1.0, 1.0, 0.0};
    GLfloat white_light[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat Light_Model_Ambient[] = {1.0, 1.0, 1.0, 1.0};

    
    glShadeModel(GL_SMOOTH);
/*
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
*/
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glClearColor(0.18627451, 0.67254902, 0.884313725, 1.0);
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

void loadScene(char* filename) {
    ifstream inf;
    inf.open(filename);
    if(inf) {
        printf("Loading scene...\n");
        Point surfacePts[10500];
        Vector normals[10500];
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
                    /*
                    n[0] = (n1.x + n2.x) / 2;
                    n[1] = (n1.y + n2.y) / 2;
                    n[2] = (n1.z + n2.z) / 2;
                    */
                    n[0] = n1.x;
                    n[1] = n1.y;
                    n[2] = n1.z;
                    /*
                    n[0] = normals[normalIndex].x;
                    n[1] = normals[normalIndex].y;
                    n[2] = normals[normalIndex].z;
                    */

                    //char* str = "mount.obj";
                    if(!strcmp(filename, "mount.obj")) {
                        if(p1.y + p2.y + p3.y > 90) {
                            glMaterialfv(GL_FRONT, GL_DIFFUSE, mount_top_diffuse);
                            glMaterialfv(GL_FRONT, GL_AMBIENT, mount_top_ambient);
                            glMaterialfv(GL_FRONT, GL_SPECULAR, mount_top_specular);
                        }

                        else if(p1.y + p2.y + p3.y > 60) {
                            glMaterialfv(GL_FRONT, GL_AMBIENT, no_ambient);
                            glMaterialfv(GL_FRONT, GL_DIFFUSE, mount_mid_diffuse);
                            glMaterialfv(GL_FRONT, GL_SPECULAR, no_specular);
                        }
                        else if(p1.y + p2.y + p3.y > 30) {
                            glMaterialfv(GL_FRONT, GL_AMBIENT, no_ambient);
                            glMaterialfv(GL_FRONT, GL_DIFFUSE, mount_mid2_diffuse);
                            glMaterialfv(GL_FRONT, GL_SPECULAR, no_specular);
                        }
                        else {
                            glMaterialfv(GL_FRONT, GL_AMBIENT, no_ambient);
                            glMaterialfv(GL_FRONT, GL_DIFFUSE, mount_bottom_diffuse);
                            glMaterialfv(GL_FRONT, GL_SPECULAR, no_specular);
                        }

                    } else if (!strcmp(filename, "grass.obj")) {
                        if(p1.y + p2.y + p3.y > 5) {
                            glMaterialfv(GL_FRONT, GL_AMBIENT, no_ambient);
                            glMaterialfv(GL_FRONT, GL_DIFFUSE, grass_top_diffuse);
                            glMaterialfv(GL_FRONT, GL_SPECULAR, no_specular);
                        } else {
                            glMaterialfv(GL_FRONT, GL_AMBIENT, no_ambient);
                            glMaterialfv(GL_FRONT, GL_DIFFUSE, grass_bottom_diffuse);
                            glMaterialfv(GL_FRONT, GL_SPECULAR, no_specular);
                        }
                    } else if (!strcmp(filename, "road.obj")) {
                        glMaterialfv(GL_FRONT, GL_AMBIENT, no_ambient);
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, road_diffuse);
                        glMaterialfv(GL_FRONT, GL_SPECULAR, low_specular);
                        //glMaterialfv(GL_FRONT, GL_AMBIENT, water_ambient);
                        //glMaterialfv(GL_FRONT, GL_SPECULAR, water_specular);
                    } else if (!strcmp(filename, "water.obj")) {
                        glMaterialfv(GL_FRONT, GL_AMBIENT, water_ambient);
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, water_diffuse);
                        glMaterialfv(GL_FRONT, GL_SPECULAR, water_specular);
                        //glMaterialfv(GL_FRONT, GL_SPECULAR, car_specular);
                    } else if (!strcmp(filename, "car1.obj")) {
                        glMaterialfv(GL_FRONT, GL_AMBIENT, car_ambient);
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, car_diffuse);
                        glMaterialfv(GL_FRONT, GL_SPECULAR, car_specular);
                    } else if (!strcmp(filename, "wheel.obj")) {
                        glMaterialfv(GL_FRONT, GL_AMBIENT, no_ambient);
                        glMaterialfv(GL_FRONT, GL_DIFFUSE, wheel_diffuse);
                        glMaterialfv(GL_FRONT, GL_SPECULAR, car_specular);
                    }

                    glBegin(GL_POLYGON);
                    glNormal3dv(n);
                    glVertex3d(p1.x, p1.y, p1.z);
                    glVertex3d(p2.x, p2.y, p2.z);
                    glVertex3d(p3.x, p3.y, p3.z);
                    glEnd();
                    
                    n[0] = n2.x;
                    n[1] = n2.y;
                    n[2] = n2.z;

                    glBegin(GL_POLYGON);
                    glNormal3dv(n);
                    glVertex3d(p3.x, p3.y, p3.z);
                    glVertex3d(p4.x, p4.y, p4.z);
                    glVertex3d(p1.x, p1.y, p1.z);
                    glEnd();
                    
                    surfaceCount++;
                }
            }
        }
        printf("Scene loading complete. (%d vertices, %d surfaces)\n", surfacePtCount, surfaceCount);
        //printf("%d %d %d\n", surfacePtCount, normalCount, surfaceCount);
    }
}

void renderWheels() {
        glPushMatrix();
            glTranslatef(-1.2, 0.0, 2.0);
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glRotatef(degree * rot_ratio, 0.0, 1.0, 0.0);
            glCallList(wheel);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-1.2, 0.0, -1.35);
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glRotatef(degree * rot_ratio, 0.0, 1.0, 0.0);
            glCallList(wheel);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.2, 0.0, 2.0);
            glRotatef(-90.0, 0.0, 0.0, 1.0);
            glRotatef(-degree * rot_ratio, 0.0, 1.0, 0.0);
            glCallList(wheel);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.2, 0.0, -1.35);
            glRotatef(-90.0, 0.0, 0.0, 1.0);
            glRotatef(-degree * rot_ratio, 0.0, 1.0, 0.0);

            glCallList(wheel);
        glPopMatrix();
}

void display() {
    //printf("Rendering frame %d\n", frameCount++);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Translate (y, z, x)
    //gluLookAt(100.0, 100.0, 100.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glPushMatrix();
    if(sceneIndex == 0) {
        glRotatef(180 - degree, 0.0, 1.0, 0.0);
        glCallList(mount);
        glCallList(grass);
        glCallList(road);
        glTranslatef(0.0, 1.0, 0);
        glCallList(water);
    } else if (sceneIndex == 1) {
        if(degree > 35.0)
            lapse_speed = 0.5;
        if(degree > 40.0)
            lapse_speed = 0.25;
        glRotatef(90, 0.0, 1.0, 0.0);
        glRotatef(degree * 2, -1.0, 0.0, 0.0);
        glRotatef(degree / 45 * 80, 0.0, -1.0, 0.0);
        glTranslatef(degree * 4.778, -0.444 * degree, -1.02222 * degree);
        //glTranslatef(-degree / 3, degree * 3, degree * 1.5);
        glCallList(mount);
        glCallList(grass);
        glCallList(road);
        glTranslatef(0.0, 1.0, 0);
        glCallList(water);
    } else if (sceneIndex == 2) {
        //glRotatef(90, 1.0, 0.0, 0.0);
        glRotatef(90, 0.0, 1.0, 0.0);
        glRotatef(90, -1.0, 0.0, 0.0);
        glRotatef(80 + degree / 3.5, 0.0, -1.0, 0.0);
        //glRotatef(90, 0.0, 0.0, 0.0);
        // 215, -20, -46
        glTranslatef(215 + degree / 5, -20, -46 - degree / 10);
        glCallList(mount);
        glCallList(grass);
        glCallList(road);
        glTranslatef(0.0, 1.0, 0);
        glCallList(water);
        glTranslatef(15.8, 16.7, 100 - degree * 1.5);
        glCallList(car1);
        renderWheels();
    } else if (sceneIndex == 3) {
        glRotatef(90, 0.0, -1.0, 0.0);
        glRotatef(90, 1.0, 0.0, 0.0);
        glTranslatef(-12.5 + degree / 8.5, -19, -190);
        glCallList(mount);
        glCallList(grass);
        glCallList(road);
        glTranslatef(0.0, 1.0, 0);
        glCallList(water);
        if(degree < 55.0) {
            glTranslatef(15.8, 16.5, 100 - 55.0 * 1.5 - degree);
        } else if (degree < 61){
            lapse_speed = 0.5;
            //glTranslatef(15.8, 16.5, -37.5);
            double x = 15.8 - (degree - 55) / 2;
            double z = - 37.5 - 1.333 * (degree - 55);
            glTranslatef(x, 16.6, z);
            glRotatef((degree - 55) * 15, 0.0, 1.0, 0.0);
            wheel_rot_deg += 2;
            //glTranslatef(12.8, 16.5, -45.5);
            //glTranslatef(7.8, 16.5, -48.5);
            //double x = 15.8 - degree + 55;
            //double z = 0.8227 * x - 50.5;
            //double z = x * x * 0.052 / 15.8 - 50.5;
            //glTranslatef(x, 16.5, z);
            //glTranslatef(0, 16.5, -50.5);
        } else if (degree < 76) {
            lapse_speed = 0.5;
            //lapse_speed = 1.0;
            double x = 12.8 - (degree - 61) / 3;
            double z = -45.5 - 0.6 * (degree - 61) / 3;
            glTranslatef(x, 16.6, z);
            glRotatef(4 * (degree - 61) + 90, 0.0, 1.0, 0.0);
        } else if (degree < 100) {
            double x = 7.8 - (degree - 76) / 3.0769;
            double z = -48.5 - 0.08333 * (degree - 76);
            glTranslatef(x, 16.6, z);
            //glTranslatef(0, 16.5, -50.5);
            glRotatef(1.25 * (degree - 76) + 150, 0.0, 1.0, 0.0);
            wheel_rot_deg -= 0.75;
        } else if (degree < 130) {
            double x = -(degree - 100) * 0.26;
            double z = -50.5 + 0.1667 * (degree - 100);
            glTranslatef(x, 16.6, z);
            //glTranslatef(-7.8, 16.5, -45.5);
            glRotatef(180 + degree - 100, 0.0, 1.0, 0.0);
        } else if (degree < 145) {
            double x = -(degree - 130) * 0.3 - 7.8;
            double z = -45.5 + 0.46667 * (degree - 130);
            glTranslatef(x, 16.6, z);

            //glTranslatef(-12.3, 16.5, -38.5);
            glRotatef(210, 0.0, 1.0, 0.0);

        } else if (degree < 150) {
            lapse_speed = 0.75;
            double x = -(degree - 145) * 0.04 - 12.3;
            double z = -38.5 + 0.6 * (degree - 145);
            glTranslatef(x, 16.6, z);
            wheel_rot_deg = -20.0;
            //glTranslatef(-12.5, 16.5, -35.5);
            glRotatef(210 - (degree - 145) * 6, 0.0, 1.0, 0.0);
        } else if (degree >= 150) {
            lapse_speed = 1.0;
            wheel_rot_deg = 0.0;
            glTranslatef(-12.5, 16.6, -35.5 + degree - 150);
            glRotatef(180, 0.0, 1.0, 0.0);
        }
        glCallList(car1);
        glPushMatrix();
            glTranslatef(-1.2, 0.0, 2.0);
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glRotatef(degree * 4, 0.0, 1.0, 0.0);
            glCallList(wheel);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-1.2, 0.0, -1.35);
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glRotatef(wheel_rot_deg, 1.0, 0.0, 0.0);
            glRotatef(degree * 4, 0.0, 1.0, 0.0);
            glCallList(wheel);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.2, 0.0, 2.0);
            glRotatef(-90.0, 0.0, 0.0, 1.0);
            glRotatef(-degree * 4, 0.0, 1.0, 0.0);
            glCallList(wheel);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.2, 0.0, -1.35);
            glRotatef(-90.0, 0.0, 0.0, 1.0);
            glRotatef(-wheel_rot_deg, 1.0, 0.0, 0.0);
            glRotatef(-degree * 4, 0.0, 1.0, 0.0);

            glCallList(wheel);
        glPopMatrix();

    } else if (sceneIndex == 4) {
        glRotatef(90, 0.0, 1.0, 0.0);
        glRotatef(45, -1.0, 0.0, 0.0);
        glTranslatef(45 / 3, 45 * 3, 122.5 - degree / 1.5);
        glCallList(mount);
        glCallList(grass);
        glCallList(road);
        glTranslatef(0.0, 1.0, 0);
        glCallList(water);
        glTranslatef(-13.5 - 0.01 * degree, 16.5, 14.5 + degree);
        glRotatef(180, 0.0, 1.0, 0.0);
        glCallList(car1);
    } else if (sceneIndex == 5) {
        glRotatef(90, 0.0, 1.0, 0.0);
        glRotatef(45 - degree, -1.0, 0.0, 0.0);
        glTranslatef(15 - degree / 3, 45 * 3 - degree * 3,85.83 - degree * 1.907);
        glCallList(mount);
        glCallList(grass);
        glCallList(road);
        glTranslatef(0.0, 1.0, 0);
        glCallList(water);
        glTranslatef(-13.5 - 0.01 * 55, 16.5, 69.5 + degree / 1.5);
        glRotatef(180, 0.0, 1.0, 0.0);
        glCallList(car1);
    } else if(sceneIndex == 6) {
        glRotatef(90 - degree, 0.0, 1.0, 0.0);
        glCallList(mount);
        glCallList(grass);
        glCallList(road);
        glTranslatef(0.0, 1.0, 0);
        glCallList(water);
    } else if(sceneIndex == -1) {
        glRotatef(degree, 0.0, 0.0, 1.0);
        glCallList(car1);
        glPushMatrix();
            glTranslatef(-1.2, 0.0, 2.0);
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glRotatef(degree * 4, 0.0, 1.0, 0.0);
            glCallList(wheel);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(-1.2, 0.0, -1.35);
            glRotatef(90.0, 0.0, 0.0, 1.0);
            glRotatef(45.0, 1.0, 0.0, 0.0);
            glRotatef(degree * 4, 0.0, 1.0, 0.0);
            glCallList(wheel);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.2, 0.0, 2.0);
            glRotatef(-90.0, 0.0, 0.0, 1.0);
            glRotatef(-degree * 4, 0.0, 1.0, 0.0);
            glCallList(wheel);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.2, 0.0, -1.35);
            glRotatef(-90.0, 0.0, 0.0, 1.0);
            glRotatef(-45.0, 1.0, 0.0, 0.0);
            glRotatef(-degree * 4, 0.0, 1.0, 0.0);
            glCallList(wheel);
        glPopMatrix();
    }
    //glTranslatef(50.0, -1.0, 80.0-degree);
    //glCallList(car1);

    //glTranslatef(0.0, 0.0, 3.0);
    //glutSolidSphere(1.0, 20, 16);
    
    
    //glutSolidTeapot(0.5);
    
    glPopMatrix();
    glutSwapBuffers();
    glFlush();

}

void callback_timer(int value) {
    //printf("Presenting scene %d.\n", sceneIndex);
    if(degree >= 90.0 && sceneIndex == 0) {
        degree = 0.0;
        sceneIndex++;
    } else if(degree >= 45.0 && sceneIndex == 1) {
        degree = 0.0;
        sceneIndex++;
        lapse_speed = 0.5;
    } else if(degree >= 55.0 && sceneIndex == 2) {
        degree = 0.0;
        sceneIndex++;
        lapse_speed = 1.0;
    } else if(degree >= 200.0 && sceneIndex == 3) {
        degree = 0.0;
        sceneIndex++;
        lapse_speed = 1.0;
    } else if(degree >= 55.0 && sceneIndex == 4) {
        degree = 0.0;
        sceneIndex++;
        lapse_speed = 0.5;
    } else if(degree >= 45.0 && sceneIndex == 5) {
        degree = 0.0;
        sceneIndex++;
        lapse_speed = 2.0;
    } else if(degree >= 360.0 && sceneIndex == 6) {
        degree = 0.0;
        sceneIndex = 1;
        lapse_speed = 1.0;
    } else {
        degree = degree + lapse_speed * lapse_mult;
    }

    display();
    glutTimerFunc(1, callback_timer, 1);
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fov, (GLfloat)w/(GLfloat)h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.1, view_y, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
/*
    gluPerspective(80.0, (GLfloat)w/(GLfloat)h, 1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.1, 250.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
*/
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
    glutTimerFunc(10, callback_timer, 1);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(callback_keyboard);
    glutMouseFunc(callback_mouse);

    glutMainLoop();  // use to stay running

    return 0;
}