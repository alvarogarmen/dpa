#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <mpi.h>

#define VECTOR_SIZE 1000000
#define NUM_TESTS 10

void my_broadcast(std::vector<int>& data, int root, MPI_Comm communicator) {
    int rank, size;
    MPI_Comm_rank(communicator, &rank);
    MPI_Comm_size(communicator, &size);

    if (rank == root) {
        // Send the data to the next process in the ring
        MPI_Send(data.data(), data.size(), MPI_INT, (rank + 1) % size, 0, communicator);
    } else {
        // Receive the data from the previous process in the ring
        MPI_Recv(data.data(), data.size(), MPI_INT, (rank + size - 1) % size, 0, communicator, MPI_STATUS_IGNORE);
        if (rank != (root + size - 1) % size) {
            // Forward the received data to the next process in the ring
            MPI_Send(data.data(), data.size(), MPI_INT, (rank + 1) % size, 0, communicator);
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;
    std::vector<int> vector;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 100);

    std::ofstream outfile;
    if (rank == 0) {
        outfile.open("broadcast_times.csv");
        outfile << "Vector Size, Broadcast Time (seconds)\n";
    }

    for (int i = 0; i < NUM_TESTS; ++i) {
        int curr_vector_size = VECTOR_SIZE * (i + 1);

        if (rank == 0) {
            // Generate random vector on the source process
            vector.resize(curr_vector_size);
            for (int j = 0; j < curr_vector_size; j++) {
                vector[j] = dist(gen);
            }
        } else {
            vector.resize(curr_vector_size);
        }

        // Synchronize processes before starting the broadcast
        MPI_Barrier(MPI_COMM_WORLD);

        // Broadcast the vector from the source process to all other processes
        double start_time, end_time;
        if (rank == 0) {
            start_time = MPI_Wtime();
        }
        my_broadcast(vector, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            end_time = MPI_Wtime();
            double elapsed_time = end_time - start_time;
            outfile << curr_vector_size << ", " << elapsed_time << "\n";
        }
    }

    if (rank == 0) {
        outfile.close();
    }

    MPI_Finalize();

    return 0;
}
