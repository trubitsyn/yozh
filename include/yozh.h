#pragma once

#include <stdbool.h>
#include <stddef.h>

int yozh_init();

void yozh_loop();

void yozh_input(char **line, size_t capacity);

void yozh_execute_line(char *line);

void yozh_execute_line2(const char *line);

void yozh_execute_command(int argc, char *argv[]);

int yozh_execute_builtin(int argc, char *argv[]);

bool yozh_should_terminate();

int yozh_terminate();
