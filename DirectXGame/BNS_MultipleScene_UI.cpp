#include "BNS_MultipleScene_UI.h"
#include "BNS_Camera.h"
#include "BNS_CameraHandler.h"
#include "BNS_FileExplorer.h"
#include "BNS_PrimitiveCreation.h"
#include "BNS_Texture.h"
#include "P3_ObjectID.h"
#include "IETThread.h"

BNS_MultipleScene_UI::BNS_MultipleScene_UI(std::string name, int ID) : BNS_AUIScreen(name, ID)
{
	PC_instance = BNS_PrimitiveCreation::Instance();
	
	for(int i = 0 ; i < 5; i++)
	{
		
		for(int j = 0; j < 5; j++)
		{
			P3_Transforms *trans_1 = new P3_Transforms{ Vector3D( -5 + (5 * j) + i,-5 + (5 * j)+ i,-5 + (5 * j)+ i)};
			unused.emplace_back(trans_1);
		}
		
	}

	// initialize semaphore
	testSem = new std::counting_semaphore<1>(1);
	mutexSem = new std::counting_semaphore<1>(1);
	showAllSem = new std::counting_semaphore<5>(0);
	int counter = 0;

	for(int i =0; i <5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			P3_ObjectID* obj_1 = new P3_ObjectID(counter, static_cast<P3_ObjectType>(counter), i);
			objectsToLoad.emplace_back(obj_1);
			counter++;
		}
		
	}
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
			//std::cout << "Execute Load Scene: " << sceneIndex << std::endl;
			// create the object; object is still not active
			ExecuteObject(objectWithSceneIndex);
			mutexSem->release();

			IETThread::sleep(1000);
			mutexSem->acquire();
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
			testSem->acquire();
			currentScenesCompleted += 1;
			showAllSem->release();
			std::cout << "Semaphore Release: " << currentScenesCompleted << "/5" << std::endl;
			testSem->release();

			if (!isViewAll && PC_instance->sceneStatusDictionary[sceneIndex]->isComplete && !PC_instance->sceneStatusDictionary[sceneIndex]->isEmpty && !PC_instance->sceneStatusDictionary[sceneIndex]->isLoading)
			{
				ShowScene(sceneIndex);
			}
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
			isViewAll = true;
		}

		// it needs 5 keys
		for (int i = 0; i < 5; i++)
		{
			showAllSem->acquire();
			testSem->acquire();
			currentScenesCompleted -= 1;
			std::cout << "Semaphore Acquire: " << currentScenesCompleted << "/5" << std::endl;
			testSem->release();
		}

		for (int index = 0; index < 5; ++index)
		{
			PC_instance->sceneStatusDictionary[index]->isViewed = true;
		}
		PC_instance->ShowAll();
		for (int i = 0; i < 5; i++)
		{
			showAllSem->release();
			testSem->acquire();
			currentScenesCompleted += 1;
			std::cout << "Semaphore Release: " << currentScenesCompleted << "/5" << std::endl;
			testSem->release();

			isViewAll = false;
		}

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

		// creates an icon to the file
		TexturePtr icon = BNS_FileExplorer::GetInstance()->GetImageHashTable()["scene_" + std::to_string(i)];
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
	
	Vector3D pos;
	std::srand(std::time(nullptr));
	int randomNumber = rand() % unused.size();
	used.emplace_back(unused[randomNumber]);
	pos = unused[randomNumber]->pos;
	unused[randomNumber]->sceneIndex = objectID->sceneIndex;
	//std::cout << "Unused Positions: " << unused[randomNumber]->sceneIndex << std::endl;
	//std::cout << "Used Positions: " << used[used.size() - 1]->sceneIndex << std::endl;
	unused.erase(unused.begin() + randomNumber);

	BNS_AGameObject *objectToCreate = nullptr;
	switch(objectID->objectType)
	{
	case P3_ObjectType::TEAPOT:
		objectToCreate = PC_instance->CreateTeapot(pos, objectID->scale, true);
		break;
	case P3_ObjectType::STATUE:
		objectToCreate = PC_instance->CreateStatue(pos, objectID->scale, true);
		break;
	case P3_ObjectType::BUNNY:
		objectToCreate = PC_instance->CreateBunny(pos, objectID->scale, true);
		break;
	case P3_ObjectType::ARMADILLO:
		objectToCreate = PC_instance->CreateArmadillo(pos, objectID->scale, true);
		break;
	case P3_ObjectType::EARTH:
		objectToCreate = PC_instance->CreateEarth(pos, objectID->scale, true);
		break;

	case P3_ObjectType::CHARMANDER:
		objectToCreate = PC_instance->CreateCharmander(pos, objectID->scale, true);
		break;
	case P3_ObjectType::CUBONE:
		objectToCreate = PC_instance->CreateCubone(pos, objectID->scale, true);
		break;
	case P3_ObjectType::EEVEE:
		objectToCreate = PC_instance->CreateEevee(pos, objectID->scale, true);
		break;
	case P3_ObjectType::FARFETCH:
		objectToCreate = PC_instance->CreateFarfetch(pos, objectID->scale, true);
		break;
	case P3_ObjectType::GROWLITHE:
		objectToCreate = PC_instance->CreateGrowlithe(pos, objectID->scale, true);
		break;

	case P3_ObjectType::MACHOP:
		objectToCreate = PC_instance->CreateMachop(pos, objectID->scale, true);
		break;
	case P3_ObjectType::NIDORANF:
		objectToCreate = PC_instance->CreateNidoranF(pos, objectID->scale, true);
		break;
	case P3_ObjectType::NIDORANM:
		objectToCreate = PC_instance->CreateNidoranM(pos, objectID->scale, true);
		break;
	case P3_ObjectType::ODDISH:
		objectToCreate = PC_instance->CreateOddish(pos, objectID->scale, true);
		break;
	case P3_ObjectType::PIDGEY:
		objectToCreate = PC_instance->CreatePidgey(pos, objectID->scale, true);
		break;

	case P3_ObjectType::PIKACHU:
		objectToCreate = PC_instance->CreatePikachu(pos, objectID->scale, true);
		break;
	case P3_ObjectType::PALKIA:
		objectToCreate = PC_instance->CreatePalkia(pos, objectID->scale, true);
		break;
	case P3_ObjectType::PSYDUCK:
		objectToCreate = PC_instance->CreatePsyduck(pos, objectID->scale, true);
		break;
	case P3_ObjectType::SEEL:
		objectToCreate = PC_instance->CreateSeel(pos, objectID->scale, true);
		break;
	case P3_ObjectType::SQUIRTLE:
		objectToCreate = PC_instance->CreateSquirtle(pos, objectID->scale, true);
		break;

	case P3_ObjectType::VULPIX:
		objectToCreate = PC_instance->CreateVulpix(pos, objectID->scale, true);
		break;
	case P3_ObjectType::JIGGLYPUFF:
		objectToCreate = PC_instance->CreateJigglypuff(pos, objectID->scale, true);
		break;
	case P3_ObjectType::GIRATINA:
		objectToCreate = PC_instance->CreateGiratina(pos, objectID->scale, true);
		break;
	case P3_ObjectType::CYNDAQUIL:
		objectToCreate = PC_instance->CreateCyndaquil(pos, objectID->scale, true);
		break;
	case P3_ObjectType::DIALGA:
		objectToCreate = PC_instance->CreateDialga(pos, objectID->scale, true);
		break;
	}
	objectToCreate->SetActive(false);
	// add it to the sceneObjectDictionary
	PC_instance->sceneObjectDictionary[objectID->sceneIndex].emplace_back(objectToCreate);
}

