#include "draw.h"
#include "stdafx.h"
#include "math.h"
#include "cstdio"

#define RGB32(r, g, b) static_cast<uint32_t>((((static_cast<uint32_t>(b) << 8) | g) << 8) | r)

float angle = M_PI / 8;
int  dxLine = 0, dyLine = 0, dxWindow = 0, dyWindow = 0, area = 0, positionX = 0, positionY = 0;


void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	pixels[(y * surface->w) + x] = pixel;
}

Uint32 get_pixel32(SDL_Surface *surface, int x, int y)
{
	Uint32 *pixels = (Uint32 *)surface->pixels;
	return pixels[(y * surface->w) + x];
}

void drawLineBresenhem(SDL_Surface *s, int x1, int x2, int y1, int y2, int visible, int isWindow) {
	bool flag;
	int dx, dy, positionXX, positionYY, buffer, t, currentX = x1, currentY = y1;
	if ((x2 - x1)>0) positionXX = 1;
	if ((x2 - x1)<0) positionXX = -1;
	if ((x2 - x1) == 0) positionXX = 0;
	if ((y2 - y1)>0) positionYY = 1;
	if ((y2 - y1)<0) positionYY = -1;
	if ((y2 - y1) == 0) positionYY = 0;
	dx = abs(x2 - x1);
	dy = abs(y2 - y1);
	if (dy > dx) {
		buffer = dx;
		dx = dy;
		dy = buffer;
		flag = true;
	}
	else
		flag = false;
	t = 2 * dy - dx;
	for (int w = 1;w<dx;w++) 
	{
		if (visible == 1) 
		{
			if (isWindow == 1) //Drawing window.
			{
				if (currentY>0 && currentY<480 && currentX>0 && currentX<640)
					put_pixel32(s, currentX, currentY, RGB(250, 0, 200));
			}
			else  //Drawing visible lines of figure.
			{
				if (currentY>0 && currentY<480 && currentX>0 && currentX<640)
					put_pixel32(s, currentX, currentY, RGB(30, 180, 90));
			}
		}
		else  //Drawing invisible line of figure.
		{
			if (w % 10 != 0 && (w - 1) % 10 != 0 && (w + 1) % 10 != 0)
				if (currentY>0 && currentY<480 && currentX>0 && currentX<640)
					put_pixel32(s, currentX, currentY, RGB(120, 50, 250));
		}
		while (t >= 0)
		{
			if (flag)
				currentX += positionXX;
			else
				currentY += positionYY;
			t -= 2 * dx;
		}
		if (flag)
			currentY += positionYY;
		else
			currentX += positionXX;
		t += 2 * dy;
	}
}

