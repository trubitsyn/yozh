#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <getopt.h>
#include <errno.h>

#include "builtins.h"

void yozh_cmd_cd(int argc, char *argv[])
{
    if (argc == 1)
    {
        const char *homeDir = getenv("HOME");
        chdir(homeDir);
        return;
    }

    const char *path = argv[1];
    if (chdir(path) == -1)
    {
        printf("cd: ");
        printf("%s", path);
        printf(": No such file or directory\n");
    }
}

void yozh_cmd_bg(int argc, char *argv[])
{
    // TODO
}

void yozh_cmd_echo(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("\n");
        return;
    }
    for (size_t i = 1; i < argc; i++)
        printf("%s ", argv[i]);
	printf("\n");
}

void yozh_cmd_exit(int argc, char *argv[])
{
    int status = (argc == 1) ? errno : atoi(argv[1]);
    exit(status);
}

void yozh_cmd_export(int argc, char *argv[])
{
    if (argc != 2)
        return;
    char *keyValuePair = argv[1];
    char *key = strsep(&keyValuePair, "=");
    if (key == NULL)
        return;
    char *value = keyValuePair;
    setenv(key, value, 1);
}

void yozh_cmd_fg(int argc, char *argv[])
{
    // TODO
}

void yozh_cmd_help(int argc, char *argv[])
{
    printf("yozh version 0.0.1-alpha\n");
}

static const char *historyFilePath = ".yozh_history";

void yozh_cmd_history(int argc, char *argv[])
{
    if (argc == 2 && strcmp(argv[1], "-c") == 0)
    {
        remove(historyFilePath);
        return;
    }
    FILE *historyFile = fopen(historyFilePath, "r");
    if (historyFile == NULL)
    {
        perror("cannot open history file\n");
        return;
    }

    size_t len = 0;
    size_t i = 1;
    char *line = NULL;
    while (getline(&line, &len, historyFile) != -1) {
        printf("  %ld  %s", i, line);
        i++;
    }
    fclose(historyFile);
    if (line)
        free(line);
}

void yozh_history_save(const char *line)
{
    FILE *historyFile = fopen(historyFilePath, "a");
    if (historyFile == NULL)
        return;
    fprintf(historyFile, "%s", line);
    fclose(historyFile);
}

void yozh_cmd_jobs(int argc, char *argv[])
{
    // TODO
}

void yozh_cmd_pwd(int argc, char *argv[])
{
    char buf[PATH_MAX];
    getcwd(buf, PATH_MAX);
    printf("%s\n", buf);
}
