#pragma once
#include "Shaders.h"

void DrawPixel(unsigned int x, unsigned int y, A_Pixel color, float depth)
{
	if ((x >= 0) && (x < RASTERWIDTH) && 
        (y >= 0) && (y < RASTERHEIGHT) &&
		depth < depthArray[TwoDim2OneDim(x, y)])
	{
		screenArray[TwoDim2OneDim(x, y)] = color;
		depthArray[TwoDim2OneDim(x, y)] = depth;
	}
}

void ClearScreen()
{
	for (unsigned int i = 0; i < sizeof(screenArray) / sizeof(screenArray[0]); ++i)
	{
		screenArray[i] = 0x00000000;
	}
}

void ClearDepthArray()
{
	for (unsigned i = 0; i < sizeof(depthArray) / sizeof(depthArray[0]); ++i)
	{
		depthArray[i] = 1;
	}
}

My_Vector2D CartToScreen(My_Vertex vert)
{
	My_Vector2D result;
	result.posX = NDCX(vert.xyzw[0]);
	result.posY = NDCY(vert.xyzw[1]);
	return result;
}

void DrawLine(My_Vertex start, My_Vertex end)
{

	My_Vertex startCopy = start;
	My_Vertex endCopy = end;
	A_Pixel color = 0x00000000;
	if (VertexShader)
	{
		VertexShader(startCopy);
		VertexShader(endCopy);
	}
	
	My_Vector2D startVec = CartToScreen(startCopy);
	My_Vector2D endVec = CartToScreen(endCopy);

	float startX = startVec.posX;
	float endX = endVec.posX;
	float startY = startVec.posY;
	float endY = endVec.posY;
	float ratio;
	float currX;
	float currY;
	float deltaX = abs(endX - startX);
	float deltaY = abs(endY - startY);
	float depth;
	if (deltaX > deltaY)
	{

		if (startX < endX)
		{

			for (int i = startX; i < endX; ++i)
			{
				ratio = abs(((float)i - startX) / deltaX);
				currY = LERP(startY, endY, ratio);
				depth = LERP(startCopy.xyzw[2], endCopy.xyzw[2], ratio);
				DrawPixel(i, floor(currY + .05f), color, depth);
			}
		}
		else
		{
			for (int i = endX; i < startX; ++i)
			{
				ratio = abs(((float)i - startX) / deltaX);
				currY = LERP(startY, endY, ratio);
				depth = LERP(startCopy.xyzw[2], endCopy.xyzw[2], ratio);
				DrawPixel(i, floor(currY + .05f), color, depth);
			}
		}
	}
	else
	{
		if (startY < endY)
		{
			for (int i = startY; i < endY; ++i)
			{
				ratio = abs(((float)i - startY) / deltaY);
				currX = LERP(startX, endX, ratio);
				depth = LERP(startCopy.xyzw[2], endCopy.xyzw[2], ratio);
				DrawPixel(floor(currX + .05f), i, color, depth);
			}
		}
		else
		{
			for (int i = endY; i < startY; ++i)
			{
				ratio = abs(((float)i - startY) / deltaY);
				currX = LERP(startX, endX, ratio);
				depth = LERP(startCopy.xyzw[2], endCopy.xyzw[2], ratio);
				DrawPixel(floor(currX + .05f), i, color, depth);
			}
		}
	}
}

void DrawCube(My_Vertex vertArray[8])
{

	DrawLine(vertArray[0], vertArray[1]);
	DrawLine(vertArray[0], vertArray[2]);
	DrawLine(vertArray[0], vertArray[4]);

	DrawLine(vertArray[1], vertArray[3]);
	DrawLine(vertArray[1], vertArray[5]);
	DrawLine(vertArray[2], vertArray[3]);

	DrawLine(vertArray[2], vertArray[6]);
	DrawLine(vertArray[3], vertArray[7]);
	DrawLine(vertArray[4], vertArray[6]);

	DrawLine(vertArray[4], vertArray[5]);
	DrawLine(vertArray[5], vertArray[7]);
	DrawLine(vertArray[6], vertArray[7]);
}

