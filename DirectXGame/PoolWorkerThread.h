#pragma once
#include "IETThread.h"

class IWorkerAction;
class IFinishedTask;

class PoolWorkerThread : public IETThread
{
	public:
		PoolWorkerThread(int id, IFinishedTask* finishedTask);
		~PoolWorkerThread();

		int GetThreadID();
		void AssignTask(IWorkerAction* action);

private:
	void run() override;

	int id = 0;
	IWorkerAction* action;
	IFinishedTask* finishedTask;
};

