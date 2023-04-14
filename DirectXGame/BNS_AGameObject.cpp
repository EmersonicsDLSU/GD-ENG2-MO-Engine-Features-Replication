#include "BNS_AGameObject.h"
#include "BNS_AComponent.h"
#include "BNS_Camera.h"
#include "BNS_CameraHandler.h"
#include "BNS_ConstantBuffer.h"
#include "BNS_EditorAction.h"
#include "BNS_EngineTime.h"
#include "BNS_GraphicsEngine.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "IMGUI/imgui.h"

BNS_AGameObject::BNS_AGameObject(std::string name, BNS_ObjectTypes type) : name(name), ObjectType(type)
{
	m_position = Vector3D::zero();
	m_scale = Vector3D::one();
	m_rotation = Vector3D::zero();
	this->active = true;
	this->lastEditState = NULL;
	
}

BNS_AGameObject::~BNS_AGameObject()
{
	if (componentList.empty())
		return;
	for (int i = 0; i < componentList.size(); ++i)
	{
		componentList[i]->DetachOwner();
	}
	componentList.clear();
	//std::cout << "Deleted Object: " << this->GetName() << std::endl;
}

void BNS_AGameObject::UpdateTransform()
{
	// light matrix
	Matrix4x4 m_light_rot_matrix;
	m_light_rot_matrix.setRotationY(m_light_rot_y);
	m_light_rot_y += 1.57f * BNS_EngineTime::getDeltaTime();
	// update point light position
	float dist_from_origin = 1.0f;
	cc.m_light_position = Vector4D(cos(m_light_rot_y) * dist_from_origin, 3.1f, sin(m_light_rot_y) * dist_from_origin, 1.0f);
	cc.m_light_radius = 100.0f;
	cc.m_light_direction = m_light_rot_matrix.getZDirection();
	// alpha update
	cc.alpha = alpha;
	// update time
	ticks += BNS_EngineTime::getDeltaTime();
	cc.m_time = ticks;
	m_cb->update(BNS_GraphicsEngine::get()->getRenderSystem()->GetImmediateDeviceContext(), &cc);

	// check if its a child
	if (isChild)
	{
		// don't transform; parent will do the transform
		return;
	}

	// objects matrix
	Matrix4x4 temp;
	cc.m_world.setIdentity();

	if (overrideMatrix) {
		cc.m_world = m_matrix;
	}
	else
	{
		temp.setScale(m_scale);
		cc.m_world *= temp;
		temp.setRotationX(m_rotation.m_x);
		cc.m_world *= temp;
		temp.setRotationY(m_rotation.m_y);
		cc.m_world *= temp;
		temp.setRotationZ(m_rotation.m_z);
		cc.m_world *= temp;
		temp.setTranslation(m_position);
		cc.m_world *= temp;
		// update m_matrix
		m_matrix = cc.m_world;
	}
	// creating the camera matrix
	cc.m_view = BNS_CameraHandler::GetInstance()->GetSceneCameraViewMatrix();
	// setting the perspective projection
	cc.m_proj = BNS_CameraHandler::GetInstance()->GetSceneCameraProjMatrix();
	// update camera position
	cc.m_camera_position = BNS_CameraHandler::GetInstance()->GetSceneCamera()->GetMatrix().getTranslation();

	m_cb->update(BNS_GraphicsEngine::get()->getRenderSystem()->GetImmediateDeviceContext(), &cc);

	// update child transforms
	UpdateChildListTransform();
}

void BNS_AGameObject::Draw(const BlenderPtr& m_blender)
{

}

void BNS_AGameObject::UpdateChildListTransform()
{
	for (auto child : childList)
	{
		child->ChildTransform(m_matrix);
	}
}

