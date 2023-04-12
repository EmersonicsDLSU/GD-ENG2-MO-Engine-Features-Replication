#ifndef STRUCT_HANDLER_H
#define STRUCT_HANDLER_H
#include "BNS_Color.h"
#include "BNS_UIManager.h"
#include "Vector2D.h"

struct BNS_vertex_tex
{
	Vector3D position;
	Vector2D texcoord;
};

struct BNS_vertex_color
{
	Vector3D position;
	Vector3D color;
};

struct BNS_vertex_color_lerp
{
	Vector3D position;
	Vector3D color;
	Vector3D color1;
};

struct BNS_ShaderByteData
{
	void* m_byte_code[2048];
	size_t m_size;
};

struct BNS_PC_Cube_ColorData
{
	BNS_PC_Cube_ColorData() : is_CL1_initialized(false), is_CL2_initialized(false)
	{
		Vector3D color_temp[8] =
		{
			//FRONT FACE
			BNS_Color::White,
			BNS_Color::White,
			BNS_Color::White,
			BNS_Color::White,
			//BACK FACE
			BNS_Color::White,
			BNS_Color::White,
			BNS_Color::White,
			BNS_Color::White,
		};
		memcpy(color_list_1, color_temp, sizeof(color_temp));
		memcpy(color_list_2, color_temp, sizeof(color_temp));
	}
	Vector3D color_list_1[8];
	bool is_CL1_initialized;
	Vector3D color_list_2[8];
	bool is_CL2_initialized;
};

struct BNS_Cam_Struct
{
public:
	float fovInDegrees = 90.0f;
	float nearPlane = 0.1f;
	float farPlane = 1000.0f;
	float aspectRatio = BNS_UIManager::WINDOW_WIDTH / BNS_UIManager::WINDOW_HEIGHT;
};

struct P3_SceneStatus
{
public:
	bool isEmpty = true;
	bool isComplete = false;
	bool isLoading = false;
	bool isViewed = false;
};

class BNS_StructHandler
{
	
};

#endif
