#pragma once
#include "Defines.h"

unsigned int TwoDim2OneDim(unsigned int x, unsigned int y, unsigned width = RASTERWIDTH)
{
	return (y * width) + x;
}

float DegreeToRadian(float angle)
{
	return angle * (M_PI / 180.0f);
}

float LERP(float p1, float p2, float r)
{
	return p1 + (r * (p2 - p1));
}

unsigned int LERP_UI(unsigned int p1, unsigned int p2, float r)
{
	return p1 + (r * ((int)p2 - (int)p1));
}

float NDCX(float x)
{
	return floor(((x + 1) * (RASTERWIDTH / 2)));
}

float NDCY(float y)
{
	return floor(((1 - y) * (RASTERHEIGHT / 2)));
}

My_Matrix4x4 MatrixFOAInverse(My_Matrix4x4 matrix)
{
	My_Matrix4x4 result;
	float temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			result.matrix[i][j] = matrix.matrix[j][i];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		result.matrix[i][3] = matrix.matrix[i][3];
	}

	for (int i = 0; i < 3; i++)
	{
		temp = 0.0f;
		for (int j = 0; j < 3; j++)
		{
			temp += (matrix.matrix[3][j] * result.matrix[j][i]);
		}
		temp *= -1;
		result.matrix[3][i] = temp;
	}

	return result;
}

My_Matrix4x4 MatrixIdentity()
{
	My_Matrix4x4 IDMatrix = { 1,0,0,0,
							  0,1,0,0,
							  0,0,1,0,
							  0,0,0,1 };
	return IDMatrix;
}

My_Matrix4x4 MatrixMultMatrix(My_Matrix4x4 matrix1, My_Matrix4x4 matrix2)
{
	My_Matrix4x4 result;
	float temp;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			temp = 0.0f;
			for (int k = 0; k < 4; k++)
			{
				temp += (matrix1.matrix[i][k] * matrix2.matrix[k][j]);
			}
			result.matrix[i][j] = temp;
		}
	}

	return result;
}

My_Vertex MatrixMultVector_XYZW(My_Vertex vec, My_Matrix4x4 matrix)
{
	My_Vertex result;
	result = vec;
	float temp;
	for (int i = 0; i < 4; i++)
	{
		temp = 0.0f;
		for (int j = 0; j < 4; j++)
		{
			temp += (vec.xyzw[j] * matrix.matrix[j][i]);
		}
		result.xyzw[i] = temp;
	}
	return result;
}

My_Vertex MatrixMultVector_NORMAL(My_Vertex vec, My_Matrix4x4 matrix)
{
	My_Vertex result = vec;
	float temp;
	for (int i = 0; i < 3; i++)
	{
		temp = 0.0f;
		for (int j = 0; j < 3; j++)
		{
			temp += (vec.normal[j] * matrix.matrix[j][i]);
		}
		result.normal[i] = temp;
	}
	return result;
}

My_Matrix4x4 MatrixTranslation(float x, float y, float z)
{
	My_Matrix4x4 result = { 1,0,0,0,
							0,1,0,0,
							0,0,1,0,
							x,y,z,1 };
	return result;
}

My_Matrix4x4 MatrixXRotation(float radian)
{
	float c = cosf(radian);
	float s = sinf(radian);
	My_Matrix4x4 result = { 1,     0,     0, 0,
							0,     c,-1 * s, 0,
							0,     s,     c, 0,
							0,     0,     0, 1 };
	return result;
}

My_Matrix4x4 MatrixYRotation(float radian)
{
	float c = cosf(radian);
	float s = sinf(radian);
	My_Matrix4x4 result = {      c, 0, s, 0,
							     0, 1, 0, 0,
							-1 * s, 0, c, 0,
							     0, 0, 0, 1 };
	return result;
	/*
	Transpose
	c 0 s 0
	0 1 0 0
   -s 0 c 0
	0 0 0 1
	*/
}

My_Matrix4x4 MatrixZRotation(float radian)
{
	float c = cosf(radian);
	float s = sinf(radian);
	My_Matrix4x4 result = {      c, s, 0, 0,
							-1 * s, c, 0, 0,
							     0, 0, 1, 0,
							     0, 0, 0, 1 };
	return result;
}

float ImplicitLineEquation(My_Vector2D startLine, My_Vector2D endLine, My_Vector2D point)
{
	return ((startLine.posY - endLine.posY) * point.posX) + 
		   ((endLine.posX - startLine.posX) * point.posY) + 
			(startLine.posX * endLine.posY) - 
			(startLine.posY * endLine.posX);
}

