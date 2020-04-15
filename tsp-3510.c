#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include <time.h>

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
Node best[1024];
clock_t start;
int maxt;

// function delarations
void build(char *filepath);
int distance(Node *tour);
void compute();
Node* sa();
void early_exit();

void build(char *filepath) {
    FILE *fp;

    if((fp = fopen(filepath, "r")) == NULL) {
        printf("FAILED TO LOAD FILE\n");
        return;
    }
    // nodes = (Node *)malloc(sizeof(Node) * 1024); // fix size

    for (int i = 0; i < 1024; i++) {
        char buffer[64];
        fgets(buffer, 64, fp);
        sscanf(buffer, "%d %f %f", &nodes[NUM_NODES].id, &nodes[NUM_NODES].x, &nodes[NUM_NODES].y);
        if (NUM_NODES > 1 && nodes[NUM_NODES].id == nodes[NUM_NODES-1].id) {
            NUM_NODES--;
            break;
        }
        // printf("%d %f %f\n", nodes[NUM_NODES].id, nodes[NUM_NODES].x, nodes[NUM_NODES].y);
        NUM_NODES++;
    }

    fclose(fp);

    // allcate size for the distance table
    // distances = (int **)calloc(sizeof(int *) * (NUM_NODES), 1); // fix size
    // for (int i = 1; i <= NUM_NODES; i++)
    //     distances[i] = (int *)calloc((NUM_NODES) * sizeof(int), 1);

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

    // PRINT TABLE FOR TESTING
    // for (int i = 1; i <= NUM_NODES; i++) {
    //     for (int j = 1; j <= NUM_NODES; j++) {
    //         printf("%d ", distances[i][j]);
    //     }
    //     printf("\n");
    // }
}

int distance(Node *tour) {
    int result = 0;

    for (int i = 1; i < NUM_NODES; i++)
        result += distances[tour[i].id][tour[i+1].id];

    result += distances[tour[1].id][tour[NUM_NODES].id];

    return result;
}

void compute(char *filepath) {
    FILE *fp;
    fp = fopen (filepath, "w");
    int results[10];

    for (int i = 1; i <= 10; i++) {
        Node *sa_result = sa();
        fprintf(fp, "Tour %d:\n[", i);
        for (int j = 1; j <= NUM_NODES; j++)
            fprintf(fp, "%d ", sa_result[j].id);
        fprintf(fp, "%d]\n", sa_result[1].id);
        results[i-1] = distance(sa_result);
        fprintf(fp, "Cost: %d\n\n", results[i-1]);
    }

    float mean = 0;
    for (int i = 0; i < 10; i++)
        mean += results[i];
    mean /= 10;
    fprintf(fp, "\nMean: %f\n\n", mean);


    //Calculate standard devation here
    fclose(fp);


    // print results to console
    for (int i = 0; i < 10; i++)
        printf("%d\n", results[i]);
    printf("\n");
    printf("mean: %f\n", mean);
}

Node* sa() {
    // best seems to be 100, .0001
    double t = 10000;
    float cr = .0001;

    if ((((double) (clock() - t)) / CLOCKS_PER_SEC) >= maxt) {
        early_exit();
    }

    for (int i = 1; i <= NUM_NODES; i++)
        current[i] = nodes[i];

    for (int i = 1; i <= NUM_NODES; i++)
        best[i] = nodes[i];

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
            // current = new
            for (int i = 1; i <= NUM_NODES; i++)
                current[i] = new[i];
        }

        if (n < c) {
            // best = new;
            for (int i = 1; i <= NUM_NODES; i++)
                best[i] = new[i];
        }

        t *= 1 - cr;
    }

    return best;
}

void early_exit() {
    printf("early exit\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    start = clock();
    maxt = atoi(argv[3]);

    time_t tm;
    srand((unsigned) time(&tm));

    build(argv[1]);
    compute(argv[2]);

    return 0;
}
