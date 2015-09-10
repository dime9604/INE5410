    #include <unistd.h>
    #include <sys/types.h>
    #include <stdio.h>
    #include <pthread.h>

    #define MAX_THREAD 128

    pthread_mutex_t mutex;

    int x = 0;

    void* incrementa () {
      pthread_mutex_lock(&mutex);
      int i;
      for (i=0; i<100; i++) {
        x++;
      }
      pthread_mutex_unlock(&mutex);
    }

    int main(int argc, char** argv) {
      pthread_mutex_init(&mutex, NULL);
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
      pthread_mutex_destroy(&mutex);
      return 0;
    }
