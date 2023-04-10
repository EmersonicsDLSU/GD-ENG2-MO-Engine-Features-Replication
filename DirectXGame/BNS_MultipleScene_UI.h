#pragma once
#include "BNS_AGameObject.h"
#include "BNS_AUIScreen.h"
#include "IExecutionEvent.h"
#include "Vector3D.h"

class BNS_MultipleScene_UI : public BNS_AUIScreen, public IExecutionEvent
{
public:
	BNS_MultipleScene_UI(std::string name, int ID);
	~BNS_MultipleScene_UI();

	void onExecute(int index);
	void DrawUI() override;
private:
	bool active;
};



