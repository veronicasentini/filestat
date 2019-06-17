/*
struct fileinfo {
    struct timespec date;
    uid_t uid;
    gid_t gid;
    off_t dim;
    mode_t perm;
    struct timespec acc;
    struct timespec mod;
    struct timespec change;
    nlink_t nlink;
};
*/

struct path {
    char *name;
    struct stat *scans;
};

struct path *data;

int main(int argc, char *argv[])
{
    options_parse(argc, argv);

    /* TODO */

    return 0;
}

/*
dati ---+--(path: "path1"
        |   scansioni: -+-- (scansione1)
        |               +-- (scansione2)
        |               +-- (scansione3))
        |
        +
        |
        +
*/
