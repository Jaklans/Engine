#include "threading.h"
#include <stdexcept>



namespace threading {
	// Task Queue ----------------------------------

	tsQueue::tsQueue(ui32 size)
	{
		//Increace the size to the next power of 2
		SIZE = 1;
		while (SIZE < size) {
			SIZE *= 2;
		}
		SIZE_MASK = SIZE - 1;

		m_buffer = (void**)malloc(sizeof(void*) * SIZE);
	}

	tsQueue::~tsQueue()
	{
		free(m_buffer);
	}

	void tsQueue::Push(void* object) {
		ui32 temporaryIndex = _InterlockedIncrement(&m_head) - 1;
		while (temporaryIndex >= m_tail + SIZE) {
			//Queue has run out of space, start a fire
			throw std::runtime_error("Queue has run out of space!");
		}

		m_buffer[temporaryIndex & SIZE_MASK] = object;
	}

	void* tsQueue::Pop() {
		ui32 temporaryIndex = _InterlockedIncrement(&m_tail) - 1;
		while (temporaryIndex >= m_head) {
			std::this_thread::yield();
		}

		return m_buffer[temporaryIndex & SIZE_MASK];
	}


	// Thread --------------------------------------

	int Thread::begin()
	{
		*threadPtr = std::thread();
		return RESULT_SUCCESS;
	}
	int Thread::end()
	{
		return 0;
	}

	Result Thread::internalLoop()
	{

		while (active) {

			//Call Current Task
		}


		return RESULT_SUCCESS;
	}
}



