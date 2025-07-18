namespace math {
    template <typename T>
    T constrainValue(T x, T lowerBound, T upperBound) {
        return (x < lowerBound) ? lowerBound : (x > upperBound) ? upperBound : x;
    }

    template <typename InputType, typename OutputType>
    OutputType mapValue(InputType x, InputType fromLow, InputType fromHigh, InputType toLow, InputType toHigh) {
        if (fromHigh == fromLow) return static_cast<OutputType>(toLow);
        return static_cast<OutputType>((x - fromLow) * ((toHigh - toLow) / static_cast<double>(fromHigh - fromLow)) + toLow);
    }

    template <typename InputType, typename OutputType>
    OutputType mapClampedInput(InputType x, InputType fromLow, InputType fromHigh, InputType toLow, InputType toHigh) {
        auto clampedVal = constrainValue(x, fromLow, fromHigh);
        return mapValue<InputType, OutputType>(clampedVal, fromLow, fromHigh, toLow, toHigh);
    }
}
