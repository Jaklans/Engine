#pragma once
#include "threadingConstructs.h"
#include <stdexcept>


namespace threading {
	void executeJob(job* job) 
	{ 
		if (job != NULL) {
			job->task(job->data);
		}
	}

	// Thread --------------------------------------

	Thread::~Thread()
	{
		thread->join();
		delete thread;
	}

	void Thread::initialize(Callback threadCallback, void* callbackData, ui32 affinity)
	{
		#ifdef WIN32
			thread = new std::thread(threadCallback, callbackData);
			//Set Affinity

		#endif // WIN32
	}

	void fiberCallback(void* data)
	{
		job job;
		while (!Manager.Job->shutingDown()) {
			if (Manager.Job->getJob(job)) {
				job.task(job.data);
			}
			Sleep(1);
		}
	}

	void threading::threadCallback(void* data)
	{
		//is it nessessary?
		((Thread*)data)->threadFiber.CreateFromCaller();
		Manager.Job->getNewFiber()->SwitchTo();
	}


	// Fiber --------------------------------------
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
