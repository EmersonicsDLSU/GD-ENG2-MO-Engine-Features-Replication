#include "BNS_GameObjectManager.h"

#include "BNS_AGameObject.h"

BNS_GameObjectManager* BNS_GameObjectManager::m_instance = nullptr;

BNS_GameObjectManager::BNS_GameObjectManager()
{
}

BNS_GameObjectManager::~BNS_GameObjectManager()
{
}

BNS_GameObjectManager* BNS_GameObjectManager::get()
{
	return m_instance;
}

void BNS_GameObjectManager::create()
{
	if (BNS_GameObjectManager::m_instance)
		throw std::exception("BNS_GameObjectManager already created");
	BNS_GameObjectManager::m_instance = new BNS_GameObjectManager();
}

void BNS_GameObjectManager::release()
{
	if (!BNS_GameObjectManager::m_instance)
		return;
	delete BNS_GameObjectManager::m_instance;
}

std::vector<BNS_AGameObject*>& BNS_GameObjectManager::GetObjectList()
{
	return objectList;
}

std::vector<BNS_AGameObject*>& BNS_GameObjectManager::GetRenderObjectList()
{
	render_objectList = objectList;
	return render_objectList;
}


void BNS_GameObjectManager::DeleteObject(BNS_AGameObject* gameObject)
{
	int indexObjList = -1;
	int indexRenderList = -1;
	BNS_AGameObject* objToBeDeleted = nullptr;
	// DELETE AT OBJECT LIST
	for (int i = 0; i < objectList.size(); i++)
	{
		if (objectList[i] == gameObject)
		{
			indexObjList = i;
			objToBeDeleted = objectList[i];
			break;
		}
	}
	
	for (int i = 0; i < render_objectList.size(); i++)
	{
		if (render_objectList[i] == gameObject)
		{
			indexRenderList = i;
			break;
		}
	}

	if (indexObjList != -1)
	{
		//std::cout << "Object to be deleted: " << objectList[indexObjList]->GetName() << std::endl;
		objectList.erase(objectList.begin() + indexObjList);
	}
	if (indexRenderList != -1)
	{
		//std::cout << "Render to be deleted: " << render_objectList[indexRenderList]->GetName() << std::endl;
		render_objectList.erase(render_objectList.begin() + indexRenderList);
	}

	//delete objToBeDeleted;
}

void BNS_GameObjectManager::DeleteObjectByName(std::string name)
{
	int indexObjList = -1;
	int indexRenderList = -1;

	// DELETE AT OBJECT LIST
	for (int i = 0; i < objectList.size(); i++)
	{
		if (objectList[i]->GetName() == name)
		{
			indexObjList = i;
			delete objectList[i];
			break;
		}
	}

	for (int i = 0; i < render_objectList.size(); i++)
	{
		if (render_objectList[i]->GetName() == name)
		{
			indexRenderList = i;
			break;
		}
	}

	if (indexObjList != -1)
	{
		objectList.erase(objectList.begin() + indexObjList);
	}
	if (indexRenderList != -1)
	{
		render_objectList.erase(render_objectList.begin() + indexRenderList);
	}
}

BNS_AGameObject* BNS_GameObjectManager::FindObject(BNS_AGameObject* gameObject)
{
	for (int i = 0; i < this->objectList.size(); i++)
	{
		if (this->objectList[i] == gameObject)
			return this->objectList[i];
	}

	std::cout << "Object not found!";
	return NULL;
}

BNS_AGameObject* BNS_GameObjectManager::FindObjectByName(std::string name)
{
	for (int i = 0; i < this->objectList.size(); i++)
	{
		if (this->objectList[i]->GetName() == name)
			return this->objectList[i];
	}

	std::cout << "Object " << name << " not found!";
	return NULL;
}

void BNS_GameObjectManager::saveEditStates()
{
	for (int i = 0; i < this->objectList.size(); i++) {
		this->objectList[i]->saveEditState();
	}
}

void BNS_GameObjectManager::restoreEditStates()
{
	for (int i = 0; i < this->objectList.size(); i++) {
		this->objectList[i]->restoreEditState();
	}
}

void BNS_GameObjectManager::applyEditorAction(BNS_EditorAction* action)
{
	BNS_AGameObject* object = this->FindObjectByName(action->GetOwnerName());
	if (object != nullptr) {
		object->RecomputeMatrix(action->GetStoredMatrix().GetMatrix());
		object->SetPosition(action->GetStoredPos());
		object->SetRotation(action->GetRotation());
		object->SetScale(action->GetStoredScale());

	}
}

void BNS_GameObjectManager::SetSkyBox(BNS_SkyBox* skybox)
{
	current_SkyBox = skybox;
}

BNS_SkyBox* BNS_GameObjectManager::GetSkyBox()
{
	return current_SkyBox;
}



