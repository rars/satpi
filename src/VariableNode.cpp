#include "VariableNode.hpp"

#include "BooleanVariable.hpp"
#include "Clause.hpp"
#include "FactorNode.hpp"
#include "Literal.hpp"

#include <algorithm>
#include <sstream>

namespace satpi
{
    VariableNode::VariableNode(const std::shared_ptr<BooleanVariable>& booleanVariable)
        : m_booleanVariable(booleanVariable)
    {}

    VariableNode::~VariableNode()
    {}

    std::uint32_t VariableNode::GetId() const
    {
        return m_booleanVariable->GetId();
    }

    double VariableNode::Bias() const
    {
        double probabilityNoWarningsFromPositiveConnections = 1.0;
        for (const Survey<FactorNode>& survey : m_positiveSurveys)
        {
            probabilityNoWarningsFromPositiveConnections *= (1.0 - survey.SurveyValue);
        }

        double probabilityNoWarningsFromNegativeConnections = 1.0;
        for (const Survey<FactorNode>& survey : m_negativeSurveys)
        {
            probabilityNoWarningsFromNegativeConnections *= (1.0 - survey.SurveyValue);
        }

        const double probabilityTrue = probabilityNoWarningsFromNegativeConnections * (1.0 - probabilityNoWarningsFromPositiveConnections);
        const double probabilityFalse = probabilityNoWarningsFromPositiveConnections * (1.0 - probabilityNoWarningsFromNegativeConnections);
        const double probabilityUnconstrained = probabilityNoWarningsFromNegativeConnections * probabilityNoWarningsFromPositiveConnections;
        const double normalisationConstant = probabilityTrue + probabilityFalse + probabilityUnconstrained;

        return std::abs((probabilityTrue - probabilityFalse) / normalisationConstant);
    }

    void VariableNode::AddConnection(const std::shared_ptr<FactorNode>& factorNode)
    {
        const std::shared_ptr<Literal>& literal = GetLiteral(factorNode);

        if (literal->IsNegated())
        {
            m_negativeSurveys.push_back(Survey<FactorNode>(factorNode));
        }
        else
        {
            m_positiveSurveys.push_back(Survey<FactorNode>(factorNode));
        }
    }

    void VariableNode::RemoveConnection(const std::shared_ptr<FactorNode>& factorNode)
    {
        const std::shared_ptr<Literal>& literal = GetLiteral(factorNode);

        const size_t sizeBefore = m_positiveSurveys.size() + m_negativeSurveys.size();
        if (literal->IsNegated())
        {
            m_negativeSurveys.erase(
                std::remove_if(m_negativeSurveys.begin(), m_negativeSurveys.end(),
                    [&factorNode](const Survey<FactorNode>& survey) { return survey.SourceNode->GetClause()->GetId() == factorNode->GetClause()->GetId(); }),
                m_negativeSurveys.end());
        }
        else
        {
            m_positiveSurveys.erase(
                std::remove_if(m_positiveSurveys.begin(), m_positiveSurveys.end(),
                    [&factorNode](const Survey<FactorNode>& survey) { return survey.SourceNode->GetClause()->GetId() == factorNode->GetClause()->GetId(); }),
                m_positiveSurveys.end());
        }

        const size_t sizeAfter = m_positiveSurveys.size() + m_negativeSurveys.size();
        if (sizeAfter != sizeBefore - 1)
        {
            std::stringstream ss;
            ss << "Failed to remove connection for specified factor node from variable node with id '"
                << m_booleanVariable->GetId() << "': "
                << "there were " << sizeBefore << " connections before and " << sizeAfter << " connections after";
            throw std::runtime_error(ss.str());
        }
    }

    const std::shared_ptr<Literal>& VariableNode::GetLiteral(const std::shared_ptr<FactorNode>& factorNode) const
    {
        for (const std::shared_ptr<Literal>& literal : factorNode->GetClause()->GetLiterals())
        {
            if (literal->GetVariable()->GetId() == m_booleanVariable->GetId())
            {
                return literal;
            }
        }

        std::stringstream ss;
        ss << "Could not find variable with id '"
            << m_booleanVariable->GetId()
            << "' in the clause corresponding to the specified factor node";
    }

