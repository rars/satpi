/* GenerateOptions.hpp : Defines the options for the 'generate' command.
 *
 * Copyright (c) 2016 Richard Russell <https://rars.github.io/>
 *
 */

#pragma once

#ifndef SATPI_GENERATEOPTIONS_HPP
#define SATPI_GENERATEOPTIONS_HPP

#include <cstdint>
#include <memory>
#include <string>

namespace satpi
{
    class ProgramArguments;

    struct GenerateOptions
    {
    public:
        GenerateOptions(const std::shared_ptr<ProgramArguments>& programArguments);

        const std::uint32_t NumVariables;
        const std::uint32_t LiteralsPerClause;
        const double ClausesPerVariableRatio;
        const std::string OutputFilepath;
    };
}

#endif /* SATPI_GENERATEOPTIONS_HPP */
