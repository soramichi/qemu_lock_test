mutex:
	gcc memory_access.c -pthread -O0 -DMUTEX -o memory_access
spin:
	gcc memory_access.c -pthread -O0 -o memory_access
