
#include "GenerateOptions.hpp"
#include "ProgramArguments.hpp"

#include <string>

namespace satpi
{
    GenerateOptions::GenerateOptions(
        const std::shared_ptr<ProgramArguments>& programArguments)
        : NumVariables(std::stoul(programArguments->GetOptionValue("-numVariables"))),
        LiteralsPerClause(std::stoul(programArguments->GetOptionValue("-literalsPerClause"))),
        ClausesPerVariableRatio(std::stod(programArguments->GetOptionValue("-clausesPerVariableRatio"))),
        OutputFilepath(programArguments->GetOptionValue("-outputFilepath"))
    {}
}