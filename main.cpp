#include <cassert>
#include <cstring>
#include <iostream>
#include <cstdlib>

using namespace std;

template<typename Value>
class Optional {
private:
    bool hasValue = false;
    char container[sizeof(Value)];

public:
    Optional<Value>() {}

    Optional<Value>(const Value &&val) {
        hasValue = true;
        new(container) Value(val);
    }

    bool HasValue() { return this->hasValue; }

    const Value *getValue() const {
        assert(this->hasValue == true);
        return reinterpret_cast<const Value *>(container);
    }
};

template<typename Value>
struct ListNode {
    ListNode<Value> *next = nullptr;
    ListNode<Value> *prev = nullptr;
    Value value;

    explicit ListNode<Value>(Value value) : value(value) {}

    ~ListNode() = default;
};

template<typename Value>
class List {
private:
    ListNode<Value> *root = nullptr;
    ListNode<Value> *end = nullptr;
    size_t size = 0;

public:
    void AddLast(Value val) {
        auto *node = new ListNode<Value>(val);
        if (size > 0) {
            end->next = node;
            node->prev = end;
            end = node;
        } else {
            root = end = node;
        }
        size++;
    }

    void AddFirst(Value val) {
        auto *node = new ListNode<Value>(val);
        if (size > 0) {
            root->prev = node;
            node->next = root;
            root = node;
        } else {
            root = end = node;
        }
        size++;
    }

    Optional<Value> PopFirst() {
        if (size == 0) {
            return Optional<Value>();
        }
        Value val = root->value;
        auto *oldRoot = root;
        root = root->next;
        delete oldRoot;
        size--;
        return Optional<Value>(std::move(val));
    }

    Optional<Value> PopLast() {
        if (size == 0) {
            return Optional<Value>();
        }
        Value &val = end->value;
        auto *oldEnd = end;
        end = end->prev;
        delete oldEnd;
        size--;
        return Optional<Value>(std::move(val));
    }

    ListNode<Value> *GetFirst() const { return this->root; }

    int getLength() const { return size; }

    ~List() {
        auto node = root;
        auto next = root;
        while (node != nullptr) {
            next = node->next;
            delete node;
            node = next;
        }
    }
};

template<typename Value>
class Stack {
private:
    List<Value> list;

public:
    int getLength() const { return list.getLength(); }

    Optional<Value> pop() { return list.PopFirst(); }

    void push(Value val) { return list.AddFirst(val); }
    const ListNode<Value>* peek() const { return list.GetFirst();}
};

enum Token {
    PARENTHESSIS_START,
    PARENTHESSIS_END,
    NEGATE,
    MULTIPLY,
    DIVIDE,
    ADD,
    SUBSTRACT,
    FUNCTION,
    ARGUMENT_SEP,
    VALUE
};

char stringifyToken(const Token &token) {
    switch (token) {
        case PARENTHESSIS_START:
            return '(';
        case PARENTHESSIS_END:
            return ')';
        case NEGATE:
            return 'N';
        case SUBSTRACT:
            return '-';
        case MULTIPLY:
            return '*';
        case DIVIDE:
            return '/';
        case ADD:
            return '+';
        default:
            return '\0';
    }
}

int getTokenPriority(const Token &token) {
    if (token == PARENTHESSIS_START || token == PARENTHESSIS_END)
        return 6;
    else if (token == NEGATE || token == FUNCTION)
        return 5;
    else if (token == MULTIPLY || token == DIVIDE)
        return 4;
    else if (token == ADD || token == SUBSTRACT)
        return 3;
    else if (token == VALUE)
        return 2;
    return 0;
}

class Function {
protected:
    int argumentCount = 0;
public:
    virtual int Calculate(Stack<int>&) = 0;
    void SetArgumentCount(int count) {
        argumentCount = count;
    }
    int GetArgumentCount() const {
        return argumentCount;
    }
    virtual const char *Name() = 0;

    virtual ~Function() = default;

    Function() = default;

    Function(const Function &function) = default;

    Function &operator=(const Function &r) { return *this; }
    virtual void print() {
        printf("%s%d", this->Name(), this->argumentCount);
    }
};

class MaxFunction : public Function {
public:
    int Calculate(Stack<int>& values) override {
        if (values.getLength() == 0)
            return 0;
        int max = -999999999;
        for (int i = 0; i < argumentCount; i++) {
            int value = *values.pop().getValue();
            max = max < value ? value : max;
        }
        return max;
    }

    const char *Name() override { return "MAX"; }
};

class MinFunction : public Function {
public:
    int Calculate(Stack<int>& values) override {
        if (values.getLength() == 0)
            return 0;
        int min = 999999999;
        for (int i = 0; i < argumentCount; i++) {
            int value = *values.pop().getValue();
            min = min > value ? value : min;
        }
        return min;
    }

    const char *Name() override { return "MIN"; }
};

class IfFunction : public Function {
public:
    int Calculate(Stack<int>& values) override {
        assert(values.getLength() >= 3);
        int c = *values.pop().getValue(), b = *values.pop().getValue(), a = *values.pop().getValue();
        return a > 0 ? b : c;
    }

    const char *Name() override { return "IF"; }
    void print() override {
        printf("%s", this->Name());
    }
};

struct TokenValue {
    Token token;
    void *value = nullptr;
    size_t *ownerCounter = new size_t(1);

