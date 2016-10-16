/* FactorNode.hpp : Defines a factor node in a factor graph.
*
* Copyright (c) 2016 Richard Russell <https://rars.github.io/>
*
*/

#pragma once

#ifndef SATPI_FACTORNODE_HPP
#define SATPI_FACTORNODE_HPP

#include "Survey.hpp"

#include <vector>
#include <memory>

namespace satpi
{
    class VariableNode;
    class Clause;
    class Literal;

    class FactorNode
    {
    public:
        FactorNode(const std::shared_ptr<Clause>& clause);
        virtual ~FactorNode();

        const std::shared_ptr<Clause>& GetClause() const;
        void AddConnection(const std::shared_ptr<VariableNode>& variableNode);
        void RemoveConnection(const std::shared_ptr<VariableNode>& variableNode);
        bool UpdateSurvey(const VariableNode& sourceVariableNode, const bool suggestedVariableValue, double surveyValue);
        bool UpdateOutgoingSurveys();

    private:
        const std::shared_ptr<Literal>& GetLiteral(const std::shared_ptr<VariableNode>& variableNode) const;
        double GetOutgoingSurvey(const VariableNode& variableNode) const;

    private:
        const std::shared_ptr<Clause> m_clause;
        std::vector<Survey<VariableNode>> m_positiveSurveys;
        std::vector<Survey<VariableNode>> m_negativeSurveys;
    };
}

#endif /* SATPI_FACTORNODE_HPP */
