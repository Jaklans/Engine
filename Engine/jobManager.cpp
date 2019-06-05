#pragma once
#include "jobManager.h"
#include <stdexcept>

using namespace threading;

#define HIGH_PRIORITY_JOB_Q_SIZE    128
#define REGULAR_PRIORITY_JOB_Q_SIZE 512
#define LOW_PRIORITY_JOB_Q_SIZE     256

class JobLinkedList
{
public:
	JobLinkedList();
	~JobLinkedList();

	void Push(job job);
	job Pop();
private:

};

JobLinkedList::JobLinkedList()
{
}

JobLinkedList::~JobLinkedList()
{
}

JobManager::JobManager(ui32 fiberCount, ui32 fiberStackSize, job startupJob)
	:
	highPrioJobQ(HIGH_PRIORITY_JOB_Q_SIZE),
	jobQ(REGULAR_PRIORITY_JOB_Q_SIZE),
	lowPrioJobQ(LOW_PRIORITY_JOB_Q_SIZE),
	shuttingDown(false)
{
	Manager.Job = this;

	//Check threadcount
	ui32 threadCount = std::thread::hardware_concurrency();
	if (threadCount < 1) {
		//Warn
		threadCount = 2;
	}
	if (threadCount > 64) {
		//Warn
		threadCount = 64;
	}

	//Initialize fibers
	FiberNode* fibers = new FiberNode[fiberCount];
	for (ui32 i = 1; i < fiberCount; i++) {
		fibers[i].fiber.fiber = CreateFiber(FIBER_STACK_SIZE, &fiberCallback, 0);
		fibers[i].next = fibers + i + 1;
	}
	inactiveFibers = fibers + 1;

	//Initialize threads
	threads = new Thread[threadCount];
	for (ui32 i = 0; i < threadCount; i++) {
		threads[i].initialize(threadCallback, &threads[i], 0);
	}

	//Run Startup Job
	highPrioJobQ.Enqueue(startupJob);

	//Convert this thread to fiber worker, and place it in inactive list
	fibers[0].fiber.CreateFromCaller();
	fibers[fiberCount - 1].next = fibers;
	fiberCallback(&fibers->fiber);
}

JobManager::~JobManager()
{
	shuttingDown = true;

	delete[] threads;
	delete[] fibers;
}

void JobManager::runJob(job& job, priority jobPriority)
{
	switch (jobPriority)
	{
	case HIGH_PRIORITY:
		highPrioJobQ.Enqueue(job);
		break;
	case REGULAR_PRIORITY:
		jobQ.Enqueue(job);
		break;
	case LOW_PRIORITY:
		lowPrioJobQ.Enqueue(job);
		break;
	default:
		jobQ.Enqueue(job);
		break;
	}
} 

void JobManager::runJobs(job* job, ui32 jobCount, priority jobPriority)
{
	switch (jobPriority)
	{
	case HIGH_PRIORITY:
		highPrioJobQ.Enqueue(job, jobCount);
		break;
	case REGULAR_PRIORITY:
		jobQ.Enqueue(job, jobCount);
		break;
	case LOW_PRIORITY:
		lowPrioJobQ.Enqueue(job, jobCount);
		break;
	default:
		jobQ.Enqueue(job, jobCount);
		break;
	}
}



//Gets a new fiber. Waiting fibers are prioritized
Fiber* JobManager::getNewFiber()
{
	FiberNode* temp = waitingFibers;

	if (waitingFibers != 0) {
		//Mutex for checking waiting fiber list
		checkingFibers.increment();
		if (checkingFibers.counter <= 1) {

			if (temp->fiber.wakeCondition->counter <= 0) {
				waitingFibers = temp->next;
				return &temp->fiber;
			}

			while (temp->next != 0) {
				if (temp->next->fiber.wakeCondition->counter < 0) {
					temp->next = temp->next->next;
					return &temp->fiber;
				}

				temp = temp->next;
			}
		}
		checkingFibers.decrement();
	}

	if (inactiveFibers != 0) {
		temp = inactiveFibers;
		inactiveFibers = inactiveFibers->next;
		return &temp->fiber;
	}
	else {
		throw std::runtime_error("Failed to get ready fiber");
	}
}

bool JobManager::getJob(job& jobRef)
{
	if (highPrioJobQ.Dequeue(jobRef)) return true;
	if (jobQ.Dequeue(jobRef)) return true;
	if (lowPrioJobQ.Dequeue(jobRef)) return true;

	return false;
}
