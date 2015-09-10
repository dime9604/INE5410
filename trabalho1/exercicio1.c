	#include <stdio.h>
	#include <unistd.h>

	int main (int argc, char **argv) {
	  fork();

	  printf("novo processo!\n");
	  return 0;
	}
