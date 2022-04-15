#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <gl/glut.h>


using namespace std;

float _angle = 0.0f, _angle2 = 0.0f;
const float DEG2RAD = 3.14159 / 180;
float radius = 0.25;
float b1_color = 0.0, b2_color = 0.0;
int drc = 0, flage = 1, val;
int numOfBall = 1;
int speed = 100;


/*  Create checkerboard texture  */
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][3];

static GLuint texName;

void makeCheckImage(void)
{
	int i, j, c;

	for (i = 0; i < checkImageHeight; i++) {
		for (j = 0; j < checkImageWidth; j++) {
			c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
		}
	}
}
void backgroundTexture_InLightSteelBlue(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(176/255.0f, 196 /255.0f, 222 /255.0f, 0.0);
	glShadeModel(GL_FLAT);

	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);;
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 1, GL_RGB, GL_UNSIGNED_BYTE, checkImage);
}

void MoveBall2() {

	if (_angle2 == 0 && drc == 1) {
		b2_color = 0.0;
		drc = 0;
		val = 0;
		flage = 1;

	}
	else
	{
		b2_color = 1.0;
		if (_angle2 < 45 && drc == 0)  _angle2 += 5;
		else if (_angle2 <= 45 && _angle2 > 0) { _angle2 -= 5; drc = 1; }

	}

}

void moveBall1() {


	if (_angle == 0 && drc == 1) {
		b1_color = 0.0;
		drc = 0;
		val = 1;
		flage = 2;
	}
	else
	{
		b1_color = 1.0;
		if (_angle > -45 && drc == 0)  _angle -= 5;
		else if (_angle <= -45 || _angle < 0) { _angle += 5; drc = 1; }

	}
}

void update(int value) {

	if (flage == 1) {
		moveBall1();
	}
	else if (flage == 2)
	{
		MoveBall2();
	}

	//display has changed
	glutPostRedisplay();

	// call update again in (speed) 50 milliseconds and this (speed) 50 ms
	//you can change it when you press space key
	glutTimerFunc(speed, update, 0);
}

//Called when the window is resized
void Resize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

//Called when a key is pressed
void handleKeypress(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27: //Escape key
		exit(0);
		break;
	case 32: //space key
		update(1);
		break;
	}
}

void DrowLeftPendulum(void)
{
	//draw first pendulums

	glTranslatef(-0.03, 2.5f, 0.0f);
	glRotatef(_angle, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, -2.5f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);

	//line
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.25f, 0.0f);
	glVertex3f(0.0f, 2.5f, 0.0f);
	glEnd();

	//circle
	glColor3f(b1_color, 0.0f, 1.0 - b1_color);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
	}
	glEnd();

}
void DrowRightPendulum(void)
{
	int count = 1;

	glPushMatrix();

	glTranslatef(0.47 * count, 2.5f, 0.0f);
	glRotatef(_angle2, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, -2.5f, 0.0f);
	glColor3f(0.5f, 0.5f, 0.5f);

	//line
	glBegin(GL_LINES);
	glVertex3f(0.0f, 0.25f, 0.0f);
	glVertex3f(0.0f, 2.5f, 0.0f);
	glEnd();

	//circle
	glColor3f(b2_color, 0.0f, 1.0 - b2_color);
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++)
	{
		float degInRad = i * DEG2RAD;
		glVertex2f(cos(degInRad) * radius, sin(degInRad) * radius);
	}
	glEnd();

	glPopMatrix();

	count++;

}
void DrowPendulums(void)
{
	glTranslatef(0.0f, -1, 0.0f);

	glPushMatrix();
	DrowLeftPendulum();
	glPopMatrix();

	glPushMatrix();
	DrowRightPendulum();
	glPopMatrix();
}
void DrowHolder(void)
{

	glTranslatef(0.0f, -2.5f, 0.0f);

	glColor3f(0.5f, 0.3f, 0.6f);

	glBegin(GL_POLYGON);
	glVertex3f(-2.0f, 1.5f, 0.0f);
	glVertex3f(4.0f, 1.5f, 0.0f);
	glVertex3f(4.0f, 1.8f, 0.0f);
	glVertex3f(-2.0f, 1.8f, 0.0f);
	glEnd();

}
void DrowColumns(void)
{
	glColor3f(0.5f, 0.3f, 0.6f);

	glTranslatef(1.0f, 0.0f, 0.0f);

	glBegin(GL_QUADS);
	//left column
	glVertex3f(-2.8f, -3.0f, 0.0f);
	glVertex3f(-2.5f, -3.0f, 0.0f);
	glVertex3f(-2.5f, 1.8f, 0.0f);
	glVertex3f(-2.8f, 1.8f, 0.0f);

	//right column
	glVertex3f(2.8, -3.0f, 0.0f);
	glVertex3f(2.5, -3.0f, 0.0f);
	glVertex3f(2.5, 1.8f, 0.0f);
	glVertex3f(2.8, 1.8f, 0.0f);
	glEnd();

}
void DrowBase(void)
{
	glColor3f(0, 0, 0);

	glBegin(GL_QUADS);
	glVertex3f(-4.0f, -3.0f, 0.0f);
	glVertex3f(4.0f, -3.0f, 0.0f);
	glVertex3f(4.0f, -2.5f, 0.0f);
	glVertex3f(-4.0f, -2.5f, 0.0f);
	glEnd();

}

void Drow() {

	backgroundTexture_InLightSteelBlue();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexCoord2f(0.0, 0.0);
	glTexCoord2f(0.0, 1.0); ;
	glTexCoord2f(1.0, 1.0);
	glTexCoord2f(1.0, 0.0);


	glFlush();
	glDisable(GL_TEXTURE_2D);


	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective

	glTranslatef(0.0f, 0.0f, -10.0f); //Move forward 10 units 

	glPushMatrix();
	glScalef(.9f, .8f, 0.8f); // the speedup using space didnot work without this stetement

	DrowBase();
	glTranslatef(-1, 3, 0.0f);
	DrowHolder();
	DrowColumns();
	DrowPendulums();

	glPopMatrix();

	glutSwapBuffers();
}


int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1000, 800);

	//Create the window
	glutCreateWindow("Pendulums");

	

	//Set handler functions
	glutDisplayFunc(Drow);


	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(Resize);

	glutMainLoop();
	return 0;
}
