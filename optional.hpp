//
// Created by wired-mac on 09/03/2024.
//

#ifndef KALKULATOR_OPTIONAL_H
#define KALKULATOR_OPTIONAL_H

#include <memory>
#include <cassert>
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
#endif //KALKULATOR_OPTIONAL_H
