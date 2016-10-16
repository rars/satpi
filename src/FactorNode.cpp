#include "FactorNode.hpp"

#include "VariableNode.hpp"
#include "Clause.hpp"
#include "BooleanVariable.hpp"
#include "Literal.hpp"
#include "Survey.hpp"

#include <algorithm>
#include <sstream>

namespace satpi
{
    FactorNode::FactorNode(const std::shared_ptr<Clause>& clause)
        : m_clause(clause)
    {}

    FactorNode::~FactorNode()
    {}

    const std::shared_ptr<Clause>& FactorNode::GetClause() const
    {
        return m_clause;
    }

    void FactorNode::AddConnection(const std::shared_ptr<VariableNode>& variableNode)
    {
        const std::shared_ptr<Literal>& literal = GetLiteral(variableNode);

        if (literal->IsNegated())
        {
            m_negativeSurveys.push_back(Survey<VariableNode>(variableNode));
        }
        else
        {
            m_positiveSurveys.push_back(Survey<VariableNode>(variableNode));
        }
    }

    void FactorNode::RemoveConnection(const std::shared_ptr<VariableNode>& variableNode)
    {
        const std::shared_ptr<Literal>& literal = GetLiteral(variableNode);

        const size_t sizeBefore = m_positiveSurveys.size() + m_negativeSurveys.size();
        if (literal->IsNegated())
        {
            m_negativeSurveys.erase(
                std::remove_if(m_negativeSurveys.begin(), m_negativeSurveys.end(),
                    [&variableNode](const Survey<VariableNode>& survey) { return survey.SourceNode->GetId() == variableNode->GetId(); }),
                m_negativeSurveys.end());
        }
        else
        {
            m_positiveSurveys.erase(
                std::remove_if(m_positiveSurveys.begin(), m_positiveSurveys.end(),
                    [&variableNode](const Survey<VariableNode>& survey) { return survey.SourceNode->GetId() == variableNode->GetId(); }),
                m_positiveSurveys.end());
        }

        const size_t sizeAfter = m_positiveSurveys.size() + m_negativeSurveys.size();
        if (sizeAfter != sizeBefore - 1)
        {
            std::stringstream ss;
            ss << "Failed to remove connection for specified variable node with id '"
                << variableNode->GetId() << "' from factor node: "
                << "there were " << sizeBefore << " connections before and " << sizeAfter << " connections after";
            throw std::runtime_error(ss.str());
        }
    }

    bool FactorNode::UpdateSurvey(
        const VariableNode& sourceVariableNode,
        const bool suggestedVariableValue,
        double surveyValue)
    {
        const std::uint32_t variableNodeId = sourceVariableNode.GetId();

        std::vector<Survey<VariableNode>>::iterator it;
        if (suggestedVariableValue)
        {
            it = std::find_if(m_positiveSurveys.begin(), m_positiveSurveys.end(),
                [variableNodeId](const Survey<VariableNode>& survey) { return survey.SourceNode->GetId() == variableNodeId; });

            if (it == m_positiveSurveys.end())
            {
                throw std::runtime_error("Could not update survey value because the factor node could not be found");
            }
        }
        else
        {
            it = std::find_if(m_negativeSurveys.begin(), m_negativeSurveys.end(),
                [variableNodeId](const Survey<VariableNode>& survey) { return survey.SourceNode->GetId() == variableNodeId; });

            if (it == m_negativeSurveys.end())
            {
                throw std::runtime_error("Could not update survey value because the factor node could not be found");
            }
        }

        if (std::abs(it->SurveyValue - surveyValue) > 0.0001)
        {
            it->SurveyValue = surveyValue;
            return true;
        }
        return false;
    }

    bool FactorNode::UpdateOutgoingSurveys()
    {
        bool anySurveysUpdated = false;
        for (const Survey<VariableNode>& survey : m_positiveSurveys)
        {
            const double surveyValue = GetOutgoingSurvey(*survey.SourceNode);
            anySurveysUpdated |= survey.SourceNode->UpdateSurvey(*this, true, surveyValue);
        }

        for (const Survey<VariableNode>& survey : m_negativeSurveys)
        {
            const double surveyValue = GetOutgoingSurvey(*survey.SourceNode);
            anySurveysUpdated = survey.SourceNode->UpdateSurvey(*this, false, surveyValue);
        }
        return anySurveysUpdated;
    }

    double FactorNode::GetOutgoingSurvey(const VariableNode& variableNode) const
    {
        double outgoingSurvey = 1.0;

        for (const Survey<VariableNode>& survey : m_positiveSurveys)
        {
            if (survey.SourceNode->GetId() != variableNode.GetId())
            {
                outgoingSurvey *= survey.SurveyValue;
            }
        }

        for (const Survey<VariableNode>& survey : m_negativeSurveys)
        {
            if (survey.SourceNode->GetId() != variableNode.GetId())
            {
                outgoingSurvey *= survey.SurveyValue;
            }
        }

        return outgoingSurvey;
    }

    const std::shared_ptr<Literal>& FactorNode::GetLiteral(const std::shared_ptr<VariableNode>& variableNode) const
    {
        for (const std::shared_ptr<Literal>& literal : m_clause->GetLiterals())
        {
            if (literal->GetVariable()->GetId() == variableNode->GetId())
            {
                return literal;
            }
        }

        std::stringstream ss;
        ss << "Could not find literal for  variable with id '" << variableNode->GetId()
            << "' in the clause corresponding to this factor node";
        throw std::runtime_error(ss.str());
    }
}