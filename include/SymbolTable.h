#ifndef EXAM_LIB_SYMBOLTABLE_H
#define EXAM_LIB_SYMBOLTABLE_H
#pragma once
#include <map>
#include <string>
#include <stdexcept>
#include <vector>
#include <memory>
#include <map>
#include <iostream>

template<typename Key, typename Value>
class SymbolTable {
public:
    SymbolTable() = default;  // Default constructor
    SymbolTable(const SymbolTable& other);  // Copy constructor
    SymbolTable& operator=(const SymbolTable& other);  // Copy assignment operator
    ~SymbolTable(){}

    void add(const Key& key, const Value& value);
    void update(const Key& key, const Value& value);
    const Value& lookup(const Key& key) const;
    std::map<Key, Value>& get_table();  // Non-const version
    const std::map<Key, Value>& get_table() const;  // Const version

private:
    std::map<Key, Value> table;
};

template<typename Key, typename Value>
SymbolTable<Key, Value>::SymbolTable(const SymbolTable& other) : table(other.table) {}

template<typename Key, typename Value>
SymbolTable<Key, Value>& SymbolTable<Key, Value>::operator=(const SymbolTable& other) {
    if (this != &other) {
        table = other.table;
    }
    return *this;
}

template<typename Key, typename Value>
void SymbolTable<Key, Value>::add(const Key& key, const Value& value) {
    if (table.find(key) != table.end()) {
        throw std::runtime_error("Symbol already exists");
    }
    table[key] = value;
}

template<typename Key, typename Value>
void SymbolTable<Key, Value>::update(const Key& key, const Value& value) {
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

template<typename Key, typename Value>
std::map<Key, Value>& SymbolTable<Key, Value>::get_table() {
    return table;
}

template<typename Key, typename Value>
const std::map<Key, Value>& SymbolTable<Key, Value>::get_table() const {
    return table;
}

#endif //EXAM_LIB_SYMBOLTABLE_H
