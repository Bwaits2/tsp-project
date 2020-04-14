#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct Node {
    int id;
    float x;
    float y;
} Node;

int NUM_NODES = 1;
Node *nodes;
int **distances;

void build(char *filepath) {
    FILE *fp;
    
    nodes = (Node *)malloc(sizeof(Node) * 1024); // fix size

    fp = fopen(filepath, "r");

    for (int i = 0; i < 300; i++) {
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
    distances = (int **)calloc(sizeof(Node) * NUM_NODES, 1); // fix size
    for (int i = 1; i <= NUM_NODES; i++) {
        distances[i] = (int *)calloc(NUM_NODES * sizeof(int), 1);
    }

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

int main(int argc, char *argv[]) {

    build(argv[1]);
    return 0;
}