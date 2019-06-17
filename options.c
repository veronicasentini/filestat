#include <unistd.h>
#include <getopt.h>

#include <stdbool.h>

#include "options.h"

bool verbose = false;
bool recap   = false;
bool report  = false;

#define DEFAULT_SIZE 16

char **history = NULL;
size_t history_size;
size_t history_len;

intmax_t min = 0;
intmax_t max = INTMAX_MAX;

bool l_found = false;

bool parse_range(char *s, intmax_t *min, intmax_t *max);

void options_parse(int argc, char *argv[])
{
    int opt;

    while ((opt = getopt(argc, argv, "vsrh:u:g:l:")) != -1) {
        switch (opt) {
        case 'v':
            verbose = true;
            break;

        case 's':
            totals = true;
            break;

        case 'r':
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

        case 'u':
            /* TODO: come case 'h' man in uid_t */
            break;

        case 'g':
            /* TODO: come case 'h' man in gid_t */
            break;

        case 'l':
            if (l_found) {
                /* TODO: problema! */
                /* L'opzione non può essere usata due volte */
            }

            if (parse_range(optarg, &min, &max)) {
                /* TODO: problema! */
            }

            l_found = true;
            break;

        case '?':
            /* TODO: problema! */
            break;

        default:
            /* TODO: problema! */
        }
    }
}
