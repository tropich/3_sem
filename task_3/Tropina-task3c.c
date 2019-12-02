#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int bytes = 1024*1024;

int main()
{
   int     i, fd, result;

   size_t  size;
   char    resstring;
   char    name[] = "aaa.fifo";

   umask(0);

   if(mknod(name, S_IFIFO | 0666, 0) < 0){
     printf("Can\'t create FIFO\n");
     exit(-1);
   }

   if((result = fork()) < 0){

      printf("Can\t fork child\n");
      exit(-1);

   } else if (result > 0) {

     /* Parent process */

      if((fd = open(name, O_WRONLY)) < 0){
         printf("Can\'t open FIFO for writting\n");
	 exit(-1);
      }

      for(i=0; i<bytes;i++)
      {
        char x = 'a';
        size = write(fd, &x, 1);

        if(size != 1){
            printf("Can\'t write all string to FIFO\n");
            exit(-1);
        }
      }

      if(close(fd) < 0){
        printf("parent: Can\'t close FIFO\n"); exit(-1);
      }

   } else {

      /* Child process */

      if((fd = open(name, O_RDONLY)) < 0){
         printf("Can\'t open FIFO for reading\n");
	 exit(-1);
      }

      clock_t start = clock();
      for(i=0; i<bytes;i++)
      {

        size = read(fd, &resstring, 1);

        if(size < 0){
            printf("Can\'t read string from FIFO\n");
            exit(-1);
        }
      }
      printf("Time: %.3f ms\n", 1000*((double)clock()-start)/((double)CLOCKS_PER_SEC));

      if(close(fd) < 0){
        printf("child: Can\'t close FIFO\n"); exit(-1);
      }
      remove("aaa.fifo");

   }
   return 0;
}
