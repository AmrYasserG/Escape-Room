#include "TextureBuilder.h"
#include "Model_3DS.h"
#include <math.h>
#include "GLTexture.h"
#include <glut.h>
#include <iostream>

//int WIDTH = 1280;
int WIDTH = 840;
//int HEIGHT = 720;
int HEIGHT = 560;

boolean front = false;
boolean side = false;
boolean top = true;

boolean thirdPersonCamera = true;

GLuint tex;
char title[] = "Escape Room";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;

//Top view


class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector eye(3, 50, 9);
Vector at(3, 6, 3);
Vector up(0, 1, 0);

int cameraZoom = 0;
//decreasing the moveX moves the player to the left and right vice versa
double moveX = -250.0;
//increasing the moveZ moves the player to the back and front vice versa
double moveZ = 1000.0; //350.0

int charRotationAngle = 180;
int fps = 1;

bool criticalTime = false;
bool lInc = true;
bool lleft = true;
double x2 = 0.0f, z2 = 25.0f;
double  xt = 0.0f, yt = 80.0f, zt = 0.0f;

boolean inLevel2 = false;
boolean openedDresser = false;
boolean openedLocker = false;
boolean openedCloset = false;
boolean collectedCoin1 = false;
boolean collectedCoin2 = false;
int coinsCount = 0;

boolean openedTvTable = false;
boolean openedsofa = false;
boolean collectedKey1 = false;
boolean collectedKey2 = false;
int keysCount = 0;

int timeout = 0;

// Model Variables
// Level1 Models
Model_3DS model_bed;
Model_3DS model_dresser1;
Model_3DS model_dresser2;
Model_3DS model_locker1;
Model_3DS model_locker2;
Model_3DS model_closet1;
Model_3DS model_closet2;

// Level2 Models
Model_3DS model_tv;
Model_3DS model_tvTable1;
Model_3DS model_tvTable2;
Model_3DS model_sofa1;
Model_3DS model_sofa2;
Model_3DS model_smallsofa;
Model_3DS model_coffeeTable1;

//Collectables Models
Model_3DS model_coin;
Model_3DS model_key;

//Door Model
Model_3DS model_door1;
Model_3DS model_door2;

//Character Model
Model_3DS model_char1;
Model_3DS model_char2;

//Level1 Textures
GLTexture level1_ground;
GLTexture level1_wall;

//Level2 Textures
GLTexture level2_ground;
GLTexture level2_wall;

//=======================================================================
// Lighting Configuration Function
//=======================================================================

void setupLights() {
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);
	GLfloat lmodel_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	//printf("x %f,y %f,z %f", xt, yt, zt);
	GLfloat l0Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l0Spec[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat l0Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat l0Position[] = { xt, yt, zt, true };
	GLfloat l0Direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, l0Diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, l0Ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, l0Position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, l0Direction);

	glEnable(GL_LIGHT1);
	//GLfloat lmodel_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	//printf("x %f,y %f,z %f", xt, yt, zt);
	//GLfloat l0Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat l0Spec[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//GLfloat l0Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//GLfloat l0Position[] = { x2, yt, z2, true };
	//GLfloat l0Direction[] = { 0.0, -1.0, 0.0 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, l0Diffuse);
	glLightfv(GL_LIGHT1, GL_AMBIENT, l0Ambient);
	glLightfv(GL_LIGHT1, GL_POSITION, l0Position);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 90.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, l0Direction);
}

//=======================================================================
// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================

