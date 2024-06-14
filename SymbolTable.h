//
// Created by martinmortensen on 6/14/24.
//

#ifndef EXAM_LIB_SYMBOLTABLE_H
#define EXAM_LIB_SYMBOLTABLE_H
#pragma once
#include <map>
#include <string>
#include <stdexcept>

template<typename Key, typename Value>
class SymbolTable {
public:
    void add(const Key& key, const Value& value);
    const Value& lookup(const Key& key) const;

private:
    std::map<Key, Value> table;
};

template<typename Key, typename Value>
void SymbolTable<Key, Value>::add(const Key& key, const Value& value) {
    if (table.find(key) != table.end()) {
        throw std::runtime_error("Symbol already exists");
    }
    table[key] = value;
}

template<typename Key, typename Value>
const Value& SymbolTable<Key, Value>::lookup(const Key& key) const {
    auto it = table.find(key);
    if (it == table.end()) {
        throw std::runtime_error("Symbol not found");
    }
    return it->second;
}

#endif //EXAM_LIB_SYMBOLTABLE_H
