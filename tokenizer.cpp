//
// Created by wired-mac on 09/03/2024.
//

#include "tokenizer.hpp"
#include <iostream>
#include <cstring>

using namespace std;

void Tokenizer::FlushBuffer()
{
    if (tokenBuffer != nullptr)
    {
        parser.Parse(TokenValue(Token::VALUE, atoi(tokenBuffer)));
        free(tokenBuffer);
        tokenBuffer = nullptr;
    }
}

bool Tokenizer::AddTokenAndFlush(Token token)
{
    FlushBuffer();
    parser.Parse(TokenValue(token));
    return true;
}

bool Tokenizer::ReadNextCharacter()
{
    char c = cin.get();
    // end of formula
    if (c == '.')
    {
        FlushBuffer();
        parser.pullEnd();
        return false;
    }
    switch (c)
    {
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
    case '(':
        {
            if (tokenBuffer && strlen(tokenBuffer) > 0)
            {
                Function* function = nullptr;
                if (strcmp("MAX", tokenBuffer) == 0)
                    function = new MaxFunction();
                else if (strcmp("MIN", tokenBuffer) == 0)
                    function = new MinFunction();
                else if (strcmp("IF", tokenBuffer) == 0)
                    function = new IfFunction();
                else
                    FlushBuffer();
                if (function)
                {
                    parser.Parse(TokenValue(Token::FUNCTION, function));
                    free(tokenBuffer);
                    tokenBuffer = nullptr;
                }
            }
            parser.Parse(TokenValue(Token::PARENTHESSIS_START));
            return true;
        }
    }
    // we only care about numbers and alphabet at this point so we skip rest
    if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'z') &&
        !(c >= 'A' && c <= 'Z'))
        return true;
    // add character to buffer
    if (!tokenBuffer)
    {
        tokenBufferSize = 0;
        tokenBuffer = (char*)malloc(14);
    }
    if (c == 'N' && tokenBufferSize == 0)
    {
        parser.Parse(TokenValue(Token::NEGATE));
        // we only discard the buffer here, because it's not passed to tokens!
        if (tokenBuffer)
            free(tokenBuffer);
        tokenBuffer = nullptr;
    }
    else
    {
        tokenBuffer[tokenBufferSize++] = c;
        tokenBuffer[tokenBufferSize] = '\0';
    }
    return true;
}

void Tokenizer::ReadTokens()
{
    while (ReadNextCharacter());
}
