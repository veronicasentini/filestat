#include <unistd.h>
#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "options.h"

bool verbose = false;
bool recap   = false;
bool report  = false;
bool noscan  = false;

#define DEFAULT_SIZE 16

char **history = NULL;
size_t history_size;
size_t history_len;

uid_t *uid = NULL;
size_t uid_size;
size_t uid_len;

gid_t *gid = NULL;
size_t gid_size;
size_t gid_len;

intmax_t min = 0;
intmax_t max = INTMAX_MAX;

bool l_found = false;

size_t strip(char **sp)
{
    char *s = *sp;

    while (isspace(*s)) {
        s++;
    }

    size_t len = strlen(s);
    size_t i = len - 1;

    while (i < len) {
        if (!isspace(s[i])) {
            s[i+1] = '\0';
            break;
        }

        i--;
    }

    *sp = s;

    return i+1;
}

int parse_range(char *s, intmax_t *min, intmax_t *max)
{
    size_t l = strip(&s);

    bool colon_found = false;

    size_t i;
    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == ':') {
            if (colon_found) {
                return 1;
            }

            colon_found = true;
            continue;
        }

        if (!isdigit(s[i])) {
            return 1;
        }
    }

    int pos;
    sscanf(s, "%ji%n", min, &pos);

    if (s[pos] == '\0') {
        return 0;
    }

    sscanf("%ji", s + pos + 1, max);

    return 0;
}

int options_parse(int argc, char *argv[])
{
    int opt;

    struct option longopts[] = {
        {
            "verbose",
            no_argument,
            NULL,
            'v'
        }, {
            "stat",
            no_argument,
            NULL,
            's'
        }, {
            "report",
            no_argument,
            NULL,
            'r'
        }, {
            "history",
            required_argument,
            NULL,
            'h'
        }, {
            "user",
            required_argument,
            NULL,
            'u'
        }, {
            "group",
            required_argument,
            NULL,
            'g'
        }, {
            "length",
            required_argument,
            NULL,
            'l'
        }, {
            "noscan",
            no_argument,
            NULL,
            'n'
        }
    };

    while ((opt = getopt_long(argc, argv, "vsrh:u:g:l:", longopts, NULL)) != -1) {
        switch (opt) {
        case 'v':
            verbose = true;
            break;

        case 's':
            recap = true;
            break;

        case 'r':
            if (noscan) {
                fputs("Errore: argomenti incompatibili (report e noscan)\n", stderr);
                return 1;
            }

            report = true;
            break;

        case 'h':
            if (history == NULL) {
                history_size = DEFAULT_SIZE;
                history_len  = 0;
                history = malloc(history_size * sizeof history[0]);
            }

            if (history_len >= history_size) {
                history_size = history_size * 3 / 2;
                history = realloc(history, history_size * sizeof history[0]);
            }

            history[history_len++] = optarg;

            break;

        case 'u': {
            int tmp, pos;
            sscanf(optarg, "%d %n", &tmp, &pos);

            if (optarg[pos] != '\0') {
                fprintf(stderr, "Errore: '%s' non è un numero valido\n", optarg);
                return 1;
            }

            if (uid == NULL) {
                uid_size = DEFAULT_SIZE;
                uid_len  = 0;
                uid = malloc(uid_size * sizeof uid[0]);
            }

            if (uid_len >= uid_size) {
                uid_size = uid_size * 3 / 2;
                uid = realloc(uid, uid_size * sizeof uid[0]);
            }

            uid[uid_len++] = tmp;
            break;
        }

        case 'g': {
            int tmp, pos;
            sscanf(optarg, "%d %n", &tmp, &pos);

            if (optarg[pos] != '\0') {
                fprintf(stderr, "Errore: '%s' non è un numero valido\n", optarg);
                return 1;
            }

            if (gid == NULL) {
                gid_size = DEFAULT_SIZE;
                gid_len  = 0;
                gid = malloc(gid_size * sizeof gid[0]);
            }

            if (gid_len >= gid_size) {
                gid_size = gid_size * 3 / 2;
                gid = realloc(gid, gid_size * sizeof gid[0]);
            }

            gid[gid_len++] = tmp;
            break;
        }

        case 'l':
            if (l_found) {
                fputs("Errore: l'opzione length non puo' essere passata due volte\n", stderr);
                return 1;
            }

            if (parse_range(optarg, &min, &max)) {
                fprintf(stderr, "Errore: range non valido: '%s'\n", optarg);
                return 1;
            }

            l_found = true;
            break;

        case 'n':
            if (report) {
                fputs("Errore: argomenti incompatibili (report e noscan)\n", stderr);
                return 1;
            }

            noscan = true;
            break;

        case '?':
            return 1;

        default:
            return 2;
        }
    }

    return 0;
}
