//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_LIST_HPP
#define KALKULATOR_LIST_HPP

#include <cstdlib>
#include <utility>
#include "optional.hpp"

template<typename Value>
struct ListNode {
    ListNode<Value> *next = nullptr;
    ListNode<Value> *prev = nullptr;
    Value value;

    explicit ListNode<Value>(Value value) : value(value) {}
    explicit ListNode<Value>(Value &&value) : value(value) {}

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
#endif //KALKULATOR_LIST_HPP
