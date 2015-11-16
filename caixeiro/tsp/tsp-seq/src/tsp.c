#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <mpi.h>

int tsp(int matrix[51][51], int cities, int distance, int initial, int current, int ok[51], int total, int shortest) {
	int i, d;

	if (total == cities)
		return distance + matrix[current][initial];

	if (shortest < distance)
		return distance;

	for (i = 1; i <= cities; i++) {
		if (i != current && !ok[i]) {
			ok[i] = 1;
			d = tsp(matrix, cities, distance + matrix[current][i], initial, i, ok, total + 1, shortest);
			if (d < shortest)
				shortest = d;
			ok[i] = 0;
		}
	}
	return shortest;
}

int readinputfile(int matrix[51][51], char *filename) {
	FILE *file;
	int c, e, i, j, w;

	if ((file = fopen(filename, "r")) == NULL) {
		perror(filename);
		exit(1);
	}

	fscanf(file, "%d%d", &c, &e);

	printf("vertices: %d\nedges: %d\n", c, e);

	for (int k = 0; k < e; k++) {
		fscanf(file, "%d%d%d", &i, &j, &w);
		matrix[i][j] = w;
		matrix[j][i] = w;
	}

	fclose(file);

	return c;
}

void writeoutputfile(int shortest_distance) {
	FILE *file;

	if ((file = fopen("output/tsp.out", "w")) == NULL) {
		perror("output/tsp.out");
		exit(1);
	}
	fprintf(file, "%d\n", shortest_distance);
	fclose(file);
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Error: provide an input file!\nUsage: %s <input file>\n\n", argv[0]);
		exit(EXIT_FAILURE);
	}

    MPI_Status status;

    int rank, size;
    int best_cost = 99999999;
    int s = 51;

    MPI_Init(argc, argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int c, initial;
	int matrix[51][51], ok[51], shortest_path;

    int row = 51, column = 51;


    memset(matrix, 0, sizeof(matrix));
    memset(ok, 0, sizeof(ok));

    c = readinputfile(matrix, argv[1]);

    /* starts from city 1 */
    if(rank == 0) {
        int i, j;
        for (i = 1; i < size; i++) {
            MPI_Send(&matrix[0][0], size*size, MPI_LONG, i, 0, MPI_COMM_WORLD);
        }
        int winner;
        int nodes[size-1];
        int node_index = 0;
        for(i = 0; i < size-1; i++) {
            nodes[i] = i+1;
        }
        for(i = 1; i < s; i++) {
            int temp_best_cost, node;
            node = nodes[node_index];
            if(node_index < size-2) {
                node_index++;
            } else {
                node_index = 0;
            }
            int *temp_shortest_path = calloc(sizeof(int), s+1);

            MPI_Recv(&temp_best_cost. 1. MPI_INT, node, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&temp_shortest_path[0], s+1; MPI_INT, node, 0, MPI_COMM_WORLD, &status);

            if(temp_best_cost < best_cost) {
                winner = node;
                best_cost = temp_best_cost;
                for(j = 0; j<s+1; j++) {
                    shortest_path = temp_shortest_path[j];
                }
            }
            MPI_Send(&best_cost, 1, MPI_INT, node, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&(matrix[0][0]), row*column, MPI_LONG, 0, 0, MPI_COMM_WORLD, &status);
        int i;
        for(i = rank; i < s; i+=(size-1)) {
	       initial = 1;
	       ok[initial] = 1;
	       tsp(matrix, c, 0, initial, initial, ok, 1, INT_MAX);

           MPI_Send(&best_cost, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
           MPI_Send(shortest_path, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

           MPI_Recv(&best_cost, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        }
    }

	writeoutputfile(shortest_path);

    MPI_Finalize();

	return EXIT_SUCCESS;
}
