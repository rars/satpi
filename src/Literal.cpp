
#include "Literal.hpp"
#include "BooleanVariable.hpp"

#include <stdexcept>
#include <sstream>

namespace satpi
{
    Literal::Literal(
        const bool isNegated,
        const std::shared_ptr<BooleanVariable>& variable)
        : m_isNegated(isNegated), m_variable(variable)
    {}

    Literal::~Literal()
    {}

    bool Literal::IsNegated() const
    {
        return m_isNegated;
    }

    bool Literal::GetSatisfyingValue() const
    {
        return !m_isNegated;
    }

    bool Literal::GetValue() const
    {
        if (!m_variable->IsSet())
        {
            std::stringstream ss;
            ss << "Attempted to GetValue of unset variable with id '"
                << m_variable->GetId() << "'";
            throw std::runtime_error(ss.str());
        }

        return m_isNegated ?
            !m_variable->GetValue()
            : m_variable->GetValue();
    }

    const std::shared_ptr<BooleanVariable>& Literal::GetVariable() const
    {
        return m_variable;
    }
}
