//
// Created by wired-mac on 09/03/2024.
//

#include "functions.hpp"


int MaxFunction::Calculate(Stack<int>& values, int arguments)
{
    if (values.GetLength() == 0)
        return 0;
    int max = -999999999;
    for (int i = 0; i < arguments; i++)
    {
        int value = *values.Pop().GetValue();
        max = max < value ? value : max;
    }
    return max;
}


int MinFunction::Calculate(Stack<int>& values, int arguments)
{
    if (values.GetLength() == 0)
        return 0;
    int min = 999999999;
    for (int i = 0; i < arguments; i++)
    {
        int value = *values.Pop().GetValue();
        min = min > value ? value : min;
    }
    return min;
}


int IfFunction::Calculate(Stack<int>& values, int _arguments)
{
    assert(values.GetLength() >= 3);
    int c = *values.Pop().GetValue(), b = *values.Pop().GetValue(), a = *values.Pop().GetValue();
    return a > 0 ? b : c;
}
