#pragma once
#include "threadsafeQueue.h"
#include "threadingConstructs.h"
#include "application.h"

namespace threading {
	struct Fiber;
	struct Thread;
	class tsQueue;
	struct atomicCounter;
}
struct App;


struct jobDecl
{
	void(*task)(void* data);
	void* data;
};

enum priority {
	HIGH_PRIORITY,
	REGULAR_PRIORITY,
	LOW_PRIORITY
};

class JobManager
{
public:
	JobManager(ui32 fiberCount, ui32 fiberStackSize, jobDecl startupJobs);
	~JobManager();

	void runJobs(jobDecl* jobs, ui32 jobCount, priority jobPriority);
	//void runJobs(jobDecl* jobs, ui32 jobCount, atomicCounter** counter, priority jobPriority);

	void waitForCounter(threading::atomicCounter* counter, ui32 value);

	threading::Fiber* getReadyFiber();
	bool getJob(jobDecl* &returnPtr);

	inline bool shutingDown() { return shuttingDown; };

private:
	threading::Thread* threads;
	ui32 threadCount;

	threading::Fiber* fibers;
	ui32 fiberCount;

	threading::tsQueue readyFibers;
	threading::tsQueue runningFibers;
	threading::tsQueue waitingFibers;
	ui32 fiberStackSize;

	threading::tsQueue highPrioJobQ;
	threading::tsQueue jobQ;
	threading::tsQueue lowPrioJobQ;
	bool shuttingDown;
};