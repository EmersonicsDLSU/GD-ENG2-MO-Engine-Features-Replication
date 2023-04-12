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

	// initialize semaphore
	mutexSem = new std::counting_semaphore<1>(1);
	showAllSem = new std::counting_semaphore<5>(0);

	// Objects in SCENE 1
	P3_ObjectID* obj_1 = new P3_ObjectID(0, P3_ObjectType::TEAPOT, 0);
	P3_ObjectID* obj_2 = new P3_ObjectID(1, P3_ObjectType::CHARMANDER, 0);
	P3_ObjectID* obj_3 = new P3_ObjectID(2, P3_ObjectType::CUBONE, 0);
	P3_ObjectID* obj_4 = new P3_ObjectID(3, P3_ObjectType::EEVEE, 0);
	P3_ObjectID *obj_5 = new P3_ObjectID(4, P3_ObjectType::FARFETCH, 0);
	// Objects in SCENE 2
	P3_ObjectID* obj_6 = new P3_ObjectID(5, P3_ObjectType::STATUE, 1);
	P3_ObjectID* obj_7 = new P3_ObjectID(6, P3_ObjectType::MACHOP, 1);
	P3_ObjectID* obj_8 = new P3_ObjectID(7, P3_ObjectType::NIDORANF, 1);
	P3_ObjectID* obj_9 = new P3_ObjectID(8, P3_ObjectType::NIDORANM, 1);
	P3_ObjectID* obj_10 = new P3_ObjectID(9, P3_ObjectType::ODDISH, 1);
	// Objects in SCENE 3
	P3_ObjectID* obj_11 = new P3_ObjectID(10, P3_ObjectType::BUNNY, 2);
	P3_ObjectID* obj_12 = new P3_ObjectID(11, P3_ObjectType::PIKACHU, 2);
	P3_ObjectID* obj_13 = new P3_ObjectID(12, P3_ObjectType::GROWLITHE, 2);
	P3_ObjectID* obj_14 = new P3_ObjectID(13, P3_ObjectType::PIDGEY, 2);
	P3_ObjectID* obj_15 = new P3_ObjectID(14, P3_ObjectType::PALKIA, 2);
	// Objects in SCENE 4
	P3_ObjectID* obj_16 = new P3_ObjectID(15, P3_ObjectType::ARMADILLO, 3);
	P3_ObjectID* obj_17 = new P3_ObjectID(16, P3_ObjectType::JIGGLYPUFF, 3);
	P3_ObjectID* obj_18 = new P3_ObjectID(17, P3_ObjectType::GIRATINA, 3);
	P3_ObjectID* obj_19 = new P3_ObjectID(18, P3_ObjectType::CYNDAQUIL, 3);
	P3_ObjectID* obj_20 = new P3_ObjectID(19, P3_ObjectType::DIALGA, 3);
	// Objects in SCENE 5
	P3_ObjectID* obj_21 = new P3_ObjectID(20, P3_ObjectType::EARTH, 4);
	P3_ObjectID* obj_22 = new P3_ObjectID(21, P3_ObjectType::PSYDUCK, 4);
	P3_ObjectID* obj_23 = new P3_ObjectID(22, P3_ObjectType::SEEL, 4);
	P3_ObjectID* obj_24 = new P3_ObjectID(23, P3_ObjectType::SQUIRTLE, 4);
	P3_ObjectID* obj_25 = new P3_ObjectID(24, P3_ObjectType::VULPIX, 4);
	objectsToLoad.emplace_back(obj_1);
	objectsToLoad.emplace_back(obj_2);
	objectsToLoad.emplace_back(obj_3);
	objectsToLoad.emplace_back(obj_4);
	objectsToLoad.emplace_back(obj_5);
	objectsToLoad.emplace_back(obj_6);
	objectsToLoad.emplace_back(obj_7);
	objectsToLoad.emplace_back(obj_8);
	objectsToLoad.emplace_back(obj_9);
	objectsToLoad.emplace_back(obj_10);
	objectsToLoad.emplace_back(obj_11);
	objectsToLoad.emplace_back(obj_12);
	objectsToLoad.emplace_back(obj_13);
	objectsToLoad.emplace_back(obj_14);
	objectsToLoad.emplace_back(obj_15);
	objectsToLoad.emplace_back(obj_16);
	objectsToLoad.emplace_back(obj_17);
	objectsToLoad.emplace_back(obj_18);
	objectsToLoad.emplace_back(obj_19);
	objectsToLoad.emplace_back(obj_20);
	objectsToLoad.emplace_back(obj_21);
	objectsToLoad.emplace_back(obj_22);
	objectsToLoad.emplace_back(obj_23);
	objectsToLoad.emplace_back(obj_24);
	objectsToLoad.emplace_back(obj_25);
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
			mutexSem->acquire();
			std::cout << "Execute Load Scene: " << sceneIndex << std::endl;
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
			mutexSem->release();
		}
		// if there's no more object to execute, then we break this thread
		else
		{
			std::cout << "Break" << std::endl;
			// set the scene status
			PC_instance->sceneStatusDictionary[sceneIndex]->isEmpty = false;
			PC_instance->sceneStatusDictionary[sceneIndex]->isLoading = false;
			PC_instance->sceneStatusDictionary[sceneIndex]->isComplete = true;
			// release one completed scene
			showAllSem->release();
			break;
		}
	}
}