void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspectRatio, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if (front) {
		switch (fps)
		{
		case 1:
			gluLookAt(moveX / 40, 4.5, moveZ / 40, moveX / 40, 1.5, -30, 0.0, 1.0, 0.0);
			break;
		case 2:
			gluLookAt(moveX / 40, 4.5, (moveZ / 38) + 0.5 * (moveZ / 1000) + 0.2, moveX / 40, 1.5, 50, 0.0, 1.0, 0.0);
			break;
		case 3:
			gluLookAt(moveX / 40, 4.5, moveZ / 40, -15, 1.5, moveZ / 40, 0.0, 1.0, 0.0);
			break;
		case 4:
			gluLookAt(moveX / 40, 4.5, moveZ / 40, 15, 1.5, moveZ / 40, 0.0, 1.0, 0.0);
			break;
		default:
			break;
		}
	}
	else if (side) {
		switch (fps)
		{
		case 1:
			gluLookAt((moveX / 40) - 1, 4.5, (moveZ / 38) + 5, (moveX / 40) + 1, 1.5, -30, 0.0, 1.0, 0.0);
			break;
		case 2:
			gluLookAt((moveX / 40) - 1, 4.5, (moveZ / 38) + 0.5 * (moveZ / 1000) + 0.2 - 5, (moveX / 40) + 1, 1.5, 50, 0.0, 1.0, 0.0);
			break;
		case 3:
			gluLookAt((moveX / 40) + 5, 4.5, (moveZ / 40) - 1, -30, 1.5, (moveZ / 40) + 1, 0.0, 1.0, 0.0);
			break;
		case 4:
			gluLookAt((moveX / 40) - 5, 4.5, (moveZ / 40) - 1, 30, 1.5, (moveZ / 40) + 1, 0.0, 1.0, 0.0);
			break;
		default:
			break;
		}
	}
	else if (top) {
		//Level1 Top view Camera
		gluLookAt(2, 30, 20, 0, 1, 20, 0, 1, 0);
		//Level2 Top view Camera
		//gluLookAt(2, 30, 0, 0, 5, 0, 0, 1, 0);
	}
	InitMaterial();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	//glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	//////////////////////////////////////////////////////////////Level1
	//ground
	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level1_ground.texture[0]);	// Bind the ground texture
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 0, 30);
	glTexCoord2f(15, 0);
	glVertex3f(10, 0, 30);
	glTexCoord2f(15, 15);
	glVertex3f(10, 0, 10.5);
	glTexCoord2f(0, 15);
	glVertex3f(-10, 0, 10.5);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//left side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level1_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 10, 30);
	glTexCoord2f(15, 0);
	glVertex3f(-10, 0, 30);
	glTexCoord2f(15, 15);
	glVertex3f(10, 0, 30);
	glTexCoord2f(0, 15);
	glVertex3f(10, 10, 30);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//right1.0 side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level1_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 10, 10.5);
	glTexCoord2f(15, 0);
	glVertex3f(-10, 0, 10.5);
	glTexCoord2f(15, 15);
	glVertex3f(5.9, 0, 10.5);
	glTexCoord2f(0, 15);
	glVertex3f(5.9, 10, 10.5);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//right2.0 side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level1_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(8.1, 10, 10.5);
	glTexCoord2f(15, 0);
	glVertex3f(8.1, 0, 10.5);
	glTexCoord2f(15, 3);
	glVertex3f(10, 0, 10.5);
	glTexCoord2f(0, 3);
	glVertex3f(10, 10, 10.5);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//right3.0 side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level1_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(5.9, 10, 10.5);
	glTexCoord2f(6, 0);
	glVertex3f(5.9, 5.9, 10.5);
	glTexCoord2f(6, 3);
	glVertex3f(8.1, 5.9, 10.5);
	glTexCoord2f(0, 3);
	glVertex3f(8.1, 10, 10.5);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//back side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level1_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 10, 30);
	glTexCoord2f(15, 0);
	glVertex3f(-10, 0, 30);
	glTexCoord2f(15, 15);
	glVertex3f(-10, 0, 10.5);
	glTexCoord2f(0, 15);
	glVertex3f(-10, 10, 10.5);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//front side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level1_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(10, 10, 30);
	glTexCoord2f(15, 0);
	glVertex3f(10, 0, 30);
	glTexCoord2f(15, 15);
	glVertex3f(10, 0, 10.5);
	glTexCoord2f(0, 15);
	glVertex3f(10, 10, 10.5);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Ceiling
	/*glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level1_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 10, 30);
	glTexCoord2f(15, 0);
	glVertex3f(10, 10, 30);
	glTexCoord2f(15, 15);
	glVertex3f(10, 10, 10.5);
	glTexCoord2f(0, 15);
	glVertex3f(-10, 10, 10.5);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);*/

	//////////////////////////////////////////////////////////////Level2
	//ground
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level2_ground.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 0, -10);
	glTexCoord2f(15, 0);
	glVertex3f(10, 0, -10);
	glTexCoord2f(15, 15);
	glVertex3f(10, 0, 10.5);
	glTexCoord2f(0, 15);
	glVertex3f(-10, 0, 10.5);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//left1.0 side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level2_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 10, 10.4);
	glTexCoord2f(15, 0);
	glVertex3f(-10, 0, 10.4);
	glTexCoord2f(15, 15);
	glVertex3f(5.9, 0, 10.4);
	glTexCoord2f(0, 15);
	glVertex3f(5.9, 10, 10.4);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//left2.0 side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level2_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(8.1, 10, 10.4);
	glTexCoord2f(15, 0);
	glVertex3f(8.1, 0, 10.4);
	glTexCoord2f(15, 3);
	glVertex3f(10, 0, 10.4);
	glTexCoord2f(0, 3);
	glVertex3f(10, 10, 10.4);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//left3.0 side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level2_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(5.9, 10, 10.4);
	glTexCoord2f(6, 0);
	glVertex3f(5.9, 5.9, 10.4);
	glTexCoord2f(6, 3);
	glVertex3f(8.1, 5.9, 10.4);
	glTexCoord2f(0, 3);
	glVertex3f(8.1, 10, 10.4);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//right1.0 side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level2_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 10, -10);
	glTexCoord2f(15, 0);
	glVertex3f(-10, 0, -10);
	glTexCoord2f(15, 15);
	glVertex3f(5.9, 0, -10);
	glTexCoord2f(0, 15);
	glVertex3f(5.9, 10, -10);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//right2.0 side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level2_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(8.1, 10, -10);
	glTexCoord2f(15, 0);
	glVertex3f(8.1, 0, -10);
	glTexCoord2f(15, 3);
	glVertex3f(10, 0, -10);
	glTexCoord2f(0, 3);
	glVertex3f(10, 10, -10);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//right3.0 side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level2_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(5.9, 10, -10);
	glTexCoord2f(6, 0);
	glVertex3f(5.9, 5.9, -10);
	glTexCoord2f(6, 3);
	glVertex3f(8.1, 5.9, -10);
	glTexCoord2f(0, 3);
	glVertex3f(8.1, 10, -10);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//back side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level2_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 10, 10.4);
	glTexCoord2f(15, 0);
	glVertex3f(-10, 0, 10.4);
	glTexCoord2f(15, 15);
	glVertex3f(-10, 0, -10);
	glTexCoord2f(0, 15);
	glVertex3f(-10, 10, -10);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//front side wall
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level2_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(10, 10, 10.4);
	glTexCoord2f(15, 0);
	glVertex3f(10, 0, 10.4);
	glTexCoord2f(15, 15);
	glVertex3f(10, 0, -10);
	glTexCoord2f(0, 15);
	glVertex3f(10, 10, -10);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);

	//Ceiling
	/*glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, level2_wall.texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 0);
	glVertex3f(-10, 10, -10.5);
	glTexCoord2f(15, 0);
	glVertex3f(10, 10, -10.5);
	glTexCoord2f(15, 15);
	glVertex3f(10, 10, 10.5);
	glTexCoord2f(0, 15);
	glVertex3f(-10, 10, 10.5);
	glPopMatrix();
	glEnd();
	glDisable(GL_TEXTURE_2D);*/

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

