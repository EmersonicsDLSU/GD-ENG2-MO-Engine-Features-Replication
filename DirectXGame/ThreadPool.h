#pragma once
#include <queue>
#include <unordered_map>

#include "IETThread.h"
#include "IFinishedTask.h"

class PoolWorkerThread;
class IWorkerAction;

class ThreadPool : public IETThread, public IFinishedTask
{
private:
	typedef  std::string String;
	typedef std::queue<PoolWorkerThread*> ThreadList;
	typedef std::unordered_map<int, PoolWorkerThread*> ActiveThreadMap;
	typedef std::queue<IWorkerAction*> ActionList;

public:
	ThreadPool(String name, int numWorkers);
	~ThreadPool();

	void StartScheduler();
	void StopScheduler();
	void ScheduleTask(IWorkerAction* action);

private:
	void run() override;
	void OnFinished(int threadID) override;

	String name;
	bool running = false;
	int numWorkers = 0;
	ActiveThreadMap activeThreads;
	ThreadList inactiveThreads;
	ActionList pendingActions;
};

