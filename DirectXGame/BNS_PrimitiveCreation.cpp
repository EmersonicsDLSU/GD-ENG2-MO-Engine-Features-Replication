#include "BNS_PrimitiveCreation.h"
#include <iostream>

#include "BNS_ActionHistory.h"
#include "BNS_AppWindow.h"
#include "BNS_Color.h"
#include "BNS_Cube.h"
#include "BNS_GameObjectManager.h"
#include "BNS_GraphicsEngine.h"
#include "BNS_PhysicsComponent.h"
#include "BNS_PhysicsSystem.h"
#include "BNS_Physics_Prerequisites.h"
#include "BNS_Plane.h"
#include "BNS_ShaderEngine.h"
#include "BNS_SkyBox.h"
#include "BNS_TransformComponent.h"
#include "BNS_MathUtils.h"
#include "StreamAssetLoader.h"
#include "ThreadPool.h"

BNS_PrimitiveCreation* BNS_PrimitiveCreation::sharedInstance = nullptr;

BNS_PrimitiveCreation::BNS_PrimitiveCreation()
{
	Vector3D cube_position[] =
	{
		//FRONT FACE
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
	UINT cube_size = sizeof(cube_position);
	::memcpy(cube_positionList, cube_position, cube_size);

	// 5 scenes only
	for (int i = 0; i < 5; ++i)
	{
		std::vector<BNS_AGameObject*> sceneObjects;
		sceneObjectDictionary[i] = sceneObjects;
	}


	threadPool = new ThreadPool("threadPool", 5);
	threadPool->StartScheduler();
}

BNS_PrimitiveCreation::~BNS_PrimitiveCreation()
{

}

void BNS_PrimitiveCreation::ChangeVB_IB_Buffer(BNS_VertexShaderType vs_type, VertexBufferPtr& m_vb, IndexBufferPtr& m_ib, BNS_PC_Cube_ColorData color_data)
{
	switch (vs_type)
	{
	case BNS_VertexShaderType::TEXTURE:
		GetCube_Tex(m_vb, m_ib);
		break;
	case BNS_VertexShaderType::COLOR:
		GetCube_Color(m_vb, m_ib, color_data);
		break;
	case BNS_VertexShaderType::COLOR_LERP:
		GetCube_Color_Lerp(m_vb, m_ib, color_data);
		break;
	}
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateCube(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "cube";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CUBE);
	cube->SetVertex_Index_Buffer(BNS_VertexShaderType::COLOR);
	cube->SetVertexShader(BNS_VertexShaderType::COLOR);
	cube->SetPixelShader(BNS_PixelShaderType::COLOR);
	cube->SetPosition(pos);
	cube->SetScale(scale);


	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);

	BNS_ActionHistory::GetInstance()->recordAction(cube->GetName());

	return cube;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateCylinder(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "Cylinder";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CYLINDER);
	cube->SetMesh(L"Assets\\Meshes\\Cylinder.OBJ");
	cube->SetTexture(L"Assets\\Textures\\GREY.png");
	cube->SetVertexShader(BNS_VertexShaderType::TEXTURE);
	cube->SetPixelShader(BNS_PixelShaderType::TEXTURE);
	cube->SetPosition(pos);
	cube->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	return cube;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateTexturedCube(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "physCube";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CUBE);
	cube->SetVertex_Index_Buffer(BNS_VertexShaderType::TEXTURE);
	cube->SetVertexShader(BNS_VertexShaderType::TEXTURE);
	cube->SetPixelShader(BNS_PixelShaderType::TEXTURE);
	cube->SetTexture(L"Assets\\Textures\\brick.png");
	cube->SetPosition(pos);
	cube->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);
	// adding physics component
	BNS_PhysicsComponent* physicsComp = new BNS_PhysicsComponent("PhysCube", cube);
	cube->AttachComponent(physicsComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	return cube;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreatePhysicsPlane(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "phys_plane";
	CheckGameObjectName(name);
	BNS_Plane* plane = new BNS_Plane(name, BNS_ObjectTypes::PLANE);
	plane->SetVertex_Index_Buffer(BNS_VertexShaderType::COLOR);
	plane->SetVertexShader(BNS_VertexShaderType::COLOR);
	plane->SetPixelShader(BNS_PixelShaderType::COLOR);
	plane->SetPosition(pos);
	plane->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", plane);
	plane->AttachComponent(transformComp);


	// adding physics component
	BNS_PhysicsComponent* physicsComp = new BNS_PhysicsComponent("PhysPlane", plane);
	physicsComp->GetRigidBody()->setType(BodyType::KINEMATIC);
	plane->AttachComponent(physicsComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(plane);
	return plane;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreatePlane(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "plane";
	CheckGameObjectName(name);
	BNS_Plane* plane = new BNS_Plane(name, BNS_ObjectTypes::PLANE);
	plane->SetVertex_Index_Buffer(BNS_VertexShaderType::COLOR);
	plane->SetVertexShader(BNS_VertexShaderType::COLOR);
	plane->SetPixelShader(BNS_PixelShaderType::COLOR);
	plane->SetPosition(pos);
	plane->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", plane);
	plane->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(plane);
	return plane;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateTeapot(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "teapot";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CUBE);
	cube->SetMesh(L"Assets\\Meshes\\teapot.obj");
	cube->SetTexture(L"Assets\\Textures\\brick.png");
	cube->SetVertexShader(BNS_VertexShaderType::TEXTURE);
	cube->SetPixelShader(BNS_PixelShaderType::MESH);
	cube->SetPosition(pos);
	cube->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	return cube;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateStatue(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "statue";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CUBE);
	cube->SetMesh(L"Assets\\Meshes\\statue.obj");
	cube->SetVertexShader(BNS_VertexShaderType::LIGHTING);
	cube->SetPixelShader(BNS_PixelShaderType::LIGHTING);
	cube->SetPosition(pos);
	cube->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	return cube;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateBunny(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "bunny";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CUBE);
	cube->SetMesh(L"Assets\\Meshes\\bunny.obj");
	cube->SetVertexShader(BNS_VertexShaderType::TEXTURE);
	cube->SetPixelShader(BNS_PixelShaderType::MESH);
	cube->SetTexture(L"Assets\\Textures\\brick.png");
	cube->SetPosition(pos);
	cube->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	return cube;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateArmadillo(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "armadillo";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CUBE);
	cube->SetMesh(L"Assets\\Meshes\\armadillo.obj");
	cube->SetTexture(L"Assets\\Textures\\brick.png");
	cube->SetVertexShader(BNS_VertexShaderType::TEXTURE);
	cube->SetPixelShader(BNS_PixelShaderType::MESH);
	cube->SetPosition(pos);
	cube->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	return cube;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateEarth(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "earth";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CUBE);
	cube->SetMesh(L"Assets\\Meshes\\sphere.obj");
	cube->SetTexture(L"Assets\\Textures\\earth_color.jpg");
	cube->SetTexture(L"Assets\\Textures\\earth_spec.jpg");
	cube->SetTexture(L"Assets\\Textures\\clouds.jpg");
	cube->SetTexture(L"Assets\\Textures\\earth_night.jpg");
	cube->SetVertexShader(BNS_VertexShaderType::LIGHTING);
	cube->SetPixelShader(BNS_PixelShaderType::EARTH);
	cube->SetPosition(pos);
	cube->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	return cube;
}

void BNS_PrimitiveCreation::CreateSkyBox()
{
	std::string name = "skybox";
	CheckGameObjectName(name);
	BNS_SkyBox* sky_box = new BNS_SkyBox(name, BNS_ObjectTypes::SKYBOX);
	sky_box->SetMesh(L"Assets\\Meshes\\sphere.obj");
	sky_box->SetTexture(L"Assets\\Textures\\stars_map.jpg");
	sky_box->SetVertexShader(BNS_VertexShaderType::LIGHTING);
	sky_box->SetPixelShader(BNS_PixelShaderType::SKYBOX);
	sky_box->SetPosition(Vector3D{ 0, 0, 0 });
	
	BNS_GameObjectManager::get()->SetSkyBox(sky_box);
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateScene(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "scene";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CUBE);
	cube->SetMesh(L"Assets\\Meshes\\scene.obj");
	cube->SetTexture(L"Assets\\Textures\\wall.jpg");
	cube->SetVertexShader(BNS_VertexShaderType::POINT_LIGHT);
	cube->SetPixelShader(BNS_PixelShaderType::POINT_LIGHT);
	cube->SetPosition(pos);
	cube->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	return cube;
}

void BNS_PrimitiveCreation::CreateMeshFromFile(std::string full_filepath, std::string localName)
{
	
	std::wstring tempPath = std::wstring(full_filepath.begin(), full_filepath.end());
	LPCWSTR convertedFilepath = tempPath.c_str();

	std::string tempname;
	std::string::size_type pos = localName.find('.');
	if (pos != std::string::npos)
	{
		tempname = localName.substr(0, pos);
	}
	else
	{
		tempname = localName;
	}
	std::string name = tempname.c_str();
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CUBE);
	cube->SetVertex_Index_Buffer(BNS_VertexShaderType::TEXTURE);
	cube->SetVertexShader(BNS_VertexShaderType::TEXTURE);
	cube->SetPixelShader(BNS_PixelShaderType::TEXTURE);
	cube->SetMesh(convertedFilepath);
	cube->SetTexture(L"Assets\\Textures\\brick.png");
	cube->SetPosition(Vector3D{ 0, 0, 0 });

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	

}

void BNS_PrimitiveCreation::createPrimitiveFromFile(std::string name, BNS_ObjectTypes type, Vector3D pos, Vector3D rot, Vector3D scale, bool hasPhysics, float mass, int BodyType)
{
	BNS_AGameObject* obj;

	if (type == BNS_ObjectTypes::CUBE)
	{
		std::cout << "create cube" << std::endl;
		obj = new BNS_Cube(name, type);
		CheckGameObjectName(obj->GetName());
		dynamic_cast<BNS_Cube*>(obj)->SetTexture(L"Assets\\Textures\\sand.jpg");
		//dynamic_cast<BNS_Plane*>(obj)->SetVertex_Index_Buffer(BNS_VertexShaderType::TEXTURE);
		dynamic_cast<BNS_Plane*>(obj)->SetVertexShader(BNS_VertexShaderType::TEXTURE);
		dynamic_cast<BNS_Plane*>(obj)->SetPixelShader(BNS_PixelShaderType::TEXTURE);
	}

	else if (type == BNS_ObjectTypes::PLANE)
	{
		
		obj = new BNS_Plane(name, type);
		CheckGameObjectName(obj->GetName());
		dynamic_cast<BNS_Cube*>(obj)->SetTexture(L"Assets\\Textures\\brick.png");
		//dynamic_cast<BNS_Plane*>(obj)->SetVertex_Index_Buffer(BNS_VertexShaderType::TEXTURE);
		dynamic_cast<BNS_Plane*>(obj)->SetVertexShader(BNS_VertexShaderType::TEXTURE);
		dynamic_cast<BNS_Plane*>(obj)->SetPixelShader(BNS_PixelShaderType::TEXTURE);
	}

	else if (type == BNS_ObjectTypes::SPHERE)
	{
		std::cout << "create sphere" << std::endl;
		obj = new BNS_Cube(name, BNS_ObjectTypes::SPHERE);
		CheckGameObjectName(obj->GetName());
		dynamic_cast<BNS_Cube*>(obj)->SetMesh(L"Assets\\Meshes\\sphere.obj");
		dynamic_cast<BNS_Cube*>(obj)->SetVertexShader(BNS_VertexShaderType::COLOR);
		dynamic_cast<BNS_Cube*>(obj)->SetPixelShader(BNS_PixelShaderType::COLOR);
	}

	else if (type == BNS_ObjectTypes::CAPSULE)
	{
		std::cout << "create capsule" << std::endl;
		obj = new BNS_Cube(name, BNS_ObjectTypes::CAPSULE);
		CheckGameObjectName(obj->GetName());
		dynamic_cast<BNS_Cube*>(obj)->SetMesh(L"Assets\\Meshes\\capsule.obj");
		dynamic_cast<BNS_Cube*>(obj)->SetVertexShader(BNS_VertexShaderType::COLOR);
		dynamic_cast<BNS_Cube*>(obj)->SetPixelShader(BNS_PixelShaderType::COLOR);
	}

	else if (type == BNS_ObjectTypes::CYLINDER)
	{
		std::cout << "create cylinder" << std::endl;
		obj = new BNS_Cube(name, BNS_ObjectTypes::CYLINDER);
		CheckGameObjectName(obj->GetName());
		dynamic_cast<BNS_Cube*>(obj)->SetMesh(L"Assets\\Meshes\\Cylinder.OBJ");
		dynamic_cast<BNS_Cube*>(obj)->SetTexture(L"Assets\\Textures\\GREY.png");
		dynamic_cast<BNS_Cube*>(obj)->SetVertexShader(BNS_VertexShaderType::TEXTURE);
		dynamic_cast<BNS_Cube*>(obj)->SetPixelShader(BNS_PixelShaderType::TEXTURE);
	}

	else
	{
		return;
	}
	
	obj->SetPosition(pos);
	obj->SetRotation(rot);
	obj->SetScale(scale);

	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", obj);
	obj->AttachComponent(transformComp);

	if (hasPhysics)
	{
		BNS_PhysicsComponent* physicsComp = new BNS_PhysicsComponent("PhysPlane", obj);
		physicsComp->GetRigidBody()->setType((reactphysics3d::BodyType)BodyType);
		physicsComp->GetRigidBody()->setMass(mass);

		reactphysics3d::BodyType bodyType = physicsComp->GetRigidBody()->getType();
		physicsComp->UpdateRigidBody();
		physicsComp->GetRigidBody()->setType(bodyType);

		obj->AttachComponent(physicsComp);
	}

	BNS_GameObjectManager::get()->GetObjectList().emplace_back(obj);
	BNS_ActionHistory::GetInstance()->recordAction(obj->GetName());
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateSphere(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "sphere";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::SPHERE);
	cube->SetMesh(L"Assets\\Meshes\\sphere.obj");
	cube->SetVertexShader(BNS_VertexShaderType::COLOR);
	cube->SetPixelShader(BNS_PixelShaderType::COLOR);
	cube->SetPosition(pos);
	cube->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	return cube;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreateCapsule(Vector3D pos, Vector3D scale, bool isAddedToScene)
{
	std::string name = "capsule";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CAPSULE);
	cube->SetMesh(L"Assets\\Meshes\\capsule.obj");
	cube->SetVertexShader(BNS_VertexShaderType::COLOR);
	cube->SetPixelShader(BNS_PixelShaderType::COLOR);
	cube->SetPosition(pos);
	cube->SetScale(scale);

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);

	if (isAddedToScene)
		BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	return cube;
}

BNS_AGameObject* BNS_PrimitiveCreation::CreatePhysicsCube(float x, float y, float z)
{
	std::string name = "cube";
	CheckGameObjectName(name);
	BNS_Cube* cube = new BNS_Cube(name, BNS_ObjectTypes::CUBE);
	cube->SetVertex_Index_Buffer(BNS_VertexShaderType::TEXTURE);
	cube->SetVertexShader(BNS_VertexShaderType::TEXTURE);
	cube->SetPixelShader(BNS_PixelShaderType::TEXTURE);
	cube->SetTexture(L"Assets\\Textures\\brick.png");

	cube->SetPosition(Vector3D(x, y, z));

	// adding transform component
	BNS_TransformComponent* transformComp = new BNS_TransformComponent("PhysTransform", cube);
	cube->AttachComponent(transformComp);
	// adding physics component
	BNS_PhysicsComponent* physicsComp = new BNS_PhysicsComponent("PhysCube", cube);
	cube->AttachComponent(physicsComp);

	BNS_AComponent* physics_comp = cube->FindComponentOfType(ComponentType::Physics);
	if (physics_comp != nullptr)
	{
		BNS_PhysicsComponent* physicsComp = dynamic_cast<BNS_PhysicsComponent*>(physics_comp);
		BodyType bodyType = physicsComp->GetRigidBody()->getType();
		physicsComp->UpdateRigidBody();
		physicsComp->GetRigidBody()->setType(bodyType);
	}

	BNS_GameObjectManager::get()->GetObjectList().emplace_back(cube);
	BNS_ActionHistory::GetInstance()->recordAction(cube->GetName());

	return cube;
}

void BNS_PrimitiveCreation::CheckGameObjectName(std::string& name)
{
	bool isValid = true;
	int iterator = 1;
	std::string origName = name;
	do
	{
		for (auto obj : BNS_GameObjectManager::get()->GetObjectList())
		{
			if (name == obj->GetName())
			{
				isValid = false;
				name = origName + "(" + std::to_string(iterator++) + ")";
				continue;
			}
		}
		isValid = true;
	} while (!isValid);
}

void BNS_PrimitiveCreation::GetCube_Tex(VertexBufferPtr& m_vb, IndexBufferPtr& m_ib)
{

	// UV Coords
	Vector2D texcoord_list[] =
	{
		{ Vector2D(0.0f,0.0f) },
		{ Vector2D(0.0f,1.0f) },
		{ Vector2D(1.0f,0.0f) },
		{ Vector2D(1.0f,1.0f) }
	};

	// list of all the BNS_vertex_tex in the 3D BNS_Cube
	BNS_vertex_tex vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ cube_positionList[0],texcoord_list[1] },
		{ cube_positionList[1],texcoord_list[0] },
		{ cube_positionList[2],texcoord_list[2] },
		{ cube_positionList[3],texcoord_list[3] },


		{ cube_positionList[4],texcoord_list[1] },
		{ cube_positionList[5],texcoord_list[0] },
		{ cube_positionList[6],texcoord_list[2] },
		{ cube_positionList[7],texcoord_list[3] },


		{ cube_positionList[1],texcoord_list[1] },
		{ cube_positionList[6],texcoord_list[0] },
		{ cube_positionList[5],texcoord_list[2] },
		{ cube_positionList[2],texcoord_list[3] },

		{ cube_positionList[7],texcoord_list[1] },
		{ cube_positionList[0],texcoord_list[0] },
		{ cube_positionList[3],texcoord_list[2] },
		{ cube_positionList[4],texcoord_list[3] },

		{ cube_positionList[3],texcoord_list[1] },
		{ cube_positionList[2],texcoord_list[0] },
		{ cube_positionList[5],texcoord_list[2] },
		{ cube_positionList[4],texcoord_list[3] },

		{ cube_positionList[7],texcoord_list[1] },
		{ cube_positionList[6],texcoord_list[0] },
		{ cube_positionList[1],texcoord_list[2] },
		{ cube_positionList[0],texcoord_list[3] }


	};

	// list of all the triangle index with their BNS_vertex_tex compositions
	// this index list should reflect the BNS_vertex_tex list
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
	m_ib = BNS_GraphicsEngine::get()->getRenderSystem()->CreateIndexBuffer
	(index_list, size_index_list);

	// create VB
	m_vb = BNS_GraphicsEngine::get()->getRenderSystem()->CreateVertexBuffer(
		vertex_list,
		sizeof(BNS_vertex_tex), size_list,
		BNS_InputLayoutType::TEXTURE);
}