//=======================================================================
// Camera Function
//=======================================================================
void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1000 / 600, 0.001, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (thirdPersonCamera) {
		gluLookAt(1.9, 6.5, -10, 1, 6.0, 3.0, 0.0, 1.0, 0.0);
		//gluLookAt(0.0, 0.0, 0.1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	}

	else {
		gluLookAt(1.9, 6.5, -10, 1, 6.0, 3.0, 0.0, 1.0, 0.0);
	}
}

void checkforCollisions() {
	//------------------------------------------------------------Level1 collision------------------------------------------
	//collision with level boundries
	if (moveZ >= 1093 && !inLevel2) {
		moveZ -= 1.5;
	}

	if (coinsCount == 2) {
		if (moveZ <= 406 && (moveX <= 242 || moveX >= 272) && !inLevel2) {
			moveZ += 1.5;
		}
	}
	else {
		if (moveZ <= 406 && !inLevel2) {
			moveZ += 1.5;
		}
	}

	if (moveX <= -349 && !inLevel2) {
		moveX += 1.5;
	}
	if (moveX >= 350 && !inLevel2) {
		moveX -= 1.5;
	}

	//collision with Dresser
	if (moveX <= -300 && (moveZ <= 707.5 && moveZ >= 700)) {
		moveZ += 1.5;
	}
	if (moveX <= -300 && (moveZ <= 550 && moveZ >= 544)) {
		moveZ -= 1.5;
	}
	if (moveX <= -295 && (moveZ <= 707.5 && moveZ >= 544)) {
		moveX += 1.5;
	}

	//collision with Locker
	if (moveX >= 305 && (moveZ <= 691 && moveZ >= 680)) {
		moveZ += 1.5;
	}
	if (moveX >= 305 && (moveZ <= 615 && moveZ >= 611.5)) {
		moveZ -= 1.5;
	}
	if (moveX >= 299 && (moveZ <= 691 && moveZ >= 611.5)) {
		moveX -= 1.5;
	}

	//collision with bed
	if ((moveX >= -142 && moveX <= -135) && moveZ >= 901) {
		moveX -= 1.5;
	}
	if ((moveX <= 65 && moveX >= 55) && moveZ >= 901) {
		moveX += 1.5;
	}
	if ((moveX >= -142 && moveX <= 65) && moveZ >= 865) {
		moveZ -= 1.5;
	}

	//collision with closet
	if ((moveX >= 129.5 && moveX <= 135) && moveZ >= 1042) {
		moveX -= 1.5;
	}
	if ((moveX <= 290 && moveX >= 280) && moveZ >= 1042) {
		moveX += 1.5;
	}
	if ((moveX >= 129.5 && moveX <= 290) && moveZ >= 1024) {
		moveZ -= 1.5;
	}

	if (moveZ <= 374) {
		inLevel2 = true;
		coinsCount += 1;
		timeout = 0;
		criticalTime = false;
	}
	else {
		inLevel2 = false;
	}

	//------------------------------------------------------------Level2 collision------------------------------------------
	//collision with level boundries
	if (moveZ >= 365 && inLevel2) {
		moveZ -= 1.5;
	}

	if (keysCount == 2) {
		if (moveZ <= -349 && (moveX <= 242 || moveX >= 272) && inLevel2) {
			moveZ += 1.5;
		}
	}

	else {
		if (moveZ <= -349 && inLevel2) {
			moveZ += 1.5;
		}
	}

	if (moveZ <= -355 || timeout >= 1800) {
		exit(0);
	}

	if (timeout >= 1200) {
		criticalTime = true;
	}


	if (moveX <= -349 && inLevel2) {
		moveX += 1.5;
	}

	if (moveX >= 350 && inLevel2) {
		moveX -= 1.5;
	}

	//collision with tv table
	if (moveX <= -300 && (moveZ <= 146 && moveZ >= 135)) {
		moveZ += 1.5;
	}
	if (moveX <= -300 && (moveZ <= -110 && moveZ >= -119.5)) {
		moveZ -= 1.5;
	}
	if (moveX <= -292 && (moveZ <= 146 && moveZ >= -119.5)) {
		moveX += 1.5;
	}

	//collision with left small sofa
	if ((moveX >= -215 && moveX <= -205) && (moveZ >= 152 && moveZ <= 310)) {
		moveX -= 1.5;
	}

	if ((moveZ <= 325 && moveZ >= 290) && (moveX >= -206.5 && moveX <= -47.5)) {
		moveZ += 1.5;
	}

	if ((moveX <= -38.5 && moveX >= -45) && (moveZ >= 152 && moveZ <= 310)) {
		moveX += 1.5;
	}

	if ((moveZ >= 160 && moveZ <= 180) && (moveX >= -206.5 && moveX <= -47.5)) {
		moveZ -= 1.5;
	}

	//collision with right small sofa
	if ((moveX >= -212.5 && moveX <= -205) && (moveZ >= -317.5 && moveZ <= -145)) {
		moveX -= 1.5;
	}

	if ((moveZ <= -152.5 && moveZ >= -160) && (moveX >= -208 && moveX <= -25)) {
		moveZ += 1.5;
	}

	if ((moveX <= -26.5 && moveX >= -35) && (moveZ >= -313 && moveZ <= -151)) {
		moveX += 1.5;
	}

	if ((moveZ >= -313 && moveZ <= -305) && (moveX >= -214 && moveX <= -25)) {
		moveZ -= 1.5;
	}

	//collsion with sofa
	if ((moveX >= -44 && moveX <= -35) && (moveZ >= -127 && moveZ <= 135.5)) {
		moveX -= 1.5;
	}

	if ((moveZ <= 126.5 && moveZ >= 115) && (moveX >= -41.5 && moveX <= 117.5)) {
		moveZ += 1.5;
	}

	if ((moveX <= 114.5 && moveX >= 100) && (moveZ >= -128.5 && moveZ <= 126.5)) {
		moveX += 1.5;
	}

	if ((moveZ >= -121 && moveZ <= -110) && (moveX >= -41.5 && moveX <= 120.5)) {
		moveZ -= 1.5;
	}

	//collision with coffee table
	if ((moveX >= -179.5 && moveX <= -165) && (moveZ >= -100 && moveZ <= 116)) {
		moveX -= 1.5;
	}

	if ((moveZ <= 114.5 && moveZ >= 105) && (moveX >= -184 && moveX <= -65.5)) {
		moveZ += 1.5;
	}

	if ((moveX <= -65.5 && moveX >= -75) && (moveZ >= -104.5 && moveZ <= 111.5)) {
		moveX += 1.5;
	}

	if ((moveZ >= -100 && moveZ <= -90) && (moveX >= -181 && moveX <= -70)) {
		moveZ -= 1.5;
	}
}

