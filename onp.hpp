//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_ONP_HPP
#define KALKULATOR_ONP_HPP

#include "tokens.hpp"
#include "list.hpp"

struct Context
{
    int arguments = 0;
    bool insideFunction;

    Context(bool insideFunction = false)
    {
        if (insideFunction) arguments = 1;
        this->insideFunction = insideFunction;
    }
};

class ONPParser
{
private:
    Stack<Context> contextStack;
    List<TokenValue> converted;
    Stack<TokenValue> operatorStack;
    void pullOperator(Stack<TokenValue>& operatorStack, int minPriority = 0);

public:
    const List<TokenValue>& GetConvertedTokenList() const { return converted; }
    const ListNode<TokenValue>* parse(TokenValue&& token);
    void pullEnd();
    void print();

    ONPParser()
    {
        contextStack.push(Context());
    }
};

class ONPEvaluator
{
public:
    static void printStack(const TokenValue& token, const Stack<int>& stack);
    static int* Evaluate(const List<TokenValue>& onpList);
};

#endif //KALKULATOR_ONP_HPP