void BNS_PrimitiveCreation::GetCube_Color(VertexBufferPtr& m_vb, IndexBufferPtr& m_ib, BNS_PC_Cube_ColorData color_data)
{
	// list of all the BNS_vertex_tex in the 3D BNS_Cube
	BNS_vertex_color vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ cube_positionList[0],color_data.color_list_1[1] },
		{ cube_positionList[1],color_data.color_list_1[0] },
		{ cube_positionList[2],color_data.color_list_1[2] },
		{ cube_positionList[3],color_data.color_list_1[3] },

		{ cube_positionList[4],color_data.color_list_1[1] },
		{ cube_positionList[5],color_data.color_list_1[0] },
		{ cube_positionList[6],color_data.color_list_1[2] },
		{ cube_positionList[7],color_data.color_list_1[3] },

		{ cube_positionList[1],color_data.color_list_1[1] },
		{ cube_positionList[6],color_data.color_list_1[0] },
		{ cube_positionList[5],color_data.color_list_1[2] },
		{ cube_positionList[2],color_data.color_list_1[3] },

		{ cube_positionList[7],color_data.color_list_1[1] },
		{ cube_positionList[0],color_data.color_list_1[0] },
		{ cube_positionList[3],color_data.color_list_1[2] },
		{ cube_positionList[4],color_data.color_list_1[3] },

		{ cube_positionList[3],color_data.color_list_1[1] },
		{ cube_positionList[2],color_data.color_list_1[0] },
		{ cube_positionList[5],color_data.color_list_1[2] },
		{ cube_positionList[4],color_data.color_list_1[3] },

		{ cube_positionList[7],color_data.color_list_1[1] },
		{ cube_positionList[6],color_data.color_list_1[0] },
		{ cube_positionList[1],color_data.color_list_1[2] },
		{ cube_positionList[0],color_data.color_list_1[3] }


	};

	// list of all the triangle index with their BNS_vertex_tex compositions
	// this index list should reflect the BNS_vertex_tex list
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
	m_ib = BNS_GraphicsEngine::get()->getRenderSystem()->CreateIndexBuffer
	(index_list, size_index_list);
	

	// create VB
	m_vb = BNS_GraphicsEngine::get()->getRenderSystem()->CreateVertexBuffer(
		vertex_list,
		sizeof(BNS_vertex_color), size_list,
		BNS_InputLayoutType::COLOR);
}

