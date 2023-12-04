#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main() 
{
    int id;
    while (1)
    {
    char *input = readline("Enter a command: ");

        if (!input) 
            // If the user entered EOF (Ctrl+D), exit the loop
            break ;
        free(input);
        rl_on_new_line();
    }
    // id = fork();
    // if (id != 0)
    //     wait(NULL);
    // return 0;
}
