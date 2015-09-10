#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMPRODUTORES 5
#define NUMCONSUMIDORES 5
#define N 2

int buffer [15];
sem_t cheio, vazio, lock_prod, lock_cons;
int f = 0, j = 0;

void* produtor () {
  while (1) {
    sem_wait(&vazio);
    sem_wait(&lock_prod);
    f = (f+1)%15;
    buffer[f] = (int)rand() %100;
    printf ("Thread %lu produzindo buffer[%d]=%d\n\n", pthread_self(), f, buffer[f]);
    sem_post(&lock_prod);
    sem_post(&cheio);
  }
  pthread_exit(NULL);
}

void* consumidor () {
  while (1) {
    sem_wait(&cheio);
    sem_wait(&lock_cons);
    j = (j+1)%15;
    printf ("Thread %lu consumindo buffer[%d]=%d\n\n", pthread_self(), j, buffer[j]);
    sem_post(&lock_cons);
    sem_post(&vazio);
    sleep(2);
  }
  pthread_exit(NULL);

}

int main (void) {
  pthread_t produtoras [NUMPRODUTORES];
  pthread_t consumidoras [NUMCONSUMIDORES];

  sem_init (&cheio, 0,0);
  sem_init (&vazio, 0, 15);
  sem_init (&lock_prod, 0, 1);
  sem_init (&lock_cons, 0, 1);

  int i;
  for (i = 0; i< NUMPRODUTORES; i++) {
    pthread_create(&produtoras[i], NULL, produtor, NULL);
  }
  for (i = 0; i< NUMCONSUMIDORES; i++) {
  pthread_create(&consumidoras[i], NULL, consumidor, NULL);
  }
  for (i = 0; i< NUMPRODUTORES; i++) {
    pthread_join(produtoras[i], NULL);
  }
  for (i = 0; i< NUMCONSUMIDORES; i++) {
    pthread_join(consumidoras[i], NULL);
  }

  sem_destroy (&cheio);
  sem_destroy (&vazio);
  sem_destroy (&lock_prod);
  sem_destroy (&lock_cons);

  pthread_exit (NULL);
}
