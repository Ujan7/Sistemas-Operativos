#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "builtin.h"
#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include <stdbool.h>
#include <unistd.h>
#include <pwd.h>
#include <limits.h> //Define names size

#define HOSTNAME_SIZE HOST_NAME_MAX
#define WORKDIR_SIZE PATH_MAX 

static void show_prompt(void) 
{
    printf("\033[1;35m"); //Some colour.
    char host_name[HOST_NAME_MAX + 1]; //"+1" --> Null char "\0"
    host_name[HOST_NAME_MAX] = '\0'; 
    gethostname(host_name, sizeof(host_name));

    unsigned int effective_user_id = geteuid();
    struct passwd* user_info = getpwuid(effective_user_id);

    char current_working_directory[WORKDIR_SIZE + 1]; //"+1" --> Null char "\0"
    current_working_directory[WORKDIR_SIZE] = '\0';
    getcwd(current_working_directory, sizeof(current_working_directory));
    
    printf("%s@%s/%s $ ", user_info->pw_name, host_name, current_working_directory);
    printf("\033[0m"); //Get back to the colour.
    fflush(stdout);
}

int main(int argc, char *argv[]) 
{
    pipeline pipe;
    Parser input;
    bool quit = false;

    input = parser_new(stdin);
    while (!quit) 
    {
        show_prompt();
        pipe = parse_pipeline(input);
        quit = parser_at_eof(input);
        if (pipe != NULL) 
        {
            quit = builtin_scommand_is_exit(pipeline_front(pipe));
            execute_pipeline(pipe);
            pipeline_destroy(pipe);
        }
    }
    parser_destroy(input); 
    input = NULL;
    
    return EXIT_SUCCESS;
}

