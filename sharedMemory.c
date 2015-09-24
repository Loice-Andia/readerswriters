/*----------------------------------------------------------------------------------------------------------------------
  Question:
  Implement a program that solves the readers/writers problem using a one word shared memory

  Understanding:
  Making a program that acts as a semaphore i.e. the program regulates read and writes of 
  programs accessing a resource.

  Approach:
  One file is being accessed by two program but before a program access a file it first reads the shared
  memory to check if the file being accessed if free or not

 ----------------------------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/ipc.h>
#include <sys/shm.h>




/*------------------------------------------------------------------------------------------------------------
   
   This are wrapper functions that will assist in making the program:

   AllocatedSharedMemory -> creates the shared memory and return the Id of the shared memory chunk

   MapSharedMemory -> maps the memory segment created by the AllocatedSharedMemory into the
   address space and returns address of the mapped block (Process Address Space)


-------------------------------------------------------------------------------------------------------------*/
int AllocateSharedMemory(int n);
void* MapSharedMemory(int id);



int main(int argc, char* argv[])
{
	int idShMem; 
	char* buf; 
	char buffer[1000], buffer2[1000];
	FILE *fileOpener;

	
	/*Get shared memory segment id off the command line.*/
	if (argc < 2)
	{
		idShMem = AllocateSharedMemory(BUFSIZ);
		buf = (char*) MapSharedMemory(idShMem);

		sprintf(buf, "%s","r");
		
		printf("Shared memory id is: %d\n",idShMem);

	}
	else
	{
		/*we have the shared memory value*/
		idShMem = atoi(argv[1]);
		buf = (char*) MapSharedMemory(idShMem);
		

		
	}
	
	
	fileOpener = fopen("shared.txt", "w");

	/*try to write*/
	printf("\nAttempting to write\n");
	if(strcmp(buf,"r")==0){

		printf("\nWriting is a GO\n");

		sprintf(buf, "%s", "w");

		scanf("%s", buffer);

		fprintf(fileOpener, "%s" ,buffer);

		printf("\n-------------------releasing the Lock------------------\n");

		sprintf(buf, "%s", "r");
	}
	else{

		printf("\nWriting is blocked\n");

		while(strcmp(buf,"r")!=0){

			/*wait until your turn*/

		}

		printf("\nWriting is a GO\n");

		sprintf(buf, "%s", "w");

		scanf("%s", buffer);

		fprintf(fileOpener, "%s" ,buffer);

		printf("\n-------------------releasing the Lock------------------\n");

		sprintf(buf, "%s", "r");



	}


	
	/*program one to wait for program 2*/
	usleep(10000);
	

	/*close the file and open it for reading */
	fclose(fileOpener);
	memset(buffer2, 0, 1000);
	fileOpener = fopen("shared.txt", "r");

	printf("\nAttempting to read\n");
	if(strcmp(buf,"r")==0){

		printf("\nReading is a GO\n");

		sprintf(buf, "%s", "w");

		fgets(buffer2, 999, fileOpener);

		puts(buffer2);

		printf("\nreleasing the Lock\n");

		sprintf(buf, "%s", "r");
	}
	else{

		printf("\nReading is blocked\n");

		while(strcmp(buf,"r")!=0){

			/*wait until your turn*/

		}

		printf("\nReading is a GO\n");

		sprintf(buf, "%s", "w");

		fgets(buffer2, 999, fileOpener);

		puts(buffer2);

		printf("\nreleasing the Lock\n");

		sprintf(buf, "%s", "r");



	}

	fclose(fileOpener);

	return 0;

}



/*------------------------------------------------------------------------------------------------------------

  Allocates a shared memory segment.

  @param n Size (in bytes) of chunk to allocate.
  @return Id of shared memory chunk.

-------------------------------------------------------------------------------------------------------------*/

int AllocateSharedMemory(int n)
{
	assert(n > 0); 
	return shmget(IPC_PRIVATE, 2*(sizeof(char)), IPC_CREAT | SHM_R | SHM_W);
}





/*------------------------------------------------------------------------------------------------------------

  Maps a shared memory segment onto our address space.

  @param id Shared memory block to map.
  @return Address of mapped block.

-------------------------------------------------------------------------------------------------------------*/

void* MapSharedMemory(int id)
{
	void* addr;
	assert(id != 0); 
	addr = shmat(id, NULL, 0); 
	shmctl(id, IPC_RMID, NULL); 
	return addr;
}



