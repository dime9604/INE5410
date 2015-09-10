  #include <unistd.h>
  #include <sys/types.h>
  #include <stdio.h>
  #include <pthread.h>

  #define MAX_THREAD 5

  void* imprime () {
    unsigned long tid = (unsigned long)pthread_self();
    printf("TID= %lu\n", tid);
    pthread_exit(NULL);
  }

  int main(int argc, char** argv) {
    pthread_t thread [MAX_THREAD];
    int i;
    for (i = 0; i< MAX_THREAD; i++) {
      pthread_create(&thread[i], NULL, imprime, NULL);
  }
    for (i = 0; i< MAX_THREAD; i++){
      pthread_join(thread[i], NULL);
    }
    return 0;
  }