void BNS_AGameObject::ChildTransform(Matrix4x4 parent_trans)
{
	if (GetName() == "cube2")
	{
		std::cout << "PARENT: " << "X: " << parent->m_position.m_x << " Y: " << parent->m_position.m_y << " Z: " << parent->m_position.m_z << std::endl;
		std::cout << "CHILD: " << "X: " << m_position.m_x << " Y: " << m_position.m_y << " Z: " << m_position.m_z << std::endl;
	}

	m_matrix = parent->m_matrix.MultiplyTo(m_matrix);

	SetPosition(m_matrix.getTranslation());
	SetScale(m_matrix.getScale());
	//SetRotation(m_matrix.getTranslation());

	cc.m_world = m_matrix;

	// creating the camera matrix
	cc.m_view = BNS_CameraHandler::GetInstance()->GetSceneCameraViewMatrix();
	// setting the perspective projection
	cc.m_proj = BNS_CameraHandler::GetInstance()->GetSceneCameraProjMatrix();
	// update camera position
	cc.m_camera_position = BNS_CameraHandler::GetInstance()->GetSceneCamera()->GetMatrix().getTranslation();

	m_cb->update(BNS_GraphicsEngine::get()->getRenderSystem()->GetImmediateDeviceContext(), &cc);

	// update child transforms
	UpdateChildListTransform();
}

void BNS_AGameObject::SetTransform(Vector3D position, Vector3D scale, Vector3D rotation)
{
	m_position = position;
	m_scale = scale;
	m_rotation = rotation;
}

void BNS_AGameObject::SetPosition(float x, float y, float z)
{
	m_position = Vector3D{ x,y,z };
	/*
	AComponentPtr temp = FindComponentOfType(ComponentType::Physics);
	if ( temp != nullptr)
	{
		dynamic_cast<BNS_PhysicsComponent*>(temp.get())->UpdateRigidBody();
	}
	*/
	this->overrideMatrix = false;
}

void BNS_AGameObject::SetPosition(Vector3D pos)
{
	m_position = pos;
	/*
	AComponentPtr temp = FindComponentOfType(ComponentType::Physics);
	if ( temp != nullptr)
	{
		dynamic_cast<BNS_PhysicsComponent*>(temp.get())->UpdateRigidBody();
	}
	*/
	this->overrideMatrix = false;
}

Vector3D BNS_AGameObject::GetLocalPosition()
{
	return m_position;
}

void BNS_AGameObject::SetScale(float x, float y, float z)
{
	m_scale = Vector3D{ x,y,z };
	/*
	AComponentPtr temp = FindComponentOfType(ComponentType::Physics);
	if ( temp != nullptr)
	{
		dynamic_cast<BNS_PhysicsComponent*>(temp.get())->UpdateRigidBody();
	}
	*/
	this->overrideMatrix = false;
}

void BNS_AGameObject::SetScale(Vector3D scale)
{
	m_scale = scale;
	/*
	AComponentPtr temp = FindComponentOfType(ComponentType::Physics);
	if ( temp != nullptr)
	{
		dynamic_cast<BNS_PhysicsComponent*>(temp.get())->UpdateRigidBody();
	}
	*/
	this->overrideMatrix = false;
}

Vector3D BNS_AGameObject::GetLocalScale()
{
	return m_scale;
}

void BNS_AGameObject::SetRotation(float x, float y, float z)
{
	m_rotation = Vector3D{ x,y,z };
	/*
	AComponentPtr temp = FindComponentOfType(ComponentType::Physics);
	if ( temp != nullptr)
	{
		dynamic_cast<BNS_PhysicsComponent*>(temp.get())->UpdateRigidBody();
	}
	*/
	this->overrideMatrix = false;
}

void BNS_AGameObject::SetRotation(Vector3D rot)
{
	m_rotation = rot;
	/*
	AComponentPtr temp = FindComponentOfType(ComponentType::Physics);
	if ( temp != nullptr)
	{
		dynamic_cast<BNS_PhysicsComponent*>(temp.get())->UpdateRigidBody();
	}
	*/
	this->overrideMatrix = false;
}

Vector3D BNS_AGameObject::GetLocalRotation()
{
	return m_rotation;
}

std::string& BNS_AGameObject::GetName()
{
	return name;
}

Matrix4x4 BNS_AGameObject::GetMatrix()
{
	return m_matrix;
}

