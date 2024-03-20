//
// Created by wired-mac on 09/03/2024.
//

#include "tokenizer.hpp"
#include <iostream>
#include <cstring>

using namespace std;


bool Tokenizer::AddTokenAndFlush(Token token)
{
    parser.Parse(TokenValue(token));
    return true;
}

bool Tokenizer::ReadNextCharacter()
{
    char str[12];
    scanf("%s", str);
    // end of formula
    switch (str[0])
    {
    case '.':
        parser.pullEnd();
        return false;
    case ',':
        return AddTokenAndFlush(Token::ARGUMENT_SEP);
    case '+':
        return AddTokenAndFlush(Token::ADD);
    case '-':
        return AddTokenAndFlush(Token::SUBSTRACT);
    case '/':
        return AddTokenAndFlush(Token::DIVIDE);
    case '*':
        return AddTokenAndFlush(Token::MULTIPLY);
    case ')':
        return AddTokenAndFlush(Token::PARENTHESSIS_END);
    case 'N':
        return AddTokenAndFlush(NEGATE);
    case 'I':
        pendingFunctionToken = new Token(IF);
        return true;
    case 'M':
        {
            if (str[1] == 'A') pendingFunctionToken = new Token(MAX);
            else pendingFunctionToken = new Token(MIN);
            return true;
        }
    case '(':
        {
            if (pendingFunctionToken)
            {
                parser.Parse(TokenValue(*pendingFunctionToken, 0));
                delete pendingFunctionToken;
                pendingFunctionToken = nullptr;
            }
            parser.Parse(TokenValue(Token::PARENTHESSIS_START));
            return true;
        }
    }
    if (str[0] < '0' || str[0] > '9') return true;
    parser.Parse(TokenValue(VALUE, atoi(str)));

    return true;
}

void Tokenizer::ReadTokens()
{
    while (ReadNextCharacter());
}
