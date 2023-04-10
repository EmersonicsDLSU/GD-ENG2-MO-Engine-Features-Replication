#include "PoolWorkerThread.h"

#include "IFinishedTask.h"
#include "IWorkerAction.h"

PoolWorkerThread::PoolWorkerThread(int id, IFinishedTask* finishedTask) : id(id), finishedTask(finishedTask)
{
	
}

PoolWorkerThread::~PoolWorkerThread()
{

}

int PoolWorkerThread::GetThreadID()
{
	return id;
}

void PoolWorkerThread::AssignTask(IWorkerAction* action)
{
	this->action = action;
}

void PoolWorkerThread::run()
{
	if (action != nullptr)
	{
		action->OnStartTask();
		finishedTask->OnFinished(id);
	}
}
