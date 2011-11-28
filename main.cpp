#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

#include "gl_png.h"
#include "block.h"

#define SCREEN_WIDTH 640.0
#define SCREEN_HEIGHT 480.0
#define M_PI 3.1415926535

float angle = 0.0;
float ORG[3] = {0,0,0};

float XP[3] = {1,0,0}, XN[3] = {-1,0,0},
      YP[3] = {0,1,0}, YN[3] = {0,-1,0},
      ZP[3] = {0,0,1}, ZN[3] = {0,0,-1};
      
unsigned int tex;     

bool mousein = false;
float camX = 0.0, camY = 0.0, camZ = 5.0;
float camYaw = 0.0;
float camPitch = 0.0;

GLuint texId; 

unsigned int LoadTexture(const char *filename)
{
	SDL_Surface* img = SDL_LoadBMP(filename);
	unsigned int id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->w, img->h, 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, img->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	SDL_FreeSurface(img);
	return id;
}	

void init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, SCREEN_WIDTH/SCREEN_HEIGHT, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	/*
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);	
	float dif[] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	float amb[] = {0.2, 0.2, 0.2, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	*/
	//glEnable(GL_TEXTURE_2D);
	texId = loadPNGTexture("./diamond.png");
}

void DrawCube(float size)
{
	glPushMatrix();
	glRotatef(angle, 1.0, 1.0, 1.0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	glBegin(GL_QUADS);
		// front face
		//glColor3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(size/2, size/2, size/2);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-size/2, size/2, size/2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-size/2, -size/2, size/2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(size/2, -size/2, size/2);
		// left face
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-size/2, size/2, size/2);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-size/2, size/2, -size/2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-size/2, -size/2, -size/2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(-size/2, -size/2, size/2);
		// back face
		glTexCoord2f(0.0, 1.0);
		glVertex3f(size/2, size/2, -size/2);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-size/2, size/2, -size/2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-size/2, -size/2, -size/2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(size/2, -size/2, -size/2);
		// right face
		glTexCoord2f(0.0, 1.0);
		glVertex3f(size/2, size/2, -size/2);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(size/2, size/2, size/2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(size/2, -size/2, size/2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(size/2, -size/2, -size/2);
		// top face	
		glTexCoord2f(0.0, 1.0);
		glVertex3f(size/2, size/2, size/2);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-size/2, size/2, size/2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-size/2, size/2, -size/2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(size/2, size/2, -size/2);		
		// bottom face	
		glTexCoord2f(0.0, 1.0);
		glVertex3f(size/2, -size/2, size/2);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-size/2, -size/2, size/2);
		glTexCoord2f(1.0, 0.0);
		glVertex3f(-size/2, -size/2, -size/2);
		glTexCoord2f(1.0, 1.0);
		glVertex3f(size/2, -size/2, -size/2);		
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();	
}

void DrawGrid(float step)
{
	glBegin(GL_LINES);
	
		glColor3f(0.0, 1.0, 0.0);
		for (GLfloat y = 0.0; y < 10; y += step) {
			glVertex3f(0.0, y, 0.0);
			glVertex3f(10.0, y, 0.0);
			for (GLfloat x = 0.0; x < 10; x += step) {
				glVertex3f(x, y, 0.0);
				glVertex3f(x, y + 10.0, 0.0);
			}
		}
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 10.0);
	
	glEnd();
}

void LockCamera()
{
	if (camPitch > 90.0)
		camPitch = 90.0;
	if (camPitch < -90.0)
		camPitch = -90.0;
	if (camYaw < 0.0) 
		camYaw += 360.0;
	if (camYaw > 360.0)
		camYaw -= 360.0;
}

void MoveCamera(float dist, float dir)
{
	float rad = (camYaw + dir) * M_PI / 180.0;
	camX -= sin(rad) * dist;
	camZ -= cos(rad) * dist;
}

void MoveCameraUp(float dist, float dir)
{
	float rad = (camPitch + dir) * M_PI / 180.0;
	camY += sin(rad) * dist;
	
}

void Control (float movevel, float mousevel, bool mi)
{
	int midX = SCREEN_WIDTH/2;
	int midY = SCREEN_HEIGHT/2;
	
	int tmpx, tmpy;
	
	SDL_GetMouseState(&tmpx, &tmpy);
	camYaw += mousevel * (midX - tmpx);
	camPitch += mousevel * (midY - tmpy);
	LockCamera();
	SDL_WarpMouse(midX, midY);
	
	Uint8 * state = SDL_GetKeyState(NULL);
	if(state[SDLK_w]) {
		MoveCamera(movevel, 0.0);
		MoveCameraUp(movevel, 0.0);
	} else if (state[SDLK_s]) {
		MoveCamera(movevel, 180.0);
		MoveCameraUp(movevel, 180.0);
	} else if (state[SDLK_a]) {
		MoveCamera(movevel, 90.0);
	} else if (state[SDLK_d]) {
		MoveCamera(movevel, 270.0);
	}
	glRotatef(-camPitch, 1.0, 0.0, 0.0);
	glRotatef(-camYaw, 0.0, 1.0, 0.0);
}

void UpdateCamera()
{
	glTranslatef(-camX, -camY, -camZ);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	Control(0.2, 0.2, mousein);
	UpdateCamera();
	glTranslatef(0.0, 0.0, -5.0);
	glColor3f(1.0, 1.0, 1.0);
	DrawCube(1.0);
	DrawGrid(1.0/16.0);
	//float pos[] = {-2.0, 2.0, -3.0, 1.0};
	//glLightfv(GL_LIGHT0, GL_POSITION, pos);	
	//glTranslatef(0.0, 0.0, -5.0);
}

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Surface * screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE | SDL_OPENGL);
	
	bool running = true;
	Uint32 start;
	SDL_Event event;
	
	init();
	
	while(running) {
		start = SDL_GetTicks();
		while (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_MOUSEBUTTONDOWN:
					mousein = true;
					SDL_ShowCursor(SDL_DISABLE);
					break;
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_p) {
						mousein = false;
						SDL_ShowCursor(SDL_ENABLE);
						break;
					}
					if(event.key.keysym.sym == SDLK_ESCAPE) {
						running = false;
						break;
					}
			}
		}
		display();
		angle += 0.5;
		if (angle > 360.0)
			angle -= 360;
		SDL_GL_SwapBuffers();	
		if (1000/30 > (SDL_GetTicks() - start))
			SDL_Delay(1000/30 - (SDL_GetTicks() - start));	
	}
	
	SDL_Quit();
	
	return 0;
}
