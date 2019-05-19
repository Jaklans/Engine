#pragma once
#include "application.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

App* app;

void job_updateSTUB(void* data) {
	char* buffer = new char[100];
	app->timeManager->timeString(buffer + 75);
	sprintf_s(buffer, 80, 
		"Worker Fiber Update! Core: %i dT: %f CurrentTime: %s", 
		GetCurrentThreadId(), 
		app->timeManager->deltaTime(),
		buffer + 75);
	LOG_DEBUG(buffer);
}
void job_updateLogger(void* data) {
	app->loggingManager->HandleMessages();
}
void job_updateSchedualer(void* data) {
	app->timeManager->UpdateTime();

	jobDecl* jobs = new jobDecl[50];

	for (int i = 0; i < 48; i++) {
		//jobs[i] = { job_updateSTUB, 0 };
	}
	jobs[48] = { &job_updateLogger, 0 };
	jobs[49] = { &job_updateSchedualer, 0 };

	app->jobManager->runJobs(jobs, 50, priority::REGULAR_PRIORITY);
}

void job_renderSchedualer(void* data) {

}
//Initializes Systems
//Data: {}
void job_startup(void* data) {
	app->loggingManager = new logging::Logger();
	logging::LoggingManager = app->loggingManager;
	app->timeManager = new TimeManager();
	jobDecl* job = new jobDecl{ job_updateSchedualer, 0 };
	app->jobManager->runJobs(job, 1, priority::HIGH_PRIORITY);
}
int main() {
	app = new App;
	jobDecl startup = { job_startup, 0 };
	new JobManager(FIBER_COUNT, FIBER_STACK_SIZE, startup);
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