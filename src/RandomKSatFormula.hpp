/* RandomKSatFormula.hpp : Defines a randomly generated K-SAT formula.
 *
 * Copyright (c) 2016 Richard Russell <https://rars.github.io/>
 *
 */

#pragma once

#ifndef SATPI_RANDOMKSATFORMULA_HPP
#define SATPI_RANDOMKSATFORMULA_HPP

#include <cstdint>
#include <vector>
#include <memory>

namespace satpi
{
    class BooleanVariable;
    class Clause;

    class RandomKSatFormula
    {
    public:
        RandomKSatFormula(
            const std::uint32_t numberOfVariables,
            const std::uint32_t clauseSize,
            const double alpha);
        virtual ~RandomKSatFormula();

        void Save(const std::string& filepath) const;
        const std::vector<std::shared_ptr<Clause>>& GetClauses() const;
        const std::vector<std::shared_ptr<BooleanVariable>>& GetVariables() const;

    private:
        void GenerateRandomKSat(
            const std::uint32_t numberOfVariables,
            const std::uint32_t clauseSize,
            const double alpha);

    private:
        std::vector<std::shared_ptr<BooleanVariable>> m_variables;
        std::vector<std::shared_ptr<Clause>> m_clauses;
    };
}

#endif /* SATPI_RANDOMKSATFORMULA_HPP */