//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	setupLights();
	myInit();
	//setupCamera();

	//Draw Two Levels Ground & Walls
	RenderGround();

	//////////////////////////////////////////////////////////////////////Level1
	// Draw bed
	glPushMatrix();
	glRotated(180, 0, 1, 0);
	glScalef(0.07, 0.07, 0.07);
	glTranslated(-80, 0, -380);
	model_bed.Draw();
	glPopMatrix();

	// Draw Dresser
	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glScalef(0.07, 0.07, 0.07);
	glTranslated(-240, 0, -130);
	if (!openedDresser)
		model_dresser1.Draw();
	else
		model_dresser2.Draw();
	glPopMatrix();

	// Draw coin1
	if (openedDresser) {
		if (!collectedCoin1) {
			glPushMatrix();
			glRotated(90, 0, 0, 1);
			glScalef(0.05, 0.05, 0.05);
			glTranslated(40, 170, 337);
			model_coin.Draw();
			glPopMatrix();
		}
	}

	// Draw Locker
	glPushMatrix();
	glRotated(-90, 0, 1, 0);
	glScalef(0.07, 0.07, 0.07);
	glTranslated(250, 0, -130);
	if (!openedLocker)
		model_locker1.Draw();
	else
		model_locker2.Draw();
	glPopMatrix();

	// Draw coin2
	if (openedLocker) {
		if (!collectedCoin2) {
			glPushMatrix();
			glRotated(90, 0, 0, 1);
			glScalef(0.05, 0.05, 0.05);
			glTranslated(40, -170, 355);
			model_coin.Draw();
			glPopMatrix();
		}
	}

	//Draw Closet
	glPushMatrix();
	glRotated(180, 0, 1, 0);
	glScalef(0.03, 0.03, 0.03);
	glTranslated(-130, 0, -999);
	if (!openedCloset)
		model_closet1.Draw();
	else
		model_closet2.Draw();
	glPopMatrix();

	// Draw door 
	glPushMatrix();
	glScalef(0.07, 0.07, 0.07);
	glTranslated(100, 0, 146);
	if (coinsCount != 2)
		model_door1.Draw();
	else
		model_door2.Draw();
	glPopMatrix();

	/////////////////////////////////////////////////////////////////////Level2
	// Draw tv Model
	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);
	glTranslated(-3, 19, -90);
	model_tv.Draw();
	glPopMatrix();

	// Draw tvTable Model
	glPushMatrix();
	glRotated(90, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);
	glTranslated(-3, 0, -100);
	if (!openedTvTable)
		model_tvTable1.Draw();
	else
		model_tvTable2.Draw();
	glPopMatrix();

	//Draw Key1
	if (openedTvTable) {
		if (!collectedKey1) {
			glPushMatrix();
			glRotated(90, 1, 0, 0);
			glScalef(0.1, 0.1, 0.1);
			/////////////x/////z///y
			glTranslated(-85, -23, -10);
			model_key.Draw();
			glPopMatrix();
		}
	}

	// Draw sofa Model
	glPushMatrix();
	glRotated(180, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);
	glTranslated(-10, 0, -1);
	if (!openedsofa)
		model_sofa1.Draw();
	else
		model_sofa2.Draw();
	glPopMatrix();

	//Draw Key2
	if (openedsofa) {
		if (!collectedKey2) {
			glPushMatrix();
			glRotated(90, 1, 0, 0);
			glScalef(0.1, 0.1, 0.1);
			/////////////x/////z///y
			glTranslated(10, 17, -10);
			model_key.Draw();
			glPopMatrix();
		}
	}

	// Draw small sofa Model
	glPushMatrix();
	glScalef(0.1, 0.1, 0.1);
	glTranslated(-33, 0, -63);
	model_smallsofa.Draw();
	glPopMatrix();

	// Draw small sofa Model
	glPushMatrix();
	glRotated(180, 0, 1, 0);
	glScalef(0.1, 0.1, 0.1);
	glTranslated(35, 0, -65);
	model_smallsofa.Draw();
	glPopMatrix();

	// Draw coffee table Model
	glPushMatrix();
	glScalef(0.25, 0.25, 0.25);
	glTranslated(-14, 0, 0.5);
	model_coffeeTable1.Draw();
	glPopMatrix();

	// Draw door 
	glPushMatrix();
	glScalef(0.07, 0.07, 0.07);
	glTranslated(100, 0, -146);
	if (keysCount != 2)
		model_door1.Draw();
	else
		model_door2.Draw();
	glPopMatrix();

	//Draw character
	glPushMatrix();
	glScalef(0.027, 0.027, 0.027);
	glTranslated(moveX, 10, moveZ);
	glRotated(charRotationAngle, 0, 1, 0);
	model_char1.Draw();
	glPopMatrix();

	checkforCollisions();

	//sky box
	glPushMatrix();
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	glTranslated(50, 0, 0);
	glRotated(90, 1, 0, 1);
	glBindTexture(GL_TEXTURE_2D, tex);
	gluQuadricTexture(qobj, true);
	gluQuadricNormals(qobj, GL_SMOOTH);
	gluSphere(qobj, 100, 100, 100);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
