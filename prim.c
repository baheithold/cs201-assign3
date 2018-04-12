#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "vertex.h"
#include "edge.h"
#include "scanner.h"
#include "avl.h"


/* options */
int vOption = 0;    /* option -v */

static int processOptions(int, char **);
static VERTEX *processEdgeFile(AVL *, AVL *, FILE *);
static VERTEX *addVertex(AVL *, int);
static void addEdge(AVL *, AVL *, int, int, int);
static void Fatal(char *,...);
static void printAuthor(void);


int main(int argc,char **argv) {
    int argIndex;
    if (argc == 1) Fatal("%d arguments!\n",argc-1);
    argIndex = processOptions(argc,argv);
    if (vOption) {
        printAuthor();
        return 0;
    }

    // Open edge file for reading
    char *edgeFilename = argv[argIndex];
    FILE *edgeFP = fopen(edgeFilename, "r");
    if (edgeFP == 0) {
        Fatal("Unable to open %s for reading!\n", edgeFilename);
    }
    // Process Edge File
    AVL *vertices = newAVL(displayVERTEX, compareVERTEX, freeVERTEX);
    AVL *edges = newAVL(displayEDGE, compareEDGE, freeEDGE);
    VERTEX *source = processEdgeFile(vertices, edges, edgeFP);
    fclose(edgeFP);

    // Check if edge file was empty
    if (source == NULL) {
        printf("EMPTY\n");
        freeAVL(vertices);
        freeAVL(edges);
        return 0;
    }

    freeVERTEX(source);
    freeAVL(vertices);
    freeAVL(edges);
    return 0;
}


static void Fatal(char *fmt, ...) {
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

static VERTEX *processEdgeFile(AVL *vertices, AVL *edges, FILE *fp) {
    assert(vertices != 0);
    VERTEX *source = NULL;
    int v1;
    int v2;
    int weight;
    char ch;
    v1 = readInt(fp);
    while (!feof(fp)) {
        if (source == NULL) source = newVERTEX(v1);
        v2 = readInt(fp);
        ch = readChar(fp);
        if (ch != ';') {
            ungetc(ch, fp);
            weight = readInt(fp);
            ch = readChar(fp); // trash
        }
        else weight = 1;
        addEdge(vertices, edges, v1, v2, weight);
        v1 = readInt(fp);
    }
    return source;
}

static VERTEX *addVertex(AVL *vertices, int v) {
    assert(vertices != 0);
    VERTEX *temp = newVERTEX(v);
    VERTEX *rv = findAVL(vertices, temp);
    if (rv) {
        freeVERTEX(temp);
    }
    else {
        insertAVL(vertices, temp);
        rv = temp;
    }
    return rv;
}

static void addEdge(AVL *vertices, AVL *edges, int u, int v, int w) {
    assert(edges != 0);
    EDGE *edgeUV = newEDGE(u, v, w);
    EDGE *edgeVU = newEDGE(v, u, w);
    if (findAVL(edges, edgeUV) || findAVL(edges, edgeVU)) {
        freeEDGE(edgeUV);
        freeEDGE(edgeVU);
        return;
    }
    VERTEX *v1 = addVertex(vertices, u);
    VERTEX *v2 = addVertex(vertices, v);
    insertVERTEXneighbor(v1, v2);
    insertVERTEXweight(v1, w);
    insertVERTEXneighbor(v2, v1);
    insertVERTEXweight(v2, w);
}

static void printAuthor(void) {
    printf("Written by Brett Heithold\n");
}
