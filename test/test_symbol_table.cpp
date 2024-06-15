#include <gtest/gtest.h>
#include "SymbolTable.h"

class SymbolTableTest : public ::testing::Test {
protected:
    SymbolTable<std::string, int> table;
};

TEST_F(SymbolTableTest, AddSymbol) {
table.add("test", 1);
EXPECT_EQ(table.lookup("test"), 1);
}

TEST_F(SymbolTableTest, AddDuplicateSymbol) {
table.add("test", 1);
EXPECT_THROW(table.add("test", 2), std::runtime_error);
}

TEST_F(SymbolTableTest, UpdateSymbol) {
table.add("test", 1);
table.update("test", 2);
EXPECT_EQ(table.lookup("test"), 2);
}

TEST_F(SymbolTableTest, LookupNonExistentSymbol) {
EXPECT_THROW(table.lookup("nonexistent"), std::runtime_error);
}

TEST_F(SymbolTableTest, GetTable) {
table.add("test", 1);
auto map = table.get_table();
EXPECT_EQ(map["test"], 1);
}

TEST_F(SymbolTableTest, GetTableConst) {
table.add("test", 1);
const SymbolTable<std::string, int>& constTable = table;
auto map = constTable.get_table();
EXPECT_EQ(map.at("test"), 1);
}
