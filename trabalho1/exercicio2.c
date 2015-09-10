    #include <stdio.h>
    #include <unistd.h>
    #include <sys/wait.h>

    int main (int argc, char** argv) {
      pid_t pai = getpid();
      pid_t pid = fork();
      int printer = 0;
      int i;
      for (i = 0; i<4; i++) {
      if (pid==0) {
        printf ("Processo filho %u, criado por %u\n", getpid(), pai);
        break;
    }else {
        printf ("Processo pai %u, criou %u\n",pai, pid);
        pid = fork();
      }
    }

      if (pid>0) {
        wait(NULL);
      }
      return 0;
    }
