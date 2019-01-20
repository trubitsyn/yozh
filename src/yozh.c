#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <malloc.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

#include "builtins.h"
#include "yozh.h"

char *PS1 = NULL;

int yozh_init()
{
    PS1 = getenv("PS1");
    if (PS1 == NULL)
        PS1 = "> ";

    // should return 1 on success
    return 1;
}

void yozh_loop()
{
    while (!yozh_should_terminate())
    {
        printf("%s", PS1);
        char *line = NULL;
        size_t initial_size = 2;
        line = (char *) malloc(initial_size);

        if (line == NULL)
            yozh_terminate();

        yozh_input(&line, initial_size);
        yozh_history_save(line);
        yozh_execute_line2(line);

        if (line)
            free(line);
    }
}

void yozh_input(char **line, size_t capacity)
{
    getline(line, &capacity, stdin);
}


enum State
{
    INITIAL,
    WHITESPACE,
    WORD,
    PIPE,
    BG,
    AND,
    OR,
    FINAL
};

void yozh_execute_line2(const char *line)
{
    int state = INITIAL;
    while (*line)
    {
        const char current = *line;
        switch (state)
        {
            case INITIAL:
                if (isalnum(current))
                {
                    state = WORD;
                }
                else if (current == ' ')
                {
                    state = WHITESPACE;
                }
                else if (current == '&')
                {
                    state = AND;
                }
                else if (current == '|')
                {
                    state = OR;
                }
                else
                {
                    state = FINAL;
                }
                break;
            case WHITESPACE:
                if (current == ' ')
                {
                    state = WHITESPACE;
                }
                else
                {
                    state = INITIAL;
                }
                break;
            case WORD:
                if (isalnum(current))
                {
                    state = WORD;
                }
                else
                {
                    printf("found: WORD\n");
                    state = INITIAL;
                }
                break;
            case AND:
                if (current == '&')
                {
                    printf("found: AND\n");
                    state = INITIAL;
                }
                else
                {
                    printf("found: BG\n");
                    state = BG;
                }
                break;
            case BG:
                state = INITIAL;
                break;
            case OR:
                if (current == '|')
                {
                    printf("found: OR\n");
                    state = INITIAL;
                }
                else
                {
                    printf("found: PIPE\n");
                    state = PIPE;
                }
                break;
            case PIPE:
                state = INITIAL;
                break;
            case FINAL:
                return;
        }
        printf("Character: %c, State: %d\n", current, state);
        line++;
    }
}

void yozh_execute_line(char *line)
{
    char cmd[128];
    char *argv[255];

    line[strlen(line) - 1] = 0; // remove newline at the end

    int argc = 0;
    char *token = strtok(line, " ");
    strcpy(cmd, token);
    while (token != NULL)
    {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    argv[argc+1] = NULL;
    yozh_execute_command(argc, argv);
}

void yozh_execute_command(int argc, char *argv[])
{
    if (yozh_execute_builtin(argc, argv) == 0)
        return;

    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(argv[0], argv);
        perror("execv failed\n");
        exit(-1);
    }
    else if (pid > 0)
    {
        while (wait(NULL) > 0);
    }
    else
    {
        perror("failed to fork\n");
    }
}

int yozh_execute_builtin(int argc, char *argv[])
{
    const char *cmd = argv[0];
    if (strcmp(cmd, "bg") == 0)
    {
        yozh_cmd_bg(argc, argv);
    }
    else if (strcmp(cmd, "cd") == 0)
    {
        yozh_cmd_cd(argc, argv);
    }
    else if (strcmp(cmd, "echo") == 0)
    {
        yozh_cmd_echo(argc, argv);
    }
    else if (strcmp(cmd, "exit") == 0)
    {
        yozh_cmd_exit(argc, argv);
    }
    else if (strcmp(cmd, "export") == 0)
    {
        yozh_cmd_export(argc, argv);
    }
    else if (strcmp(cmd, "fg") == 0)
    {
        yozh_cmd_fg(argc, argv);
    }
    else if (strcmp(cmd, "help") == 0)
    {
        yozh_cmd_help(argc, argv);
    }
    else if (strcmp(cmd, "history") == 0)
    {
        yozh_cmd_history(argc, argv);
    }
    else if (strcmp(cmd, "jobs") == 0)
    {
        yozh_cmd_jobs(argc, argv);
    }
    else if (strcmp(cmd, "pwd") == 0)
    {
        yozh_cmd_pwd(argc, argv);
    }
    else
    {
        return -1;
    }
    return 0;
}

bool yozh_should_terminate()
{
    return false;
}

int yozh_terminate()
{
    return 0;
}
