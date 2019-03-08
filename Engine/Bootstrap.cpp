#include "threading.h"

void initJobManager(void* data) {
	ui32 threadCount = std::thread::hardware_concurrency();

	if (threadCount < 1) {
		//Warn
		threadCount = 2;
	}
	if (threadCount > 64) {
		//Warn!!!
		threadCount = 64;
	}

	std::thread* threads = (std::thread*)malloc(sizeof(std::thread) * threadCount);

	JobManager = new jobManager(threads, threadCount, FIBER_COUNT, )
}

int main() {
	ConvertThreadToFiber(NULL);

	initJobManager(NULL);

	delete JobManager;
}



//Setup Tasks
	//Allocate Memory Buffers
	//Initialize Graphics API
	//Load Assets
	//Set Up Scene Graph

//Runtime Loop
	//Update (Game Objects)
	//Update (Static)
	//Render Tasks