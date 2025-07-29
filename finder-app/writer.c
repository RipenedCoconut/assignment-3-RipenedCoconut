#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {

  extern int errno;

  if (argc - 1 <= 1 || argc - 1 > 2){
	fprintf(stderr, "Error: incorrect number of arguments (%d).\n", argc -1);
	errno = EINVAL;
	return 1;
  }
  
  //initialize strings for path and file contents.
  const char *writePath = argv[1];
  const char *writeString = argv[2];
  
  //Create file newFile at writePath argument from console.
  int newFile = creat (writePath, 0644);
  
  //Check for error.
  if (newFile == -1){
	  fprintf(stderr, "Error: unable to create file.");
	  errno = EIO;
	  return 1;
  }
  
  ssize_t writeFile;
  
  writeFile = write (newFile, writeString, strlen (writeString));
  if (writeFile == -1){
	  fprintf(stderr, "Error: unable to write to file.");
	  errno = EIO;
	  return 1;
  }
 
  
  
  //DEBUG
  printf("\nfileP: %s", argv[1]);
  printf("\nsearchS: %s\n", argv[2]);
  
  return 0;
} 