void BNS_MultipleScene_UI::onExecuteAll()
{
	while (true)
	{
		// view all scenes
		for (int index = 0; index < 5; ++index)
		{
			// if the scene is currently loading or already complete its loading; then check next scene
			if (PC_instance->sceneStatusDictionary[index]->isLoading ||
				PC_instance->sceneStatusDictionary[index]->isComplete) continue;

			PC_instance->LoadAScene(index, this);
		}

		// it needs 5 keys
		for (int i = 0; i < 5; i++)
		{
			showAllSem->acquire();
		}

		for (int index = 0; index < 5; ++index)
		{
			PC_instance->sceneStatusDictionary[index]->isViewed = true;
		}
		PC_instance->ShowAll();
		break;
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
		PC_instance->LoadAllScene(this);
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

	case P3_ObjectType::CHARMANDER:
		objectToCreate = PC_instance->CreateCharmander({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::CUBONE:
		objectToCreate = PC_instance->CreateCubone({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::EEVEE:
		objectToCreate = PC_instance->CreateEevee({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::FARFETCH:
		objectToCreate = PC_instance->CreateFarfetch({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::GROWLITHE:
		objectToCreate = PC_instance->CreateGrowlithe({ 0,0,0 }, objectID->scale, true);
		break;

	case P3_ObjectType::MACHOP:
		objectToCreate = PC_instance->CreateMachop({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::NIDORANF:
		objectToCreate = PC_instance->CreateNidoranF({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::NIDORANM:
		objectToCreate = PC_instance->CreateNidoranM({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::ODDISH:
		objectToCreate = PC_instance->CreateOddish({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::PIDGEY:
		objectToCreate = PC_instance->CreatePidgey({ 0,0,0 }, objectID->scale, true);
		break;

	case P3_ObjectType::PIKACHU:
		objectToCreate = PC_instance->CreatePikachu({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::PALKIA:
		objectToCreate = PC_instance->CreatePalkia({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::PSYDUCK:
		objectToCreate = PC_instance->CreatePsyduck({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::SEEL:
		objectToCreate = PC_instance->CreateSeel({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::SQUIRTLE:
		objectToCreate = PC_instance->CreateSquirtle({ 0,0,0 }, objectID->scale, true);
		break;

	case P3_ObjectType::VULPIX:
		objectToCreate = PC_instance->CreateVulpix({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::JIGGLYPUFF:
		objectToCreate = PC_instance->CreateJigglypuff({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::GIRATINA:
		objectToCreate = PC_instance->CreateGiratina({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::CYNDAQUIL:
		objectToCreate = PC_instance->CreateCyndaquil({ 0,0,0 }, objectID->scale, true);
		break;
	case P3_ObjectType::DIALGA:
		objectToCreate = PC_instance->CreateDialga({ 0,0,0 }, objectID->scale, true);
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
				std::cout << "Progress Viewed" << std::endl;
				continue;
			}
			if (index == i && PC_instance->sceneStatusDictionary[i]->isEmpty)
			{
				std::cout << "Load Scene: " << index << std::endl;
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
			ImGui::Begin("Progress Bar");
			// Draw progress bar
			ImGui::Text("Scene: %d", index + 1);
			ImGui::ProgressBar(progress, ImVec2(-1, 0), (std::to_string(progress * 100.0f) + "%").c_str());
			ImGui::End();
		}
	}
}
