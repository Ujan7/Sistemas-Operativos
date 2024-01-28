#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#include "stdlib.h"
#include "command.h"
#include "execute_utils.h"
#include "tests/syscall_mock.h"

void wait_pids(bool has_to_wait, pid_t *pids, size_t num_pids) 
{
  if (has_to_wait) 
  {    //If has_to_wait is true, wait for each child process to complete
    for (size_t i = 0; i < num_pids; i++) 
    {
      waitpid(pids[i], NULL, 0);    //Wait for the child process with the specified PID to finish
    }
  }
  
  free(pids);
}

int **generate_pipes(size_t command_count) 
{
  int **pipe_array = calloc(command_count + 1, sizeof(int *));   //Allocate memory for an array of pipe descriptors
  pipe_array[0] = NULL;   //Set the first and last elements to NULL for boundary checking
  pipe_array[command_count] = NULL;
  for (size_t i = 1; i < command_count; i++) 
  {   //Create pipes for communication between commands
    pipe_array[i] = calloc(2, sizeof(int));   //Allocate memory for a pipe with two file descriptors
    pipe(pipe_array[i]);    //Create the pipe
  }
  return pipe_array;
}

void free_pipes(int **pipe_array, size_t command_count) 
{
  for (size_t i = 1; i < command_count; i++) 
  {
    free(pipe_array[i]);
  }
  free(pipe_array);
}

static int _file_descriptor(char *redir, int flags, int values, int target_fd) 
{
  if (redir != NULL) 
  {
    /* The open function takes flags as its second parameter. O_WRONLY allows write-only access,
      and O_CREAT creates the file if it does not exist. For O_CREAT, open takes a third parameter
      called mode for file creation permissions. In this case, we set the following flags:
        -S_IRUSR: User has read permission
        -S_IWUSR: User has write permission
     */
    int fd_redir = open(redir, flags, values);
    if (fd_redir == -1)
    {   //If open fails it sets the error message using perror and returns EXIT_FAILURE
      perror(redir);
      return (EXIT_FAILURE);
    }
    /* Use the dup2 function to duplicate the fd_redir descriptor onto target_fd
       This effectively redirects standard input or standard output to the file specified by redir
    */
    int dup2_aux = dup2(fd_redir, target_fd);
    if (dup2_aux == -1) {   //If `dup2` fails , it sets the error message using `perror` and returns EXIT_FAILURE
      perror("dup2");
      return (EXIT_FAILURE);
    }
    /* Close the fd_redir descriptor as its no longer needed. If close fails
       it sets the error message using perror but it is not expected to fail.
    */
    int close_aux = close(fd_redir);
    if (close_aux == -1)
    {
      perror("close");    //It should not fail, but if it does, it sets the perror message
      return (EXIT_FAILURE);
    }
  }
  return (EXIT_SUCCESS);
}

void file_descriptor(scommand cmd) 
{
  assert(cmd != NULL);
  char *in_dir, *out_dir;
  in_dir = scommand_get_redir_in(cmd);
  _file_descriptor(in_dir, O_RDONLY, 00400, STDIN_FILENO);
  out_dir = scommand_get_redir_out(cmd);
  _file_descriptor(out_dir, O_WRONLY | O_CREAT, 00400 | 00200, STDOUT_FILENO);
}

void pipe_closer(int apipe[2]) 
{
  if (apipe != NULL) {
    close(apipe[1]);
    close(apipe[0]);
  }
}

void pipe_descriptor(int last_pipe[2], int next_pipe[2]) 
{
  if (last_pipe != NULL) 
  {
    close(last_pipe[1]);    //Close the write end of the previous pipe
    dup2(last_pipe[0], STDIN_FILENO);     //Redirect standard input stdin to read from the read end of the previous pipe
    close(last_pipe[0]);  //Close the read end of the previous pipe as it's no longer needed
  }
  if (next_pipe != NULL) 
  {
    close(next_pipe[0]);    //Close the read end of the next pipe
    dup2(next_pipe[1], STDOUT_FILENO);    //Redirect standard output stdout to write to the write end of the next pipe 
    close(next_pipe[1]);    //Close the write end of the next pipe as its no longer needed
  }
}

char **get_args(scommand cmd) 
{
  char **args = NULL;
  char *arg;
  size_t cmd_length = scommand_length(cmd);
  args = calloc(cmd_length + 1, sizeof(char *));
  for (size_t i = 0; i < cmd_length; i++) 
  {
    arg = scommand_front(cmd);
    args[i] = strdup(arg);
    scommand_pop_front(cmd);
  }
  args[cmd_length] = NULL;
  
  return args;
}