#pragma once
#include "IETThread.h"
#include "IWorkerAction.h"

class IExecutionEvent;

class SceneLoader : public IWorkerAction
{
private:
	typedef std::string String;
public:
	SceneLoader(int sceneIndex, IExecutionEvent* executionEvent);
	~SceneLoader();
private:
	void OnStartTask() override;

	int sceneIndex;
	IExecutionEvent* execEvent;
};

