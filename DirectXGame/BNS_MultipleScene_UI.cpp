#include "BNS_MultipleScene_UI.h"
#include "BNS_Camera.h"
#include "BNS_CameraHandler.h"
#include "BNS_FileExplorer.h"
#include "BNS_PrimitiveCreation.h"
#include "BNS_Texture.h"
#include "P3_ObjectID.h"

BNS_MultipleScene_UI::BNS_MultipleScene_UI(std::string name, int ID) : BNS_AUIScreen(name, ID)
{
	// Objects in SCENE 1
	P3_ObjectID *obj_1 = new P3_ObjectID(0, P3_ObjectType::TEAPOT, 0);
	objectsToLoad.emplace_back(obj_1);
	// Objects in SCENE 2
	P3_ObjectID* obj_2 = new P3_ObjectID(1, P3_ObjectType::STATUE, 1);
	objectsToLoad.emplace_back(obj_2);
	// Objects in SCENE 3
	P3_ObjectID* obj_3 = new P3_ObjectID(2, P3_ObjectType::BUNNY, 2);
	objectsToLoad.emplace_back(obj_3);
	// Objects in SCENE 4
	P3_ObjectID* obj_4 = new P3_ObjectID(3, P3_ObjectType::ARMADILLO, 3);
	objectsToLoad.emplace_back(obj_4);
	// Objects in SCENE 5
	P3_ObjectID* obj_5 = new P3_ObjectID(4, P3_ObjectType::EARTH, 4);
	objectsToLoad.emplace_back(obj_5);
}

BNS_MultipleScene_UI::~BNS_MultipleScene_UI()
{
}

void BNS_MultipleScene_UI::onExecute(int sceneIndex)
{
	while (true)
	{
		// Check if the vector contains a P3_ObjectID structure with sceneIndex = 0
		bool containsObject = false;
		P3_ObjectID *objectWithSceneIndex = nullptr;
		// iterate all objects to load if there's still an object to load in that scene
		for (auto objectID : objectsToLoad) {
			if (objectID->sceneIndex == sceneIndex) {
				containsObject = true;
				objectWithSceneIndex = objectID;
				break;
			}
		}
		if (containsObject)
		{
			// create the object; object is still not active
			ExecuteObject(objectWithSceneIndex);

			int indexObjList = 0;
			// DELETE AT OBJECT LIST
			for (int i = 0; i < objectsToLoad.size(); i++)
			{
				if (objectsToLoad[i]->ID == objectWithSceneIndex->ID)
				{
					indexObjList = i;
					break;
				}
			}
			objectsToLoad.erase(objectsToLoad.begin() + indexObjList);
			// add object
			objectsOnScene.emplace_back(objectWithSceneIndex);
		}
		// if there's no more object to execute, then we break this thread
		else break;
	}
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

		OnEntryLeftClick(i);
		OnEntryRightClick(i);

		ImGui::PopStyleColor();

		// Get the text size
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		ImVec2 textSize = ImGui::CalcTextSize("000.00");
		ImGui::PopFont();
		// Calculate the text position
		ImVec2 textPos = { pos.x + (size.x - textSize.x) / 2.0f, pos.y + (size.y - textSize.y) / 2.0f };
		// Draw the text overlay
		float percentage = (float)BNS_PrimitiveCreation::Instance()->sceneObjectDictionary[i].size() / 
			(float)BNS_PrimitiveCreation::Instance()->maxPopulation;
		std::string percentageStr = std::to_string(static_cast<int>(percentage * 100.0f)) + "%";
		ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), percentageStr.c_str());

		ImGui::NextColumn();
		ImGui::PopID();
	}

	if (ImGui::Button("View All", ImVec2(100, 50)))
	{
		// view all scenes
	}

	ImGui::End();
}