void BNS_PrimitiveCreation::GetCube_Color_Lerp(VertexBufferPtr& m_vb, IndexBufferPtr& m_ib, BNS_PC_Cube_ColorData color_data)
{
	// list of all the BNS_vertex_tex in the 3D BNS_Cube
	BNS_vertex_color_lerp vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ cube_positionList[0],color_data.color_list_1[1], color_data.color_list_2[1] },
		{ cube_positionList[1],color_data.color_list_1[0], color_data.color_list_2[0] },
		{ cube_positionList[2],color_data.color_list_1[2], color_data.color_list_2[2] },
		{ cube_positionList[3],color_data.color_list_1[3], color_data.color_list_2[3] },

		{ cube_positionList[4],color_data.color_list_1[1], color_data.color_list_2[1] },
		{ cube_positionList[5],color_data.color_list_1[0], color_data.color_list_2[0] },
		{ cube_positionList[6],color_data.color_list_1[2], color_data.color_list_2[2] },
		{ cube_positionList[7],color_data.color_list_1[3], color_data.color_list_2[3] },

		{ cube_positionList[1],color_data.color_list_1[1], color_data.color_list_2[1] },
		{ cube_positionList[6],color_data.color_list_1[0], color_data.color_list_2[0] },
		{ cube_positionList[5],color_data.color_list_1[2], color_data.color_list_2[2] },
		{ cube_positionList[2],color_data.color_list_1[3], color_data.color_list_2[3] },

		{ cube_positionList[7],color_data.color_list_1[1], color_data.color_list_2[1] },
		{ cube_positionList[0],color_data.color_list_1[0], color_data.color_list_2[0] },
		{ cube_positionList[3],color_data.color_list_1[2], color_data.color_list_2[2] },
		{ cube_positionList[4],color_data.color_list_1[3], color_data.color_list_2[3] },

		{ cube_positionList[3],color_data.color_list_1[1], color_data.color_list_2[1] },
		{ cube_positionList[2],color_data.color_list_1[0], color_data.color_list_2[0] },
		{ cube_positionList[5],color_data.color_list_1[2], color_data.color_list_2[2] },
		{ cube_positionList[4],color_data.color_list_1[3], color_data.color_list_2[3] },

		{ cube_positionList[7],color_data.color_list_1[1], color_data.color_list_2[1] },
		{ cube_positionList[6],color_data.color_list_1[0], color_data.color_list_2[0] },
		{ cube_positionList[1],color_data.color_list_1[2], color_data.color_list_2[2] },
		{ cube_positionList[0],color_data.color_list_1[3], color_data.color_list_2[3] }
	};

	// list of all the triangle index with their BNS_vertex_tex compositions
	// this index list should reflect the BNS_vertex_tex list
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
	m_ib = BNS_GraphicsEngine::get()->getRenderSystem()->CreateIndexBuffer
	(index_list, size_index_list);

	// create VB
	m_vb = BNS_GraphicsEngine::get()->getRenderSystem()->CreateVertexBuffer(
		vertex_list,
		sizeof(BNS_vertex_color_lerp), size_list,
		BNS_InputLayoutType::COLOR_LERP);
}