void BNS_MultipleScene_UI::DeleteObjectsInScene(int sceneIndex) // 1 scene = 5 objs deleted
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

	// reset the scene
	PC_instance->sceneStatusDictionary[sceneIndex]->isComplete = false;
	PC_instance->sceneStatusDictionary[sceneIndex]->isEmpty = true;
	PC_instance->sceneStatusDictionary[sceneIndex]->isLoading = false;
	PC_instance->sceneStatusDictionary[sceneIndex]->isProgressViewed = false;
	PC_instance->sceneStatusDictionary[sceneIndex]->isViewed = false;

	objectsOnScene.shrink_to_fit();
	objectsToLoad.shrink_to_fit();
	testSem->acquire();
	showAllSem->acquire();
	currentScenesCompleted -= 1;
	std::cout << "Semaphore Acquire: " << currentScenesCompleted << "/5" << std::endl;
	testSem->release();
}

void BNS_MultipleScene_UI::OnEntryLeftClick(int index)
{
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		if (PC_instance->sceneStatusDictionary[index]->isLoading)
		{
			ResetAllButtonsProgressView();
			PC_instance->sceneStatusDictionary[index]->isProgressViewed = true;
			currentProgressViewed = index;
			std::cout << "Progress Viewed" << std::endl;
		}
		if (PC_instance->sceneStatusDictionary[index]->isEmpty && !PC_instance->sceneStatusDictionary[index]->isLoading && !PC_instance->sceneStatusDictionary[index]->isComplete)
		{
			std::cout << "Load Scene: " << index << std::endl;
			PC_instance->LoadAScene(index, this);
		}
		else if (PC_instance->sceneStatusDictionary[index]->isComplete && !PC_instance->sceneStatusDictionary[index]->isEmpty && !PC_instance->sceneStatusDictionary[index]->isLoading)
		{
			ShowScene(index);
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
			// delete all the objects in that scene
			for (int i = 0; i < 5; ++i)
			{
				if (index == i)
				{
					DeleteObjectsInScene(index);
					PC_instance->ResetScene(index);
					break;
				}
			}
			// returning the used positions
			for (auto it = used.begin(); it != used.end();)
			{
				if ((*it)->sceneIndex == index)
				{
					(*it)->sceneIndex = -1;
					unused.push_back(*it);
					it = used.erase(it);

				}
				else
				{ // only increment if not erasing an element
					++it;
				}
			}
			unused.shrink_to_fit();
			used.shrink_to_fit();
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

void BNS_MultipleScene_UI::ShowScene(int index)
{
	ResetAllButtonsView();
	PC_instance->sceneStatusDictionary[index]->isViewed = true;
	PC_instance->ShowScene(index);
}
