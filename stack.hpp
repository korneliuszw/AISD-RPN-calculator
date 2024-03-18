//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_STACK_HPP
#define KALKULATOR_STACK_HPP

#include "list.hpp"

template <typename Value>
class Stack
{
private:
    List<Value> list;

public:
    int GetLength() const { return list.GetLength(); }

    Optional<Value> Pop() { return list.PopFirst(); }

    void Push(Value val) { return list.AddFirst(val); }
    ListNode<Value>* Peek() const { return list.GetFirst(); }
};

#endif //KALKULATOR_STACK_HPP
