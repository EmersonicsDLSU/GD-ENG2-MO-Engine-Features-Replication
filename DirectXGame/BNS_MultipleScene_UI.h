#pragma once
#include "BNS_AGameObject.h"
#include "BNS_AUIScreen.h"
#include "IExecutionEvent.h"
#include "Vector3D.h"
#include <semaphore>
#include <mutex>


class P3_ObjectID;

class BNS_MultipleScene_UI : public BNS_AUIScreen, public IExecutionEvent
{
public:
	BNS_MultipleScene_UI(std::string name, int ID);
	~BNS_MultipleScene_UI();

	void onExecute(int sceneIndex);
	void DrawUI() override;
private:
	BNS_PrimitiveCreation* PC_instance;
	bool active;

	std::counting_semaphore<1>* mutexSem;
	std::counting_semaphore<5>* showAllSem;
public:
	std::vector<P3_ObjectID*> objectsToLoad;
	std::vector<P3_ObjectID*> objectsOnScene;
	void ExecuteObject(P3_ObjectID* objectID);
	void DeleteObjectInScene(int sceneIndex);
private:
	void OnEntryLeftClick(int i);
	void OnEntryRightClick(int index);
	void ResetAllButtonsView();
	void ResetAllButtonsProgressView();
	void ShowProgressBar(int index);
	void OnViewAll();
};



