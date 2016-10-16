/* VariableNode.hpp : Defines a variable node in a factor graph.
*
* Copyright (c) 2016 Richard Russell <https://rars.github.io/>
*
*/

#pragma once

#ifndef SATPI_VARIABLENODE_HPP
#define SATPI_VARIABLENODE_HPP

#include "Survey.hpp"

#include <vector>
#include <memory>

namespace satpi
{
    class BooleanVariable;
    class FactorNode;
    class Literal;

    class VariableNode
    {
    public:
        VariableNode(const std::shared_ptr<BooleanVariable>& booleanVariable);
        virtual ~VariableNode();

        std::uint32_t GetId() const;
        double Bias() const;

        void AddConnection(const std::shared_ptr<FactorNode>& factorNode);
        void RemoveConnection(const std::shared_ptr<FactorNode>& factorNode);
        bool UpdateSurvey(const FactorNode& sourceFactorNode, const bool suggestedVariableValue, const double surveyValue);
        bool UpdateOutgoingSurveys();
        
    private:
        const std::shared_ptr<Literal>& GetLiteral(const std::shared_ptr<FactorNode>& factorNode) const;
        double VariableNode::UpdateOutgoingSurvey(const FactorNode& factorNode);

    private:
        const std::shared_ptr<BooleanVariable> m_booleanVariable;
        std::vector<Survey<FactorNode>> m_positiveSurveys;
        std::vector<Survey<FactorNode>> m_negativeSurveys;
    };
}

#endif /* SATPI_VARIABLENODE_HPP */
