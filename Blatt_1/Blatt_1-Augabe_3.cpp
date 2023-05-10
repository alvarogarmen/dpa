#include <random>
#include <fstream>
#include <chrono>
#include <mpi.h>

double estimatePi(int numPoints, int rank, int numProcesses) {
    // Counter for the number of points in the unit circle
    int numPointsInsideCircle = 0;

    // Random number generator setup. Each process gets one generator
    std::random_device rd;
    std::mt19937 gen(rd() + rank);
    std::uniform_real_distribution<double> dis(-1.0, 1.0);

    int pointsPerProcess = numPoints / numProcesses;
    int localCount = 0;

    for (int i = 0; i < pointsPerProcess; ++i) {
        double x = dis(gen);
        double y = dis(gen);

        if (x * x + y * y <= 1.0) {
            localCount++;
        }
    }

    // Sum the local counts from all processes
    int totalPointsInsideCircle;
    MPI_Reduce(&localCount, &totalPointsInsideCircle, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        return 4.0 * totalPointsInsideCircle / numPoints;
    }

    return 0.0;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, numProcesses;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numProcesses);

    if (rank == 0) {
        std::ofstream outputFile;
        outputFile.open("running_time3.csv");  // Open file to write running time data
        outputFile << "numPoints,runningTime\n";  // Write header for the CSV file

        std::cout << "Running time data will be written to running_time3.csv" << std::endl;

        for (int numPoints = 10000; numPoints <= 1000000; numPoints += 10000) {
            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

            double estimatedPi = estimatePi(numPoints, rank, numProcesses);

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsedSeconds = end - begin;

            outputFile << numPoints << "," << elapsedSeconds.count() << "\n";
        }

        outputFile.close();  // Close the file

        std::cout << "Running time data has been written to running_time2.csv" << std::endl;
    } else {
        for (int numPoints = 10000; numPoints <= 1000000; numPoints += 10000) {
            estimatePi(numPoints, rank, numProcesses);
        }
    }

    MPI_Finalize();

    return 0;
}
