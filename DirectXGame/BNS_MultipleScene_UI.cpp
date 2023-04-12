#include "BNS_MultipleScene_UI.h"
#include "BNS_Camera.h"
#include "BNS_CameraHandler.h"
#include "BNS_FileExplorer.h"
#include "BNS_PrimitiveCreation.h"
#include "BNS_Texture.h"
#include "P3_ObjectID.h"

BNS_MultipleScene_UI::BNS_MultipleScene_UI(std::string name, int ID) : BNS_AUIScreen(name, ID)
{
	PC_instance = BNS_PrimitiveCreation::Instance();

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
		else
		{
			// set the scene status
			PC_instance->sceneStatusDictionary[sceneIndex]->isEmpty = false;
			PC_instance->sceneStatusDictionary[sceneIndex]->isLoading = false;
			PC_instance->sceneStatusDictionary[sceneIndex]->isComplete = true;
			break;
		}
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
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.5f)); // set button color to grey with alpha
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 0.5f));

		// Get the button position and size
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 size = { thumbnailSize, thumbnailSize };
		// Draw the button
		ImGui::GetWindowDrawList()->AddRectFilled(pos, { pos.x + size.x, pos.y + size.y }, ImGui::GetColorU32(ImGuiCol_Button), 4.0f);

		// button state
		bool buttonDisabled = !PC_instance->sceneStatusDictionary[i]->isEmpty;
		if (PC_instance->sceneStatusDictionary[i]->isComplete)
		{
			buttonDisabled = PC_instance->sceneStatusDictionary[i]->isViewed;
		}
		else if (!PC_instance->sceneStatusDictionary[i]->isComplete && PC_instance->sceneStatusDictionary[i]->isLoading)
		{
			buttonDisabled = true;
		}
		ImGui::ImageButton((void*)icon.get()->GetShaderResourceView(),
			{ thumbnailSize, thumbnailSize }, { -1, 0 }, { 0,1 },
			-1, ImVec4(1.0f, 1.0f, 1.0f, 0.5f),
			buttonDisabled ? ImVec4(0.5f, 0.5f, 0.5f, 0.5f)
			: ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

		OnEntryLeftClick(i);
		OnEntryRightClick(i);
		ShowProgressBar(i);

		ImGui::PopStyleColor(3); // restore default colors

		// Get the text size
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
		ImVec2 textSize = ImGui::CalcTextSize("000.00");
		ImGui::PopFont();
		// Calculate the text position
		ImVec2 textPos = { pos.x + (size.x - textSize.x) / 2.0f, pos.y + (size.y - textSize.y) / 2.0f };
		// Draw the text overlay
		float percentage = (float)PC_instance->sceneObjectDictionary[i].size() / 
			(float)PC_instance->maxPopulation;
		std::string percentageStr = std::to_string(static_cast<int>(percentage * 100.0f)) + "%";
		ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), percentageStr.c_str());

		ImGui::NextColumn();
		ImGui::PopID();
	}

	if (ImGui::Button("View All", ImVec2(100, 50)))
	{
		// view all scenes
		for (int index = 0; index < 5; ++index)
		{
			if (PC_instance->sceneStatusDictionary[index]->isLoading) break;

			PC_instance->LoadAScene(index, this);
		}
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
		objectToCreate = PC_instance->CreateTeapot({0,0,0}, objectID->scale, true);
		break;
	case P3_ObjectType::STATUE:
		objectToCreate = PC_instance->CreateStatue({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::BUNNY:
		objectToCreate = PC_instance->CreateBunny({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::ARMADILLO:
		objectToCreate = PC_instance->CreateArmadillo({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::EARTH:
		objectToCreate = PC_instance->CreateEarth({ 0,0,0 }, objectID->scale, true);
		break;
	}
	objectToCreate->SetActive(false);
	// add it to the sceneObjectDictionary
	PC_instance->sceneObjectDictionary[objectID->sceneIndex].emplace_back(objectToCreate);
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

void BNS_MultipleScene_UI::OnEntryLeftClick(int index)
{
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		for (int i = 0; i < 5; ++i)
		{
			if (PC_instance->sceneStatusDictionary[i]->isLoading)
			{
				ResetAllButtonsProgressView();
				PC_instance->sceneStatusDictionary[i]->isProgressViewed = true;
				break;
			}

			if (index == i && PC_instance->sceneStatusDictionary[i]->isEmpty)
			{
				PC_instance->LoadAScene(index, this);
			}
			else if (index == i && PC_instance->sceneStatusDictionary[i]->isComplete)
			{
				ResetAllButtonsView();
				PC_instance->sceneStatusDictionary[i]->isViewed = true;
				PC_instance->ShowScene(index);
			}
		}
	}
}

void BNS_MultipleScene_UI::OnEntryRightClick(int index)
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
			for (int i = 0; i < 5; ++i)
			{
				if (index == i)
				{
					DeleteObjectInScene(index);
					PC_instance->ResetScene(index);
				}
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void BNS_MultipleScene_UI::ResetAllButtonsView()
{
	for (int i = 0; i < 5; ++i)
	{
		PC_instance->sceneStatusDictionary[i]->isViewed = false;
	}
}

void BNS_MultipleScene_UI::ResetAllButtonsProgressView()
{
	for (int i = 0; i < 5; ++i)
	{
		PC_instance->sceneStatusDictionary[i]->isProgressViewed = false;
	}
}

void BNS_MultipleScene_UI::ShowProgressBar(int index)
{
	// show progress bar when loading icon is clicked
	if (PC_instance->sceneStatusDictionary[index]->isProgressViewed)
	{
		ImGui::OpenPopup("Progress");
		if (ImGui::BeginPopup("Progress")) {
			// Calculate progress value
			float progress = (float)PC_instance->sceneObjectDictionary[index].size() /
				(float)PC_instance->maxPopulation;
			if (progress >= 1.0f) {
				// Close progress window
				ImGui::CloseCurrentPopup();
				PC_instance->sceneStatusDictionary[index]->isProgressViewed = false;
				progress = 0.0f;
			}
			else {
				// Draw progress bar
				ImGui::ProgressBar(progress * 100.0f);
				std::cout << "Show Progress Bar" << std::endl;
			}

			ImGui::EndPopup();
		}
	}
}
