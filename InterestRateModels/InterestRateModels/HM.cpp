#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>

/*
 Simulates the Hull and White model.
 
 @param initialInterestRate The initial interest rate of the Hull and White model.
 @param thetaValues The deterministic function of time for the Hull and White model.
 @param alphaValues The deterministic function of time for the Hull and White model.
 @param sigmaValues The deterministic function of time for the Hull and White model.
 @param timeHorizon The time horizon of the Hull and White model.
 @param timeStep The time step of the Hull and White model.
 @param outputPath The path to the output CSV file.
 */
void simulateHullAndWhiteModel(
    const double& initialInterestRate,
    const std::vector<double>& thetaValues,
    const std::vector<double>& alphaValues,
    const std::vector<double>& sigmaValues,
    const double& timeHorizon,
    const double& timeStep,
    const std::string& outputPath) 
{
    // Set up random number generation
    std::random_device randomDevice;
    std::mt19937 randomGenerator(randomDevice());
    std::normal_distribution<double> standardNormalDistribution(0.0, 1.0);

    // Calculate the number of time steps
    int numberOfTimeSteps = static_cast<int>(timeHorizon / timeStep);

    // Initialize vectors to store time and interest rate values
    std::vector<double> timeValues(numberOfTimeSteps + 1);
    std::vector<double> interestRateValues(numberOfTimeSteps + 1);

    // Initialize the initial interest rate
    interestRateValues[0] = initialInterestRate;

    // Simulate the Hull and White model
    for (int i = 1; i <= numberOfTimeSteps; ++i) 
    {
        // Update time
        timeValues[i] = i * timeStep;

        // Generate random increments
        double randomIncrement = standardNormalDistribution(randomGenerator);

        // Calculate the integral terms for the explicit solution
        double integral1 = 0.0;
        double integral2 = 0.0;
        double integral3 = 0.0;

        for (int j = 0; j < i; ++j) 
        {
            double u = j * timeStep;
            integral1 += alphaValues[j] * timeStep;
            integral2 += thetaValues[j] * std::exp(-integral1);
            integral3 += sigmaValues[j] * std::exp(-integral1) * randomIncrement;
        }

        // Update the interest rate using the Hull and White SDE
        interestRateValues[i] = initialInterestRate * std::exp(-integral1) + integral2 + integral3;
    }

    // Output the results to a CSV file
    std::ofstream outputFile(outputPath);
    outputFile << "Time,InterestRate\n";
    for (int i = 0; i <= numberOfTimeSteps; ++i) 
    {
        outputFile << timeValues[i] << "," << interestRateValues[i] << "\n";
    }
    outputFile.close();
}

int main() 
{
    // Parameters for the Hull and White model
    double initialInterestRate = 0.02;  // Replace with the desired initial interest rate
    std::vector<double> thetaValues = { 0.03, 0.02, 0.025 };  // Replace with the desired deterministic function of time
    std::vector<double> alphaValues = { 0.01, 0.015, 0.012 };  // Replace with the desired deterministic function of time
    std::vector<double> sigmaValues = { 0.01, 0.015, 0.02 };    // Replace with the desired deterministic function of time
    double timeHorizon = 1.0;
    double timeStep = 0.01;
    std::string outputPath = "hull_and_white_simulation.csv";

    // Simulate the Hull and White model
    simulateHullAndWhiteModel(
        initialInterestRate,
        thetaValues,
        alphaValues,
        sigmaValues,
        timeHorizon,
        timeStep,
        outputPath);

    std::cout << "Simulation completed. Results saved to " << outputPath << std::endl;

    return 0;
}