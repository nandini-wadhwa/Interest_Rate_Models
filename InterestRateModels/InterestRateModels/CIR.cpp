#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>

/*
 Simulates the Cox-Ingersoll-Ross (CIR) model.
 
 @param meanReversionLevel The mean reversion level of the CIR model.
 @param meanReversionRate The mean reversion rate of the CIR model.
 @param volatility The volatility of the CIR model.
 @param initialInterestRate The initial interest rate of the CIR model.
 @param timeHorizon The time horizon of the CIR model.
 @param timeStep The time step of the CIR model.
 @param outputPath The path to the output CSV file.
 */
void simulateCoxIngersollRossModel(
    const double& meanReversionLevel,
    const double& meanReversionRate,
    const double& volatility,
    const double& initialInterestRate,
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

    // Simulate the CIR model
    for (int i = 1; i <= numberOfTimeSteps; ++i) 
    {
        // Update time
        timeValues[i] = i * timeStep;

        // Generate a random increment
        double randomIncrement = standardNormalDistribution(randomGenerator);

        // Update the interest rate using the CIR SDE
        interestRateValues[i] = std::max(0.0, interestRateValues[i - 1] +
            meanReversionRate * (meanReversionLevel - interestRateValues[i - 1]) * timeStep +
            volatility * std::sqrt(std::max(0.0, interestRateValues[i - 1])) *
            std::sqrt(timeStep) * randomIncrement);
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
    // Parameters for the CIR model
    double meanReversionLevel = 0.1;
    double meanReversionRate = 0.2;
    double volatility = 0.02;
    double initialInterestRate = 0.05;
    double timeHorizon = 1.0;
    double timeStep = 0.01;
    std::string outputPath = "cir_simulation.csv";

    // Simulate the CIR model
    simulateCoxIngersollRossModel(
        meanReversionLevel,
        meanReversionRate,
        volatility,
        initialInterestRate,
        timeHorizon,
        timeStep,
        outputPath);

    std::cout << "Simulation completed. Results saved to " << outputPath << std::endl;

    return 0;
}