void DrawGrid()
{
	//   X     Y     Z  W
	My_Vertex _n1_0  = { -0.5, 0,  -0.5, 1 };
	My_Vertex _n1_02 = { -0.5, 0,  -0.4, 1 };
	My_Vertex _n1_04 = { -0.5, 0,  -0.3, 1 };
	My_Vertex _n1_06 = { -0.5, 0,  -0.2, 1 };
	My_Vertex _n1_08 = { -0.5, 0,  -0.1, 1 };
	My_Vertex _n1_1  = { -0.5, 0,     0, 1 };
	My_Vertex _n1_12 = { -0.5, 0,   0.1, 1 };
	My_Vertex _n1_14 = { -0.5, 0,   0.2, 1 };
	My_Vertex _n1_16 = { -0.5, 0,   0.3, 1 };
	My_Vertex _n1_18 = { -0.5, 0,   0.4, 1 };
	My_Vertex _n1_2  = { -0.5, 0,   0.5, 1 };
	My_Vertex _n08_0 = { -0.4, 0,  -0.5, 1 };
	My_Vertex _n08_2 = { -0.4, 0,   0.5, 1 };
	My_Vertex _n06_0 = { -0.3, 0,  -0.5, 1 };
	My_Vertex _n06_2 = { -0.3, 0,   0.5, 1 };
	My_Vertex _n04_0 = { -0.2, 0,  -0.5, 1 };
	My_Vertex _n04_2 = { -0.2, 0,   0.5, 1 };
	My_Vertex _n02_0 = { -0.1, 0,  -0.5, 1 };
	My_Vertex _n02_2 = { -0.1, 0,   0.5, 1 };
	My_Vertex _0_0   = {    0, 0,  -0.5, 1 };
	My_Vertex _0_2   = {    0, 0,   0.5, 1 };
	My_Vertex _p02_0 = {  0.1, 0,  -0.5, 1 };
	My_Vertex _p02_2 = {  0.1, 0,   0.5, 1 };
	My_Vertex _p04_0 = {  0.2, 0,  -0.5, 1 };
	My_Vertex _p04_2 = {  0.2, 0,   0.5, 1 };
	My_Vertex _p06_0 = {  0.3, 0,  -0.5, 1 };
	My_Vertex _p06_2 = {  0.3, 0,   0.5, 1 };
	My_Vertex _p08_0 = {  0.4, 0,  -0.5, 1 };
	My_Vertex _p08_2 = {  0.4, 0,   0.5, 1 };
	My_Vertex _p1_0  = {  0.5, 0,  -0.5, 1 };
	My_Vertex _p1_02 = {  0.5, 0,  -0.4, 1 };
	My_Vertex _p1_04 = {  0.5, 0,  -0.3, 1 };
	My_Vertex _p1_06 = {  0.5, 0,  -0.2, 1 };
	My_Vertex _p1_08 = {  0.5, 0,  -0.1, 1 };
	My_Vertex _p1_1  = {  0.5, 0,     0, 1 };
	My_Vertex _p1_12 = {  0.5, 0,   0.1, 1 };
	My_Vertex _p1_14 = {  0.5, 0,   0.2, 1 };
	My_Vertex _p1_16 = {  0.5, 0,   0.3, 1 };
	My_Vertex _p1_18 = {  0.5, 0,   0.4, 1 };
	My_Vertex _p1_2  = {  0.5, 0,   0.5, 1 };

	//Horizontal Lines
	DrawLine(_n1_0, _p1_0);
	DrawLine(_n1_02, _p1_02);
	DrawLine(_n1_04, _p1_04);
	DrawLine(_n1_06, _p1_06);
	DrawLine(_n1_08, _p1_08);
	DrawLine(_n1_1, _p1_1);
	DrawLine(_n1_12, _p1_12);
	DrawLine(_n1_14, _p1_14);
	DrawLine(_n1_16, _p1_16);
	DrawLine(_n1_18, _p1_18);
	DrawLine(_n1_2, _p1_2);
	//Vertical Lines
	DrawLine(_n1_0, _n1_2);
	DrawLine(_n08_0, _n08_2);
	DrawLine(_n06_0, _n06_2);
	DrawLine(_n04_0, _n04_2);
	DrawLine(_n02_0, _n02_2);
	DrawLine(_0_0, _0_2);
	DrawLine(_p02_0, _p02_2);
	DrawLine(_p04_0, _p04_2);
	DrawLine(_p06_0, _p06_2);
	DrawLine(_p08_0, _p08_2);
	DrawLine(_p1_0, _p1_2);
}

