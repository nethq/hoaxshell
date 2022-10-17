#include <stdio.h>
#include <stdlib.h>
#include <threads.h>
#include <string.h>
typedef struct thread_args {
    long int threadID;
    long int processID;
    int hoaxshell_port;
    char *hoaxshell_ip;
    char *hoaxshell_exec_path;
}thread_args;

int win = 0;//0 = non-windows, 1 = windows
char *hoaxshell_exec = "hoaxshell.py\0";
int is_windows()
{
    char *user_os = getenv("OS");
    if (user_os != NULL)
    {
        if (strstr(user_os, "Windows") != NULL)
        {
            return 1;
        }
    }
    return 0;
}

int spawn_hoaxshell(void *args)
{
    struct thread_args *t_args = (struct thread_args *)args;
    char *command = (char*)malloc(200);
    if (win)
    {
        sprintf(command, "start cmd -k \"python %s -s %s -p %d\"",t_args->hoaxshell_exec_path,t_args->hoaxshell_ip, t_args->hoaxshell_port);

    }
    else
    {
        printf("\nExecuting : %s -s %s -p %d\n",t_args->hoaxshell_exec_path,t_args->hoaxshell_ip, t_args->hoaxshell_port);
        sprintf(command, "gnome-terminal -- python3 %s -s %s -p %d",t_args->hoaxshell_exec_path,t_args->hoaxshell_ip, t_args->hoaxshell_port);
    }
    system(command);
    free(command);
    return 0; 
}

int kill_thread(long int threadID, int isWindows)
{
    if(isWindows){
        char command[100];
        sprintf(command, "taskkill /F /T /PID %ld", threadID);
        system(command);
    }
    else{
        char command[100];
        sprintf(command, "kill %ld", threadID);
        system(command);
    }
}


void menu()
{
    //get current path
    char *path = malloc(100);
    sprintf(path, "%s", getenv("PWD"));
    printf("1) Spawn a new shell\n");
    printf("2) Terminate a shell-thread\n");
    printf("3) Termniante all shell-hoaxshellthreads\n");
    printf("4) Exit\n");
    printf("Awaiting input: ");
    int choice;
    thrd_t *thread_ids = (thrd_t)malloc(100);
    int thread_count = 0;
    scanf("%d", &choice);
    switch (choice)
    {
        case 1:
            printf("Hoaxshell port:");
            int port=8080;
            scanf("%d", &port);
            printf("Hoaxshell IP:");
            char *ip = (char*)malloc(100);
            scanf("%s", ip);
            printf("Spawning new shell-thread...");
            struct thread_args *args = malloc(sizeof(struct thread_args));
            args -> hoaxshell_port = port;
            args -> hoaxshell_ip = ip;
            char *exec_path = malloc(100);
            sprintf(exec_path, "%s/%s\0", path, hoaxshell_exec);
            args -> hoaxshell_exec_path = exec_path;
            thrd_t thread;
            thrd_create(&thread, spawn_hoaxshell, args);
            thread_ids[thread_count] = thread;
            thread_count++;
           break;
        case 2:
            printf("Thread ID:");
            long int threadID;
            scanf("%ld", &threadID);
            kill_thread(threadID, win);
            break;
        case 3:
            for (int i = 0; i < thread_count; i++)
            {
                kill_thread(thread_ids[i], win);
            }
            break;
        //default case
        default:
        break;break;
    }

}

void sys_clear(int isWindows)
{
    if (isWindows){system("cls");}
    else {system("clear");}
}

int main(){
    //do a os check
    if (is_windows()){printf("Script is running on a windows environment\n");win=1;}
    else {printf("Script is running on a non-windows environment\n");win = 0;}
    //create a list of threads
    int num_threads = 0;
    while(1)
    {
        sys_clear(win);
        menu();
    }
}