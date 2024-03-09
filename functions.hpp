//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_FUNCTIONS_HPP
#define KALKULATOR_FUNCTIONS_HPP

#include "stack.hpp"

class Function {
protected:
    int argumentCount = 0;
public:
    virtual int Calculate(Stack<int> &) = 0;

    void SetArgumentCount(int count) {
        argumentCount = count;
    }

    int GetArgumentCount() const {
        return argumentCount;
    }

    virtual const char *Name() = 0;

    virtual Function *clone() const = 0;

    virtual void print() {
        printf("%s%d", this->Name(), this->argumentCount);
    }
};

class MaxFunction : public Function {
public:
    int Calculate(Stack<int> &values) override;

    const char *Name() override { return "MAX"; }

    Function *clone() const override;
};

class MinFunction : public Function {
public:
    int Calculate(Stack<int> &values) override;

    const char *Name() override { return "MIN"; }

    Function *clone() const override;
};

class IfFunction : public Function {
public:
    int Calculate(Stack<int> &values) override;

    const char *Name() override { return "IF"; }

    void print() override {
        printf("%s", this->Name());
    }

    Function *clone() const override;
};


#endif //KALKULATOR_FUNCTIONS_HPP
