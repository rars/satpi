#pragma once

#ifndef SATPI_CLAUSE_HPP
/* Clause.hpp : Defines a clause in a satisfiability formula.
 *
 * Copyright (c) 2016 Richard Russell <https://rars.github.io/>
 *
 */

#define SATPI_CLAUSE_HPP

#include <cstdint>
#include <vector>
#include <memory>
#include <utility>

namespace satpi
{
    class Literal;

    class Clause
    {
    public:
        typedef std::shared_ptr<Literal> LiteralPtr;

        Clause(const std::uint32_t id, std::vector<LiteralPtr>&& literals);
        virtual ~Clause();

        const std::vector<LiteralPtr>& GetLiterals() const;

    private:
        std::uint32_t m_id;
        std::vector<LiteralPtr> m_literals;
    };
}

#endif /* SATPI_CLAUSE_HPP */
