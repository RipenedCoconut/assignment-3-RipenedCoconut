#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <syslog.h>

int main(int argc, char *argv[]) {

  //Set up logging facilities.
  extern int errno;
  openlog("writer", LOG_PID, LOG_USER);
  syslog(LOG_USER, "Writer app started successfully.");

  //Throw error for invalid number of args.
  if (argc - 1 <= 1 || argc - 1 > 2){
	fprintf(stderr, "Error: incorrect number of arguments (%d).\n", argc -1);
	syslog(LOG_USER, "Error: incorrect number of arguments.");
	errno = EINVAL;
	return 1;
  }
  
  //Initialize strings for path and file contents.
  const char *writePath = argv[1];
  const char *writeString = argv[2];
  
  //Create file newFile at writePath argument from console.
  int newFile = creat (writePath, 0644);
  
  //Check for error.
  if (newFile == -1){
	  fprintf(stderr, "Error: unable to create file.");
	  syslog(LOG_ERR, "Error: unable to create file.");
	  errno = EIO;
	  return 1;
  }
  
  //Begin writing to file.
  ssize_t writeFile;
  writeFile = write (newFile, writeString, strlen (writeString));
  
  //Write to log.
  syslog(LOG_DEBUG, "Writing %s to %s.", writeString, writePath);
  
  if (writeFile == -1){
	  fprintf(stderr, "Error: unable to write to file.");
	  syslog(LOG_ERR, "Error: unable to write to file.");
	  errno = EIO;
	  return 1;
  }
  
  //Close file.
  if(close(newFile) == -1){
	  fprintf(stderr, "Error: unable to close file.");
	  syslog(LOG_ERR, "Error: unable to close file.");
	  return 1;
  }
   
  return 0;
} 
