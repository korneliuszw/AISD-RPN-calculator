#include <iostream>

#include "tokenizer.hpp"
#include "onp.hpp"
#include "tokens.hpp"

using namespace std;


int main()
{
    int N;
    cin >> N;
    for (int i = 0; i < N; i++)
    {
        ONPParser parser;
        Tokenizer tokenizer(parser);
        tokenizer.ReadTokens();
        parser.Print();
        printf("\n");
        auto result = ONPEvaluator::Evaluate(parser.GetConvertedTokenList());
        if (result)
        {
            printf("%d\n", *result);
            delete result;
        }
        else printf("ERROR\n");
    }
}
