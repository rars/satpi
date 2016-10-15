
#include "ProgramArguments.hpp"

#include <iostream>
#include <sstream>

namespace satpi
{
    ProgramArguments::ProgramArguments(const int argc, const char* argv[])
    {
        ParseArguments(argc, argv);
    }

    ProgramArguments::~ProgramArguments()
    {}

    const std::string& ProgramArguments::GetCommand() const
    {
        return m_command;
    }

    const std::string& ProgramArguments::GetOptionValue(const std::string& option) const
    {
        const auto& pos = m_options.find(option);
        if (pos == m_options.end())
        {
            throw std::runtime_error(
                std::string("Could not find option: ") + option);
        }
        return pos->second;
    }

    std::string ProgramArguments::GetUsageDescription(const std::string& binaryName)
    {
        std::stringstream ss;
        ss << "Usage: " << binaryName << " <command> <options>" << std::endl;
        return ss.str();
    }

    void ProgramArguments::ParseArguments(const int argc, const char* argv[])
    {
        if (argc < 2)
        {
            throw std::runtime_error("No value provided for <command> argument");
        }

        m_command = std::string(argv[1]);

        for (int i = 2; i < argc; i += 2)
        {
            if (argv[i][0] != '-')
            {
                throw std::runtime_error(
                    std::string("Expected program option to begin with '-': ")
                    + std::string(argv[i]));
            }

            const std::string optionName(argv[i]);
            if (i >= argc - 1)
            {
                throw std::runtime_error(
                    std::string("Expected value for program option: ")
                    + optionName);
            }

            m_options[optionName] = std::string(argv[i + 1]);
        }
    }
}
