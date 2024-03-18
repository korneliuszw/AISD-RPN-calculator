// Created by wired-mac on 09/03/2024.
//

#include "onp.hpp"

void ONPParser::PullOperator(Stack<TokenValue>& operatorStack, int minPriority)
{
    Optional<TokenValue> op = operatorStack.Pop();
    while (op.HasValue())
    {
        auto opValue = op.GetValue();
        int priority = getTokenPriority(opValue->token);
        // negate is a special case i guess?
        if (opValue->token == PARENTHESSIS_START || priority < minPriority || (priority == minPriority && priority ==
            getTokenPriority(NEGATE)))
        {
            operatorStack.Push(*opValue);
            break;
        }
        converted.AddLast(*opValue);
        op = operatorStack.Pop();
    }
}

const ListNode<TokenValue>* ONPParser::Parse(TokenValue&& token)
{
    const auto ctx = contextStack.Peek();
    if (token.token == Token::VALUE)
    {
        converted.AddLast(token);
    }
    else if (token.token == FUNCTION)
    {
        operatorStack.Push(token);
    }
    else if (token.token == PARENTHESSIS_START)
    {
        if (operatorStack.GetLength() > 0 && operatorStack.Peek()->value.token == FUNCTION)
        {
            contextStack.Push(Context(true));
        }
        else
        {
            contextStack.Push(Context());
        }
        operatorStack.Push(token);
    }
    else if (token.token == PARENTHESSIS_END)
    {
        PullOperator(operatorStack);
        operatorStack.Pop();
        if (contextStack.GetLength() > 1)
        {
            const auto oldContext = contextStack.Pop();
            if (oldContext.HasValue() && oldContext.GetValue()->insideFunction)
            {
                const auto val = oldContext.GetValue();
                if (val->insideFunction)
                {
                    const auto funcToken = operatorStack.Pop().GetValue();
                    funcToken->function->SetArgumentCount(val->arguments);
                    converted.AddLast(TokenValue(funcToken->token, funcToken->function));
                }
            }
        }
    }
    else if (token.token == ARGUMENT_SEP && ctx->value.insideFunction)
    {
        ctx->value.arguments += 1;
        PullOperator(operatorStack);
    }
    else
    {
        int priority = getTokenPriority(token.token);
        PullOperator(operatorStack, priority);
        operatorStack.Push(token);
    }
    return nullptr;
}

void ONPParser::pullEnd()
{
    PullOperator(operatorStack);
}

void ONPParser::Print()
{
    auto node = converted.GetFirst();
    while (node)
    {
        if (node != converted.GetFirst()) printf(" ");
        if (node->value.token == VALUE) printf("%d", node->value.numericValue);
        else if (node->value.token == FUNCTION) node->value.function->Print();
        else printf("%c", stringifyToken(node->value.token));
        printf(" ");
        node = node->next;
    }
}

void ONPEvaluator::PrintStack(const TokenValue& token, const Stack<int>& stack)
{
    if (token.token == FUNCTION)
    {
        token.function->Print();
    }
    else
    {
        printf("%c", stringifyToken(token.token));
    }
    auto node = stack.Peek();
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
            operandStack.Push(token->value.numericValue);
            token = token->next;
            continue;
        }
        int result = 0;
        PrintStack(token->value, operandStack);
        switch (token->value.token)
        {
        case NEGATE:
            {
                result -= *operandStack.Pop().GetValue();
                break;
            }
        case MULTIPLY:
            {
                int b = *operandStack.Pop().GetValue(), a = *operandStack.Pop().GetValue();
                result += a * b;
                break;
            }
        case DIVIDE:
            {
                int b = *operandStack.Pop().GetValue(), a = *operandStack.Pop().GetValue();
                if (b == 0)
                {
                    return nullptr;
                }
                result += a / b;
                break;
            }
        case ADD:
            {
                int b = *operandStack.Pop().GetValue(), a = *operandStack.Pop().GetValue();
                result += a + b;
                break;
            }
        case SUBSTRACT:
            {
                int b = *operandStack.Pop().GetValue(), a = *operandStack.Pop().GetValue();
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
        operandStack.Push(result);
        token = token->next;
    }
    auto top = operandStack.Pop();
    return top.HasValue() ? new int(*top.GetValue()) : nullptr;
}
