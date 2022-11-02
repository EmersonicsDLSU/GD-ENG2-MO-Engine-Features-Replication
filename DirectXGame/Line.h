#pragma once
#include "Cube.h"
#include "EnumHandler.h"
#include "Prerequisites.h"
#include "StructHandler.h"

class Line : public AGameObject
{
public:
	Line(std::string name, ObjectTypes type);
	~Line();
public:
	void Update(float deltaTime, AppWindow* app_window) override;
	void Draw(const BlenderPtr& m_blender) override;
public:
	void SetVertex_Index_Buffer(PC_Cube_ColorData color_data = PC_Cube_ColorData());
	void SetVertexShader(VertexShaderType vs_type);
	void SetPixelShader(PixelShaderType ps_type);
public:
	void SetAlpha(float alpha);
	float GetAlpha();
protected:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;
	ConstantBufferPtr m_cb;
protected:
	VertexShaderPtr m_vs;
	PixelShaderPtr m_ps;
	VertexShaderType vs_type;
	PixelShaderType ps_type;
protected:
	float ticks = 0.0f;
	float deltaPos = 0.0f;
	float deltaScale = 0.0f;
	float deltaTime = 0.0f;
	float alpha = 1.0f;

private:
	vertex_color* vertex_list[];

protected:
	friend class AppWindow;
};

