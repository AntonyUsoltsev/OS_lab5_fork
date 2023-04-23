#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int global_var = 42;

int main() {
    int local_var = 10;

    printf("global var:   %p (%d)\n", &global_var, global_var);
    printf("local var:    %p (%d)\n", &local_var, local_var);

    printf("Parent pid before fork %d\n", getpid());

    int pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        printf("Child pid: %d, parent pid: %d\n", getpid(), getppid());
        sleep(20);
        printf("global var from child:   %p (%d)\n", &global_var, global_var);
        printf("local var from child:    %p (%d)\n", &local_var, local_var);

        global_var += 5;
        local_var += 4;

        printf("global var from child after change:   %p (%d)\n", &global_var, global_var);
        printf("local var from child after change:    %p (%d)\n", &local_var, local_var);

        exit(5);
    } else if (pid > 0) {
        printf("Parent pid after fork %d\n", getpid());
        printf("global var from parent:   %p (%d)\n", &global_var, global_var);
        printf("local var from parent:    %p (%d)\n", &local_var, local_var);
        sleep(20);

        int status;
        int child_pid = waitpid(pid, &status, 0);
        if (child_pid == -1) {
            perror("waitpid");
            exit(1);
        }
        if (WIFEXITED(status)) {
            printf("Child process exited with code %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child process exited due to signal %d\n", WTERMSIG(status));
        }
    }
    return 0;
}
