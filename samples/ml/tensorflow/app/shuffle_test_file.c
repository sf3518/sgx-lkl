#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



int main(int argc, char **argv)
{
       int fds[50];
       int num_fds = 0;

       if(argc < 2) {
               printf("Please provide a list of files to shuffle\n");
       }

       FILE * fp = fopen(argv[1], "rw");
       if(fp < 0) {
               printf("Error: Unable to open test file %s\n", argv[1]);
               exit(0);
       } else {
               printf("Reading from test file %s\n", argv[1]);
       }

       char * filename = NULL;
       ssize_t read;
        size_t len;
       char buffer[1024];
       while ((read = getline(&filename, &len, fp)) != -1) {
               filename[strcspn(filename, "\n")] = 0;
               filename[strcspn(filename, " ")] = 0;
               fds[num_fds] = open(filename, O_RDWR);
               if(fds[num_fds] < 0) {
                       printf("Error: Unable to open test file %s\n", filename);
                       continue;
               }
               for (int i = 0; i < 10; i++) {
                       pread(fds[num_fds], buffer, 512,i);

	       }
               num_fds++;
       }

       for(int i = 0; i < num_fds; i++) { 
               if(fds[i] > 0) 
	       		close(fds[i]);
	} 

       fclose(fp); // This initiates the shuffle
       return 0;
}
