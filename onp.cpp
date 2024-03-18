// Created by wired-mac on 09/03/2024.
//

#include "onp.hpp"

void ONPParser::pullOperator(Stack<TokenValue>& operatorStack, int minPriority)
{
    Optional<TokenValue> op = operatorStack.pop();
    while (op.HasValue())
    {
        auto opValue = op.getValue();
        int priority = getTokenPriority(opValue->token);
        // negate is a special case i guess?
        if (opValue->token == PARENTHESSIS_START || priority < minPriority || (priority == minPriority && priority ==
            getTokenPriority(NEGATE)))
        {
            operatorStack.push(*opValue);
            break;
        }
        converted.AddLast(*opValue);
        op = operatorStack.pop();
    }
}

const ListNode<TokenValue>* ONPParser::parse(TokenValue&& token)
{
    const auto ctx = contextStack.peek();
    if (token.token == Token::VALUE)
    {
        converted.AddLast(token);
    }
    else if (token.token == FUNCTION)
    {
        operatorStack.push(token);
    }
    else if (token.token == PARENTHESSIS_START)
    {
        if (operatorStack.getLength() > 0 && operatorStack.peek()->value.token == FUNCTION)
        {
            contextStack.push(Context(true));
        }
        else
        {
            contextStack.push(Context());
        }
        operatorStack.push(token);
    }
    else if (token.token == PARENTHESSIS_END)
    {
        pullOperator(operatorStack);
        operatorStack.pop();
        if (contextStack.getLength() > 1)
        {
            const auto oldContext = contextStack.pop();
            if (oldContext.HasValue() && oldContext.getValue()->insideFunction)
            {
                const auto val = oldContext.getValue();
                if (val->insideFunction)
                {
                    const auto funcToken = operatorStack.pop().getValue();
                    funcToken->function->SetArgumentCount(val->arguments);
                    converted.AddLast(TokenValue(funcToken->token, funcToken->function));
                }
            }
        }
    }
    else if (token.token == ARGUMENT_SEP && ctx->value.insideFunction)
    {
        ctx->value.arguments += 1;
        pullOperator(operatorStack);
    }
    else
    {
        int priority = getTokenPriority(token.token);
        pullOperator(operatorStack, priority);
        operatorStack.push(token);
    }
    return nullptr;
}

void ONPParser::pullEnd()
{
    pullOperator(operatorStack);
}

void ONPParser::print()
{
    auto node = converted.GetFirst();
    while (node)
    {
        if (node != converted.GetFirst()) printf(" ");
        if (node->value.token == VALUE) printf("%d", node->value.numericValue);
        else if (node->value.token == FUNCTION) node->value.function->print();
        else printf("%c", stringifyToken(node->value.token));
        printf(" ");
        node = node->next;
    }
}

void ONPEvaluator::printStack(const TokenValue& token, const Stack<int>& stack)
{
    if (token.token == FUNCTION)
    {
        token.function->print();
    }
    else
    {
        printf("%c", stringifyToken(token.token));
    }
    auto node = stack.peek();
    while (node)
    {
        printf(" %d", node->value);
        node = node->next;
    }
    printf("\n");
}

int* ONPEvaluator::Evaluate(const List<TokenValue>& onpList)
{
    Stack<int> operandStack;
    auto token = onpList.GetFirst();
    while (token)
    {
        if (token->value.token == VALUE)
        {
            operandStack.push(token->value.numericValue);
            token = token->next;
            continue;
        }
        int result = 0;
        printStack(token->value, operandStack);
        switch (token->value.token)
        {
        case NEGATE:
            {
                result -= *operandStack.pop().getValue();
                break;
            }
        case MULTIPLY:
            {
                int b = *operandStack.pop().getValue(), a = *operandStack.pop().getValue();
                result += a * b;
                break;
            }
        case DIVIDE:
            {
                int b = *operandStack.pop().getValue(), a = *operandStack.pop().getValue();
                if (b == 0)
                {
                    return nullptr;
                }
                result += a / b;
                break;
            }
        case ADD:
            {
                int b = *operandStack.pop().getValue(), a = *operandStack.pop().getValue();
                result += a + b;
                break;
            }
        case SUBSTRACT:
            {
                int b = *operandStack.pop().getValue(), a = *operandStack.pop().getValue();
                result += a - b;
                break;
            }
        case FUNCTION:
            {
                result += token->value.function->Calculate(operandStack);
                break;
            }
        default:
            {
                break;
            }
        }
        operandStack.push(result);
        token = token->next;
    }
    auto top = operandStack.pop();
    return top.HasValue() ? new int(*top.getValue()) : nullptr;
}
