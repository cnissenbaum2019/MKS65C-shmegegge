#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#define SIZE sizeof(char*) * 200

int main()
{
	//Initalize the Shared Memory values
	key_t key;
	int shmid;
	char * data;

	//Get key
	if ((key = ftok("shm.c", 'R')) == -1) {
        perror("ftok");
        exit(1);
    }

	//Connect to the segment (create if necessary)
	if ((shmid = shmget(key, SIZE, 0777 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }

	//Attach to Shared Memory
	data = shmat(shmid, (void *) 0, 0);

	//Check for errors
	if (data == (char *) (-1)) {
		perror("shmat");
		exit(1);
	}

	//Read from the Shared Memory, if it exists
	if (*data) {
		printf("Shared Memory Data in Store:\n %s\n", data);
	} else { //Otherwise notify of its creation
		printf("Shared Memory Created\n");
	}

	//Ask for any change
	printf("Would you like to change the Shared Memory? (y/n)\n");
	char * s = malloc(sizeof(char*)*3);
	fgets(s,3,stdin);
	if(strcmp(s, "y\n") == 0) {

		//Change the Shared Memory
		printf("Enter a String:\n");
		if (gets(data) == -1) {
			perror("gets");
			exit(1);
		}
	}
	free(s);

	//Ask for deletion
	printf("Would you like to delete the Shared Memory Segment? (y/n)\n");
	s = malloc(sizeof(char*)*2);
	fgets(s,2,stdin);
	if(strcmp(s, "y") == 0) {

		//Delete the Shared Memory
		printf("Deleting Shared Memory\n");
		shmctl(shmid, IPC_RMID, NULL);
	} else {

		//Detach from the Shared Memory
		if (shmdt(data) == -1) {
			perror("shmdt");
			exit(1);
		}
	}

	free(s);
	return 0;
}