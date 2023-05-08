#include <iostream>

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <chrono>
#include <random>

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
    // Number of generated points
    int numPoints;
    std::cout << "Enter the number of points: ";
    std::cin >> numPoints;

    // Keep track of the time
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    // Call our function
    double estimatedPi = estimatePi(numPoints);

    // Calculate the time passed
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsedSeconds = end - begin;

    // Print out the result
    std::cout << "Estimated value of Pi: " << estimatedPi << std::endl;
    std::cout << "Elapsed time: " << elapsedSeconds.count() << " seconds" << std::endl;

    return 0;
}