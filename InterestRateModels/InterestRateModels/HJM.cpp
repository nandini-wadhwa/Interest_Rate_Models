#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>

/*
 Simulates the Heath-Jarrow-Morton (HJM) model.
 
 @param driftTerm The drift term of the HJM model.
 @param volatility The volatility of the HJM model.
 @param timeHorizon The time horizon of the HJM model.
 @param timeStep The time step of the HJM model.
 @param numberOfPaths The number of paths to simulate.
 @param outputPath The path to the output CSV file.
 */
void simulateHeathJarrowMortonModel(
    const double& driftTerm,
    const double& volatility,
    const double& timeHorizon,
    const double& timeStep,
    const int& numberOfPaths,
    const std::string& outputPath) 
{
    // Set up random number generation
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::normal_distribution<double> standardNormalDistribution(0.0, 1.0);

    // Calculate the number of time steps
    int numberOfTimeSteps = static_cast<int>(timeHorizon / timeStep);

    // Initialize vectors to store time and forward rate values
    std::vector<double> timeValues(numberOfTimeSteps + 1);
    std::vector<std::vector<double>> forwardRateValues(numberOfPaths, std::vector<double>(numberOfTimeSteps + 1));

    // Simulate the HJM model
    for (int i = 1; i <= numberOfTimeSteps; ++i) 
    {
        // Update time
        timeValues[i] = i * timeStep;

        // Generate a random increment
        double randomIncrement = standardNormalDistribution(randomGenerator);

        // Update the forward rate using the HJM SDE
        for (int path = 0; path < numberOfPaths; ++path) 
        {
            forwardRateValues[path][i] = forwardRateValues[path][i - 1] + driftTerm * timeStep + volatility * std::sqrt(timeStep) * randomIncrement;
        }
    }

    // Output the results to a CSV file
    std::ofstream outputFile(outputPath);
    outputFile << "Time";
    for (int path = 0; path < numberOfPaths; ++path) 
    {
        outputFile << ",ForwardRate" << path + 1;
    }
    outputFile << "\n";
    for (int i = 0; i <= numberOfTimeSteps; ++i) 
    {
        outputFile << timeValues[i];
        for (int path = 0; path < numberOfPaths; ++path) 
        {
            outputFile << "," << forwardRateValues[path][i];
        }
        outputFile << "\n";
    }
    outputFile.close();
}

int main() 
{
    // Parameters for the HJM model
    double driftTerm = 0.1;
    double volatility = 0.02;
    double timeHorizon = 1.0;
    double timeStep = 0.01;
    int numberOfPaths = 5;
    std::string outputPath = "hjm_simulation.csv";

    // Simulate the HJM model
    simulateHeathJarrowMortonModel(
        driftTerm,
        volatility,
        timeHorizon,
        timeStep,
        numberOfPaths,
        outputPath);

    std::cout << "Simulation completed. Results saved to " << outputPath << std::endl;

    return 0;
}