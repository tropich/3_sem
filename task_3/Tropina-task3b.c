#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

const int bytes = 1024*1024;

int main()
{
    int msqid;
    char pathname[]="TASK3_b.c";
    key_t  key;
    int i,len, maxlen;

    struct mymsgbuf
    {
       long mtype;
       struct info{
          char x;
       } a;
    } mybuf;

    int result;

    result = fork();

    if (result<0)
        exit(-1);
    if (result>0)
    {
        key = ftok(pathname, 0);

        if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
            printf("Can\'t get msqid\n");
            exit(-1);
        }


        /* Send information */
        mybuf.mtype = 1;
        mybuf.a.x = 'a';
        len = sizeof(struct info);

        if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            exit(-1);
        }


        for (i = 1; i <= bytes; i++){
            mybuf.mtype = 1;
            len = sizeof(struct info);

            if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0){
                printf("Can\'t send message to queue\n");
                msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
                exit(-1);
            }
        }

    }
    if(result == 0)
    {
        key = ftok(pathname, 0);

        if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
            printf("Can\'t get msqid\n");
            exit(-1);
        }

        len = sizeof(struct info);

        if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, len, 0, 0)) < 0){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }

        clock_t start = clock();

        for (i = 1; i <= bytes; i++){

            if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf, len, 0, 0)) < 0){
                printf("Can\'t receive message from queue\n");
                exit(-1);
            }

        }

        printf("Time: %.3f ms\n", 1000*((double)clock()-start)/((double)CLOCKS_PER_SEC));

        msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    }

}