void draw(SDL_Surface *s, int windowX, int windowY)
{
	int screenX, screenY;
	for (screenX = 0;screenX<640;screenX++)
	{
		for (screenY = 0;screenY<480;screenY++)
			put_pixel32(s, screenX, screenY, 0x00000000);
	}
	int flag, nextIteration, nextIteration2, centerOfWindowX = 320 + dxWindow, centerofWindowY = 240 + dyWindow, x1, y1;
	float mu, delta, mu2;
	int windowX1, windowX2, windowY1, windowY2;
	int XsOfWindow[4], YsOfWindow[4];
	int baseXsOfFigure[4], baseYsOfFigure[4], codesOfMovements[4];
	//Base coords of figure,not including rotation.
	baseXsOfFigure[0] = 100 + dxLine; baseXsOfFigure[1] = 200 + dxLine; baseXsOfFigure[2] = 200 + dxLine; baseXsOfFigure[3] = 100 + dxLine; 
	baseYsOfFigure[0] = 100 + dyLine; baseYsOfFigure[1] = 100 + dyLine; baseYsOfFigure[2] = 200 + dyLine; baseYsOfFigure[3] = 200 + dyLine;

	for (int i = 0;i<4;i++)   //Calculating base coords for figure, including rotation.
	{
		x1 = 150 + (baseXsOfFigure[i] - 150)*cos(angle) - (baseYsOfFigure[i] - 150)*sin(angle);
		y1 = 150 + (baseXsOfFigure[i] - 150)*sin(angle) + (baseYsOfFigure[i] - 150)*cos(angle); 
		baseXsOfFigure[i] = x1;
		baseYsOfFigure[i] = y1;
	}
	//Entering coords of window.
	XsOfWindow[0] = centerOfWindowX - windowX / 2; XsOfWindow[1] = centerOfWindowX + windowX / 2;
	XsOfWindow[2] = centerOfWindowX + windowX / 2; XsOfWindow[3] = centerOfWindowX - windowX / 2;
	YsOfWindow[0] = centerofWindowY - windowY / 2; YsOfWindow[1] = centerofWindowY - windowY / 2;
	YsOfWindow[2] = centerofWindowY + windowY / 2; YsOfWindow[3] = centerofWindowY + windowY / 2;

	for (int i = 0;i<4;i++)               //Drawing window.
	{
		if (i != 3)
			drawLineBresenhem(s, XsOfWindow[i], XsOfWindow[i + 1], YsOfWindow[i], YsOfWindow[i + 1], 1, 1);
		else
			drawLineBresenhem(s, XsOfWindow[i], XsOfWindow[0], YsOfWindow[i], YsOfWindow[0], 1, 1);
	}

	if (positionX >= XsOfWindow[0] && positionX <= XsOfWindow[1] && positionY >= YsOfWindow[0] && positionY <= YsOfWindow[2])
		area = 1;
	else                          // Calculating position to window.
		area = 0;

	for (int w = 0;w<4;w++)
		codesOfMovements[w] = 0;

	for (int w = 0;w<4;w++)   //Analizing position of figure's vertexes to window.
	{
		if (baseXsOfFigure[w] <= XsOfWindow[0])
			codesOfMovements[w] += 1;  //Left.
		if (baseXsOfFigure[w]>XsOfWindow[1])   
			codesOfMovements[w] += 10;  //Right.
		if (baseYsOfFigure[w] <= YsOfWindow[0])
			codesOfMovements[w] += 1000; //Down.
		if (baseYsOfFigure[w]>YsOfWindow[2])
			codesOfMovements[w] += 100; //Up.
	}
	for (int w = 0;w < 4; w++)                 //Choising destination from window to figure.
	{
		if (codesOfMovements[w] == 10) codesOfMovements[w] = 2; //Right.
		if (codesOfMovements[w] == 100) codesOfMovements[w] = 4; //Up.
		if (codesOfMovements[w] == 101) codesOfMovements[w] = 5; //Left + Up.
		if (codesOfMovements[w] == 110) codesOfMovements[w] = 6; //Right + Up.
		if (codesOfMovements[w] == 1000) codesOfMovements[w] = 8; //Down.
		if (codesOfMovements[w] == 1001) codesOfMovements[w] = 9; // Left + Down.
		if (codesOfMovements[w] == 1010) codesOfMovements[w] = 10; //Right + Down.
	}
	for (int w = 0; w < 4;w++)  //For work with current and next vertexes.
	{
		if (w != 3)
			nextIteration = w + 1; 
		else
			nextIteration = 0;              //From 3 to 0 iteration. [4 points]
		//Analizing visibility.
		if (codesOfMovements[w] == 0 && codesOfMovements[nextIteration] == 0) //If invisible line.
			drawLineBresenhem(s, baseXsOfFigure[w], baseXsOfFigure[nextIteration], baseYsOfFigure[w], baseYsOfFigure[nextIteration], 0, 0);
		if (codesOfMovements[w] && codesOfMovements[nextIteration]) //If visible line.
			drawLineBresenhem(s, baseXsOfFigure[w], baseXsOfFigure[nextIteration], baseYsOfFigure[w], baseYsOfFigure[nextIteration], 1, 0);
		else //If partially visible line.
		{
			flag = 2; 
			for (int u = 0;u < 4;u++)
			{
				if (u != 3)
					nextIteration2 = u + 1;
				else
					nextIteration2 = 0;

				delta = (baseXsOfFigure[nextIteration] - baseXsOfFigure[w])*(YsOfWindow[u] - YsOfWindow[nextIteration2]) - (XsOfWindow[u] - XsOfWindow[nextIteration2])*(baseYsOfFigure[nextIteration] - baseYsOfFigure[w]);
				//Distance from figure's vertex.
				mu = ((XsOfWindow[u] - baseXsOfFigure[w])*(YsOfWindow[u] - YsOfWindow[nextIteration2]) - (XsOfWindow[u] - XsOfWindow[nextIteration2])*(YsOfWindow[u] - baseYsOfFigure[w])) / delta;
				//Distance from window's vertex.
				mu2 = ((baseXsOfFigure[nextIteration] - baseXsOfFigure[w])*(YsOfWindow[u] - baseYsOfFigure[w]) - (XsOfWindow[u] - baseXsOfFigure[w])*(baseYsOfFigure[nextIteration] - baseYsOfFigure[w])) / delta;
				if (mu >= 0 && mu <= 1 && mu2 >= 0 && mu2 <= 1)
				{
					if (flag == 2) //First case window-line point.
					{
						windowX1 = (1 - mu)*baseXsOfFigure[w] + mu*baseXsOfFigure[nextIteration];
						windowY1 = (1 - mu)*baseYsOfFigure[w] + mu*baseYsOfFigure[nextIteration];
						flag = 0;
					}
					else
					{   //Other case window-line point.
						windowX2 = (1 - mu)*baseXsOfFigure[w] + mu*baseXsOfFigure[nextIteration];
						windowY2 = (1 - mu)*baseYsOfFigure[w] + mu*baseYsOfFigure[nextIteration];
						//Choising from which side of window line is drawed.
						if (abs(baseXsOfFigure[w] - windowX1) <= abs(baseXsOfFigure[w] - windowX2)) 
						{   //If window's first side > second side.
							drawLineBresenhem(s, windowX1, baseXsOfFigure[w], windowY1, baseYsOfFigure[w], 1, 0);
							drawLineBresenhem(s, windowX2, baseXsOfFigure[nextIteration], windowY2, baseYsOfFigure[nextIteration], 1, 0);
						}
						else 
						{	//If window's second side > first side.
							drawLineBresenhem(s, windowX2, baseXsOfFigure[w], windowY2, baseYsOfFigure[w], 1, 0);
							drawLineBresenhem(s, windowX1, baseXsOfFigure[nextIteration], windowY1, baseYsOfFigure[nextIteration], 1, 0);
						}
					}
					if (codesOfMovements[w] == 0 || codesOfMovements[nextIteration] == 0)
					{
						//If current vertex is inside of window, draw from it to window-line point.
						if (codesOfMovements[w] == 0) 
						{
							drawLineBresenhem(s, baseXsOfFigure[w], windowX1, baseYsOfFigure[w], windowY1, 0, 0);
							drawLineBresenhem(s, windowX1, baseXsOfFigure[nextIteration], windowY1, baseYsOfFigure[nextIteration], 1, 0);
						}
						//If next vertex is inside of window, draw from it to window-line point.
						else 
						{
							drawLineBresenhem(s, baseXsOfFigure[nextIteration], windowX1, baseYsOfFigure[nextIteration], windowY1, 0, 0);
							drawLineBresenhem(s, windowX1, baseXsOfFigure[w], windowY1, baseYsOfFigure[w], 1, 0);
						}
					}
					else
						flag = 1;
				}
			}
			if (flag == 2 && codesOfMovements[w] != 0 && codesOfMovements[nextIteration] != 0)  //In case of figureInside, but no line-window point.
				drawLineBresenhem(s, baseXsOfFigure[nextIteration], baseXsOfFigure[w], baseYsOfFigure[nextIteration], baseYsOfFigure[w], 1, 0);
		}
	}
}
