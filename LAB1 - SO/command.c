#include <stdlib.h>
#include <stdbool.h>
#include "parsing.h"
#include "parser.h"
#include "command.h"
#include <glib.h>
#include <assert.h>
#include <string.h>
#include "strextra.h"

typedef struct scommand_s 
{
    GQueue* cmd;
    char* input;
    char* output;
} scommand_s;

scommand scommand_new(void)
{
    scommand empty_cmd = malloc(sizeof(struct scommand_s)); //Allocate memory for an empty command.
    empty_cmd->cmd = g_queue_new();
    empty_cmd->input = NULL;
    empty_cmd->output = NULL;

    assert(empty_cmd != NULL && scommand_is_empty (empty_cmd) && 
    scommand_get_redir_in (empty_cmd) == NULL && scommand_get_redir_out (empty_cmd) == NULL); //Check Condition

    return empty_cmd;
}

scommand scommand_destroy(scommand self) 
{
    assert(self != NULL);
    if (g_queue_is_empty(self->cmd)) 
    {
        g_queue_free(self->cmd);
    } 
    else 
    {
        g_queue_free_full(self->cmd,free);
    }
    free(self->input);
    free(self->output);
    self->cmd = NULL;
    self->input = NULL;
    self->output = NULL;
    free(self);
    self = NULL;
    assert(self==NULL);
    return self;
}

void scommand_push_back(scommand self, char * argument)
{
    assert(self != NULL && argument != NULL);
    g_queue_push_tail(self->cmd, argument);
    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self)
{
    assert(self != NULL && !scommand_is_empty(self));
    char* killme = g_queue_pop_head(self->cmd);
    free(killme);
    killme = NULL;
    
}

void scommand_set_redir_in(scommand self, char * filename)
{
    assert(self != NULL);
    if (self->input != NULL)
    {
        free(self->input); //Free in case that is not empty.
        self->input = NULL;
    }
    self->input = filename; //Set redir
}

void scommand_set_redir_out(scommand self, char * filename)
{
    assert(self != NULL);
    if (self->output != NULL)
    {
        free(self->output);
        self->output = NULL;
    }
    self->output = filename;
}

bool scommand_is_empty(const scommand self)
{
    assert(self != NULL);
    return g_queue_is_empty(self->cmd);
}

unsigned int scommand_length(const scommand self)
{
    assert(self != NULL);
    return (g_queue_get_length(self->cmd));
}

char * scommand_front(const scommand self)
{
    assert(self != NULL && !scommand_is_empty(self));
    char* res = g_queue_peek_head(self->cmd); 
    assert(res != NULL);
    return res;
}

char * scommand_get_redir_in(const scommand self)
{
    assert(self != NULL);
    return self->input;
}

char* scommand_get_redir_out(const scommand self)
{
    assert(self != NULL);
    return self->output;
}

char * scommand_to_string(const scommand self){
    assert(self != NULL);
    char *str = NULL;
    unsigned int index = 1;
    if (!scommand_is_empty(self)) 
    {
        str = strmerge(g_queue_peek_head(self->cmd),"");
        while (g_queue_get_length(self->cmd) > index && str != NULL) 
        {
            str = merge_with_free(str," ");
            str = merge_with_free(str, g_queue_peek_nth(self->cmd,index));
            index ++;
        }
        if (self->output != NULL) 
        {
            str = merge_with_free(str," > ");
            str = merge_with_free(str, self->output);
        }
        if (self->input != NULL) 
        {
            str = merge_with_free(str," < ");
            str = merge_with_free(str, self->input);
        }
    } else 
    {
        str = strdup("");
    }
    assert(scommand_is_empty(self) || scommand_get_redir_in(self) == NULL || scommand_get_redir_out(self) == NULL || strlen(str) > 0);
    return str;
}

struct pipeline_s 
{
    GQueue* cmds;
    bool wait;
} pipeline_s;

pipeline pipeline_new(void) 
{
    pipeline self = malloc(sizeof(struct pipeline_s));
    self->cmds = g_queue_new();
    self->wait = true;
    assert(self != NULL && pipeline_is_empty(self) && pipeline_get_wait(self));

    return self;
}

static void cmd_destroy(gpointer object_cmd) //Needed for the GLIB destroy.
{
    scommand killme = object_cmd; //"object_cmd" is the reference to the cmd.
    scommand_destroy(killme);
}

pipeline pipeline_destroy(pipeline self)
{
    assert(self != NULL);
    g_queue_free_full(self->cmds, cmd_destroy);
    self->cmds = NULL;
    free(self);
    self = NULL;
    assert(self == NULL);

    return self;
}

void pipeline_push_back(pipeline self, scommand sc)
{
    assert(self != NULL && sc != NULL);
    g_queue_push_tail(self->cmds, sc);
    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self)
{
    assert(self != NULL && !pipeline_is_empty(self));
    scommand killme = g_queue_pop_head(self->cmds);
    scommand_destroy(killme);
}

void pipeline_set_wait(pipeline self, const bool w)
{
    assert(self != NULL);
    self->wait = w;
}

bool pipeline_is_empty(const pipeline self)
{
    assert(self != NULL);
    return g_queue_is_empty(self->cmds);
}

unsigned int pipeline_length(const pipeline self)
{
    assert(self != NULL);
    return g_queue_get_length(self->cmds);
}

scommand pipeline_front(const pipeline self)
{
    assert(self != NULL && !pipeline_is_empty(self));
    scommand front = g_queue_peek_head(self->cmds);
    assert(front != NULL);
    return front;
}

bool pipeline_get_wait(const pipeline self)
{
    assert(self != NULL);
    return self->wait;
}

char * pipeline_to_string(const pipeline self) 
{
    assert(self != NULL);
    char * str = NULL;
    char * aux;
    unsigned int index = 1;
    if (!pipeline_is_empty(self)) 
    {
        str = scommand_to_string(g_queue_peek_head(self->cmds));
        while (g_queue_get_length(self->cmds) > index) 
        {
            str = merge_with_free(str," | ");
            aux = scommand_to_string(g_queue_peek_nth(self->cmds,index));
            str = merge_with_free(str, aux);
            free(aux);
            index ++;
        }
        if (self->wait == false) 
        {
            str = merge_with_free(str, " &");
        }
    } 
    else 
    {
        str = strdup("");
    }
    assert(pipeline_is_empty(self) || pipeline_get_wait(self) || strlen(str)>0);
    
    return str;
}
