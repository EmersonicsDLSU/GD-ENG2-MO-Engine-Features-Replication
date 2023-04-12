#include "SceneLoader.h"

#include "IExecutionEvent.h"

SceneLoader::SceneLoader(int sceneIndex, IExecutionEvent* executionEvent)
{
	this->sceneIndex = sceneIndex;
	execEvent = executionEvent;
}

SceneLoader::~SceneLoader()
{
	//std::cout << "Destroying stream asset loader. " << std::endl;
}

void SceneLoader::OnStartTask()
{
	//std::cout << "Running stream asset loader " << std::endl;
	// simulate loading of very large file
	IETThread::sleep(10000);

	if (execEvent != nullptr)
		execEvent->onExecute(sceneIndex);
	// delete after being done
	delete this;
}

