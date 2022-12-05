#include "BNS_TransformComponent.h"

#include "BNS_BaseComponentSystem.h"
#include "BNS_CameraHandler.h"
#include "BNS_ConstantBuffer.h"
#include "BNS_ContantBufferTypes.h"
#include "BNS_GraphicsEngine.h"
#include "BNS_TransformSystem.h"
#include "BNS_AGameObject.h"

BNS_TransformComponent::BNS_TransformComponent(String name, BNS_AGameObject* owner) : BNS_AComponent(name, ComponentType::Transform, owner)
{
	BNS_BaseComponentSystem::GetInstance()->GetTransformSystem()->RegisterComponent(this);
}

BNS_TransformComponent::~BNS_TransformComponent()
{
	BNS_BaseComponentSystem::GetInstance()->GetTransformSystem()->UnRegisterComponent(this);
	BNS_AComponent::~BNS_AComponent();
}

void BNS_TransformComponent::Perform(float deltaTime)
{
	BNS_AComponent::Perform(deltaTime);
	
	// BNS_Texture update
	BNS_constant_texture cc_texture;
	cc_texture.alpha = owner->alpha;
	owner->m_cb_texture->update(BNS_GraphicsEngine::get()->getRenderSystem()->GetImmediateDeviceContext(), &cc_texture);

	// transform update
	BNS_constant_transform cc;
	cc.m_time = ::GetTickCount();

	// objects matrix
	Matrix4x4 temp;
	cc.m_world.setIdentity();

	if (owner->overrideMatrix) {
		cc.m_world = owner->m_matrix;
	}
	else
	{
		temp.setScale(owner->m_scale);
		cc.m_world *= temp;
		temp.setRotationX(owner->m_rotation.m_x);
		cc.m_world *= temp;
		temp.setRotationY(owner->m_rotation.m_y);
		cc.m_world *= temp;
		temp.setRotationZ(owner->m_rotation.m_z);
		cc.m_world *= temp;
		temp.setTranslation(owner->m_position);
		cc.m_world *= temp;
		// update m_matrix
		owner->m_matrix = cc.m_world;
	}

	// creating the camera matrix
	Matrix4x4 cameraMatrix = BNS_CameraHandler::GetInstance()->GetSceneCameraViewMatrix();
	cc.m_view = cameraMatrix;

	// setting the perspective projection
	cc.m_proj = BNS_CameraHandler::GetInstance()->GetSceneCameraProjMatrix();

	owner->m_cb->update(BNS_GraphicsEngine::get()->getRenderSystem()->GetImmediateDeviceContext(), &cc);
}