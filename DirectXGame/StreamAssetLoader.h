#pragma once
#include "IETThread.h"
#include "IWorkerAction.h"

class IExecutionEvent;

class StreamAssetLoader : public IWorkerAction
{
private:
	typedef std::string String;
public:
	StreamAssetLoader(int sceneIndex, IExecutionEvent* executionEvent);
	~StreamAssetLoader();
private:
	void OnStartTask() override;

	int sceneIndex;
	IExecutionEvent* execEvent;
};

