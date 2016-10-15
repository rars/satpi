/* Literal.hpp : Defines a literal in a satisfiability formula.
 *
 * Copyright (c) 2016 Richard Russell <https://rars.github.io/>
 *
 */

#pragma once

#ifndef SATPI_LITERAL_HPP
#define SATPI_LITERAL_HPP

#include <memory>

namespace satpi
{
    class BooleanVariable;

    class Literal
    {
    public:
        Literal(const bool isNegated, const std::shared_ptr<BooleanVariable>& variable);
        virtual ~Literal();

        bool IsNegated() const;
        bool GetSatisfyingValue() const;
        bool GetValue() const;
        const std::shared_ptr<BooleanVariable>& GetVariable() const;

    private:
        bool m_isNegated;
        std::shared_ptr<BooleanVariable> m_variable;
    };
}

#endif /* SATPI_LITERAL_HPP */
