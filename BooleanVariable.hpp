/* BooleanVariable.hpp : Defines a Boolean variable.
 *
 * Copyright (c) 2016 Richard Russell <https://rars.github.io/>
 *
 */

#pragma once

#ifndef SATPI_BOOLEANVARIABLE_HPP
#define SATPI_BOOLEANVARIABLE_HPP

#include <cstdint>

namespace satpi
{
    class BooleanVariable
    {
    public:
        BooleanVariable(const std::uint32_t id);
        virtual ~BooleanVariable();

        long GetId() const;
        bool IsSet() const;
        bool GetValue() const;
        void SetValue(const bool& value);

    private:
        std::uint32_t m_id;
        bool m_isSet;
        bool m_value;
    };
}

#endif /* SATPI_BOOLEANVARIABLE_HPP */
