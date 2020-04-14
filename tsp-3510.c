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
Node nodes[1024];
int distances[1024][1024];
Node current[1024];
Node best[1024];

// function delarations
void build(char *filepath);
int distance(Node *tour);
void compute();
Node* sa();

void build(char *filepath) {
    FILE *fp;

    if((fp = fopen(filepath, "r")) == NULL) {
        printf("FAILED TO LOAD FILE\n");
        return;
    }
    // nodes = (Node *)malloc(sizeof(Node) * 1024); // fix size

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

    int i = 1;
    while (i < NUM_NODES) {
        result += distances[tour[i].id][tour[i-1].id];
        i += 1;
    }

    return result;
}

void compute() {
    int results[10];
    for (int i = 0; i < 10; i++) {
        Node *sa_result = sa();
        results[i] = distance(sa_result);
    }

    // print results
    for (int i = 0; i < 10; i++)
        printf("%d\n", results[i]);

    printf("\n");
    float mean = 0;
    for (int i = 0; i < 10; i++)
        mean += results[i];
    mean /= 10;
    printf("mean: %f\n", mean);
}

Node* sa() {
    int t = 1000000;
    float cr = .0001;

    // Node *current = (Node*)malloc((sizeof(Node) * NUM_NODES) + 1);
    // memcpy(current, nodes, (sizeof(Node) * NUM_NODES) + 1);
    for (int i = 1; i <= NUM_NODES; i++)
        current[i] = nodes[i];

    // Node *best = (Node*)malloc((sizeof(Node) * NUM_NODES) + 1);
    // memcpy(best, current, (sizeof(Node) * NUM_NODES) + 1);
    for (int i = 1; i <= NUM_NODES; i++)
        best[i] = nodes[i];

    while (t > 1) {

        // Node *new = malloc((sizeof(Node) * NUM_NODES) + 1);
        // memcpy(new, current, (sizeof(Node) * NUM_NODES) + 1);
        Node new[1024];
        for (int i = 1; i <= NUM_NODES; i++)
            new[i] = current[i];

        // #This is just a test, I dont want to handle edge case of edge swapping so fix this
        // p1 = random.randint(1, len(new) - 2)
        // srand (time(NULL));
        int p1 = (rand() % (NUM_NODES-1))+2;

        int p2 = p1;
        while (p2 == p1)
            p2 = (rand() % (NUM_NODES-1))+2;

        Node temp = new[p1];
        new[p1] = new[p2];
        new[p2] = temp;

        int c = distance(current);

        int curr_p1prev = current[p1 - 1].id;
        int curr_p1 = current[p1].id;
        int curr_p1next = current[p1 + 1].id;

        int curr_p2prev = current[p2 - 1].id;
        int curr_p2 = current[p2].id;
        int curr_p2next = current[p2 + 1].id;

        int c_removed, n_added = 0;

        // sequential nodes p1 < p2 
        if (p1 - p2 == -1) {
            c_removed = distances[curr_p1prev][curr_p1] + distances[curr_p2][curr_p2next];
            n_added = distances[curr_p1prev][curr_p2] + distances[curr_p1][curr_p2next];

        // sequential nodes p1 > p2 
        } else if (p1 - p2 == 1) {
            c_removed = distances[curr_p2prev][curr_p2] + distances[curr_p1][curr_p1next];
            n_added = distances[curr_p2prev][curr_p1] + distances[curr_p2][curr_p1next];

        // non-sequential nodes
        } else {
            c_removed = distances[curr_p1prev][curr_p1] + distances[curr_p1][curr_p1next] + distances[curr_p2prev][curr_p2] + distances[curr_p2][curr_p2next]; 
            n_added = distances[curr_p1prev][curr_p2] + distances[curr_p2][curr_p1next] + distances[curr_p2prev][curr_p1] + distances[curr_p1][curr_p2next];
        }

        int n = c - c_removed + n_added;

        errno = 0;
        float val = exp((c-n)/t);
        if (errno != ERANGE && val > rand())
            // current = new;
            for (int i = 1; i <= NUM_NODES; i++)
                current[i] = new[i];

        if (n < c)
            // best = new;
            for (int i = 1; i <= NUM_NODES; i++)
                best[i] = new[i];

        t *= 1 - cr;
    }

    return best;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    build(argv[1]);

    compute();
    return 0;
}