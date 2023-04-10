#pragma once
#include <reactphysics3d/components/RigidBodyComponents.h>

#include "BNS_AGameObject.h"
#include "BNS_Prerequisites.h"
#include "BNS_StructHandler.h"
#include "Vector3D.h"

class IExecutionEvent;
class ThreadPool;

namespace reactphysics3d
{
	enum class BodyType;
}

enum class BNS_VertexShaderType;

class BNS_PrimitiveCreation
{
private:
	BNS_PrimitiveCreation();
	~BNS_PrimitiveCreation();
	BNS_PrimitiveCreation(BNS_PrimitiveCreation const&) {}; // copy constructor is private
	BNS_PrimitiveCreation& operator=(BNS_PrimitiveCreation const&) {}; // assignment operator is private

public:
	void ChangeVB_IB_Buffer(BNS_VertexShaderType vs_type, VertexBufferPtr& m_vb, IndexBufferPtr& m_ib, BNS_PC_Cube_ColorData color_data);
public:
	BNS_AGameObject* CreateCube(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreateCylinder(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreateTexturedCube(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreatePhysicsPlane(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreatePlane(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreateSphere(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreateCapsule(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreateTeapot(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreateStatue(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreateBunny(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreateArmadillo(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	BNS_AGameObject* CreateEarth(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	void CreateSkyBox();
	BNS_AGameObject* CreateScene(Vector3D pos = { 0,0,0 }, Vector3D scale = { 1,1,1 }, bool isAddedToScene = true);
	void CreateMeshFromFile(std::string full_filepath, std::string localName);

	void createPrimitiveFromFile(std::string name, BNS_ObjectTypes type, Vector3D pos, Vector3D rot, Vector3D scale, bool hasPhysics = false, float mass = 0, int BodyType = -1);

	BNS_AGameObject* CreatePhysicsCube(float x, float y, float z);

private:
	void CheckGameObjectName(std::string& name);
	void GetCube_Tex(VertexBufferPtr& m_vb, IndexBufferPtr& m_ib);
	void GetCube_Color(VertexBufferPtr& m_vb, IndexBufferPtr& m_ib, BNS_PC_Cube_ColorData color_data);
	void GetCube_Color_Lerp(VertexBufferPtr& m_vb, IndexBufferPtr& m_ib, BNS_PC_Cube_ColorData color_data);

public:
	static void create();
	static void release();
	static BNS_PrimitiveCreation* Instance();

private:
	Vector3D cube_positionList[8];

private:
	static BNS_PrimitiveCreation* sharedInstance;
	friend class BNS_Cube;

	// FOR P3
public:
	ThreadPool* threadPool;

	void LoadAScene(int index, IExecutionEvent* executionEvent);
};