void myKeyboard(unsigned char button, int x, int y)
{
	switch (button)
	{
	case 'a':
		if (moveX <= -290 && (moveZ <= 707.5 && moveZ >= 544) && !openedDresser) {
			openedDresser = true;
			moveX += 13.5;
		}
		if (moveX >= 295 && (moveZ <= 691 && moveZ >= 611.5) && !openedLocker) {
			openedLocker = true;
			moveX -= 16.5;
		}
		if ((moveX >= 129.5 && moveX <= 290) && moveZ >= 1020 && !openedCloset) {
			openedCloset = true;
		}
		if (moveX <= -287 && (moveZ <= 146 && moveZ >= -119.5)) {
			openedTvTable = true;
			moveX += 21;
		}
		if ((moveX >= -48 && moveX <= -35) && (moveZ >= -127 && moveZ <= 135.5)) {
			openedsofa = true;
		}
		break;
	case 'c':
		if (moveX <= -276.5 && (moveZ <= 707.5 && moveZ >= 544) && openedDresser && !collectedCoin1) {
			collectedCoin1 = true;
			coinsCount += 1;
		}
		if (moveX >= 276.5 && (moveZ <= 691 && moveZ >= 611.5) && openedLocker && !collectedCoin2) {
			collectedCoin2 = true;
			coinsCount += 1;
		}
		if (moveX <= -266 && (moveZ <= 146 && moveZ >= -119.5) && openedTvTable && !collectedKey1) {
			collectedKey1 = true;
			keysCount += 1;
		}
		if ((moveX >= -48 && moveX <= -35) && (moveZ >= -127 && moveZ <= 135.5) && openedsofa && !collectedKey2) {
			collectedKey2 = true;
			keysCount += 1;
		}
		break;
	case 'f':
		front = true;
		side = false;
		top = false;
		break;
	case 's':
		front = false;
		side = true;
		top = false;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void Special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		charRotationAngle = 180;
		moveZ -= 1.5;
		//gluLookAt(moveX, 6.5, moveZ, moveX, 6.5, moveZ-5, 0.0, 1.0, 0.0);
		fps = 1;
		break;
	case GLUT_KEY_DOWN:
		charRotationAngle = 0;
		moveZ += 1.5;
		//gluLookAt(moveX, 6.5, moveZ, moveX, 6.5, moveZ +5, 0.0, 1.0, 0.0);
		fps = 2;
		break;
	case GLUT_KEY_LEFT:
		charRotationAngle = -90;
		moveX -= 1.5;
		//gluLookAt(moveX, 6.5, moveZ, moveX - 5, 6.5, moveZ , 0.0, 1.0, 0.0);
		fps = 3;
		break;
	case GLUT_KEY_RIGHT:
		charRotationAngle = 90;
		moveX += 1.5;
		//gluLookAt(moveX, 6.5, moveZ, moveX + 5, 6.5, moveZ, 0.0, 1.0, 0.0);
		fps = 4;
		break;
	}
	glutPostRedisplay();
}



