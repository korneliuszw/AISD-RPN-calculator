//
// Created by wired-mac on 09/03/2024.
//

#include "functions.hpp"


int MaxFunction::Calculate(Stack<int> &values) {
    if (values.getLength() == 0)
        return 0;
    int max = -999999999;
    for (int i = 0; i < argumentCount; i++) {
        int value = *values.pop().getValue();
        max = max < value ? value : max;
    }
    return max;
}

Function* MaxFunction::clone() const {
    auto clone = new MaxFunction();
    clone->SetArgumentCount(argumentCount);
    return clone;
}

int MinFunction::Calculate(Stack<int> &values) {
        if (values.getLength() == 0)
            return 0;
        int min = 999999999;
        for (int i = 0; i < argumentCount; i++) {
            int value = *values.pop().getValue();
            min = min > value ? value : min;
        }
        return min;
}
Function* MinFunction::clone() const {
    auto clone = new MinFunction();
    clone->SetArgumentCount(argumentCount);
    return clone;
}

int IfFunction::Calculate(Stack<int> &values) {
    assert(values.getLength() >= 3);
    int c = *values.pop().getValue(), b = *values.pop().getValue(), a = *values.pop().getValue();
    return a > 0 ? b : c;
}

Function* IfFunction::clone() const {
    auto clone = new IfFunction();
    clone->SetArgumentCount(argumentCount);
    return clone;
}