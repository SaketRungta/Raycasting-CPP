/* Created by Saket Rungta using openGL GLUT */

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535
#define P2 PI / 2
#define P3 3 * PI / 2

float playerXPos, playerYPos, playerDeltaX, playerDeltaY, playerAngle;

int mapX = 8, mapY = 8, mapS = 64;
int map[] =
{
	1,1,1,1,1,1,1,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,1,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1
};

void drawPlayer()
{
	glColor3f(1, 1, 0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(playerXPos, playerYPos);
	glEnd();
}

void drawMap2D()
{
	int offsetX, offsetY;
	for (int y = 0; y < mapY; y++)
	{
		for (int x = 0; x < mapX; x++)
		{
			if (map[y * mapX + x] == 1)
			{
				glColor3f(1, 1, 1);
			}
			else
			{
				glColor3f(0, 0, 0);
			}

			offsetX = x * mapS;
			offsetY = y * mapS;

			glBegin(GL_QUADS);

			glVertex2i(offsetX + 1, offsetY + 1);
			glVertex2i(offsetX + 1, offsetY + mapS - 1);
			glVertex2i(offsetX + mapS - 1, offsetY + mapS - 1);
			glVertex2i(offsetX + mapS - 1, offsetY + 1);

			glEnd();
		}
	}
}

float distanceBetweenPlayerAndRayEndPoint(float ax, float ay, float bx, float by, float angle)
{
	return (sqrt((bx - ax) * (bx - ax) + (by - ay) * (by - ay)));
}

void drawRays3D()
{
	float rayAngle = playerAngle - 0.0174533f * 360;

	if (rayAngle < 0)
	{
		rayAngle += 2 * PI;
	}
	if (rayAngle > 2 * PI)
	{
		rayAngle -= 2 * PI;
	}

	for (int raysCount = 0; raysCount < 720; raysCount++)
	{
		float distH = 1000000, hx = playerXPos, hy = playerYPos;
		{
			int depthOfField = 0;

			float aTan = -1 / tan(rayAngle);

			float rayEndXPos, rayEndYPos, rayOffsetX, rayOffsetY;

			if (rayAngle > PI)
			{
				rayEndYPos = (((int)playerYPos >> 6) << 6) - 0.0001;
				rayEndXPos = (playerYPos - rayEndYPos) * aTan + playerXPos;
				rayOffsetY = -64;
				rayOffsetX = -rayOffsetY * aTan;
			}
			if (rayAngle < PI)
			{
				rayEndYPos = (((int)playerYPos >> 6) << 6) + 64;
				rayEndXPos = (playerYPos - rayEndYPos) * aTan + playerXPos;
				rayOffsetY = 64;
				rayOffsetX = -rayOffsetY * aTan;
			}
			if (rayAngle == 0 || rayAngle == PI)
			{
				rayEndXPos = playerXPos;
				rayEndYPos = playerYPos;
				depthOfField = 8;
			}
			while (depthOfField < 8)
			{
				int mx = (int)(rayEndXPos) >> 6;
				int my = (int)(rayEndYPos) >> 6;
				int mp = my * mapX + mx;
				if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
				{
					hx = rayEndXPos;
					hy = rayEndYPos;
					distH = distanceBetweenPlayerAndRayEndPoint(playerXPos, playerYPos, rayEndXPos, rayEndYPos, rayAngle);

					depthOfField = 8;
				}
				else
				{
					rayEndXPos += rayOffsetX;
					rayEndYPos += rayOffsetY;
					depthOfField += 1;
				}
			}
		}

		float distV = 1000000, vx = playerXPos, vy = playerYPos;
		{
			int depthOfField = 0;

			float nTan = -tan(rayAngle);

			float rayEndXPos = 0, rayEndYPos = 0, rayOffsetX = 0, rayOffsetY = 0;

			if (rayAngle > P2 && rayAngle < P3)
			{
				rayEndXPos = (((int)playerXPos >> 6) << 6) - 0.0001;
				rayEndYPos = (playerXPos - rayEndXPos) * nTan + playerYPos;
				rayOffsetX = -64;
				rayOffsetY = -rayOffsetX * nTan;
			}
			if (rayAngle < P2 || rayAngle > P3)
			{
				rayEndXPos = (((int)playerXPos >> 6) << 6) + 64;
				rayEndYPos = (playerXPos - rayEndXPos) * nTan + playerYPos;
				rayOffsetX = 64;
				rayOffsetY = -rayOffsetX * nTan;
			}
			if (rayAngle == 0 || rayAngle == PI)
			{
				rayEndXPos = playerXPos;
				rayEndYPos = playerYPos;
				depthOfField = 8;
			}
			while (depthOfField < 8)
			{
				int mx = (int)(rayEndXPos) >> 6;
				int my = (int)(rayEndYPos) >> 6;
				int mp = my * mapX + mx;
				if (mp > 0 && mp < mapX * mapY && map[mp] == 1)
				{
					vx = rayEndXPos;
					vy = rayEndYPos;
					distV = distanceBetweenPlayerAndRayEndPoint(playerXPos, playerYPos, rayEndXPos, rayEndYPos, rayAngle);

					depthOfField = 8;
				}
				else
				{
					rayEndXPos += rayOffsetX;
					rayEndYPos += rayOffsetY;
					depthOfField += 1;
				}
			}
		}

		if (distH < distV)
		{
			vx = hx;
			vy = hy;
		}

		glColor3f(1, 0, 0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(playerXPos, playerYPos);
		glVertex2i(vx, vy);
		glEnd();

		rayAngle += 0.0174533f / 2.f;
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawMap2D();
	drawRays3D();
	drawPlayer();
	glutSwapBuffers();
}

void buttons(unsigned char key, int x, int y)
{
	if (key == 'a')
	{
		playerXPos -= playerDeltaX;
	}

	if (key == 'd')
	{
		playerXPos += playerDeltaX;
	}

	if (key == 'w')
	{
		playerYPos -= 5;
	}

	if (key == 's')
	{
		playerYPos += 5;
	}

	glutPostRedisplay();
}

void init()
{
	glClearColor(0.3, 0.3, 0.3, 0);
	gluOrtho2D(0, 512, 512, 0);

	playerXPos = 300, playerYPos = 300;

	playerDeltaX = cos(playerAngle) * 5;
	playerDeltaY = sin(playerAngle) * 5;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Raycaster");

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(buttons);

	glutMainLoop();

	return 0;
}