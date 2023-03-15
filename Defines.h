#pragma once
#define _USE_MATH_DEFINES
#define RASTERWIDTH 800
#define RASTERHEIGHT 600

#include <iostream>
#include <math.h>
#include <time.h>
#include <vector>



typedef unsigned int A_Pixel;
unsigned int screenArray[RASTERWIDTH * RASTERHEIGHT] = { 0, };
float depthArray[RASTERWIDTH * RASTERHEIGHT];
float xChange = -18.0f;
float yChange = 0.0f;

struct My_Matrix4x4
{
	float matrix[4][4];
};

struct My_Vertex
{
	float xyzw[4];
	float uv[2];
	float normal[3];
	unsigned int color;
};

struct My_Light
{
	float xyzw[4];
	unsigned int color;
};

struct My_Vector2D
{
	float posX;
	float posY;
};

struct My_Vector3D
{
	float xyz[3];
};

My_Matrix4x4 VS_WorldMatrix;
My_Matrix4x4 ViewMatrix;
My_Matrix4x4 ProjectionMatrix;
My_Light directionalLight;
My_Light pointLight;
float pointLightRadius;