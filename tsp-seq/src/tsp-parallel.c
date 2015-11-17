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

    int rank, size;

    int japan; // processes who have commited suicide

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);


    int c, initial;
    int matrix[51][51], ok[51], shortest_path;
    int shortest = INT_MAX, tmpShort;
    int distance, current, total;

    if(rank == 0) {
        int numProcess = size, nextCity;

        memset(matrix, 0, sizeof(matrix));
        memset(ok, 0, sizeof(ok));

        c = readinputfile(matrix, argv[1]);


        initial = 1;
        ok[initial] = 1;
        nextCity = 2;
        total = 2;
        for (int target = 1; target < size; target++) {
            if (nextCity >= c) {
                // MPI_SEND_SUICIDE - to not kill slave
                japan = 0;
                MPI_Send(&japan, 1, MPI_INT, target, 666, MPI_COMM_WORLD);
                numProcess--;
                continue;
            }
            // MPI_SEND_SUICIDE - to not kill slave
            japan = 1;
            MPI_Send(&japan, 1, MPI_INT, target, 666, MPI_COMM_WORLD);
            // MPI_SEND_DATA
            MPI_Send(matrix, 51*51, MPI_INT, target, 0, MPI_COMM_WORLD);    // const
            MPI_Send(&c, 1, MPI_INT, target, 1, MPI_COMM_WORLD);            // const
            distance = matrix[initial][nextCity];
            MPI_Send(&distance, 1, MPI_INT, target, 2, MPI_COMM_WORLD);
            MPI_Send(&initial, 1, MPI_INT, target, 3, MPI_COMM_WORLD);      // const
            current = nextCity;
            MPI_Send(&current, 1, MPI_INT, target, 4, MPI_COMM_WORLD);
            ok[current] = 1;
            MPI_Send(ok, 51, MPI_INT, target, 5, MPI_COMM_WORLD);
            ok[current] = 0;
            MPI_Send(&total, 1, MPI_INT, target, 6, MPI_COMM_WORLD);        // const
            MPI_Send(&shortest, 1, MPI_INT, target, 7, MPI_COMM_WORLD);
            nextCity++;
        }

        while (numProcess > 1) {
            // MPI_RECEIVE_ANSWER - recebe as respostas de Jebus (vulgo caminho mais curto)
            MPI_Recv(&tmpShort, 1, MPI_INT, MPI_ANY_SOURCE, 77, MPI_COMM_WORLD, &status);
            if (tmpShort < shortest) {
                shortest = tmpShort;
            }
            if (nextCity >= c) {
                // MPI_SEND_SUICIDE - to not kill slave
                japan = 0;
                MPI_Send(&japan, 1, MPI_INT, status.MPI_SOURCE, 666, MPI_COMM_WORLD);
                numProcess--;
                continue;
            }
            // MPI_SEND_SUICIDE - to not kill slave
            japan = 1;
            MPI_Send(&japan, 1, MPI_INT, status.MPI_SOURCE, 666, MPI_COMM_WORLD);
            // MPI_SEND_DATA - envia daods pro cara (escravo) processar
            distance = matrix[initial][nextCity];
            MPI_Send(&distance, 1, MPI_INT, status.MPI_SOURCE, 2, MPI_COMM_WORLD);

            current = nextCity;
            MPI_Send(&current, 1, MPI_INT, status.MPI_SOURCE, 4, MPI_COMM_WORLD);
            ok[current] = 1;
            MPI_Send(ok, 51, MPI_INT, status.MPI_SOURCE, 5, MPI_COMM_WORLD);
            ok[current] = 0;
            MPI_Send(&shortest, 1, MPI_INT, status.MPI_SOURCE, 7, MPI_COMM_WORLD);
            nextCity++;
        }

        writeoutputfile(shortest);

    } else {
        MPI_Recv(matrix, 51*51, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);    // const
        MPI_Recv(&c, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);            // const
        MPI_Recv(&initial, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, &status);      // const
        MPI_Recv(&total, 1, MPI_INT, 0, 6, MPI_COMM_WORLD, &status);        // const
        while ("Uma frase inspiradora") {
            // MPI_RECEIVE_SUICIDE - kill them all
            MPI_Recv(&japan, 1, MPI_INT, 0, 666, MPI_COMM_WORLD, &status);
            if (!japan) {
                // Allahu akbar
                break;
            }
            // MPI_RECEIVE_DATA
            MPI_Recv(&distance, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&current, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
            MPI_Recv(ok, 51, MPI_INT, 0, 5, MPI_COMM_WORLD, &status);
            MPI_Recv(&shortest, 1, MPI_INT, 0, 7, MPI_COMM_WORLD, &status);
            // jihad
            // arrumar parametros
            shortest_path = tsp(matrix, c, distance, initial, current, ok, total, shortest);
            // MPI_SEND_ANSWER - envia o shortest path
            MPI_Send(&shortest_path, 1, MPI_INT, 0, 77, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}

