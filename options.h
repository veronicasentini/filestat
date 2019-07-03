#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include <unistd.h>
#include <stdbool.h>

extern bool verbose;
extern bool recap;
extern bool report;
extern bool noscan;

extern char **history;
extern size_t history_len;

extern uid_t *uid;
extern size_t uid_len;

extern gid_t *gid;
extern size_t gid_len;

extern intmax_t min;
extern intmax_t max;

int options_parse(int argc, char *argv[]);

#endif