    bool VariableNode::UpdateSurvey(
        const FactorNode& sourceFactorNode,
        const bool suggestedVariableValue,
        double surveyValue)
    {
        const std::uint32_t factorNodeId = sourceFactorNode.GetClause()->GetId();

        std::vector<Survey<FactorNode>>::iterator it;
        if (suggestedVariableValue)
        {
             it = std::find_if(m_positiveSurveys.begin(), m_positiveSurveys.end(),
                [factorNodeId](const Survey<FactorNode>& survey) { return survey.SourceNode->GetClause()->GetId() == factorNodeId; });

             if (it == m_positiveSurveys.end())
             {
                 throw std::runtime_error("Could not update survey value because the factor node could not be found");
             }
        }
        else
        {
            it = std::find_if(m_negativeSurveys.begin(), m_negativeSurveys.end(),
                [factorNodeId](const Survey<FactorNode>& survey) { return survey.SourceNode->GetClause()->GetId() == factorNodeId; });

            if (it == m_negativeSurveys.end())
            {
                throw std::runtime_error("Could not update survey value because the factor node could not be found");
            }
        }

        if (std::abs(it->SurveyValue - surveyValue) > 0.001)
        {
            it->SurveyValue = surveyValue;
            return true;
        }
        return false;
    }

    bool VariableNode::UpdateOutgoingSurveys()
    {
        bool anyUpdatesMade = false;
        for (const Survey<FactorNode>& survey : m_positiveSurveys)
        {
            const double surveyValue = UpdateOutgoingSurvey(*survey.SourceNode);
            anyUpdatesMade |= survey.SourceNode->UpdateSurvey(*this, true, surveyValue);
        }

        double probabilityNoWarningsFromNegativeConnections = 1.0;
        for (const Survey<FactorNode>& survey : m_negativeSurveys)
        {
            const double surveyValue = UpdateOutgoingSurvey(*survey.SourceNode);
            anyUpdatesMade |= survey.SourceNode->UpdateSurvey(*this, false, surveyValue);
        }
        return anyUpdatesMade;
    }

    double VariableNode::UpdateOutgoingSurvey(const FactorNode& factorNode)
    {
        double probabilityNoWarningsFromPositiveConnections = 1.0;

        bool appearsAsPositiveLiteral = true;
        for (const Survey<FactorNode>& survey : m_positiveSurveys)
        {
            if (survey.SourceNode->GetClause()->GetId() != factorNode.GetClause()->GetId())
            {
                probabilityNoWarningsFromPositiveConnections *= (1.0 - survey.SurveyValue);
            }
        }

        double probabilityNoWarningsFromNegativeConnections = 1.0;
        for (const Survey<FactorNode>& survey : m_negativeSurveys)
        {
            if (survey.SourceNode->GetClause()->GetId() != factorNode.GetClause()->GetId())
            {
                probabilityNoWarningsFromNegativeConnections *= (1.0 - survey.SurveyValue);
            }
            else
            {
                appearsAsPositiveLiteral = false;
            }
        }

        const double probabilityTrue = probabilityNoWarningsFromNegativeConnections * (1.0 - probabilityNoWarningsFromPositiveConnections);
        const double probabilityFalse = probabilityNoWarningsFromPositiveConnections * (1.0 - probabilityNoWarningsFromNegativeConnections);
        const double probabilityUnconstrained = probabilityNoWarningsFromNegativeConnections * probabilityNoWarningsFromPositiveConnections;
        const double normalisationConstant = probabilityTrue + probabilityFalse + probabilityUnconstrained;

        if (appearsAsPositiveLiteral)
        {
            return probabilityFalse / normalisationConstant;
        }
        return probabilityTrue / normalisationConstant;
    }
}
