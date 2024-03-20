//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_FUNCTIONS_HPP
#define KALKULATOR_FUNCTIONS_HPP

#include <cstdio>

#include "stack.hpp"

class Function
{
public:
    virtual int Calculate(Stack<int>&, int) = 0;


    virtual const char* Name() = 0;

    virtual void Print(int arguments)
    {
        printf("%s%d", this->Name(), arguments);
    }
};

class MaxFunction : public Function
{
public:
    int Calculate(Stack<int>& values, int arguments) override;

    const char* Name() override { return "MAX"; }
};

class MinFunction : public Function
{
public:
    int Calculate(Stack<int>& values, int arguments) override;

    const char* Name() override { return "MIN"; }
};

class IfFunction : public Function
{
public:
    int Calculate(Stack<int>& values, int arguments) override;

    const char* Name() override { return "IF"; }

    void Print(int arguments) override
    {
        printf("%s", this->Name());
    }
};


#endif //KALKULATOR_FUNCTIONS_HPP
