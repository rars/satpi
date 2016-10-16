#include "FactorGraph.hpp"

#include "BooleanVariable.hpp"
#include "VariableNode.hpp"
#include "Clause.hpp"
#include "FactorNode.hpp"
#include "Literal.hpp"

#include <algorithm>
#include <iostream>
#include <random>

namespace satpi
{
    FactorGraph::FactorGraph(
        const std::vector<std::shared_ptr<BooleanVariable>>& variables,
        const std::vector<std::shared_ptr<Clause>>& clauses)
    {
        BuildGraph(variables, clauses);
    }

    FactorGraph::~FactorGraph()
    {}

    void FactorGraph::BuildGraph(
        const std::vector<std::shared_ptr<BooleanVariable>>& variables,
        const std::vector<std::shared_ptr<Clause>>& clauses)
    {
        for (const std::shared_ptr<BooleanVariable>& variable : variables)
        {
            if (variable->GetId() != m_variableNodes.size())
            {
                throw std::runtime_error("Attempted to build factor graph where variables were not in consecutive ascending order of id = 0, 1, ..., (N - 1)");
            }
            m_variableNodes.push_back(std::make_shared<VariableNode>(variable));
        }

        for (const std::shared_ptr<Clause>& clause : clauses)
        {
            if (clause->GetId() != m_factorNodes.size())
            {
                throw std::runtime_error("Attempted to build factor graph where clauses were not in consecutive ascending order of id = 0, 1, ..., (N - 1)");
            }

            auto factorNode = std::make_shared<FactorNode>(clause);
            for (const std::shared_ptr<Literal>& literal : factorNode->GetClause()->GetLiterals())
            {
                std::shared_ptr<VariableNode> variableNode = m_variableNodes[literal->GetVariable()->GetId()];
                variableNode->AddConnection(factorNode);
                factorNode->AddConnection(variableNode);
            }
            m_factorNodes.push_back(factorNode);
        }

        std::cout << "Built factor graph with " << m_variableNodes.size() << " variable nodes and " << m_factorNodes.size() << " factor nodes." << std::endl;
    }

    void FactorGraph::RunSurveyPropagation(const std::string& seedValue)
    {
        RandomiseSurveys(seedValue);

        std::vector<size_t> variableOrdering;
        for (size_t i = 0; i < m_variableNodes.size(); i++)
        {
            variableOrdering.push_back(i);
        }

        std::vector<size_t> factorOrdering;
        for (size_t i = 0; i < m_factorNodes.size(); i++)
        {
            factorOrdering.push_back(i);
        }

        std::uint32_t i;
        for (i = 0; i < 100; i++)
        {
            std::cout << "Iteration " << i << std::endl;
            
            std::random_shuffle(variableOrdering.begin(), variableOrdering.end());
            std::random_shuffle(factorOrdering.begin(), factorOrdering.end());

            bool anyUpdates = false;
            for (size_t index : factorOrdering)
            {
                anyUpdates |= m_factorNodes[index]->UpdateOutgoingSurveys();
            }

            for (size_t index : variableOrdering)
            {
                anyUpdates |= m_variableNodes[index]->UpdateOutgoingSurveys();
            }
            std::cout << "Updates made: " << anyUpdates << std::endl;
            if (!anyUpdates)
            {
                break;
            }
        }

        for (const std::shared_ptr<VariableNode>& variableNode : m_variableNodes)
        {
            std::cout << variableNode->GetId() << " bias=" << variableNode->Bias() << std::endl;
        }

        std::cout << "Completed after " << i << " iterations" << std::endl;
    }

    void FactorGraph::RandomiseSurveys(const std::string& seedValue)
    {
        std::seed_seq seedSeq(seedValue.begin(), seedValue.end());
        std::mt19937_64 randGen(seedSeq);
        std::uniform_real_distribution<double> uniformZeroOne(0.0, 1.0);

        std::cout << "Initialising surveys to random values" << std::endl;
        for (const std::shared_ptr<FactorNode>& factorNode : m_factorNodes)
        {
            for (const std::shared_ptr<Literal>& literal : factorNode->GetClause()->GetLiterals())
            {
                const std::shared_ptr<VariableNode>& variableNode = m_variableNodes[literal->GetVariable()->GetId()];
                variableNode->UpdateSurvey(*factorNode, !literal->IsNegated(), uniformZeroOne(randGen));
                factorNode->UpdateSurvey(*variableNode, !literal->IsNegated(), uniformZeroOne(randGen));
            }
        }
        std::cout << "Completed initialisation" << std::endl;
    }
}