//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		eye.x += -0.1;
		eye.z += -0.1;
	}
	else
	{
		eye.x += 0.1;
		eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
//=======================================================================
void myMouse(int button, int state, int x, int y)
{
	y = HEIGHT - y;

	if (state == GLUT_DOWN)
	{
		cameraZoom = y;
	}
}

void Timer(int value) {
	// set the ball's Y coordinate to a random number between 10 and 780 (since the window's height is 800)

	if (criticalTime) {
		if (yt > 80)
			lInc = false;
		if (yt < 1)
			lInc = true;
		if (lInc)
			yt++;
		else
			yt--;

		if (xt > 10)
			lleft = false;
		if (xt < -10)
			lleft = true;
		if (lleft)
			xt++;
		else
			xt--;
	}

	timeout += 1;

	// ask OpenGL to recall the display function to reflect the changes on the window
	glutPostRedisplay();

	// recall the Timer function after 20 seconds (20,000 milliseconds)
	glutTimerFunc(100, Timer, 0);
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, at.x, at.y, at.z, up.x, up.y, up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Level1 Models
	model_bed.Load("Models/Level1/Bed/bed2.3ds");
	model_locker1.Load("Models/Level1/Locker/locker1.3ds");
	model_locker2.Load("Models/Level1/Locker/locker2.3ds");
	model_dresser1.Load("Models/Level1/Dresser/dresser1.3ds");
	model_dresser2.Load("Models/Level1/Dresser/dresser2.3ds");
	model_closet1.Load("Models/Level1/Closet/closet1.3ds");
	model_closet2.Load("Models/Level1/Closet/closet2.3ds");

	//Level2 Models
	model_tv.Load("Models/Level2/Tv Table/tv.3ds");
	model_tvTable1.Load("Models/Level2/Tv Table/tv table1.3ds");
	model_tvTable2.Load("Models/Level2/Tv Table/tv table2.3ds");
	model_sofa1.Load("Models/Level2/Sofa/sofa5.3ds");
	model_sofa2.Load("Models/Level2/Sofa/sofa6.3ds");
	model_smallsofa.Load("Models/Level2/Small Sofa/smallsofa2.3ds");;
	model_coffeeTable1.Load("Models/Level2/Coffee Table/coffee table.3ds");

	//Collectables Models
	model_key.Load("Models/Level2/Collectables/Keys/key.3ds");
	model_coin.Load("Models/Level1/Collectables/Coins/coin.3ds");

	//Door Model
	model_door1.Load("Models/Door/door 1.3ds");
	model_door2.Load("Models/Door/door 2.3ds");

	//Character Model
	model_char1.Load("Models/Character/char1.3ds");
	model_char2.Load("Models/Character/char2.3ds");

	//Level1 Textures
	level1_ground.Load("Textures/bedroom ground.bmp");
	level1_wall.Load("Textures/bedroom wall.bmp");

	//Level2 Textures
	level2_ground.Load("Textures/living room ground.bmp");
	level2_wall.Load("Textures/living room wall.bmp");
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(330, 120);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);

	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(Special);

	glutTimerFunc(0, Timer, 0);
	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT1);
	glShadeModel(GL_SMOOTH);

	glutMainLoop();
}