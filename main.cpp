//Windows
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//Open GL
#include<glut.h>
#include<glu.h>
#include<gl.h>

//Sound
#pragma comment (lib, "winmm.lib")
#include <mmsystem.h>

//Header FIle
#include "bot.h"
#include "map.h"

//SoundFIlePath
#define SOUND_FILE_GUN_FIRE "sounds/Gun_Fire.wav"
#define SOUND_FILE_GUN_RELOAD "sounds/Gun_reload.wav"
#define SOUND_FILE_GUN_NON "sounds/Gun_nonbullet.wav"

#define PI 3.1415


GLdouble sitdown = 4.0;


GLfloat yawX, pitchY; //카메라 y축, x축 기준  회전각 변화량
GLfloat CurrentX = 0.0f, CurrentY = 0.0f; //현재 마우스 좌표
GLfloat moveX = 0.0f, moveZ = 0.0f; // X,Z축 시점 이동변화량
GLfloat mX = 0.0f, mZ = 0.0f; // X,Z축 총 이동량
GLfloat rotX = 0.0f, rotY = 0.0f; //FpsView func 전달인자, 총 회전각
int bullet = 20;

//함수 원형 선언
void JumpTimer(int value);
void init();


void FpsView(GLfloat yaw, GLfloat pitch) {
	gluLookAt(0, sitdown, 0, 0, sitdown, -1, 0, 1, 0);
	glRotatef(yaw, 0.0, 1.0, 0.0);
	glRotatef(pitch, 1.0, 0.0, 0.0);
}

void MyDisplay() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glPushMatrix();
	FpsView(rotX, rotY);
	glTranslated(mX, 0, mZ);
	Doom_map();
	glPopMatrix();
	glutSwapBuffers();
}

void MyReshape(int NW, int NH) {

	glViewport(0, 0, NW, NH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, NW / NH, 1, 50);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MyKeyBoard(unsigned char KeyPressed, int X, int Y) {
	printf("%d\n", KeyPressed);
	double movespeed = 0.1;
	switch (KeyPressed)
	{
	case 'w':
		moveX = sin((rotX) * (PI / 180));
		moveZ = cos((rotX) * (PI / 180));

		mX += -moveX;
		mZ += moveZ;
		break;
	case 'a':
		moveX = sin((rotX) * (PI / 180) + (PI / 2));
		moveZ = cos((rotX) * (PI / 180) + (PI / 2));

		mX += moveX;
		mZ += -moveZ;
		break;
	case 's':
		moveX = sin((rotX) * (PI / 180));
		moveZ = cos((rotX) * (PI / 180));

		mX += moveX;
		mZ += -moveZ;
		break;
	case 'd':
		moveX = sin((rotX) * (PI / 180) + (PI / 2));
		moveZ = cos((rotX) * (PI / 180) + (PI / 2));

		mX += -moveX;
		mZ += moveZ;
		break;

	case 'r':
		printf("GUN_RELOAD\n");
		PlaySound(TEXT(SOUND_FILE_GUN_RELOAD), NULL, SND_ASYNC);
		bullet = 20;
		break;
	case 32:
		glutTimerFunc(10, JumpTimer, 1);
		break;

	default:
		break;
	}
	glutPostRedisplay();
}

void MySpecial(int key, int X, int Y) {
	printf("%d", key);
	if (key == 114) {
		if (sitdown == 4) {
			sitdown = 2;
		}
		else {
			sitdown = 4;
		}
	}
	glutPostRedisplay();
}

void MyMouseClick(GLint Button, GLint State, GLint X, GLint Y) {
	if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN) {
		if (bullet >= 0) {
			printf("GUN_FIRE\n");
			PlaySound(TEXT(SOUND_FILE_GUN_FIRE), NULL, SND_ASYNC);
			bullet--;
		}
		if (bullet < 0) {
			printf("bullet out\n");
			PlaySound(TEXT(SOUND_FILE_GUN_NON), NULL, SND_ASYNC);
		}
		
	}
}

void MyMouseMove(GLint X, GLint Y) {
	printf("%i, %i\n", X, Y);
}

//마우스로 시점 변환시 사용
void MyMousePassiveMove(GLint X, GLint Y) {
	printf("%i, %i\n", X, Y);
	yawX = X - CurrentX;
	//pitchY = Y-CurrentY;
	rotX += yawX * 0.7;
	//rotY += pitchY * 0.5;
	CurrentX = X;
	//CurrentY = Y;

	glutPostRedisplay();
}

void MyIdle() {

}

void MyTimer(int Value) {

}

void MenuProc(int entryID) {

}

void MenuFunc() {

}

void JumpTimer(int value) {
	printf("점프 실행중, value = %d\n", value);
	if (value == 1 && sitdown <= 5.4) {
		sitdown += 0.1;
		glutTimerFunc(10, JumpTimer, 1);
	}
	else if (value == 1 && sitdown >= 5.4) {
		glutTimerFunc(10, JumpTimer, -1);
	}
	else if (value == -1 && sitdown >= 4) {
		sitdown -= 0.1;
		glutTimerFunc(10, JumpTimer, -1);
	}	
	glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(1600, 900);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Doom");
	glClearColor(0.0, 0.0, 0.0, 1.0);

	init();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);
	glutKeyboardFunc(MyKeyBoard);
	glutSpecialFunc(MySpecial);
	glutMouseFunc(MyMouseClick);
	//glutMotionFunc(MyMouseMove);
	glutPassiveMotionFunc(MyMousePassiveMove);
	//glutIdleFunc(MyIdle);
	//glutTimerFunc(40, MyTimer, 1);
	//MenuFunc();

	glutMainLoop();
	return 0;
}
