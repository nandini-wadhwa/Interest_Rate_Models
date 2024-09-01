#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>

/*
 Simulates the Chan-Karolyi-Longstaff-Sanders (CKLS) model.
 
 @param driftTerm The drift term of the CKLS model.
 @param meanReversionRate The mean reversion rate of the CKLS model.
 @param elasticity The elasticity of the CKLS model.
 @param volatility The volatility of the CKLS model.
 @param initialInterestRate The initial interest rate of the CKLS model.
 @param timeHorizon The time horizon of the CKLS model.
 @param timeStep The time step of the CKLS model.
 @param outputPath The path to the output CSV file.
 */

void simulateChanKarolyiLongstaffSandersModel(
    const double& driftTerm,
    const double& meanReversionRate,
    const double& elasticity,
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

    // Simulate the CKLS model
    for (int i = 1; i <= numberOfTimeSteps; ++i) 
    {
        // Update time
        timeValues[i] = i * timeStep;

        // Generate a random increment
        double randomIncrement = standardNormalDistribution(randomGenerator);

        // Update the interest rate using the CKLS SDE
        interestRateValues[i] = interestRateValues[i - 1] +
            (driftTerm - meanReversionRate * interestRateValues[i - 1]) * timeStep +
            volatility * std::pow(std::abs(interestRateValues[i - 1]), elasticity) *
            std::sqrt(timeStep) * randomIncrement;
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
    // Parameters for the CKLS model
    double driftTerm = 0.1;
    double meanReversionRate = 0.2;
    double elasticity = 0.5;
    double volatility = 0.02;
    double initialInterestRate = 0.05;
    double timeHorizon = 1.0;
    double timeStep = 0.01;
    std::string outputPath = "ckls_simulation.csv";

    // Simulate the CKLS model
    simulateChanKarolyiLongstaffSandersModel(
        driftTerm,
        meanReversionRate,
        elasticity,
        volatility,
        initialInterestRate,
        timeHorizon,
        timeStep,
        outputPath);

    std::cout << "Simulation completed. Results saved to " << outputPath << std::endl;

    return 0;
}