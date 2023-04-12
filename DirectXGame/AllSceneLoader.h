#pragma once
#include "IETThread.h"
#include "IWorkerAction.h"

class IExecutionEvent;

class AllSceneLoader : public IWorkerAction
{
private:
	typedef std::string String;
public:
	AllSceneLoader(IExecutionEvent* executionEvent);
	~AllSceneLoader();
private:
	void OnStartTask() override;

	int sceneIndex;
	IExecutionEvent* execEvent;
};