My_Vector3D BarycentricCoordinates(My_Vector2D pointA, My_Vector2D pointB, My_Vector2D pointC, My_Vector2D pointP)
{
	My_Vector3D result;
	float beta  = ImplicitLineEquation(pointA, pointC, pointB);//ACB
	float gamma = ImplicitLineEquation(pointB, pointA, pointC);//BAC
	float alpha = ImplicitLineEquation(pointC, pointB, pointA);//CBA
	float b     = ImplicitLineEquation(pointA, pointC, pointP);//ACP
	float y     = ImplicitLineEquation(pointB, pointA, pointP);//BAP
	float a     = ImplicitLineEquation(pointC, pointB, pointP);//CBP

	result.xyz[0] = (b/beta);
	result.xyz[1] = (y/gamma);
	result.xyz[2] = (a/alpha);

	return result;
}

float BERP(float aZ, float bZ, float cZ, My_Vector3D vec)
{
	return ((aZ * vec.xyz[2]) + (bZ * vec.xyz[0]) + (cZ * vec.xyz[1]));
}

float Largest(float a, float b, float c)
{
	if (a >= b && a >= c)
	{
		return a;
	}
	else if (b >= c)
	{
		return b;
	}
	else
	{
		return c;
	}
}

float Smallest(float a, float b, float c)
{
	if (a <= b && a <= c)
	{
		return a;
	}
	else if (b <= c)
	{
		return b;
	}
	else
	{
		return c;
	}
}

unsigned int C2C(unsigned int BGRA)
{
	unsigned int AByte = (BGRA & 0x000000FF);
	unsigned int RByte = (BGRA & 0x0000FF00) >> 8;
	unsigned int GByte = (BGRA & 0x00FF0000) >> 16;
	unsigned int BByte = (BGRA & 0xFF000000) >> 24;
	unsigned int ARGB = ((AByte << 24) | (RByte << 16) | (GByte << 8) | (BByte));
	return ARGB;
}

float DotProduct(My_Vector3D vert1, My_Vector3D vert2)
{	
	return (vert1.xyz[0] * vert2.xyz[0]) 
		 + (vert1.xyz[1] * vert2.xyz[1]) 
		 + (vert1.xyz[2] * vert2.xyz[2]);
}

My_Vector3D CrossProduct(My_Vertex vert1, My_Vertex vert2)
{
	My_Vector3D result;

	result.xyz[0] = (vert1.xyzw[1] * vert2.xyzw[2]) - (vert1.xyzw[2] * vert2.xyzw[1]);
	result.xyz[1] = (vert1.xyzw[2] * vert2.xyzw[0]) - (vert1.xyzw[0] * vert2.xyzw[2]);
	result.xyz[2] = (vert1.xyzw[0] * vert2.xyzw[2]) - (vert1.xyzw[2] * vert2.xyzw[0]);

	return result;
}

My_Vector3D Normalize_Vec3(My_Vector3D vert)
{
	My_Vector3D result;

	float normal = sqrt((vert.xyz[0] * vert.xyz[0]) + 
						(vert.xyz[1] * vert.xyz[1]) + 
						(vert.xyz[2] * vert.xyz[2]));

	result.xyz[0] = vert.xyz[0] / normal;
	result.xyz[1] = vert.xyz[1] / normal;
	result.xyz[2] = vert.xyz[2] / normal;

	return result;
}

unsigned int CombineColors(unsigned int ARGB_1, unsigned int ARGB_2)
{
	unsigned int ARGB;

	unsigned int AByte_1 = (ARGB_1 & 0xFF000000) >> 24;
	unsigned int RByte_1 = (ARGB_1 & 0x00FF0000) >> 16;
	unsigned int GByte_1 = (ARGB_1 & 0x0000FF00) >> 8;
	unsigned int BByte_1 = (ARGB_1 & 0x000000FF);
							
	unsigned int AByte_2 = (ARGB_2 & 0xFF000000) >> 24;
	unsigned int RByte_2 = (ARGB_2 & 0x00FF0000) >> 16;
	unsigned int GByte_2 = (ARGB_2 & 0x0000FF00) >> 8;
	unsigned int BByte_2 = (ARGB_2 & 0x000000FF);

	unsigned int AByte = AByte_1 + AByte_2;
	unsigned int RByte = RByte_1 + RByte_2;
	unsigned int GByte = GByte_1 + GByte_2;
	unsigned int BByte = BByte_1 + BByte_2;

	ARGB = ((AByte << 24) | (RByte << 16) | (GByte << 8) | (BByte));
	return ARGB;
}

