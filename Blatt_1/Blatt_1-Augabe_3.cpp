#include <iostream>
#include <random>
#include <mpi.h>

int main(int argc, char** argv) {
    int numPoints = 1000000; // Number of points to generate
    int numPointsInsideCircle = 0;
    int rank, numProcesses;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    int pointsPerProcess = numPoints / numProcesses;
    int localCount = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    for (int i = 0; i < pointsPerProcess; ++i) {
        double x = dis(gen);
        double y = dis(gen);

        if (x * x + y * y <= 1.0) {
            localCount++;
        }
    }

    MPI_Reduce(&localCount, &numPointsInsideCircle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double piApprox = 4.0 * numPointsInsideCircle / numPoints;
        std::cout << "Estimated value of pi: " << piApprox << std::endl;
    }

    MPI_Finalize();

    return 0;
}
