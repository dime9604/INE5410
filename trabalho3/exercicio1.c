#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_THREAD 128

pthread_mutex_t mutex;
sem_t semaphore;

int x = 0;

void* incrementa () {
  sem_wait(&semaphore);
  int i;
  for (i=0; i<100; i++) {
    x++;
  }
  sem_post(&semaphore);
  pthread_exit (NULL);
}

int main(int argc, char** argv) {
  sem_init(&semaphore, 0 ,1);
  printf("%d\n", 100*MAX_THREAD);
  pthread_t thread[MAX_THREAD];
  int i;
  for (i = 0; i< MAX_THREAD; i++) {
    pthread_create(&thread[i], NULL, incrementa, NULL);
}
  for (i = 0; i< MAX_THREAD; i++) {
    pthread_join(thread[i], NULL);
  }
  printf ("%d\n", x);
  sem_destroy(&semaphore);
  return 0;
}
