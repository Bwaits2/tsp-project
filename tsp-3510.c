#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <limits.h>

// globals
typedef struct Node {
    int id;
    float x;
    float y;
} Node;

int NUM_NODES = 1;
int distances[1024][1024];
Node nodes[1024];
Node current[1024];
Node new[1024];
Node best[1024]; //best tour for each sa run
clock_t start;
int maxt;

Node *bestTour; //best tour out of 10 sa runs
int bestTourLength = INT_MAX; //best total length

FILE *outputFile;
char *outputFilePath;

// function delarations
void build(char *filepath);
int distance(Node *tour);
void compute();
Node* sa();
void export_final_results();

void build(char *filepath) {
    FILE *fp;

    if((fp = fopen(filepath, "r")) == NULL) {
        printf("FAILED TO LOAD FILE\n");
        return;
    }

    for (int i = 0; i < 1024; i++) {
        char buffer[64];
        fgets(buffer, 64, fp);
        sscanf(buffer, "%d %f %f", &nodes[NUM_NODES].id, &nodes[NUM_NODES].x, &nodes[NUM_NODES].y);
        if (NUM_NODES > 1 && nodes[NUM_NODES].id == nodes[NUM_NODES-1].id) {
            NUM_NODES--;
            break;
        }
        NUM_NODES++;
    }

    fclose(fp);

    // populate the distances table
    for (int i = 1; i <= NUM_NODES; i++) {
        for (int j = 1; j <= NUM_NODES; j++) {
            if (i == j) {
                distances[i][j] = 0;
            } else if (distances[j][i] != 0) {
                distances[i][j] = distances[j][i];
            } else {
                distances[i][j] = (int)(sqrt(pow(nodes[i].x - nodes[j].x,2) + pow(nodes[i].y - nodes[j].y, 2)));
            }
        }
    }
}

int distance(Node *tour) {
    int result = 0;

    for (int i = 1; i < NUM_NODES; i++)
        result += distances[tour[i].id][tour[i+1].id];

    result += distances[tour[1].id][tour[NUM_NODES].id];

    return result;
}

void compute() {
    printf("Candidate Tours:\n");
    for (int i = 1; i <= 10; i++) {
        Node *sa_result = sa();
        int sa_distance = distance(sa_result);
        printf("%d\n", sa_distance);
        if (sa_distance < bestTourLength) {
            bestTour = sa_result;
            bestTourLength = sa_distance;
        }
    }
}

Node* sa() {
    double t = 1000;
    float cr = .00001;

    if ((((double) (clock() - t)) / CLOCKS_PER_SEC) >= maxt) {
        printf("\nTIME LIMIT EXPIRED\n");
        export_final_results();
    }

    for (int i = 1; i <= NUM_NODES; i++)
        best[i] = nodes[i];

    for (int i = 1; i <= NUM_NODES; i++)
        current[i] = best[i];


    while (t > 1) {

        for (int i = 1; i <= NUM_NODES; i++)
            new[i] = current[i];

        int p1 = (rand() % (NUM_NODES))+1;
        int p2 = p1;
        while (p2 == p1)
            p2 = (rand() % (NUM_NODES))+1;

        Node temp = new[p1];
        new[p1] = new[p2];
        new[p2] = temp;

        int c = distance(current);
        int n = distance(new);

        if (exp((c-n)/t) > (rand()/(double)RAND_MAX)) {
            for (int i = 1; i <= NUM_NODES; i++)
                current[i] = new[i];
        }

        if (n < c) {
            for (int i = 1; i <= NUM_NODES; i++)
                best[i] = new[i];
        }

        t *= 1 - cr;
    }

    return best;
}

void export_final_results() {
    
    printf("\nMin: %d\n", bestTourLength);
    printf("\nFinal results located in %s\n", outputFilePath);


    outputFile = fopen (outputFilePath, "w");
    fprintf(outputFile, "Tour:\n[");
    for (int j = 1; j <= NUM_NODES; j++) {
        fprintf(outputFile, "%d ", bestTour[j].id);
    }
    fprintf(outputFile, "%d]\n", bestTour[1].id);
    fprintf(outputFile, "Total Cost: %d\n", bestTourLength);
    fclose(outputFile);
    exit(0);
}

int main(int argc, char *argv[]) {
    start = clock();
    maxt = atoi(argv[3]);
    outputFilePath = argv[2];

    time_t tm;
    srand((unsigned) time(&tm));

    build(argv[1]);
    compute();
    export_final_results();

    return 0;
}
