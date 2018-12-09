make: shm.c
	gcc -o Shm shm.c

run: Shm
	./Shm

clean: Shm
	rm Shm