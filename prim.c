#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "string.h"
#include "scanner.h"

/* options */
int vOption = 0;    /* option -v            */

char *edgeFilename;

static int processOptions(int, char **);
void Fatal(char *,...);
void printAuthor(void);

int main(int argc,char **argv) {
    int argIndex;

    if (argc == 1) Fatal("%d arguments!\n",argc-1);

    argIndex = processOptions(argc,argv);

    if (vOption) {
        printAuthor();
        return 0;
    }

    if (argIndex == argc) {
        Fatal("Too few arguments!\n");
    }

    // Open edge file for reading
    edgeFilename = argv[argIndex];
    FILE *edgeFP = fopen(edgeFilename, "r");
    if (edgeFP == 0) {
        Fatal("Unable to open %s for reading!\n", edgeFilename);

    }

    // Process Edge File

    fclose(edgeFP);
    return 0;
}

void Fatal(char *fmt, ...) {
    va_list ap;

    fprintf(stderr,"An error occured: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    exit(-1);
}

static int processOptions(int argc, char **argv) {
    int argIndex = 1;

    while (argIndex < argc && *argv[argIndex] == '-') {
        /* check if stdin, represented by "-" is an argument */
        /* if so, the end of options has been reached */
        if (argv[argIndex][1] == '\0') return argIndex;

        switch (argv[argIndex][1]) {
            case 'v':
                vOption = 1;
                break;
            default:
                Fatal("option %s not understood\n",argv[argIndex]);
        }
        ++argIndex;
    }

    return argIndex;
}


void printAuthor(void) {
    printf("Written by Brett Heithold\n");
}
