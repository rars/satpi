/* FactorGraph.hpp : Defines a factor graph: a bipartite graph with variable and factor nodes.
*
* Copyright (c) 2016 Richard Russell <https://rars.github.io/>
*
*/

#pragma once

#ifndef SATPI_FACTORGRAPH_HPP
#define SATPI_FACTORGRAPH_HPP

#include <vector>
#include <memory>

namespace satpi
{
    class BooleanVariable;
    class Clause;
    class FactorNode;
    class VariableNode;

    class FactorGraph
    {
    public:
        FactorGraph(
            const std::vector<std::shared_ptr<BooleanVariable>>& variables,
            const std::vector<std::shared_ptr<Clause>>& clauses);
        virtual ~FactorGraph();

    public:
        void RunSurveyPropagation(const std::string& seedValue);

    private:
        void BuildGraph(
            const std::vector<std::shared_ptr<BooleanVariable>>& variables,
            const std::vector<std::shared_ptr<Clause>>& clauses);

        void RandomiseSurveys(const std::string& seedValue);

    private:
        std::vector<std::shared_ptr<VariableNode>> m_variableNodes;
        std::vector<std::shared_ptr<FactorNode>> m_factorNodes;
    };
}

#endif SATPI_FACTORGRAPH_HPP /* SATPI_FACTORGRAPH_HPP */
