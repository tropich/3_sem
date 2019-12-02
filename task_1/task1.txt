#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[])
{
    char bufer[15];

    int result, error = 0;
    int j, i = 0;
    char ** s;
    while(1)
    {
        if (i==0)
        {
            s = (char**)malloc(1*sizeof(char*));
        }

        scanf("%s", bufer);

        if (bufer[0] == '?')
        {
             for(j=0; j < i; j++)
                free(s[j]);
            free(s);
            break;
        }

        if (bufer[0] == '!')
        {
            s[i] = (char*)NULL;

            result = fork();

            if(result < 0)
            {
                for(j=0; j < i; j++)
                    free(s[j]);
                free(s);
                exit(-1);
            }
            if(result == 0)
            {
                execvpe(s[0], s, envp);
                for(j=0; j < i; j++) //If we are here, this is error
                    free(s[j]);
                free(s);
                exit(-1);
            }

            waitpid(result, &j, NULL);
            printf("Process has ended with exit code: %d\n", j);
            for(j=0; j < i; j++)
                free(s[j]);

            free(s);
            i=0;
            continue;
        }



        s = (char**)realloc(s, i+2);
        s[i] = (char*)malloc(15*sizeof(char));
        strcpy(s[i], bufer);
        i++;
    }

    return 0;
}