void BNS_AGameObject::AttachComponent(BNS_AComponent* component)
{
	componentList.push_back(component);
	component->AttachOwner(this);
}

void BNS_AGameObject::DetachComponent(BNS_AComponent* component)
{
	int index = -1;
	for (int i = 0; i < this->componentList.size(); i++) {
		if (this->componentList[i] == component) {
			index = i;
			break;
		}
	}
	if (index != -1) {
		this->componentList.erase(this->componentList.begin() + index);
	}
}

BNS_AComponent* BNS_AGameObject::FindComponentByName(String name)
{
	for (int i = 0; i < this->componentList.size(); i++) {
		if (this->componentList[i]->GetName() == name) {
			return this->componentList[i];
		}
	}

	return nullptr;
}

BNS_AComponent* BNS_AGameObject::FindComponentOfType(ComponentType type, String name)
{
	if (name.empty())
	{
		for (int i = 0; i < componentList.size(); i++) {
			if (componentList[i]->GetType() == type) {
				return componentList[i];
			}
		}
	}
	else
	{
		for (int i = 0; i < componentList.size(); i++) {
			if (componentList[i]->GetName() == name && componentList[i]->GetType() == type) {
				return componentList[i];
			}
		}
	}

	return nullptr;
}

BNS_AGameObject::ComponentList BNS_AGameObject::GetComponentsOfType(ComponentType type)
{
	ComponentList foundList;
	for (int i = 0; i < this->componentList.size(); i++) {
		if (this->componentList[i]->GetType() == type) {
			foundList.push_back(this->componentList[i]);
		}
	}

	return foundList;
}

BNS_AGameObject::ComponentList BNS_AGameObject::GetComponentsOfTypeRecursive(ComponentType type)
{
	ComponentList foundList;
	for (int i = 0; i < this->componentList.size(); i++) {
		if (this->componentList[i]->GetType() == type) {
			foundList.push_back(this->componentList[i]);
		}
	}

	return foundList;
}

bool BNS_AGameObject::GetActive()
{
	return this->active;
}

void BNS_AGameObject::SetActive(bool flag)
{
	this->active = flag;
}

void BNS_AGameObject::saveEditState()
{
	if (this->lastEditState == NULL) {
		this->lastEditState = new BNS_EditorAction(this);
	}
}

void BNS_AGameObject::restoreEditState()
{
	if (this->lastEditState != NULL) {
		this->RecomputeMatrix(this->lastEditState->GetStoredMatrix().GetMatrix());
		this->SetPosition(this->lastEditState->GetStoredPos());
		this->SetScale(this->lastEditState->GetStoredScale());
		this->SetRotation(this->lastEditState->GetRotation());

		BNS_AComponent* physics_comp = this->FindComponentOfType(ComponentType::Physics);
		if (physics_comp != nullptr)
		{
			BNS_PhysicsComponent* physicsComp = dynamic_cast<BNS_PhysicsComponent*>(physics_comp);
			BodyType bodyType = physicsComp->GetRigidBody()->getType();
			physicsComp->UpdateRigidBody();
			physicsComp->GetRigidBody()->setType(bodyType);
		}
			

		this->lastEditState = NULL;
	}
	else {
		std::cout << "Edit state is null. Cannot restore. \n";
	}
}

void BNS_AGameObject::AttachParent(BNS_AGameObject* parent)
{
	this->parent = parent;
	isChild = true;
}

void BNS_AGameObject::DetachParent()
{
	parent = nullptr;
	isChild = false;
}

void BNS_AGameObject::AttachChild(BNS_AGameObject* child)
{
	childList.push_back(child);
	child->AttachParent(this);
}

void BNS_AGameObject::DetachChild(BNS_AGameObject* child)
{
	int index = -1;
	for (int i = 0; i < this->childList.size(); i++) {
		if (childList[i] == child) {
			index = i;
			break;
		}
	}
	if (index != -1) {
		childList[index]->DetachParent();
		childList.erase(childList.begin() + index);
	}
}

