//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_TOKENIZER_HPP
#define KALKULATOR_TOKENIZER_HPP

#include "list.hpp"
#include "onp.hpp"
#include "tokens.hpp"

class Tokenizer
{
private:
    char* tokenBuffer = nullptr;
    size_t tokenBufferSize = 0;
    ONPParser& parser;

    void flushBuffer();

    bool addTokenAndFlush(Token token);

    bool readNextCharacter();

public:
    explicit Tokenizer(ONPParser& parser): parser(parser)
    {
    }

    void readTokens();
};


#endif //KALKULATOR_TOKENIZER_HPP
