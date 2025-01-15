#include <errno.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

bool is_newer(struct timespec a, struct timespec b) {
    return a.tv_sec < b.tv_sec || (a.tv_sec == b.tv_sec && a.tv_nsec < b.tv_nsec);
}

int get_newest_timestamp(int path_count, char** paths, struct timespec* timestamp) {
    int out = -1;
    struct stat file_stat = {0};
    for (int i = 0; i < path_count; ++i) {
        if(0 != stat(paths[i], &file_stat)) {
            fprintf(stderr, "ERROR: Failed to read '%s'\n", paths[i]);
            exit(errno);
        }
        if (is_newer(*timestamp, file_stat.st_mtim)) {
            *timestamp = file_stat.st_mtim;
            out = i;
        }
    }
    return out;
}

void print_strings(char** cmd) {
    for (int i = 0; cmd[i] != NULL; ++i) {
        printf("%s", cmd[i]);
        if (cmd[i+1] != NULL) printf(" ");
    }
}

void print_execution_header(time_t time, char* file_name, char** cmd) {
    struct tm * time_struct = localtime(&time);
    char timestamp[255] = {0};
    strftime(timestamp, 254, "%Y-%m-%d %H:%M:%S", time_struct);
    printf("==== %s | Changed:%s | Calling: ", timestamp, file_name);
    print_strings(cmd);
    printf(" ====\n");
}

static char optstring[] = "hf:n:";
static struct option long_options[] = {
    {"help",    no_argument, 0,  'h' },
    {"file",    required_argument, 0,  'f' },
    {"poll-frequency",    required_argument, 0,  'n' },
    {0,         0,                 0,  0 }
};

#define USAGE_FMT \
    "Usage: %s [-h] [-n polling_ferquency] -f <file_path> [-f <file_path> ... ] -- <command ... >\n"

#define FLAGS_SPEC \
    "\t--help  -h             Show this help message and exit.\n" \
    "\t--file  -f             File to watch for changes (can be specified more than once).\n" \
    "\t--poll-frequency  -n   Time in seconds waiting between checks for changes.\n" \

#define HELP_FMT \
    "Watch a file for changes, and run a command whenever the file is modified.\n" \
    "If more than one file is provided, then the command will be exected whenever any of\n" \
    "the provided files changes.\n" \
    "\n"  \
    USAGE_FMT \
    "\n" \
    FLAGS_SPEC

int main (int argc, char** argv) {

    char* progname = argv[0];
    int file_polling_frequency = 1;
    char** file_names = NULL;
    int file_count = 0;
    char** command = NULL;

    file_names = malloc(argc * sizeof(char*));
    memset(file_names, 0, argc);

    /// Process inputs
    char c = -1;
    while ((c = getopt_long(argc, argv, optstring, long_options, NULL)) != -1) {
        switch(c) {
            case('h'):
                fprintf(stdout, HELP_FMT, argv[0]);
                exit(EXIT_SUCCESS);
            case('f'):
                file_names[file_count++] = optarg;
                break;
            case('n'):
                file_polling_frequency = atoi(optarg);
                break;
        }
    }
    command = &argv[optind];
    if (!file_names[0]) {
        fprintf(stderr, "ERROR: Missing file argument!\n");
        fprintf(stderr, USAGE_FMT, argv[0]);
        exit(EXIT_FAILURE);
    }
    if (optind == argc) {
        fprintf(stderr, "ERROR: Missing command argument!\n");
        fprintf(stderr, USAGE_FMT, argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Watch changes to files:");
    print_strings(file_names);
    printf("\n");
    printf("On change will execute: ");
    print_strings(command);
    printf("\n\n");

    /// create shared memory location for error reporting
    static int* child_errno;
    child_errno = mmap(NULL, sizeof *child_errno, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    /// Prepare loop variables
    struct timespec last_modified = {0};
    while (true) {
        int const newest = get_newest_timestamp(file_count, file_names, &last_modified);
        if (newest == -1) {
            sleep(file_polling_frequency);
            continue;
        }

        pid_t tid = fork();
        int child_status = -1;
        switch (tid) {
            case(-1):
                // failed to fork()
                fprintf(stderr, "ERROR: Failed to fork!");
                exit(EXIT_FAILURE);
                break;
            case(0):
                // running child
                print_execution_header(last_modified.tv_sec, file_names[newest], command);
                execvp(*command, command);
                /// Put error into shared memory so parent can read it
                *child_errno = errno;
                exit(EXIT_FAILURE);
                break;
            default:
                // parent branch
                waitpid(tid, &child_status, 0);
                if (WIFEXITED(child_status)) {
                    if (*child_errno != 0) {
                        /// Report exit code in child
                        printf("Command exited abnormally with exit code: %d\n", *child_errno);
                    }
                }
                else
                    exit(EXIT_FAILURE);
                // TODO handle interrupt (should kill if there is a running child)
                // TODO add timeout
                break;
        }
        sleep(file_polling_frequency);
    }
    munmap(child_errno, sizeof *child_errno);
    return 0;
}
