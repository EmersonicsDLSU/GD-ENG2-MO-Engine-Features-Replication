#include "BNS_MultipleScene_UI.h"
#include "BNS_Camera.h"
#include "BNS_CameraHandler.h"
#include "BNS_FileExplorer.h"
#include "BNS_PrimitiveCreation.h"
#include "BNS_Texture.h"

BNS_MultipleScene_UI::BNS_MultipleScene_UI(std::string name, int ID) : BNS_AUIScreen(name, ID)
{
}

BNS_MultipleScene_UI::~BNS_MultipleScene_UI()
{
}

void BNS_MultipleScene_UI::DrawUI()
{

	//DRAW HERE
	ImGui::Begin("Multiple Scene");

	static float padding = 16.0f;
	static float thumbnailSize = 128.0f;
	float cellSize = thumbnailSize + padding;
	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	// creates an icon to the file
	TexturePtr icon = BNS_FileExplorer::GetInstance()->GetImageHashTable()["folder_icon"];

	for (int i = 0; i < 5; ++i)
	{
		ImGui::PushID(i);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

		// Get the button position and size
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 size = { thumbnailSize, thumbnailSize };
		// Draw the button
		ImGui::GetWindowDrawList()->AddRectFilled(pos, { pos.x + size.x, pos.y + size.y }, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);

		ImGui::ImageButton((void*)icon.get()->GetShaderResourceView(),
			{ thumbnailSize, thumbnailSize }, { -1, 0 }, { 0,1 });

		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			//BNS_PrimitiveCreation::Instance()->CreateTexturedCube();
			if (i == 0)
				BNS_PrimitiveCreation::Instance()->CreatePlane();
			else if (i == 1)
				BNS_PrimitiveCreation::Instance()->CreateBunny();
			else if (i == 2)
				BNS_PrimitiveCreation::Instance()->CreateArmadillo();
			else if (i == 3)
				BNS_PrimitiveCreation::Instance()->CreateTeapot();
			else if (i == 4)
				BNS_PrimitiveCreation::Instance()->CreateEarth();
		}
		ImGui::PopStyleColor();

		// Get the text size
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		ImVec2 textSize = ImGui::CalcTextSize("00");
		ImGui::PopFont();
		// Calculate the text position
		ImVec2 textPos = { pos.x + (size.x - textSize.x) / 2.0f, pos.y + (size.y - textSize.y) / 2.0f };
		// Draw the text overlay
		float percentage = 100.0f;
		std::string percentageStr = std::to_string(static_cast<int>(percentage)) + "%";
		ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), percentageStr.c_str());


		ImGui::NextColumn();
		ImGui::PopID();
	}

	ImGui::End();
}
