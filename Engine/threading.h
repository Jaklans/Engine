#pragma once
#include <thread>

//Threadsafe Lockless Queue to hold ui64 (pointers)
class tsQueue {
public:
	tsQueue(ui32 size);
	~tsQueue();
	void Push(void* object);
	void* Pop();
private:
	volatile ui32 m_head;
	volatile ui32 m_tail;
	ui32 SIZE;
	ui32 SIZE_MASK;
	void** m_buffer;
};