void BNS_PrimitiveCreation::create()
{
	if (BNS_PrimitiveCreation::sharedInstance)
		throw std::exception("BNS_PrimitiveCreation already created");
	else
	{
		BNS_PrimitiveCreation::sharedInstance = new BNS_PrimitiveCreation();
	}
}

void BNS_PrimitiveCreation::release()
{
	if (!BNS_PrimitiveCreation::sharedInstance)
		return;
	delete BNS_PrimitiveCreation::sharedInstance;
}

BNS_PrimitiveCreation* BNS_PrimitiveCreation::Instance()
{
	return sharedInstance;
}

void BNS_PrimitiveCreation::ShowScene(int sceneIndex)
{
	if (sceneObjectDictionary[sceneIndex].size() < maxPopulation) return;

	HideAll();
	// show the objects in the specific scene
	for (auto obj : sceneObjectDictionary[sceneIndex])
	{
		obj->SetActive(true);
	}
}

void BNS_PrimitiveCreation::ResetScene(int sceneIndex)
{
	if (sceneObjectDictionary[sceneIndex].size() < maxPopulation) return;

	// delete all the objects
	for (auto obj : sceneObjectDictionary[sceneIndex])
	{
		BNS_GameObjectManager::get()->DeleteObject(obj);
	}
	sceneObjectDictionary[sceneIndex].clear();
	sceneObjectDictionary[sceneIndex].shrink_to_fit();
}

void BNS_PrimitiveCreation::ShowAll()
{
	for (auto it = sceneObjectDictionary.begin(); it != sceneObjectDictionary.end(); ++it) {
		int key = it->first;
		std::vector<BNS_AGameObject*>& sceneObjectsRef = it->second;

		for (auto objs : sceneObjectsRef)
		{
			objs->SetActive(true);
		}
	}
}

void BNS_PrimitiveCreation::HideAll()
{
	for (auto it = sceneObjectDictionary.begin(); it != sceneObjectDictionary.end(); ++it) {
		int key = it->first;
		std::vector<BNS_AGameObject*>& sceneObjectsRef = it->second;

		for (auto objs : sceneObjectsRef)
		{
			objs->SetActive(false);
		}
	}
}

// This is called by a class(inherited by IExecutionEvent) that calls a function(switch 5 cases) to process the scene
void BNS_PrimitiveCreation::LoadAScene(int index, IExecutionEvent* executionEvent)
{
	StreamAssetLoader* assetLoader = new StreamAssetLoader(index, executionEvent);

	threadPool->ScheduleTask(assetLoader);
}
