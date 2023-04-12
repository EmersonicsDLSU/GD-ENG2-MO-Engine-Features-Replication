#include "AllSceneLoader.h"

#include "IExecutionEvent.h"

AllSceneLoader::AllSceneLoader(IExecutionEvent* executionEvent)
{
	execEvent = executionEvent;
}

AllSceneLoader::~AllSceneLoader()
{
	//std::cout << "Destroying stream asset loader. " << std::endl;
}

void AllSceneLoader::OnStartTask()
{
	//std::cout << "Running stream asset loader " << std::endl;
	// simulate loading of very large file

	if (execEvent != nullptr)
		execEvent->onExecuteAll();
	// delete after being done
	delete this;
}

