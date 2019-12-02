#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

struct dpipe_t
{
    int txd[2];
    int rxd[2];
};

const int amount = 5;
int main()
{
    int result, i;
    struct dpipe_t fd;

    size_t size;
    char  c, snd;

    if(pipe(fd.txd) < 0)
    {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    if(pipe(fd.rxd) < 0)
    {
        printf("Can\'t open pipe\n");
        exit(-1);
    }

    result = fork();

    if(result < 0)
    {
        printf("Can\'t fork child\n");
        exit(-1);
    }
    else
        if (result > 0)
        {
            /* Parent process */
            snd = 'P';

            if(close(fd.txd[0]) < 0)
            {
                printf("Parent: Can\'t close reading side of pipe\n");
                exit(-1);
            }
            if(close(fd.rxd[1]) < 0)
            {
                printf("Oh, Parent cant close writing\n");
                exit(-1);
            }


            for(i=0;i<amount;i++)
            {

                size = write(fd.txd[1], &snd, 1);

                if(size < 0)
                {
                    printf("Parent can\'t write all string to pipe\n");
                    exit(-1);
                }



                size = read(fd.rxd[0], &c, 1);
                if( size < 0 )
                {
                    printf("Parent can't read\n");
                    exit(-1);
                }

                printf("Parent received: %c\n", c);
            }

            if( close(fd.rxd[0])<0)
            {
                printf("Parent cant close pipe\n");
                exit(-1);
            }

            if(close(fd.txd[1]) < 0)
            {
                printf("Parent: Can\'t close writing side of pipe\n");
                exit(-1);
            }
        }
        else
        {
            /* Child process */
            snd = 'C';

            if(close(fd.txd[1]) < 0)
            {
                printf("Child: Can\'t close writing side of pipe\n");
                exit(-1);
            }
            if(close(fd.rxd[0]) < 0)
            {
                printf("Child can\'t close channel\n");
                exit(-1);
            }

            for(i=0;i<amount;i++)
            {
                size = read(fd.txd[0], &c, 1);

                if(size < 0)
                {
                    printf("Can\'t read string from pipe\n");
                    exit(-1);
                }

                printf("Child received: %c\n", c);

                size = write(fd.rxd[1], &snd, 1);
                if (size < 0)
                {
                    printf("Child can\'t write\n");
                    exit(-1);
                }
            }

            if(close(fd.txd[0]) < 0)
            {
                printf("Child: Can\'t close reading side of pipe\n");
                exit(-1);
            }
            if(close(fd.rxd[1]) < 0)
            {
                printf("Child can\'t close pipe\n");
                exit(-1);
            }
        }

   return 0;
}
