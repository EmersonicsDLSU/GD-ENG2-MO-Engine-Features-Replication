#pragma once
#include "BNS_AUIScreen.h"
#include "BNS_CameraHandler.h"
#include "BNS_Camera.h"

class BNS_MenuToolbar_UI : public BNS_AUIScreen
{
public:
	BNS_MenuToolbar_UI(std::string name);
	~BNS_MenuToolbar_UI();
	void DrawUI() override;

};

