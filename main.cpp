#include <cassert>
#include <cstddef>
#include <cstring>
#include <initializer_list>
#include <iostream>
#include <ostream>

using namespace std;

template <typename Value> class Optional {
private:
  Value val;
  bool hasValue = false;

public:
  Optional<Value>(){};
  Optional<Value>(Value val) : val{val} {}
  bool HasValue() { return this->hasValue; }
  Value getValue() {
    assert(this->hasValue == true);
    return this->val;
  }
};

template <typename Value> struct ListNode {
  ListNode<Value> *next = NULL;
  ListNode<Value> *prev = NULL;
  Value value;
  ListNode<Value>(Value value) : value{value} {}
  ~ListNode() {}
};

template <typename Value> class List {
private:
  ListNode<Value> *root = NULL;
  ListNode<Value> *end = NULL;
  size_t size = 0;

public:
  void AddLast(Value val) {
    ListNode<Value> *node = new ListNode<Value>(val);
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
    ListNode<Value> *node = new ListNode<Value>(val);
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
    return Optional<Value>(val);
  }
  Optional<Value> PopLast() {
    if (size == 0) {
      return Optional<Value>();
    }
    Value val = end->value;
    auto *oldEnd = end;
    end = end->prev;
    delete oldEnd;
    size--;
    return Optional<Value>(val);
  }
  ListNode<Value> *GetFirst() const { return this->root; }
  int getLength() { return size; }
  ~List() {
    auto node = root;
    auto next = root;
    while (node != NULL) {
      next = node->next;
      delete node;
      node = next;
    }
  }
};

template <typename Value> class Stack {
private:
  List<Value> list;

public:
  int getLength() { return list.size; }
  Optional<Value> pop() { return list.pop_first(); }
  void push(Value val) { return list.pop_first(val); }
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

int getTokenSpecificity(const Token &token) {
  if (token == PARENTHESSIS_START || token == PARENTHESSIS_END)
    return 6;
  else if (token == NEGATE)
    return 5;
  else if (token == MULTIPLY || token == DIVIDE)
    return 4;
  else if (token == ADD || token == SUBSTRACT)
    return 3;
  else if (token == FUNCTION)
    return 2;
  else if (token == VALUE)
    return 1;
  return 0;
}

class Function {
public:
  virtual int Calculate(initializer_list<int>) = 0;
  virtual const char *Name() = 0;
};

class MaxFunction : public Function {
public:
  int Calculate(initializer_list<int> values) {
    if (values.size() == 0)
      return 0;
    int max = *values.begin();
    for (const auto &val : values) {
      max = val > max ? val : max;
    }
    return max;
  }
  const char *Name() { return "MAX"; }
};
class MinFunction : public Function {
public:
  int Calculate(initializer_list<int> values) {
    if (values.size() == 0)
      return 0;
    int min = *values.begin();
    for (const auto &val : values) {
      min = val < min ? val : min;
    }
    return min;
  }
  const char *Name() { return "MIN"; }
};
class IfFunction : public Function {
public:
  int Calculate(initializer_list<int> values) {
    assert(values.size() == 3);
    int a = *values.begin(), b = *values.begin() + 1, c = *values.begin() + 2;
    return a > 0 ? b : c;
  }
  const char *Name() { return "IF"; }
};

struct TokenValue {
  Token token;
  void *value;
  friend ostream &operator<<(ostream &out, const TokenValue &token) {
    switch (token.token) {
    case MULTIPLY: {
      out << "MULTIPLY";
      break;
    }
    case PARENTHESSIS_END:
    case PARENTHESSIS_START: {
      out << "PAREN";
      break;
    }
    case DIVIDE: {
      out << "DIVIDE";
      break;
    }
    case ADD: {
      out << "ADD";
      break;
    }
    case SUBSTRACT: {
      out << "SUBSTRACT";
      break;
    }
    case FUNCTION: {
      out << "FUNCTION " << reinterpret_cast<Function *>(token.value)->Name();
      break;
    }
    case VALUE: {
      out << "VALUE: " << reinterpret_cast<char *>(token.value);
      break;
    }
    case NEGATE: {
      out << "NEGATE";
      break;
    }
    case ARGUMENT_SEP:
      out << "Argument seperator";
      break;
    }
    return out;
  }
};

class Tokenizer {
private:
  List<TokenValue> tokens;
  char *tokenBuffer = NULL;
  size_t tokenBufferSize = 0;
  void flushBuffer() {
    if (tokenBuffer != NULL) {
      tokens.AddLast({Token::VALUE, tokenBuffer});
      tokenBuffer = NULL;
    }
  }
  bool readNextCharacter() {
    char c = cin.get();
    // end of formula
    if (c == '.') {
      // flush remaining buffer
      if (tokenBuffer != NULL) {
        tokens.AddLast({Token::VALUE, tokenBuffer});
        tokenBuffer = NULL;
      }
      return false;
    }
    switch (c) {
    case ',': {
      flushBuffer();
      tokens.AddLast({Token::ARGUMENT_SEP});
      return true;
    }
    case ' ': {
      flushBuffer();
      return true;
    }
    case '+': {
      tokens.AddLast({Token::ADD});
      return true;
    }
    case '-': {
      tokens.AddLast({Token::SUBSTRACT});
      return true;
    }
    case '/': {
      tokens.AddLast({Token::DIVIDE});
      return true;
    }
    case '*': {
      tokens.AddLast({Token::MULTIPLY});
      return true;
    }
    case '(': {
      if (tokenBuffer && strlen(tokenBuffer) > 0) {
        Function *function = NULL;
        if (strcmp("MAX", tokenBuffer) == 0)
          function = new MaxFunction();
        else if (strcmp("MIN", tokenBuffer) == 0)
          function = new MinFunction();
        else if (strcmp("IF", tokenBuffer) == 0)
          function = new IfFunction();
        tokens.AddLast({Token::FUNCTION, function});
        delete tokenBuffer;
        tokenBuffer = NULL;
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
    if (tokenBuffer == NULL) {
      tokenBufferSize = 0;
      tokenBuffer = new char[255];
    }
    tokenBuffer[tokenBufferSize++] = c;
    return true;
  }

public:
  const List<TokenValue> &readTokens() {
    while (readNextCharacter())
      ;
    return tokens;
  }
};

int main() {
  int N;
  cin >> N;
  for (int i = 0; i < N; i++) {
    Tokenizer tokenizer;
    auto &tokens = tokenizer.readTokens();
    auto token = tokens.GetFirst();
    while (token != nullptr) {
      cout << token->value << "\n";
      token = token->next;
    }
  }
}
