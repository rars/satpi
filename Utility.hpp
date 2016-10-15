/* Utility.hpp : Defines utility functions for SatPi.
 *
 * Copyright (c) 2016 Richard Russell <https://rars.github.io/>
 *
 */

#pragma once

#ifndef SATPI_UTILITY_HPP
#define SATPI_UTILITY_HPP

#include <algorithm>
#include <utility>
#include <sstream>

namespace satpi
{
    template <class ElementType, typename RandomNumberGenerator>
    std::vector<ElementType> ChooseNElements(
        const std::uint32_t numElements,
        std::vector<ElementType> collection,
        RandomNumberGenerator randomNumberGenerator)
    {
        if (numElements > collection.size())
        {
            std::stringstream ss;
            ss << "Attempted to choose " << numElements
                << " from a collection containing only " << collection.size()
                << " elements.";
            throw std::runtime_error(ss.str());
        }

        std::vector<ElementType> result;
        result.reserve(numElements);

        for (std::uint32_t index = 0; index < numElements; index++)
        {
            std::uniform_int_distribution<std::uint32_t> distribution(index, collection.size() - 1);
            const std::uint32_t selectedPosition = distribution(randomNumberGenerator);

            if (index != selectedPosition)
            {
                std::swap(collection[index], collection[selectedPosition]);
            }
            result.push_back(collection[index]);
        }

        return result;
    }
}

#endif /* SATPI_UTILITY_HPP */
