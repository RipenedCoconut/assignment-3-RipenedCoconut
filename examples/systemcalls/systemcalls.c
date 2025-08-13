#include "systemcalls.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdbool.h> 
#include <stdarg.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{
	//create process using system with command 'cmd'
	int sysRet = system(cmd);
	bool successRet = true;
	
	//return false if failed
	if (sysRet < 0) {
		successRet = false;
	}
		
    return successRet;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    
    //last element in array must be null for execv
    command[count] = NULL;

	//initialize process id and status variables for child
	int status;
	pid_t pid = fork();
	
	//in case of fork() error (-1), report and return
	if (pid == -1) {
		perror ("fork");
		return false;
	}
	
	//clean process using command at first element in command[]
	if(pid == 0){	
		int ret = execv(command[0], command);
		if (ret == -1) {
			perror("execv");
			exit(EXIT_FAILURE);
		}
	}
	
	//start wait and store status, report and return if failed
	int waitRet = waitpid(pid, &status, 0);
	if (waitRet == -1) {
		perror("wait");
		return false;
	}
	//check for exit status from wait
	if ((WEXITSTATUS(status) != 0) || WIFSIGNALED(status))
    {
        return false;
    }
	
    va_end(args);
    return true;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
   
    //last element in array must be null for execv
    command[count] = NULL;

	//initialize process id and status variables for child
	pid_t pid;
	int status;
	
	//open file for process output
	int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);
	if (fd < 0){ 
		perror("open");
		return false;
	}
	
	//in case of fork() error (-1), report and return
	switch (pid = fork()) {
		case -1: 
			perror("fork");
			close(fd);
			return false;
			
		//clean process using command at first element in command[]
		case 0:
			dup2(fd, 1);
			int ret = execv(command[0], command);
			if (ret == -1) {
				perror("execv");
				exit(EXIT_FAILURE);
			}

		default:
	}
    
    //start wait and store status, report and return if failed
	int waitRet = waitpid(pid, &status, 0);
	if (waitRet == -1) {
		perror("wait");
		close(fd);
		return false;
	}
	
	//check for exit status from wait
	if ((WEXITSTATUS(status) != 0) || WIFSIGNALED(status))
    {
		close(fd);
        return false;
    }
    
    //close process output
	close(fd);
    va_end(args);

    return true;
}
