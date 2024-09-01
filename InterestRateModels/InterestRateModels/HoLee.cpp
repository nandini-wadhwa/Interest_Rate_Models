#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>

/*
 Simulates the Ho and Lee model.
 
 @param driftTerm The drift term of the Ho and Lee model.
 @param volatility The volatility of the Ho and Lee model.
 @param timeHorizon The time horizon of the Ho and Lee model.
 @param timeStep The time step of the Ho and Lee model.
 @param outputPath The path to the output CSV file.
 */

void simulateHoAndLeeModel(
    const double& driftTerm,
    const double& volatility,
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
    interestRateValues[0] = 0.0;

    // Simulate the Ho and Lee model
    for (int i = 1; i <= numberOfTimeSteps; ++i) 
    {
        // Update time
        timeValues[i] = i * timeStep;

        // Generate a random increment
        double randomIncrement = standardNormalDistribution(randomGenerator);

        // Update the interest rate using the Ho and Lee SDE
        interestRateValues[i] = interestRateValues[i - 1] + driftTerm * timeValues[i] * timeStep + volatility * std::sqrt(timeStep) * randomIncrement;
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
    // Parameters for the Ho and Lee model
    double driftTerm = 0.02;  // Replace with the desired deterministic function of time
    double volatility = 0.01;
    double timeHorizon = 1.0;
    double timeStep = 0.01;
    std::string outputPath = "ho_and_lee_simulation.csv";

    // Simulate the Ho and Lee model
    simulateHoAndLeeModel(
        driftTerm,
        volatility,
        timeHorizon,
        timeStep,
        outputPath);

    std::cout << "Simulation completed. Results saved to " << outputPath << std::endl;

    return 0;
}