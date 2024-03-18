//
// Created by wired-mac on 09/03/2024.
//

#include "functions.hpp"


int MaxFunction::Calculate(Stack<int> &values) {
    if (values.GetLength() == 0)
        return 0;
    int max = -999999999;
    for (int i = 0; i < argumentCount; i++) {
        int value = *values.Pop().GetValue();
        max = max < value ? value : max;
    }
    return max;
}

Function* MaxFunction::Clone() const {
    auto clone = new MaxFunction();
    clone->SetArgumentCount(argumentCount);
    return clone;
}

int MinFunction::Calculate(Stack<int> &values) {
        if (values.GetLength() == 0)
            return 0;
        int min = 999999999;
        for (int i = 0; i < argumentCount; i++) {
            int value = *values.Pop().GetValue();
            min = min > value ? value : min;
        }
        return min;
}
Function* MinFunction::Clone() const {
    auto clone = new MinFunction();
    clone->SetArgumentCount(argumentCount);
    return clone;
}

int IfFunction::Calculate(Stack<int> &values) {
    assert(values.GetLength() >= 3);
    int c = *values.Pop().GetValue(), b = *values.Pop().GetValue(), a = *values.Pop().GetValue();
    return a > 0 ? b : c;
}

Function* IfFunction::Clone() const {
    auto clone = new IfFunction();
    clone->SetArgumentCount(argumentCount);
    return clone;
}