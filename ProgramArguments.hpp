/* ProgramArguments.hpp : Defines a program arguments parser for SatPi.
 *
 * Copyright (c) 2016 Richard Russell <https://rars.github.io/>
 *
 */

#pragma once

#ifndef SATPI_PROGRAMARGUMENTS_HPP
#define SATPI_PROGRAMARGUMENTNS_HPP

#include <string>
#include <map>

namespace satpi
{
    class ProgramArguments
    {
    public:
        ProgramArguments(const int argc, const char* argv[]);
        virtual ~ProgramArguments();

        const std::string& GetCommand() const;
        const std::string& GetOptionValue(const std::string& option) const;

        static std::string GetUsageDescription(const std::string& binaryName);

    private:
        void ParseArguments(const int argc, const char* argv[]);

    private:
        std::string m_command;
        std::map<std::string, std::string> m_options;
    };
}

#endif /* SATPI_PROGRAMARGUMENTNS_HPP */
