#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "tests/syscall_mock.h"
#include "command.h"
#include "strextra.h"
#include "builtin.h"


bool builtin_scommand_is_exit(const scommand cmd) 
{
    assert(cmd != NULL);
    return strcmp(scommand_front(cmd), "exit") == 0;
}

bool builtin_scommand_is_cd(const scommand cmd) 
{
    assert(cmd != NULL);
    return strcmp(scommand_front(cmd), "cd") == 0;
}

bool builtin_scommand_is_help(const scommand cmd) 
{
    assert(cmd != NULL);
    return strcmp(scommand_front(cmd), "help") == 0;
}

bool builtin_is_internal(scommand cmd) 
{
    assert(cmd != NULL);
    return builtin_scommand_is_exit(cmd) || builtin_scommand_is_cd(cmd) || builtin_scommand_is_help(cmd);
}

bool builtin_alone(pipeline p) 
{
    assert(p != NULL);
    bool result = (pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p)));
    return result;
}

static void cd_ex(scommand cmd) 
{
    assert(builtin_scommand_is_cd(cmd));
    scommand_pop_front(cmd);    // Move to the next argument after cd

    if (scommand_length(cmd) > 1u) 
    {
        printf("mybash: cd: too many arguments.\n");
        return;
    }

    char *arg;
    if (scommand_length(cmd) == 1u) {
    arg = scommand_front(cmd);
    } else {
    arg = getenv("HOME");
    }

    int ret_code = chdir(arg); // Use chdir directly

    if (ret_code != 0) 
    {
        printf("Error: %s\n", strerror(errno));
    }
}

static void help_ex(scommand cmd) {
    assert(builtin_scommand_is_help(cmd));
    printf("Simplified bash, version 1.0-release \n"                                //Prints to the console the possible internal commands
            "Creators: Eugenio Arcana, Juan Diaz, Julian Piva and Bruno Espinosa \n"
            "Internal commands available: [cd, help, exit] \n"
            "exit: terminate shell \n"
            "cd: change the current working directory \n"
            "help: provide information of the available internal commands \n");
}

static void exit_ex(scommand cmd) 
{
    assert(builtin_scommand_is_exit(cmd));
    exit(EXIT_SUCCESS);
}

void builtin_run(scommand cmd) 
{
    assert(builtin_is_internal(cmd));
    if (builtin_scommand_is_help(cmd)) 
    {
        help_ex(cmd);
    } 
    else if (builtin_scommand_is_cd(cmd)) 
    {
        cd_ex(cmd);
    } 
    else 
    {
        exit_ex(cmd);
    }
}