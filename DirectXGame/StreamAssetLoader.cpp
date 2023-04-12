#include "StreamAssetLoader.h"

#include "IExecutionEvent.h"

StreamAssetLoader::StreamAssetLoader(int sceneIndex, IExecutionEvent* executionEvent)
{
	this->sceneIndex = sceneIndex;
	execEvent = executionEvent;
}

StreamAssetLoader::~StreamAssetLoader()
{
	//std::cout << "Destroying stream asset loader. " << std::endl;
}

void StreamAssetLoader::OnStartTask()
{
	//std::cout << "Running stream asset loader " << std::endl;
	// simulate loading of very large file
	IETThread::sleep(10000);

	if (execEvent != nullptr)
		execEvent->onExecute(sceneIndex);
	// delete after being done
	delete this;
}

