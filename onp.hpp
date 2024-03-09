//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_ONP_HPP
#define KALKULATOR_ONP_HPP

#include "tokens.hpp"
#include "list.hpp"

class ONPParser {
private:
    List<TokenValue> converted;
    void pullOperator(Stack<TokenValue> &operatorStack, int minPriority = 0);

public:
    const List<TokenValue>& GetConvertedTokenList() const { return converted; }
    const ListNode<TokenValue> *parse(const ListNode<TokenValue> *token, bool context,
                                      bool isInsideFunction = false, int *functionArgsCounter = nullptr);
    void print();
};

class ONPEvaluator {
public:
    static void printStack(const TokenValue& token, const Stack<int>& stack);
    static int* Evaluate(const List<TokenValue>& onpList);
};

#endif //KALKULATOR_ONP_HPP
