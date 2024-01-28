#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "execute.h"
#include "builtin.h"
#include "execute_utils.h"
#include "tests/syscall_mock.h"

// Execute a single internal command if it exists in the pipeline.
static bool execute_single_builtin(pipeline apipe)
{
  scommand cmd = pipeline_front(apipe);
  builtin_run(cmd);
  pipeline_pop_front(apipe);

  return false; // It won't execute further after an internal command.
}

// Execute the command and replace the current program with a new one.
static void execute_scommand(scommand cmd) 
{
  char **args = get_args(cmd);
  execvp(args[0], args);
  perror("execvp"); // Display an error message in case of failure.
  exit(EXIT_FAILURE);
}

// Execute a complex pipeline with multiple commands.
static pid_t execute_complex_pipeline(int last_pipe[2], int next_pipe[2], pipeline apipe, unsigned int cant_com)
{
  assert(!pipeline_is_empty(apipe));
  scommand cmd;

  if (last_pipe != NULL)
  {
    pipeline_pop_front(apipe); /* Remove what was executed in the first child */
  }

  pid_t pid_stat = fork();

  if (pid_stat < 0)
  { // Fork error.
    perror("fork"); // Display an error message in case of failure.
    exit(EXIT_FAILURE);
  }
  else if (pid_stat == 0)
  { // Execute a single command.
    pipe_descriptor(last_pipe, next_pipe);

    cmd = pipeline_front(apipe);

    if (builtin_is_internal(cmd))
    {
      execute_single_builtin(apipe); // Try to execute an internal command if it exists.
      exit(EXIT_SUCCESS);
    }

    file_descriptor(cmd);
    execute_scommand(cmd);

    exit(EXIT_SUCCESS);
  }

  
  pipe_closer(last_pipe);

  return pid_stat;
}

void execute_pipeline(pipeline apipe)
{
  assert(apipe != NULL);

  unsigned int q_cmd = pipeline_length(apipe); // Number of commands.
  int **pipe_dscpt;
  pid_t *child_pids;

  if (pipeline_is_empty(apipe))
  {
    return; // No commands to execute.
  }

  if (builtin_alone(apipe))
  { 
    execute_single_builtin(apipe); // Execute an internal command if it exists.
    return; // Executed an internal command, there are no more commands in the pipeline.
  }

  child_pids = calloc(q_cmd, sizeof(pid_t)); // Alloc ate memory for all commands.
  pipe_dscpt = generate_pipes(q_cmd); // Create pipes to communicate the commands.

  for (size_t i = 0; i < q_cmd; i++)
  {
    child_pids[i] = execute_complex_pipeline(pipe_dscpt[i], pipe_dscpt[i + 1], apipe, q_cmd);
  }
 
  bool wait = pipeline_get_wait(apipe); 
  wait_pids(wait, child_pids, q_cmd);
  free_pipes(pipe_dscpt, q_cmd);
}

