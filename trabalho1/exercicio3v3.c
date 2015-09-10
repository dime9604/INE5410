    #include <stdio.h>
    #include <unistd.h>
    #include <sys/wait.h>
    #include <stdlib.h>

    int main () {
      pid_t pid1, pid2;
      pid1 = fork();
        if (pid1>0) {
          printf ("Processo pai %u, criou %u\n",getpid(), pid1);
          pid2 = fork();
            if (pid2>0) {
              printf ("Processo pai %u, criou %u\n",getpid(), pid2);
            }
            else {
              printf ("Processo filho %u, criado por %u\n", getpid(), getppid());

            }
          }
      else {
        printf ("Processo filho %u, criado por %u\n", getpid(), getppid());

      }
      if (pid1>0 && pid2>0) {
        wait (NULL);
        return 0;
      }
      pid1 = fork();
        if (pid1>0) {
          printf ("Processo pai %u, criou %u\n",getpid(), pid1);
          pid2 = fork();
            if (pid2>0) {
              printf ("Processo pai %u, criou %u\n",getpid(), pid2);
            }
            else {
              printf ("Processo filho %u, criado por %u\n", getpid(), getppid());
            }
          }
      else {
        printf ("Processo filho %u, criado por %u\n", getpid(), getppid());

      }
      if (pid1>0 && pid2>0) {
        wait (NULL);
      }
      return 0;
    }
