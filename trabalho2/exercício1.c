	#include <unistd.h>
	#include <sys/types.h>
	#include <stdio.h>
	#include <pthread.h>

	void* imprime () {
	  unsigned long tid =(unsigned long) pthread_self();
	  printf("TID= %lu\n", tid);
	  pthread_exit(NULL);
	}

	int main(int argc, char** argv) {
	  pthread_t thread;
	  pthread_create(&thread, NULL, imprime, NULL);
	  pthread_join(thread, NULL);
	  pthread_exit(NULL);
	}
