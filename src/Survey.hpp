/* Survey.hpp : Defines a survey from a node.
*
* Copyright (c) 2016 Richard Russell <https://rars.github.io/>
*
*/

#pragma once

#ifndef SATPI_SURVEY_HPP
#define SATPI_SURVEY_HPP

#include <memory>

namespace satpi
{
    template <class NodeType>
    struct Survey
    {
    public:
        Survey(const std::shared_ptr<NodeType>& sourceNode)
            : SourceNode(sourceNode), SurveyValue(0.0), IsOld(true)
        {}

    public:
        std::shared_ptr<NodeType> SourceNode;
        double SurveyValue;
        bool IsOld;
    };
}

#endif /* SATPI_SURVEY_HPP */
