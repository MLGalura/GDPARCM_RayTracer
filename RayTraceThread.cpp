#include "RayTraceThread.h"
#include "IWorkerAction.h"
#include "ThreadPool.h"

RayTraceThread::RayTraceThread(int id, IFinishedTask* task)
{
	this->id = id;
	this->finishedTask = task;
}

RayTraceThread::~RayTraceThread()
{
}

void RayTraceThread::run()
{

    try {
        if (!this->action) {
            throw std::runtime_error("RayTraceThread::run() - action is null.");
        }

        this->action->onStartTask();

        if (!this->finishedTask) {
            throw std::runtime_error("RayTraceThread::run() - finishedTask is null.");
        }

        this->finishedTask->onFinished(this->id);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in RayTraceThread #" << this->id << ": " << e.what() << std::endl;
    }
    catch (...) {
        std::cerr << "Unknown error occurred in RayTraceThread #" << this->id << std::endl;
    }
}

void RayTraceThread::assignTask(IWorkerAction* action)
{
	this->action = action;
}

int RayTraceThread::getThreadID()
{
	return this->id;
}