    ~TokenValue() {
        if (token == VALUE)
            delete (char *) value;
        else if (token == FUNCTION && *ownerCounter == 1)
            delete (Function *) value;
        *ownerCounter -= 1;
        if (*ownerCounter == 0) delete ownerCounter;
    }

    TokenValue(Token token) : token{token} {};

    TokenValue(Token token, void *value) : token{token}, value{value} {};

    TokenValue(const TokenValue &t) {
        if (t.token == VALUE) {
            value = malloc(strlen((char*) t.value) + 1);
            strcpy((char*) value, (char*) t.value);
        } else if (t.token == FUNCTION) {
            value = t.value;
        }
        delete ownerCounter;
        ownerCounter = t.ownerCounter;
        *ownerCounter += 1;
        token = t.token;
    }

    TokenValue &operator=(const TokenValue &r) { return *this; }
};

class Tokenizer {
private:
    List<TokenValue> tokens;
    char *tokenBuffer = nullptr;
    size_t tokenBufferSize = 0;

    void flushBuffer() {
        if (tokenBuffer != nullptr) {
            tokens.AddLast({Token::VALUE, tokenBuffer});
            tokenBuffer = nullptr;
        }
    }

    bool readNextCharacter() {
        char c = cin.get();
        // end of formula
        if (c == '.') {
            // flush remaining buffer
            if (tokenBuffer != nullptr) {
                tokens.AddLast({Token::VALUE, tokenBuffer});
                tokenBuffer = nullptr;
            }
            return false;
        }
        switch (c) {
            case ',': {
                flushBuffer();
                tokens.AddLast({Token::ARGUMENT_SEP});
                return true;
            }
            case '+': {
                flushBuffer();
                tokens.AddLast({Token::ADD});
                return true;
            }
            case '-': {
                flushBuffer();
                tokens.AddLast({Token::SUBSTRACT});
                return true;
            }
            case '/': {
                flushBuffer();
                tokens.AddLast({Token::DIVIDE});
                return true;
            }
            case '*': {
                flushBuffer();
                tokens.AddLast({Token::MULTIPLY});
                return true;
            }
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
                        tokens.AddLast({Token::FUNCTION, function});
                        delete tokenBuffer;
                        tokenBuffer = nullptr;
                    }
                }
                tokens.AddLast({Token::PARENTHESSIS_START});
                return true;
            }
            case ')': {
                flushBuffer();
                tokens.AddLast({Token::PARENTHESSIS_END});
                return true;
            }
        }
        // we only care about numbers and alphabet at this point so we skip rest
        if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'z') &&
            !(c >= 'A' && c <= 'Z'))
            return true;
        // add character to buffer
        if (tokenBuffer == nullptr) {
            tokenBufferSize = 0;
            tokenBuffer = new char[255];
        }
        if (c == 'N' && tokenBufferSize == 0) {
            tokens.AddLast({Token::NEGATE});
            delete tokenBuffer;
            tokenBuffer = nullptr;
        }
        else {
            tokenBuffer[tokenBufferSize++] = c;
            tokenBuffer[tokenBufferSize] = '\0';
        }
        return true;
    }

public:
    const List<TokenValue> &readTokens() {
        while (readNextCharacter());
        return tokens;
    }
};

class ONPParser {
private:
    List<TokenValue> converted;


    void pullOperator(Stack<TokenValue> &operatorStack, int minPriority = 0) {
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

public:
    const List<TokenValue>& GetConvertedTokenList() const { return converted; }
    const ListNode<TokenValue> *parse(const ListNode<TokenValue> *token, bool context,
                                      bool isInsideFunction = false, int *functionArgsCounter = nullptr) {
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
                    ((Function*) functionCallPointer->value)->SetArgumentCount(functionArgs);
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

    void print() {
        auto node = converted.GetFirst();
        while (node) {
            if (node != converted.GetFirst()) printf(" ");
            if (node->value.token == VALUE) printf("%s",(char*) node->value.value);
            else if (node->value.token == FUNCTION) ((Function*) node->value.value)->print();
            else printf("%c", stringifyToken(node->value.token));
            printf(" ");
            node = node->next;
        }
    }
};

class ONPEvaluator {
public:
    static void printStack(const TokenValue& token, const Stack<int>& stack) {
        if (token.token == FUNCTION) {
            ((Function*) token.value)->print();
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
    static int* Evaluate(const List<TokenValue>& onpList) {
        Stack<int> operandStack;
        auto token = onpList.GetFirst();
        while (token) {
            if (token->value.token == VALUE) {
                operandStack.push(atoi((char*) token->value.value));
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
                    int b = *operandStack.pop().getValue(), a= *operandStack.pop().getValue();
                    result += a * b;
                    break;
                }
                case DIVIDE: {
                    int b = *operandStack.pop().getValue(), a= *operandStack.pop().getValue();
                    if (b == 0) {
                        return nullptr;
                    }
                    result += a / b;
                    break;
                }
                case ADD: {
                    int b = *operandStack.pop().getValue(), a= *operandStack.pop().getValue();
                    result += a + b;
                    break;
                }
                case SUBSTRACT: {
                    int b = *operandStack.pop().getValue(), a= *operandStack.pop().getValue();
                    result += a - b;
                    break;
                }
                case FUNCTION: {
                    result += ((Function*) token->value.value)->Calculate(operandStack);
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
};

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
        if (result)
            printf("%d\n", *result);
        else printf("ERROR\n");
    }
}
