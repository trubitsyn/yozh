#pragma once

void yozh_cmd_cd(int argc, char *argv[]);

void yozh_cmd_bg(int argc, char *argv[]);

void yozh_cmd_echo(int argc, char *argv[]);

void yozh_cmd_exit(int argc, char *argv[]);

void yozh_cmd_export(int argc, char *argv[]);

void yozh_cmd_fg(int argc, char *argv[]);

void yozh_cmd_help(int argc, char *argv[]);

void yozh_cmd_history(int argc, char *argv[]);

void yozh_history_save(const char *line);

void yozh_cmd_jobs(int argc, char *argv[]);

void yozh_cmd_pwd(int argc, char *argv[]);
