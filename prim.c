#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include "vertex.h"
#include "edge.h"
#include "scanner.h"
#include "avl.h"
#include "binomial.h"
#include "queue.h"
#include "integer.h"


/* options */
int vOption = 0;    /* option -v */

static int processOptions(int, char **);
static VERTEX *processEdgeFile(BINOMIAL *, AVL *, AVL *, FILE *);
static VERTEX *addVertex(BINOMIAL *, AVL *, int);
static void addEdge(BINOMIAL *, AVL *, AVL *, int, int, int);
static void Fatal(char *,...);
static void printAuthor(void);
static void update(void *, void *);
static QUEUE *primMST(BINOMIAL *, VERTEX *);
static void displayMST(QUEUE *);


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
    BINOMIAL *heap = newBINOMIAL(displayVERTEX, compareVERTEX, update, 0);
    VERTEX *source = processEdgeFile(heap, vertices, edges, edgeFP);
    fclose(edgeFP);

    // Check if edge file was empty
    if (source == NULL) {
        printf("EMPTY\n");
        freeAVL(vertices);
        freeAVL(edges);
        return 0;
    }


    // Find MST
    QUEUE *mst = primMST(heap, source);
    displayMST(mst);

    /*
    freeVERTEX(source);
    freeAVL(vertices);
    freeAVL(edges);
    */
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

static VERTEX *processEdgeFile(BINOMIAL *heap, AVL *vertices, AVL *edges, FILE *fp) {
    assert(vertices != 0);
    VERTEX *source = NULL;
    int v1;
    int v2;
    int weight;
    char ch;
    v1 = readInt(fp);
    while (!feof(fp)) {
        if (source == NULL) source = addVertex(heap, vertices, v1);
        v2 = readInt(fp);
        ch = readChar(fp);
        if (ch != ';') {
            ungetc(ch, fp);
            weight = readInt(fp);
            ch = readChar(fp); // trash
        }
        else weight = 1;
        addEdge(heap, vertices, edges, v1, v2, weight);
        v1 = readInt(fp);
    }
    return source;
}

static VERTEX *addVertex(BINOMIAL *heap, AVL *vertices, int v) {
    assert(vertices != 0);
    VERTEX *temp = newVERTEX(v);
    VERTEX *rv = findAVL(vertices, temp);
    if (rv) {
        freeVERTEX(temp);
    }
    else {
        insertAVL(vertices, temp);
        setVERTEXowner(temp, insertBINOMIAL(heap, temp));
        rv = temp;
    }
    return rv;
}

static void addEdge(BINOMIAL *heap, AVL *vertices, AVL *edges, int u, int v, int w) {
    assert(edges != 0);
    EDGE *edgeUV = newEDGE(u, v, w);
    EDGE *edgeVU = newEDGE(v, u, w);
    if (findAVL(edges, edgeUV) || findAVL(edges, edgeVU)) {
        freeEDGE(edgeUV);
        freeEDGE(edgeVU);
        return;
    }
    VERTEX *v1 = addVertex(heap, vertices, u);
    VERTEX *v2 = addVertex(heap, vertices, v);
    insertVERTEXneighbor(v1, v2);
    insertVERTEXweight(v1, w);
    insertVERTEXneighbor(v2, v1);
    insertVERTEXweight(v2, w);
}

static void printAuthor(void) {
    printf("Written by Brett Heithold\n");
}

static void update(void *v, void *n) {
    assert(v != 0);
    VERTEX *p = v;
    setVERTEXowner(p, n);
}

static QUEUE *primMST(BINOMIAL *heap, VERTEX *source) {
    assert(heap != 0);
    assert(source != 0);
    QUEUE *mst = newQUEUE(displayVERTEX, 0);
    VERTEX *u;
    VERTEX *v;
    DLL *neighbors;
    DLL *weights;
    setVERTEXkey(source, 0);
    decreaseKeyBINOMIAL(heap, getVERTEXowner(source), source);
    while (sizeBINOMIAL(heap) > 0) {
        u = extractBINOMIAL(heap);
        if (getVERTEXkey(u) == -1) break;
        if (getVERTEXpred(u) != NULL) {
            insertVERTEXsuccessor(getVERTEXpred(u), u);
        }
        setVERTEXflag(u, 1);
        neighbors = getVERTEXneighbors(u);
        weights = getVERTEXweights(u);
        firstDLL(neighbors);
        firstDLL(weights);
        int weightUV;
        while (moreDLL(neighbors)) {
            v = currentDLL(neighbors);
            weightUV = getINTEGER(currentDLL(weights));
            if (!getVERTEXflag(v)) {
                if (weightUV < getVERTEXkey(v) || getVERTEXkey(v) == -1) {
                    setVERTEXkey(v, weightUV);
                    decreaseKeyBINOMIAL(heap, getVERTEXowner(v), v);
                    setVERTEXpred(v, u);
                }
            }
            nextDLL(neighbors);
            nextDLL(weights);
        }
        enqueue(mst, u);
    }
    return mst;
}

static void displayMST(QUEUE *mst) {
    assert(mst != 0);
    int level = 0;
    int totalWeight = 0;
    VERTEX *v;
    DLL *successors;
    if (sizeQUEUE(mst) > 0) {
        v = dequeue(mst);
        printf("%d: ", level);
        displayVERTEX(v, stdout);
        printf("\n");
        level++;
    }
    while (sizeQUEUE(mst) > 0) {
        successors = getVERTEXsuccessors(v);
        if (sizeDLL(successors) > 0) {
            printf("%d: ", level);
            lastDLL(successors);
            while (moreDLL(successors)) {
                displayVERTEX(currentDLL(successors), stdout);
                printf("(");
                displayVERTEX(getVERTEXpred(currentDLL(successors)), stdout);
                printf(")");
                printf("%d", getVERTEXkey(currentDLL(successors)));
                if (currentDLL(successors) != getDLL(successors, 0)) {
                    printf(" ");
                }
                totalWeight += getVERTEXkey(currentDLL(successors));
                prevDLL(successors);
            }
            printf("\n");
            level++;
        }
        v = dequeue(mst);
    }
    printf("weight: %d\n", totalWeight);
}
