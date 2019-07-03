#ifndef _DATA_H_
#define _DATA_H_

struct path {
    char *name;
    struct stat *scans;
    size_t scans_len;
    size_t scans_size;
};

extern struct path *data;
extern size_t data_len;

#endif
