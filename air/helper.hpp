#pragma once

namespace math {
    template <typename T>
    T constrainValue(T x, T lowerBound, T upperBound);

    template <typename InputType, typename OutputType>
    OutputType mapValue(InputType x, InputType fromLow, InputType fromHigh, InputType toLow, InputType toHigh); 
    
    template <typename InputType, typename OutputType>
    OutputType mapClampedInput(InputType x, InputType fromLow, InputType fromHigh, InputType toLow, InputType toHigh);
}

#include "helper.tpp"