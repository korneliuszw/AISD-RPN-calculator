//
// Created by wired-mac on 09/03/2024.
//

#include "onp.hpp"

void ONPParser::pullOperator(Stack<TokenValue> &operatorStack, int minPriority) {
    Optional<TokenValue> op = operatorStack.pop();
    while (op.HasValue()) {
        auto opValue = op.getValue();
        int priority = getTokenPriority(opValue->token);
        // negate is a special case i guess?
        if (priority < minPriority || (priority == minPriority && priority == getTokenPriority(NEGATE))) {
            operatorStack.push(*opValue);
            break;
        }
        converted.AddLast(*opValue);
        op = operatorStack.pop();
    }
}

const ListNode<TokenValue> *ONPParser::parse(const ListNode<TokenValue> *token, bool context,
                                             bool isInsideFunction, int *functionArgsCounter) {
    TokenValue *functionCallPointer = nullptr;
    Stack<TokenValue> operatorStack;
    int functionArgs = 0;
    while (token != nullptr) {
        if (token->value.token == Token::VALUE) {
            converted.AddLast(token->value);
        } else if (token->value.token == FUNCTION) {
            functionCallPointer = new TokenValue(token->value);
            functionArgs = 0;
        } else if (token->value.token == PARENTHESSIS_START) {
            token = parse(token->next, true, functionCallPointer != nullptr, &functionArgs);
            if (functionCallPointer) {
                functionCallPointer->function->SetArgumentCount(functionArgs);
                converted.AddLast(*functionCallPointer);
                delete functionCallPointer;
            }
            functionCallPointer = nullptr;
        } else if (token->value.token == PARENTHESSIS_END) {
            pullOperator(operatorStack);
            if (isInsideFunction && functionArgsCounter) *functionArgsCounter += 1;
            if (context)
                return token;
        } else if (token->value.token == ARGUMENT_SEP && isInsideFunction && functionArgsCounter) {
            pullOperator(operatorStack);
            *functionArgsCounter += 1;
        } else {
            int priority = getTokenPriority(token->value.token);
            pullOperator(operatorStack, priority);
            operatorStack.push(token->value);
        }
        token = token->next;
    }
    pullOperator(operatorStack);
    return nullptr;
}

void ONPParser::print() {
    auto node = converted.GetFirst();
    while (node) {
        if (node != converted.GetFirst()) printf(" ");
        if (node->value.token == VALUE) printf("%d", node->value.numericValue);
        else if (node->value.token == FUNCTION) node->value.function->print();
        else printf("%c", stringifyToken(node->value.token));
        printf(" ");
        node = node->next;
    }
}

void ONPEvaluator::printStack(const TokenValue &token, const Stack<int> &stack) {
    if (token.token == FUNCTION) {
        token.function->print();
    } else {
        printf("%c", stringifyToken(token.token));
    }
    auto node = stack.peek();
    while (node) {
        printf(" %d", node->value);
        node = node->next;
    }
    printf("\n");
}

int *ONPEvaluator::Evaluate(const List<TokenValue> &onpList) {
    Stack<int> operandStack;
    auto token = onpList.GetFirst();
    while (token) {
        if (token->value.token == VALUE) {
            operandStack.push(token->value.numericValue);
            token = token->next;
            continue;
        }
        int result = 0;
        printStack(token->value, operandStack);
        switch (token->value.token) {
            case NEGATE: {
                result -= *operandStack.pop().getValue();
                break;
            }
            case MULTIPLY: {
                int b = *operandStack.pop().getValue(), a = *operandStack.pop().getValue();
                result += a * b;
                break;
            }
            case DIVIDE: {
                int b = *operandStack.pop().getValue(), a = *operandStack.pop().getValue();
                if (b == 0) {
                    return nullptr;
                }
                result += a / b;
                break;
            }
            case ADD: {
                int b = *operandStack.pop().getValue(), a = *operandStack.pop().getValue();
                result += a + b;
                break;
            }
            case SUBSTRACT: {
                int b = *operandStack.pop().getValue(), a = *operandStack.pop().getValue();
                result += a - b;
                break;
            }
            case FUNCTION: {
                result += token->value.function->Calculate(operandStack);
                break;
            }
            default: {
                break;
            }
        }
        operandStack.push(result);
        token = token->next;
    }
    auto top = operandStack.pop();
    return top.HasValue() ? new int(*top.getValue()) : nullptr;
}
