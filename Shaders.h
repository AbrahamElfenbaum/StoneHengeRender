#pragma once
#include "MyMath.h"
#include "StoneHenge.h"
#include "StoneHenge_Texture.h"
#define IDENTITY MatrixIdentity()
#define LIGHTRADIUS 2.0f

void (*VertexShader) (My_Vertex&) = 0;
void (*PixelShader) (A_Pixel&, float u, float v) = 0;

void VS_World(My_Vertex& multiplier)//this is already a copy. make copy outside func, in main
{
	multiplier = MatrixMultVector_XYZW(multiplier, VS_WorldMatrix);	//Local -> World
	multiplier = MatrixMultVector_XYZW(multiplier, ViewMatrix);		//World -> View
	multiplier = MatrixMultVector_XYZW(multiplier, ProjectionMatrix);//View  -> Projection
	multiplier.xyzw[0] /= multiplier.xyzw[3];
	multiplier.xyzw[1] /= multiplier.xyzw[3];
	multiplier.xyzw[2] /= multiplier.xyzw[3];
	//multiplier.xyzw[3] /= multiplier.xyzw[3];
}


void VS_Lighting(My_Vertex& multiplier)//this is already a copy. make copy outside func, in main
{
	multiplier = MatrixMultVector_XYZW(multiplier, VS_WorldMatrix);	//Local -> World
	multiplier = MatrixMultVector_NORMAL(multiplier, VS_WorldMatrix);

	My_Vector3D surfaceNormal = { multiplier.normal[0], multiplier.normal[1] , multiplier.normal[2] };
	My_Vector3D lightDir_Directional = { -1.0f * directionalLight.xyzw[0], -1.0f * directionalLight.xyzw[1], -1.0f * directionalLight.xyzw[2] };

	//DirectionalLight
	float lightRatio_Directional = CLAMP(DotProduct(lightDir_Directional, surfaceNormal));
	unsigned int dColor = ColorLerp( 0x000000, directionalLight.color, lightRatio_Directional);
	
	//point math
	My_Vector3D lightDir_Point = { pointLight.xyzw[0] - multiplier.xyzw[0], pointLight.xyzw[1] - multiplier.xyzw[1], pointLight.xyzw[2] - multiplier.xyzw[2] };
	lightDir_Point = Normalize_Vec3(lightDir_Point);
	float lightRatio_Point = CLAMP(DotProduct(lightDir_Point, surfaceNormal));

	float attenuation = 1.0f - CLAMP(Length_Vec3(lightDir_Point) / pointLightRadius);
	float lightRatio_Point_Final = lightRatio_Point * attenuation;
	unsigned int pColor = ColorLerp(0x000000, pointLight.color, lightRatio_Point_Final);

	multiplier.color = CombineColors(dColor, pColor);
	//multiplier.color = dColor;
	multiplier = MatrixMultVector_XYZW(multiplier, ViewMatrix);		//World -> View
	multiplier = MatrixMultVector_XYZW(multiplier, ProjectionMatrix);//View  -> Projection
	multiplier.xyzw[0] /= multiplier.xyzw[3];
	multiplier.xyzw[1] /= multiplier.xyzw[3];
	multiplier.xyzw[2] /= multiplier.xyzw[3];
	//multiplier.xyzw[3] /= multiplier.xyzw[3];
}

void PS_Red(A_Pixel& color, float u = 0, float v = 0)
{
	color = 0x00FF0000;
}

void PS_Green(A_Pixel& color, float u = 0, float v = 0)
{
	color = 0x0000FF00;
}

void PS_Blue(A_Pixel& color, float u = 0, float v = 0)
{
	color = 0x000000FF;
}

void PS_Purple(A_Pixel& color, float u = 0, float v = 0)
{
	color = 0x00FF00FF;
}

void PS_Yellow(A_Pixel& color, float u = 0, float v = 0)
{
	color = 0x00FFFF00;
}

void PS_Orange(A_Pixel& color, float u = 0, float v = 0)
{
	color = 0x00FF7E00;
}

void PS_White(A_Pixel& color, float u = 0, float v = 0)
{
	color = 0x00FFFFFF;
}

void PS_StoneHenge(A_Pixel& color, float u = 0, float v = 0)
{
	u *= StoneHenge_width;
	v *= StoneHenge_height;
	color = C2C(StoneHenge_pixels[TwoDim2OneDim(u, v, StoneHenge_width)]);
}