void BNS_MultipleScene_UI::ExecuteObject(P3_ObjectID *objectID)
{
	// create the object
	BNS_AGameObject *objectToCreate = nullptr;
	switch(objectID->objectType)
	{
	case P3_ObjectType::TEAPOT:
		objectToCreate = BNS_PrimitiveCreation::Instance()->CreateTeapot({0,0,0}, objectID->scale, true);
		break;
	case P3_ObjectType::STATUE:
		objectToCreate = BNS_PrimitiveCreation::Instance()->CreateStatue({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::BUNNY:
		objectToCreate = BNS_PrimitiveCreation::Instance()->CreateBunny({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::ARMADILLO:
		objectToCreate = BNS_PrimitiveCreation::Instance()->CreateArmadillo({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::EARTH:
		objectToCreate = BNS_PrimitiveCreation::Instance()->CreateEarth({ 0,0,0 }, objectID->scale, true);
		break;
	}
	objectToCreate->SetActive(false);
	// add it to the sceneObjectDictionary
	BNS_PrimitiveCreation::Instance()->sceneObjectDictionary[objectID->sceneIndex].emplace_back(objectToCreate);
}

void BNS_MultipleScene_UI::DeleteObjectInScene(int sceneIndex)
{
	for (auto it = objectsOnScene.begin(); it != objectsOnScene.end();)
	{
		if ((*it)->sceneIndex == sceneIndex)
		{
			objectsToLoad.push_back(*it);
			it = objectsOnScene.erase(it);
		}
		else
		{ // only increment if not erasing an element
			++it;
		}
	}
}

void BNS_MultipleScene_UI::OnEntryLeftClick(int i)
{
	bool isEmpty = true, isComplete = false;
	if (BNS_PrimitiveCreation::Instance()->sceneObjectDictionary[i].size() != 0)
		isEmpty = false;
	if (BNS_PrimitiveCreation::Instance()->sceneObjectDictionary[i].size() == BNS_PrimitiveCreation::Instance()->maxPopulation)
		isComplete = true;

	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		// if not yet
		if (i == 0 && isEmpty)
		{
			BNS_PrimitiveCreation::Instance()->LoadAScene(0, this);
		}
		else if (i == 0 && isComplete)
		{
			BNS_PrimitiveCreation::Instance()->ShowScene(0);
		}
		else if (i == 1 && isEmpty)
		{
			BNS_PrimitiveCreation::Instance()->LoadAScene(1, this);
		}
		else if (i == 1 && isComplete)
		{
			BNS_PrimitiveCreation::Instance()->ShowScene(1);
		}
		else if (i == 2 && isEmpty)
		{
			BNS_PrimitiveCreation::Instance()->LoadAScene(2, this);
		}
		else if (i == 2 && isComplete)
		{
			BNS_PrimitiveCreation::Instance()->ShowScene(2);
		}
		else if (i == 3 && isEmpty)
		{
			BNS_PrimitiveCreation::Instance()->LoadAScene(3, this);
		}
		else if (i == 3 && isComplete)
		{
			BNS_PrimitiveCreation::Instance()->ShowScene(3);
		}
		else if (i == 4 && isEmpty)
		{
			BNS_PrimitiveCreation::Instance()->LoadAScene(4, this);
		}
		else if (i == 4 && isComplete)
		{
			BNS_PrimitiveCreation::Instance()->ShowScene(4);
		}
	}
}

void BNS_MultipleScene_UI::OnEntryRightClick(int i)
{
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		
	}
	else if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup("ImageContextMenu");
	}
	if (ImGui::BeginPopup("ImageContextMenu"))
	{
		if (ImGui::Button("X", ImVec2(25, 25)))
		{
			if (i == 0)
			{
				DeleteObjectInScene(0);
				BNS_PrimitiveCreation::Instance()->ResetScene(0);
			}
			else if (i == 1)
			{
				DeleteObjectInScene(1);
				BNS_PrimitiveCreation::Instance()->ResetScene(1);
			}
			else if (i == 2)
			{
				DeleteObjectInScene(2);
				BNS_PrimitiveCreation::Instance()->ResetScene(2);
			}
			else if (i == 3)
			{
				DeleteObjectInScene(3);
				BNS_PrimitiveCreation::Instance()->ResetScene(3);
			}
			else if (i == 4)
			{
				DeleteObjectInScene(4);
				BNS_PrimitiveCreation::Instance()->ResetScene(4);
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}
