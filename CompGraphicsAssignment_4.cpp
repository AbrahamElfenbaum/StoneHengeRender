#include "DrawFunctions.h"
#include "RasterSurface.h"
#include "XTime.h"
#define VERTICALFOV DegreeToRadian(90.0f)
#define NEARPLANE 0.1f
#define FARPLANE 1000.0f
#define ROTATIONSPEED 2.0f
#define ASPECTRATIO (float)RASTERHEIGHT/RASTERWIDTH
#define	CUBEROTATIONSPEED 0.025f

int main()
{
	bool isAtMaxRadius = true;
	srand(time(NULL));
	My_Matrix4x4 CameraMatrix = MatrixMultMatrix(MatrixTranslation(0, 0, -3), MatrixXRotation(DegreeToRadian(-18.0f)));
	My_Matrix4x4 Mult;
	ViewMatrix = MatrixFOAInverse(CameraMatrix);
	XTime timer;
	timer.Restart();
	float timeElapsed = 0.0f;
	pointLightRadius = 2.0f;

	directionalLight = { -0.577f, -0.577f, 0.577f, 0, 0xFFC0C0F0 };
	pointLight = { -1.0f, 0.5f, 1.0f, 1, 0xFFFFFF00 };

	My_Vertex v;
	std::vector<My_Vertex> starVec;
	for (int i = 0; i < 3000; ++i)
	{
		v.xyzw[0] = RandomFloat(-1.0f, 1.0f) * 50.0f;
		v.xyzw[1] = RandomFloat(-1.0f, 1.0f) * 50.0f;
		v.xyzw[2] = RandomFloat(-1.0f, 1.0f) * 50.0f;
		v.xyzw[3] = 1.0f;
		starVec.push_back(v);
	}
	std::vector<My_Vertex> SHVertexes = StoneHengeVector();

#pragma region Perspective Matrix
	//Yscale = cotangent ( 1/2 * Vertical FOV )
	float yScale = 1.0f / (tanf(0.5f * VERTICALFOV));

	//Xscale = Yscale * Aspect Ratio
	float xScale = yScale * ASPECTRATIO;

	//zFar / ( zFar – zNear )
	float matrix_33 = FARPLANE / (FARPLANE - NEARPLANE);

	//-( zFar * zNear ) / ( zFar – zNear )
	float matrix_43 = (-1.0f * (FARPLANE * NEARPLANE)) / (FARPLANE - NEARPLANE);

	ProjectionMatrix = { xScale,      0,         0, 0,
							  0, yScale,         0, 0,
							  0,      0, matrix_33, 1,
							  0,      0, matrix_43, 0 };
#pragma endregion

	RS_Initialize("Abraham Elfenbaum", RASTERWIDTH, RASTERHEIGHT);
	do
	{
		timer.Signal();
		ClearScreen();//makes the screen all black
		ClearDepthArray();//makes all the values of the depth array 1

		VertexShader = VS_Lighting;
		timeElapsed += timer.Delta();
		if (timeElapsed >= (1.0f / 20.0f))
		{
			if (isAtMaxRadius)
			{
				if (pointLightRadius < 1.0f)
				{
					isAtMaxRadius = false;
				}
				else
				{
					pointLightRadius -= 0.1f;
				}
			}
			else
			{
				if (pointLightRadius > 2.0f)
				{
					isAtMaxRadius = true;
				}
				else
				{
					pointLightRadius += 0.1f;
				}
			}
			
			timeElapsed = 0.0f;

		}
		
		VS_WorldMatrix = IDENTITY;
		DrawStar(starVec);
		PixelShader = PS_StoneHenge;
		for (int i = 0; i < 2532; i += 3)
		{
			FillTriangle(SHVertexes[StoneHenge_indicies[i]], SHVertexes[StoneHenge_indicies[i + 1]], SHVertexes[StoneHenge_indicies[i + 2]]);
		}
		
#pragma region Camera Control
		if (GetAsyncKeyState(0x57) & 0x01)//W Key
		{
			xChange -= ROTATIONSPEED;
		}

		if (GetAsyncKeyState(0x53) & 0x01)//S Key
		{
			xChange += ROTATIONSPEED;
		}

		if (GetAsyncKeyState(0x44) & 0x01)//D Key
		{
			yChange += ROTATIONSPEED;
		}

		if (GetAsyncKeyState(0x41) & 0x01)//A Key
		{
			yChange -= ROTATIONSPEED;
		}

		if (GetAsyncKeyState(0x52) & 0x01)//R Key
		{
			xChange = -18.0f;
			yChange = 0.0f;
		}
#pragma endregion
	
		Mult = MatrixMultMatrix(MatrixXRotation(DegreeToRadian(xChange)), MatrixYRotation(DegreeToRadian(yChange)));
		CameraMatrix = MatrixMultMatrix(MatrixTranslation(0, 0, -3), Mult);
		ViewMatrix = MatrixFOAInverse(CameraMatrix);
		

	} while (RS_Update(screenArray, sizeof(screenArray) >> 2));
	RS_Shutdown();
	return 0;
}