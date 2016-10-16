#include "Clause.hpp"

namespace satpi
{
    Clause::Clause(
        const std::uint32_t id,
        std::vector<LiteralPtr>&& literals)
        : m_id(id),
        m_literals(std::move(literals))
    {}

    Clause::~Clause()
    {}

    std::uint32_t Clause::GetId() const
    {
        return m_id;
    }

    const std::vector<Clause::LiteralPtr>& Clause::GetLiterals() const
    {
        return m_literals;
    }
}
