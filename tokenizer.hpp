//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_TOKENIZER_HPP
#define KALKULATOR_TOKENIZER_HPP

#include "list.hpp"
#include "tokens.hpp"

class Tokenizer {
private:
    List<TokenValue> tokens;
    char *tokenBuffer = nullptr;
    size_t tokenBufferSize = 0;

    void flushBuffer();

    bool addTokenAndFlush(Token token);

    bool readNextCharacter();

public:
    const List<TokenValue> &readTokens();
};



#endif //KALKULATOR_TOKENIZER_HPP
