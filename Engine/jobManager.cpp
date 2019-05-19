#pragma once
#include "jobManager.h"
#include <stdexcept>

using namespace threading;

#define HIGH_PRIORITY_JOB_Q_SIZE    128
#define REGULAR_PRIORITY_JOB_Q_SIZE 512
#define LOW_PRIORITY_JOB_Q_SIZE     256

JobManager::JobManager(ui32 fiberCount, ui32 fiberStackSize, jobDecl startupJob)
	:
	readyFibers(fiberCount),
	runningFibers(fiberCount),
	waitingFibers(fiberCount),
	highPrioJobQ(HIGH_PRIORITY_JOB_Q_SIZE),
	jobQ(REGULAR_PRIORITY_JOB_Q_SIZE),
	lowPrioJobQ(LOW_PRIORITY_JOB_Q_SIZE),
	shuttingDown(false)
{
	app->jobManager = this;
	ui32 threadCount = std::thread::hardware_concurrency();

	if (threadCount < 1) {
		//Warn
		threadCount = 2;
	}
	if (threadCount > 64) {
		//Warn
		threadCount = 64;
	}

	fibers = new Fiber[fiberCount];
	for (ui32 i = 1; i < fiberCount; i++) {
		fibers[i].fiber = CreateFiber(FIBER_STACK_SIZE, (threading::Callback)fiberCallback, &(fibers[i]));
		fibers[i].manager = this;
		readyFibers.Enqueue(&fibers[i]);
	}

	threads = new Thread[threadCount];
	for (ui32 i = 0; i < threadCount; i++) {
		threads[i].initialize(threadCallback, &threads[i], this, 0);
	}

	highPrioJobQ.Enqueue(&startupJob);

	fibers[0].CreateFromCaller();
	fibers[0].manager = this;
	readyFibers.Enqueue(&fibers[0]);
	fiberCallback(fibers);
}

JobManager::~JobManager()
{
	shuttingDown = true;

	delete[] threads;
	delete[] fibers;
}

void JobManager::runJobs(jobDecl* jobs, ui32 jobCount, priority jobPriority)
{
	jobDecl** pointers = new jobDecl*[jobCount];
	for (ui32 i = 0; i < jobCount; i++) {
		pointers[i] = jobs + i;
	}

	switch (jobPriority)
	{
	case HIGH_PRIORITY:
		highPrioJobQ.Enqueue(reinterpret_cast<void**>(pointers), jobCount);
		break;
	case REGULAR_PRIORITY:
		jobQ.Enqueue(reinterpret_cast<void**>(pointers), jobCount);
		break;
	case LOW_PRIORITY:
		lowPrioJobQ.Enqueue(reinterpret_cast<void**>(pointers), jobCount);
		break;
	default:
		jobQ.Enqueue(reinterpret_cast<void**>(pointers), jobCount);
		break;
	}

	delete pointers;
}


Fiber* JobManager::getReadyFiber()
{
	void* returnPtr = 0;
	if (readyFibers.Dequeue(returnPtr)) {
		runningFibers.Enqueue(returnPtr);
	}
	else {
		throw std::runtime_error("Failed to get ready fiber");
	}
	return (Fiber*)returnPtr;
}

bool JobManager::getJob(jobDecl* &returnPtr)
{
	if (highPrioJobQ.Dequeue(reinterpret_cast<void*&>(returnPtr))) 
		return true;
	if (jobQ.Dequeue(reinterpret_cast<void*&>(returnPtr))) return true;
	if (lowPrioJobQ.Dequeue(reinterpret_cast<void*&>(returnPtr))) return true;
	return false;
}
