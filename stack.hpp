//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_STACK_HPP
#define KALKULATOR_STACK_HPP

#include "list.hpp"

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

#endif //KALKULATOR_STACK_HPP
