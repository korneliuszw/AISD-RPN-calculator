//
// Created by wired-mac on 09/03/2024.
//

#include "tokenizer.hpp"
#include <iostream>
#include <cstring>

using namespace std;

void Tokenizer::flushBuffer() {
    if (tokenBuffer != nullptr) {
        TokenValue val(Token::VALUE, atoi(tokenBuffer));
        tokens.AddLast(std::move(val));
        free(tokenBuffer);
        tokenBuffer = nullptr;
    }
}
bool Tokenizer::addTokenAndFlush(Token token) {
    flushBuffer();
    TokenValue val(token);
    tokens.AddLast(std::move(val));
    return true;
}
bool Tokenizer::readNextCharacter(){
    char c = cin.get();
    // end of formula
    if (c == '.') {
        flushBuffer();
        return false;
    }
    switch (c) {
        case ',':
            return addTokenAndFlush(Token::ARGUMENT_SEP);
        case '+':
            return addTokenAndFlush(Token::ADD);
        case '-':
            return addTokenAndFlush(Token::SUBSTRACT);
        case '/':
            return addTokenAndFlush(Token::DIVIDE);
        case '*':
            return addTokenAndFlush(Token::MULTIPLY);
        case ')':
            return addTokenAndFlush(Token::PARENTHESSIS_END);
        case '(': {
            if (tokenBuffer && strlen(tokenBuffer) > 0) {
                Function *function = nullptr;
                if (strcmp("MAX", tokenBuffer) == 0)
                    function = new MaxFunction();
                else if (strcmp("MIN", tokenBuffer) == 0)
                    function = new MinFunction();
                else if (strcmp("IF", tokenBuffer) == 0)
                    function = new IfFunction();
                else
                    flushBuffer();
                if (function) {
                    tokens.AddLast(TokenValue(Token::FUNCTION, function));
                    free(tokenBuffer);
                    tokenBuffer = nullptr;
                }
            }
            tokens.AddLast(TokenValue(Token::PARENTHESSIS_START));
            return true;
        }

    }
    // we only care about numbers and alphabet at this point so we skip rest
    if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'z') &&
        !(c >= 'A' && c <= 'Z'))
        return true;
    // add character to buffer
    if (!tokenBuffer) {
        tokenBufferSize = 0;
        tokenBuffer = (char*) malloc(14);
    }
    if (c == 'N' && tokenBufferSize == 0) {
        tokens.AddLast(TokenValue(Token::NEGATE));
        // we only discard the buffer here, because it's not passed to tokens!
        if (tokenBuffer)
            free(tokenBuffer);
        tokenBuffer = nullptr;
    }
    else {
        tokenBuffer[tokenBufferSize++] = c;
        tokenBuffer[tokenBufferSize] = '\0';
    }
    return true;
}

const List<TokenValue> &Tokenizer::readTokens() {
    while (readNextCharacter());
    return tokens;
}