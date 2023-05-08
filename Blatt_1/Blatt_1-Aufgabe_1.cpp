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
    //Set the parameters for the random distribution
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    //Counter for the number of points in the unit circle
    int numPointsInsideCircle = 0;

    //Generation of points
    for (int i = 0; i < numPoints; ++i) {
        double x = dis(gen);
        double y = dis(gen);
        if (x * x + y * y <= 1.0) {
            numPointsInsideCircle++;
        }
    }
    // Calculate the ratio. The 4 comes up because the area of the circle is Pi/4
    // Aka (areaOfCircle)/(areaOfSquare)=Pi/4
    return 4.0 * numPointsInsideCircle / numPoints;
}

int main() {
    std::ofstream outputFile;
    outputFile.open("running_time.gp");  // Open file to write GNUplot commands

    outputFile << "set xlabel 'Number of Points'\n";
    outputFile << "set ylabel 'Running Time (seconds)'\n";
    outputFile << "set title 'Running Time vs. Number of Points'\n";
    outputFile << "set grid\n";
    outputFile << "set terminal png\n";
    outputFile << "set output 'running_time.png'\n";
    outputFile << "plot 'running_time.csv' using 1:2 with linespoints pointtype 7 pointsize 1.5 title 'Running Time'\n";
    outputFile << "quit\n";

    outputFile.close();  // Close the file

    // Calculate pi and record running time
    std::ofstream dataFile;
    dataFile.open("running_time.csv");
    dataFile << "numPoints,runningTime\n";

    for (int numPoints = 10000; numPoints <= 1000000; numPoints += 10000) {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        double estimatedPi = estimatePi(numPoints);

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsedSeconds = end - begin;

        dataFile << numPoints << "," << elapsedSeconds.count() << "\n";
    }

    dataFile.close();  // Close the data file

    std::system("gnuplot running_time.gp");  // Execute the GNUplot script

    std::cout << "Plot generated. Check running_time.png" << std::endl;

    return 0;
}