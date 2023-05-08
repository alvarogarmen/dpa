#include <iostream>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <random>
#include <fstream>
#include <string>

double estimatePi(int numPoints) {
    //Counter for the number of points in the unit circle
    int numPointsInsideCircle = 0;

    // Random number generator setup. Each thread gets one generator
    std::random_device rd;
#pragma omp parallel
    {
        std::mt19937 gen(rd());
        std::uniform_real_distribution<double> dis(-1.0, 1.0);

#pragma omp for reduction(+:numPointsInsideCircle)
        for (int i = 0; i < numPoints; ++i) {
            double x = dis(gen);
            double y = dis(gen);

            if (x * x + y * y <= 1.0) {
                numPointsInsideCircle++;
            }
        }
    }
    return 4.0 * numPointsInsideCircle /numPoints;
}

int main() {
    std::ofstream outputFile;
    outputFile.open("running_time2.csv");  // Open file to write running time data
    outputFile << "numPoints,runningTime\n";  // Write header for the CSV file

    for (int numPoints = 10000; numPoints <= 1000000; numPoints += 10000) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        double estimatedPi = estimatePi(numPoints);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = end - begin;

        outputFile << numPoints<<","<<elapsedSeconds.count() << "\n";
    }

    outputFile.close();  // Close the file

    std::cout << "Running time data has been written to running_time2.csv" << std::endl;

    return 0;
}