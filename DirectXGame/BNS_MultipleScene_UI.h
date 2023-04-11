#pragma once
#include "BNS_AGameObject.h"
#include "BNS_AUIScreen.h"
#include "IExecutionEvent.h"
#include "Vector3D.h"


class P3_ObjectID;

class BNS_MultipleScene_UI : public BNS_AUIScreen, public IExecutionEvent
{
public:
	BNS_MultipleScene_UI(std::string name, int ID);
	~BNS_MultipleScene_UI();

	void onExecute(int sceneIndex);
	void DrawUI() override;
private:
	bool active;
public:
	std::vector<P3_ObjectID*> objectsToLoad;
	std::vector<P3_ObjectID*> objectsOnReserve;
	void ExecuteObject(P3_ObjectID *objectID);
};



