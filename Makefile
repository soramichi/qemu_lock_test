mutex:
	gcc qemu_lock_test.c -pthread -O0 -DMUTEX -o qemu_lock_test
spin:
	gcc qemu_lock_test.c -pthread -O0 -o qemu_lock_test
