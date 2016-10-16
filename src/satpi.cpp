/* satpi.cpp : Defines the entry point for the application.
 *
 * Copyright (c) 2016 Richard Russell <https://rars.github.io/>
 *
 */

#include "RandomKSatFormula.hpp"
#include "ProgramArguments.hpp"
#include "GenerateOptions.hpp"

#include <random>
#include <iostream>
#include <memory>
#include <vector>
#include <cstdlib>

std::shared_ptr<satpi::ProgramArguments> ParseArguments(const int argc, const char* argv[])
{
    std::shared_ptr<satpi::ProgramArguments> programArguments;
    try
    {
        programArguments = std::shared_ptr<satpi::ProgramArguments>(new satpi::ProgramArguments(argc, argv));
    }
    catch (const std::runtime_error& error)
    {
        std::cout << "Failed to parse program arguments due to runtime error: "
            << error.what() << std::endl;
    }
    return programArguments;
}

int main(const int argc, const char* argv[])
{
    std::shared_ptr<satpi::ProgramArguments> programArguments = ParseArguments(argc, argv);

    if (!programArguments)
    {
        std::cout
            << "Please see usage instructions for this program:" << std::endl
            << satpi::ProgramArguments::GetUsageDescription(std::string(argv[0])) << std::endl;
        return 0;
    }

    try
    {
        if (programArguments->GetCommand() == "generate")
        {
            satpi::GenerateOptions options(programArguments);

            satpi::RandomKSatFormula formula(
                options.NumVariables,
                options.LiteralsPerClause,
                options.ClausesPerVariableRatio);

            formula.Save(options.OutputFilepath);
        }
        else
        {
            throw std::runtime_error(
                std::string("Unknown command: ") + programArguments->GetCommand());
        }
    }
    catch (const std::runtime_error& error)
    {
        std::cout << "Encountered unexpected error: " << error.what() << std::endl;
        std::cout << "Terminating process" << std::endl;
    }

    return 0;
}