BNS_AGameObject* BNS_AGameObject::GetParent()
{
	return parent;
}

BNS_AGameObject::ChildList BNS_AGameObject::GetChildList()
{
	return childList;
}


void BNS_AGameObject::SetAlpha(float alpha)
{
	this->alpha = alpha;
}

float BNS_AGameObject::GetAlpha()
{
	return alpha;
}

float* BNS_AGameObject::GetPhysicsLocalMatrix()
{
	Matrix4x4 allMatrix; allMatrix.setIdentity();
	Matrix4x4 translationMatrix; translationMatrix.setIdentity();
	translationMatrix.setTranslation(this->GetLocalPosition());
	Matrix4x4 scaleMatrix; scaleMatrix.setScale(Vector3D::one()); //physics 3D only accepts uniform scale for rigidbody
	Vector3D rotation = this->GetLocalRotation();
	Matrix4x4 xMatrix; xMatrix.setRotationX(rotation.m_x);
	Matrix4x4 yMatrix; yMatrix.setRotationY(rotation.m_y);
	Matrix4x4 zMatrix; zMatrix.setRotationZ(rotation.m_z);

	//Scale --> Rotate --> Transform as recommended order.
	Matrix4x4 rotMatrix; rotMatrix.setIdentity();
	rotMatrix = rotMatrix.MultiplyTo(xMatrix.MultiplyTo(yMatrix.MultiplyTo(zMatrix)));

	allMatrix = allMatrix.MultiplyTo(rotMatrix.MultiplyTo(scaleMatrix));
	allMatrix = allMatrix.MultiplyTo(translationMatrix);

	return allMatrix.GetMatrix();
}


void BNS_AGameObject::RecomputeMatrix(float matrix[16])
{
	float matrix4x4[4][4];
	matrix4x4[0][0] = matrix[0];
	matrix4x4[0][1] = matrix[1];
	matrix4x4[0][2] = matrix[2];
	matrix4x4[0][3] = matrix[3];

	matrix4x4[1][0] = matrix[4];
	matrix4x4[1][1] = matrix[5];
	matrix4x4[1][2] = matrix[6];
	matrix4x4[1][3] = matrix[7];

	matrix4x4[2][0] = matrix[8];
	matrix4x4[2][1] = matrix[9];
	matrix4x4[2][2] = matrix[10];
	matrix4x4[2][3] = matrix[11];

	matrix4x4[3][0] = matrix[12];
	matrix4x4[3][1] = matrix[13];
	matrix4x4[3][2] = matrix[14];
	matrix4x4[3][3] = matrix[15];

	Matrix4x4 newMatrix; newMatrix.setMatrix(matrix4x4);
	Matrix4x4 scaleMatrix; scaleMatrix.setScale(m_scale);
	newMatrix *= scaleMatrix;
	Matrix4x4 rotMatrix; rotMatrix.setRotationX(m_rotation.m_x);
	newMatrix *= rotMatrix;
	rotMatrix.setRotationY(m_rotation.m_y);
	newMatrix *= rotMatrix;
	rotMatrix.setRotationZ(m_rotation.m_z);
	newMatrix *= rotMatrix;
	Matrix4x4 transMatrix; transMatrix.setTranslation(m_position);
	newMatrix *= transMatrix;
	m_matrix = newMatrix;
	this->overrideMatrix = true;
}

/*
 * 
	Matrix4x4 scaleMatrix; scaleMatrix.setScale(m_scale);
	newMatrix *= scaleMatrix;
	Matrix4x4 rotMatrix; rotMatrix.setRotationX(m_rotation.m_x);
	newMatrix *= rotMatrix;
	rotMatrix.setRotationY(m_rotation.m_y);
	newMatrix *= rotMatrix;
	rotMatrix.setRotationZ(m_rotation.m_z);
	newMatrix *= rotMatrix;
	Matrix4x4 transMatrix; transMatrix.setTranslation(m_position);
	newMatrix *= transMatrix;
	m_matrix = newMatrix;
 */