unsigned int ModulateColors(unsigned int ARGB_1, unsigned int ARGB_2)
{
	unsigned int ARGB;

	unsigned int AByte_1 = (ARGB_1 & 0xFF000000) >> 24;
	unsigned int RByte_1 = (ARGB_1 & 0x00FF0000) >> 16;
	unsigned int GByte_1 = (ARGB_1 & 0x0000FF00) >> 8;
	unsigned int BByte_1 = (ARGB_1 & 0x000000FF);

	unsigned int AByte_2 = (ARGB_2 & 0xFF000000) >> 24;
	unsigned int RByte_2 = (ARGB_2 & 0x00FF0000) >> 16;
	unsigned int GByte_2 = (ARGB_2 & 0x0000FF00) >> 8;
	unsigned int BByte_2 = (ARGB_2 & 0x000000FF);

	float aRatio_1 = AByte_1 / 255.0f;
	float rRatio_1 = RByte_1 / 255.0f;
	float gRatio_1 = GByte_1 / 255.0f;
	float bRatio_1 = BByte_1 / 255.0f;

	float aRatio_2 = AByte_2 / 255.0f;
	float rRatio_2 = RByte_2 / 255.0f;
	float gRatio_2 = GByte_2 / 255.0f;
	float bRatio_2 = BByte_2 / 255.0f;

	unsigned int AByte = (aRatio_1 * aRatio_2) * 255;
	unsigned int RByte = (rRatio_1 * rRatio_2) * 255;
	unsigned int GByte = (gRatio_1 * gRatio_2) * 255;
	unsigned int BByte = (bRatio_1 * bRatio_2) * 255;

	ARGB = ((AByte << 24) | (RByte << 16) | (GByte << 8) | (BByte));
	return ARGB;
}

float CLAMP(float x)
{
	if (x > 1)
	{
		return 1.0f;
	}
	else if (x < 0)
	{
		return 0.0f;
	}
	else
	{
		return x;
	}
}

unsigned int CLAMP_UI(unsigned int x)
{
	if (x > 0xFFFFFFFF)
	{
		return 0xFFFFFFFF;
	}
	else if (x < 0x00000000)
	{
		return 0x00000000;
	}
	else
	{
		return x;
	}
}

float Length_Vec3(My_Vector3D vert)
{
	return sqrt((vert.xyz[0] * vert.xyz[0]) +
				(vert.xyz[1] * vert.xyz[1]) +
				(vert.xyz[2] * vert.xyz[2]));

	
}

float RandomFloat(float min, float max)
{
	return min + ((rand() / (float)RAND_MAX) * (max - min));
}

unsigned int ColorLerp(unsigned int color1, unsigned int color2, float ratio)
{
	unsigned int AByte_1 = (color1 & 0xFF000000) >> 24;
	unsigned int RByte_1 = (color1 & 0x00FF0000) >> 16;
	unsigned int GByte_1 = (color1 & 0x0000FF00) >> 8;
	unsigned int BByte_1 = (color1 & 0x000000FF);

	unsigned int AByte_2 = (color2 & 0xFF000000) >> 24;
	unsigned int RByte_2 = (color2 & 0x00FF0000) >> 16;
	unsigned int GByte_2 = (color2 & 0x0000FF00) >> 8;
	unsigned int BByte_2 = (color2 & 0x000000FF);


	unsigned int AByte = LERP_UI(AByte_1, AByte_2, ratio);
	unsigned int RByte = LERP_UI(RByte_1, RByte_2, ratio);
	unsigned int GByte = LERP_UI(GByte_1, GByte_2, ratio);
	unsigned int BByte = LERP_UI(BByte_1, BByte_2, ratio);

	unsigned int ARGB = ((AByte << 24) | (RByte << 16) | (GByte << 8) | (BByte));
	return ARGB;
}

unsigned int ColorBERP(unsigned int color1, unsigned int color2, unsigned int color3, My_Vector3D ratio)
{
	unsigned int AByte_1 = (color1 & 0xFF000000) >> 24;
	unsigned int RByte_1 = (color1 & 0x00FF0000) >> 16;
	unsigned int GByte_1 = (color1 & 0x0000FF00) >> 8;
	unsigned int BByte_1 = (color1 & 0x000000FF);

	unsigned int AByte_2 = (color2 & 0xFF000000) >> 24;
	unsigned int RByte_2 = (color2 & 0x00FF0000) >> 16;
	unsigned int GByte_2 = (color2 & 0x0000FF00) >> 8;
	unsigned int BByte_2 = (color2 & 0x000000FF);

	unsigned int AByte_3 = (color2 & 0xFF000000) >> 24;
	unsigned int RByte_3 = (color2 & 0x00FF0000) >> 16;
	unsigned int GByte_3 = (color2 & 0x0000FF00) >> 8;
	unsigned int BByte_3 = (color2 & 0x000000FF);

	unsigned int AByte = BERP(AByte_1, AByte_2, AByte_3, ratio);
	unsigned int RByte = BERP(RByte_1, RByte_2, RByte_3, ratio);
	unsigned int GByte = BERP(GByte_1, GByte_2, GByte_3, ratio);
	unsigned int BByte = BERP(BByte_1, BByte_2, BByte_3, ratio);

	unsigned int ARGB = ((AByte) | (RByte << 8) | (GByte << 16) | (BByte << 24));
	return ARGB;
}