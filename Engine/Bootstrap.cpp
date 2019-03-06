#include "threading.h"


void initJobManager(void* data) {
	JobManager = new jobManager;
	*JobManager = {5};
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