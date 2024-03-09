#include <iostream>

#include "tokenizer.hpp"
#include "onp.hpp"

using namespace std;




int main() {
    int N;
    cin >> N;
    for (int i = 0; i < N; i++) {
        Tokenizer tokenizer;
        auto &tokens = tokenizer.readTokens();
        ONPParser parser;
        parser.parse(tokens.GetFirst(), false);
        parser.print();
        printf("\n");
        auto result = ONPEvaluator::Evaluate(parser.GetConvertedTokenList());
        if (result) {
            printf("%d\n", *result);
            delete result;
        }
        else printf("ERROR\n");
    }
}
