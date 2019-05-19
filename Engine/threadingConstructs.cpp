#pragma once
#include "threadingConstructs.h"
#include <stdexcept>


namespace threading {
	void executeJob(jobDecl* job) 
	{ 
		if (job != NULL) {
			job->task(job->data);
			//delete job;
		}
	}

	// Thread --------------------------------------

	Thread::~Thread()
	{
		thread->join();
		delete thread;
	}

	void Thread::initialize(Callback threadCallback, void* callbackData, JobManager* managerPtr, ui32 affinity)
	{
		#ifdef WIN32
			manager = managerPtr;
			thread = new std::thread(threadCallback, callbackData);
			//Set Affinity

		#endif // WIN32
	}

	void fiberCallback(Fiber* data)
	{
		jobDecl* job;
		while (!data->manager->shutingDown()) {
			if (data->manager->getJob(job)) {
				job->task(job->data);
			}
			Sleep(1);
		}
	}

	void threading::threadCallback(void* data)
	{
		//is it nessessary?
		((Thread*)data)->threadFiber.CreateFromCaller();
		((Thread*)data)->manager->getReadyFiber()->SwitchTo();
	}


	void Fiber::CreateFromCaller()
	{
		fiber = ConvertThreadToFiber(NULL);
	}
	Fiber::~Fiber()
	{
		//DeleteFiber(fiber);
	}
	void Fiber::SwitchTo()
	{
		SwitchToFiber(fiber);
	}
}
