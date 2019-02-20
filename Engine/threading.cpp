#include "threading.h"

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

void tsQueue::Push(void* object){
	i32 temporaryIndex = _InterlockedIncrement(&m_head) - 1;
	while (temporaryIndex >= m_tail + SIZE) {
		//Consider Replacing this with a call of the next Pop's function
		std::this_thread::yield();
	}

	m_buffer[temporaryIndex & SIZE_MASK] = object;
}

void* tsQueue::Pop() {
	i32 temporaryIndex = _InterlockedIncrement(&m_tail) - 1;
	while (temporaryIndex >= m_head) {
		std::this_thread::yield();
	}

	return m_buffer[temporaryIndex & SIZE_MASK];
}
