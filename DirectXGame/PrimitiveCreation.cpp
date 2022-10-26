#include "PrimitiveCreation.h"

#include <iostream>

#include "AppWindow.h"

PrimitiveCreation* PrimitiveCreation::sharedInstance = nullptr;

PrimitiveCreation::PrimitiveCreation()
{

}

PrimitiveCreation::~PrimitiveCreation()
{

}

void PrimitiveCreation::GetCubeWithTexture(VertexBufferPtr& m_vb, IndexBufferPtr& m_ib)
{
	// Position Coords
	Vector3D position_list[] =
	{
		{ Vector3D(-0.5f,-0.5f,-0.5f)},
		{ Vector3D(-0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,-0.5f,-0.5f)},

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f) },
		{ Vector3D(0.5f,0.5f,0.5f) },
		{ Vector3D(-0.5f,0.5f,0.5f)},
		{ Vector3D(-0.5f,-0.5f,0.5f) }
	};

	// UV Coords
	Vector2D texcoord_list[] =
	{
		{ Vector2D(0.0f,0.0f) },
		{ Vector2D(0.0f,1.0f) },
		{ Vector2D(1.0f,0.0f) },
		{ Vector2D(1.0f,1.0f) }
	};

	// list of all the vertex_tex in the 3D Cube
	vertex_tex vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ position_list[0],texcoord_list[1] },
		{ position_list[1],texcoord_list[0] },
		{ position_list[2],texcoord_list[2] },
		{ position_list[3],texcoord_list[3] },


		{ position_list[4],texcoord_list[1] },
		{ position_list[5],texcoord_list[0] },
		{ position_list[6],texcoord_list[2] },
		{ position_list[7],texcoord_list[3] },


		{ position_list[1],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[2],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[0],texcoord_list[0] },
		{ position_list[3],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[3],texcoord_list[1] },
		{ position_list[2],texcoord_list[0] },
		{ position_list[5],texcoord_list[2] },
		{ position_list[4],texcoord_list[3] },

		{ position_list[7],texcoord_list[1] },
		{ position_list[6],texcoord_list[0] },
		{ position_list[1],texcoord_list[2] },
		{ position_list[0],texcoord_list[3] }


	};

	// list of all the triangle index with their vertex_tex compositions
	// this index list should reflect the vertex_tex list
	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
		//BACK SIDE
		4,5,6,
		6,7,4,
		//TOP SIDE
		8,9,10,
		10,11,8,
		//BOTTOM SIDE
		12,13,14,
		14,15,12,
		//RIGHT SIDE
		16,17,18,
		18,19,16,
		//LEFT SIDE
		20,21,22,
		22,23,20
	};

	UINT size_list = ARRAYSIZE(vertex_list);

	UINT size_index_list = ARRAYSIZE(index_list);

	// create IB
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer
	(index_list, size_index_list);

	// gets the byte code and size of the vertex_tex shader
	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::get()->getVertexShaderManager()->Get_VS_Default(&shader_byte_code, &size_shader);

	// create VB
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(
		vertex_list,
		sizeof(vertex_tex), size_list,
		shader_byte_code, size_shader);
}

void PrimitiveCreation::create()
{
	if (PrimitiveCreation::sharedInstance)
		throw std::exception("PrimitiveCreation already created");
	else
	{
		PrimitiveCreation::sharedInstance = new PrimitiveCreation();
	}
}

void PrimitiveCreation::release()
{
	if (!PrimitiveCreation::sharedInstance)
		return;
	delete PrimitiveCreation::sharedInstance;
}

PrimitiveCreation* PrimitiveCreation::Instance()
{
	return sharedInstance;
}
