#include "threading.h"

int main() {
	tsQueue queue = tsQueue(14);

	queue.Push((void*)15);
	queue.Push((void*)12);
	queue.Pop();
}