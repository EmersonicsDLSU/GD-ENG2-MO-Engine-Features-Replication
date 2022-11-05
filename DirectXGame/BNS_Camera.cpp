#include "BNS_Camera.h"

#include "BNS_AppWindow.h"
#include "BNS_EngineTime.h"
#include "InputSystem.h"

BNS_Camera::BNS_Camera(std::string name, BNS_ObjectTypes type) : AGameObject(name, type)
{
	m_matrix.setTranslation(Vector3D{ 0.0f, 0.0f, -2.0f });
	SetPosition(Vector3D{ 0.0f, 0.0f, -2.0f });
	this->UpdateViewMatrix();
	// subscribe this class to the InputSystem
	InputSystem::get()->addListener(this);
}

BNS_Camera::~BNS_Camera()
{
	InputSystem::get()->removeListener(this);
}

void BNS_Camera::Update(float deltaTime, BNS_AppWindow* app_window)
{
}

Matrix4x4 BNS_Camera::GetCamViewMatrix()
{
	return m_view_cam;
}

Matrix4x4 BNS_Camera::GetCameraOrthoMatrix()
{
	// setting for the orthographic projection
	Matrix4x4 orthoMatrix;
	orthoMatrix.setOrthoLH
	(
		Window::WIDTH / 300.0f,
		Window::HEIGHT / 300.0f,
		-40.0f,
		40.0f
	);

	return orthoMatrix;
}

Matrix4x4 BNS_Camera::GetCamProjectionMatrix()
{
	int width = Window::WIDTH;
	int height = Window::HEIGHT;
	float aspectRatio = (float)width / (float)height;
	float fov = fovInDegrees * (3.1415926f / 180.0f);

	Matrix4x4 projectionMatrix;
	projectionMatrix.setPerspectiveFovLH(fov, aspectRatio, nearPlane, farPlane);

	return projectionMatrix;
}

void BNS_Camera::UpdateViewMatrix()
{
	Matrix4x4 world_cam; world_cam.setIdentity();
	Matrix4x4 temp; temp.setIdentity();
	// rotation first
	Vector3D localRot = this->GetLocalRotation();
	temp.setRotationX(localRot.m_x);
	world_cam *= temp;
	temp.setRotationY(localRot.m_y);
	world_cam *= temp;
	// position next
	// moving or setting the camera position in the x,y,z axis
	Vector3D new_pos = m_matrix.getTranslation() + world_cam.getZDirection() * (m_forward * 1.0f);
	new_pos = new_pos + world_cam.getYDirection() * (m_upward * 1.0f);
	new_pos = new_pos + world_cam.getXDirection() * (m_rightward * 1.0f);
	temp.setTranslation(new_pos);
	world_cam *= temp;
	m_matrix = world_cam;

	// convert camera matrix to view matrix
	world_cam.inverse();
	m_view_cam = world_cam;

}

void BNS_Camera::onKeyDown(int key)
{
	Vector3D localPos = this->GetLocalPosition();
	float x = localPos.m_x;
	float y = localPos.m_y;
	float z = localPos.m_z;
	if (key == 'W')
	{
		z += BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		SetPosition(x, y, z);
		UpdateViewMatrix();
		m_forward = 1.0f * BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'S')
	{
		z -= BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		SetPosition(x, y, z);
		UpdateViewMatrix();
		m_forward = -1.0f * BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'A')
	{
		x -= BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		SetPosition(x, y, z);
		UpdateViewMatrix();
		m_rightward = -1.0f * BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'D')
	{
		x += BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		SetPosition(x, y, z);
		UpdateViewMatrix();
		m_rightward = 1.0f * BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'Q')
	{
		y += BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		SetPosition(x, y, z);
		UpdateViewMatrix();
		m_upward = 1.0f * BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
	else if (key == 'E')
	{
		y -= BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
		SetPosition(x, y, z);
		UpdateViewMatrix();
		m_upward = -1.0f * BNS_EngineTime::getDeltaTime() * NAVIGATE_SPEED;
	}
}

void BNS_Camera::onKeyUp(int key)
{
	m_upward = 0.0f;
	m_forward = 0.0f;
	m_rightward = 0.0f;
}

void BNS_Camera::onMouseMove(const Point& mouse_pos)
{
	if (this->mouseDown) {
		float x = GetLocalRotation().m_x;
		float y = GetLocalRotation().m_y;
		float z = GetLocalRotation().m_z;

		float speed = 0.1f;
		x += (mouse_pos.m_y - (Window::HEIGHT / 2.0f)) * BNS_EngineTime::getDeltaTime() * speed;
		y += (mouse_pos.m_x - (Window::WIDTH / 2.0f)) * BNS_EngineTime::getDeltaTime() * speed;
		
		InputSystem::get()->setCursorPosition(Point((int)(Window::WIDTH / 2.0f), (int)(Window::HEIGHT / 2.0f)));

		
		SetRotation(x, y, z);
		UpdateViewMatrix();
	}
}

void BNS_Camera::onLeftMouseDown(const Point& delta_mouse_pos)
{
}

void BNS_Camera::onLeftMouseUp(const Point& delta_mouse_pos)
{
}

void BNS_Camera::onRightMouseDown(const Point& delta_mouse_pos)
{
	mouseDown = true;
	// hides the cursor
	InputSystem::get()->showCursor(false);
}

void BNS_Camera::onRightMouseUp(const Point& delta_mouse_pos)
{
	mouseDown = false;
	// displays the cursor
	InputSystem::get()->showCursor(true);
}