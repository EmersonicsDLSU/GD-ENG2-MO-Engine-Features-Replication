#include "ThreadPool.h"

#include "PoolWorkerThread.h"

ThreadPool::ThreadPool(String name, int numWorkers) : name(name), numWorkers(numWorkers)
{
	for (int i = 0; i < numWorkers; ++i)
	{
		PoolWorkerThread* tempWorker = new PoolWorkerThread(i, this);
		inactiveThreads.push(tempWorker);
	}
}

ThreadPool::~ThreadPool()
{
	StopScheduler();
	activeThreads.clear();
	while (!inactiveThreads.empty())
	{
		inactiveThreads.pop();
	}
}

void ThreadPool::StartScheduler()
{
	running = true;
	start();
}

void ThreadPool::StopScheduler()
{
	running = false;
}

void ThreadPool::ScheduleTask(IWorkerAction* action)
{
	pendingActions.push(action);
}

void ThreadPool::run()
{
	while (running)
	{
		if (!pendingActions.empty())
		{
			if (!inactiveThreads.empty())
			{
				PoolWorkerThread* thread = inactiveThreads.front();
				inactiveThreads.pop();
				activeThreads.insert({thread->GetThreadID(), thread});

				thread->AssignTask(pendingActions.front());
				thread->start();
				pendingActions.pop();
			}
		}
	}
}

void ThreadPool::OnFinished(int threadID)
{
	if (activeThreads[threadID] != nullptr)
	{
		// create new instance
		delete activeThreads[threadID];
		activeThreads.erase(threadID);

		inactiveThreads.push(new PoolWorkerThread(threadID, this));
	}
}