void FillTriangle(My_Vertex A, My_Vertex B, My_Vertex C)
{

	My_Vertex aCopy = A;
	My_Vertex bCopy = B;
	My_Vertex cCopy = C;
	A_Pixel color = 0xFFFFFFFF;
	if (VertexShader)
	{
		VertexShader(aCopy);
		VertexShader(bCopy);
		VertexShader(cCopy);
	}
	if (PixelShader)
	{
		PixelShader(color, 0, 0);
	}
	My_Vector2D pointA = CartToScreen(aCopy);
	My_Vector2D pointB = CartToScreen(bCopy);
	My_Vector2D pointC = CartToScreen(cCopy);

	My_Vector2D point;
	My_Vector3D bary;

	
	float startX = Smallest(pointA.posX, pointB.posX, pointC.posX);
	float endX = Largest(pointA.posX, pointB.posX, pointC.posX);
	float startY = Smallest(pointA.posY, pointB.posY, pointC.posY);
	float endY = Largest(pointA.posY, pointB.posY, pointC.posY);
	float dBerp;
	float uBerp;
	float vBerp;
	unsigned int cBerp;
	for (int i = startX; i < endX; i++)
	{
		for (int j = startY; j < endY; j++)
		{
			point.posX = i;
			point.posY = j;
			bary = BarycentricCoordinates(pointA, pointB, pointC, point);
			if (bary.xyz[0] >= 0 && bary.xyz[0] <= 1 &&
				bary.xyz[1] >= 0 && bary.xyz[1] <= 1 &&
				bary.xyz[2] >= 0 && bary.xyz[2] <= 1)
			{
				
				dBerp = BERP(aCopy.xyzw[2], bCopy.xyzw[2], cCopy.xyzw[2], bary);
				uBerp = BERP(aCopy.uv[0], bCopy.uv[0], cCopy.uv[0], bary);
				vBerp = BERP(aCopy.uv[1], bCopy.uv[1], cCopy.uv[1], bary);
				cBerp = ColorBERP(aCopy.color, bCopy.color, cCopy.color, bary);
				if (PixelShader)
				{
					PixelShader(color, uBerp, vBerp);
				}
				color = ModulateColors(cBerp, color);
				DrawPixel(i, j, color, dBerp);
			}
		}
	}
}

void DrawStar(std::vector<My_Vertex> starVec)
{
	if (VertexShader)
	{
		for (int i = 0; i < starVec.size(); i++)
		{
			VertexShader(starVec[i]);
		}
	}

	for (int i = 0; i < starVec.size(); i++)
	{
		My_Vector2D cart = CartToScreen(starVec[i]);
		DrawPixel(cart.posX, cart.posY, 0xFFFFFFFF, starVec[i].xyzw[2]);
	}
}

std::vector<My_Vertex> StoneHengeVector()
{
	std::vector<My_Vertex> SHVertexes;
	My_Vertex v;
	for (int i = 0; i < sizeof(StoneHenge_data)/sizeof(StoneHenge_data[0]); i++)
	{
		v.xyzw[0]   = StoneHenge_data[i].pos[0] * 0.1f;//x
		v.xyzw[1]   = StoneHenge_data[i].pos[1] * 0.1f;//y
		v.xyzw[2]   = StoneHenge_data[i].pos[2] * 0.1f;//z
		v.xyzw[3]   = 1.0f;//w
		v.uv[0]     = StoneHenge_data[i].uvw[0];//u
		v.uv[1]     = StoneHenge_data[i].uvw[1];//v
		v.normal[0] = StoneHenge_data[i].nrm[0];
		v.normal[1] = StoneHenge_data[i].nrm[1];
		v.normal[2] = StoneHenge_data[i].nrm[2];

		SHVertexes.push_back(v);
	}

	return SHVertexes;
}