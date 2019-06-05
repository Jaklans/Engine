#pragma once

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


ApplicationManager Manager = ApplicationManager();


void job_updateSTUB(void* data) {
	char buffer[100];
	Manager.Time->timeString(buffer + 75);
	sprintf_s(buffer, 80, 
		"Worker Fiber Update! Core: %i dT: %f CurrentTime: %s", 
		GetCurrentThreadId(), 
		Manager.Time->deltaTime(),
		buffer + 75);
	LOG_DEBUG(buffer);
}
void job_updateLogger(void* data) {
	Manager.Log->HandleMessages();
}
void job_updateSchedualer(void* data) {
	Manager.Time->UpdateTime();

	job* jobs = new job[50];

	for (int i = 0; i < 48; i++) {
		jobs[i] = { job_updateSTUB, 0 };
	}
	jobs[48] = { &job_updateLogger, 0 };
	jobs[49] = { &job_updateSchedualer, 0 };

	Manager.Job->runJobs(jobs, 50, priority::REGULAR_PRIORITY);
}

void job_renderSchedualer(void* data) {

}
//Initializes Systems
//Data: {}
void job_startup(void* data) {
	Manager.Log = new logging::Logger();
	Manager.Time = new TimeManager();
	job updateSchedualer = { job_updateSchedualer, 0 };
	Manager.Job->runJob(updateSchedualer, priority::HIGH_PRIORITY);
}

int main() {
	Manager.Job = new JobManager(FIBER_COUNT, FIBER_STACK_SIZE, { job_startup, 0 });
	//_CrtDumpMemoryLeaks();
}



//Setup Tasks
	//Initialize Job Manager

	//Allocate Memory Buffers
void initMemoryBuffers() {
	//Stubby
}
	//Initialize Graphics API
void initVulkan() {
	//Stubs
}
	//Load Assets

	//Set Up Scene Graph

//Runtime Loop
	//Update (Game Objects)
	//Update (Static)
	//Render Tasks

//Shutdown
	//Cleanup
void cleanup() {
	delete logging::LoggingManager;
}