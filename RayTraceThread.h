#pragma once

#include "IETThread.h"

class IWorkerAction;
class IFinishedTask;

class RayTraceThread : public IETThread
{
public:
	RayTraceThread(int id, IFinishedTask* task);
	~RayTraceThread();

private:
	void run() override;

public:
	void assignTask(IWorkerAction* action);
	int getThreadID();

private:
	int id = 0;
	IWorkerAction* action;
	IFinishedTask* finishedTask;
};

