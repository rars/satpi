
#include "BooleanVariable.hpp"

#include <exception>
#include <sstream>

namespace satpi
{
    BooleanVariable::BooleanVariable(const std::uint32_t id)
        : m_id(id), m_isSet(false), m_value(false)
    {}

    BooleanVariable::~BooleanVariable()
    {}

    long BooleanVariable::GetId() const
    {
        return m_id;
    }

    bool BooleanVariable::IsSet() const
    {
        return m_isSet;
    }

    bool BooleanVariable::GetValue() const
    {
        if (!m_isSet)
        {
            std::stringstream ss;
            ss << "Could not GetValue of BooleanVariable with id='"
                << m_id << "' because it has not been set yet.";
            throw std::runtime_error(ss.str());
        }

        return m_value;
    }

    void BooleanVariable::SetValue(const bool& value)
    {
        if (m_isSet)
        {
            std::stringstream ss;
            ss << "Attempted to set BooleanVariable with id='"
                << m_id << "' to the value '" << value
                << "' but the variable was already set.";
            throw std::runtime_error(ss.str());
        }

        m_isSet = true;
        m_value = value;
    }
}
