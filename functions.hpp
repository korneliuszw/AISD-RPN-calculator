//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_FUNCTIONS_HPP
#define KALKULATOR_FUNCTIONS_HPP

#include <cstdio>

#include "stack.hpp"

class Function
{
protected:
    int argumentCount = 0;

public:
    virtual int Calculate(Stack<int>&) = 0;

    void SetArgumentCount(int count)
    {
        argumentCount = count;
    }

    int GetArgumentCount() const
    {
        return argumentCount;
    }

    virtual const char* Name() = 0;

    virtual Function* Clone() const = 0;

    virtual void Print()
    {
        printf("%s%d", this->Name(), this->argumentCount);
    }
};

class MaxFunction : public Function
{
public:
    int Calculate(Stack<int>& values) override;

    const char* Name() override { return "MAX"; }

    Function* Clone() const override;
};

class MinFunction : public Function
{
public:
    int Calculate(Stack<int>& values) override;

    const char* Name() override { return "MIN"; }

    Function* Clone() const override;
};

class IfFunction : public Function
{
public:
    int Calculate(Stack<int>& values) override;

    const char* Name() override { return "IF"; }

    void Print() override
    {
        printf("%s", this->Name());
    }

    Function* Clone() const override;
};


#endif //KALKULATOR_FUNCTIONS_HPP
