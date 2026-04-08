/**
 * Mini SQL Compiler for Query Validation
 * =======================================
 * File: symbol_table.cpp
 * Description: Symbol Table Implementation
 * Team Member: Member 3
 *
 * This implementation creates a simulated database schema for testing
 * semantic validation. In a real system, this information would come
 * from the database catalog/metadata.
 */

#include "../include/symbol_table.h"
#include <iostream>

namespace MiniSQL {

// Constructor - Initialize with sample schema
SymbolTable::SymbolTable() {
  // Create sample tables for demonstration

  // Table: employees
  TableInfo employees("employees");
  employees.addColumn("id", "INT");
  employees.addColumn("name", "VARCHAR");
  employees.addColumn("age", "INT");
  employees.addColumn("salary", "FLOAT");
  employees.addColumn("department", "VARCHAR");
  addTable(employees);

  // Table: departments
  TableInfo departments("departments");
  departments.addColumn("id", "INT");
  departments.addColumn("name", "VARCHAR");
  departments.addColumn("budget", "FLOAT");
  addTable(departments);

  // Table: users
  TableInfo users("users");
  users.addColumn("id", "INT");
  users.addColumn("username", "VARCHAR");
  users.addColumn("email", "VARCHAR");
  users.addColumn("age", "INT");
  users.addColumn("status", "VARCHAR");
  addTable(users);

  // Table: products
  TableInfo products("products");
  products.addColumn("id", "INT");
  products.addColumn("name", "VARCHAR");
  products.addColumn("price", "FLOAT");
  products.addColumn("quantity", "INT");
  addTable(products);
}

void SymbolTable::addTable(const TableInfo &table) {
  tables[table.name] = table;
}

bool SymbolTable::tableExists(const std::string &tableName) const {
  return tables.find(tableName) != tables.end();
}

bool SymbolTable::columnExists(const std::string &tableName,
                               const std::string &columnName) const {
  auto it = tables.find(tableName);
  if (it == tables.end())
    return false;
  return it->second.hasColumn(columnName);
}

const TableInfo *SymbolTable::getTable(const std::string &tableName) const {
  auto it = tables.find(tableName);
  if (it == tables.end())
    return nullptr;
  return &(it->second);
}

std::vector<std::string> SymbolTable::getTableNames() const {
  std::vector<std::string> names;
  for (const auto &pair : tables) {
    names.push_back(pair.first);
  }
  return names;
}

void SymbolTable::print() const {
  std::cout << "\n--- Symbol Table (Database Schema) ---\n";
  std::cout << "+------------------+------------------+----------+\n";
  std::cout << "| Table            | Column           | Type     |\n";
  std::cout << "+------------------+------------------+----------+\n";

  for (const auto &pair : tables) {
    bool firstCol = true;
    for (const auto &col : pair.second.columns) {
      if (firstCol) {
        printf("| %-16s | %-16s | %-8s |\n", pair.first.c_str(),
               col.name.c_str(), col.dataType.c_str());
        firstCol = false;
      } else {
        printf("| %-16s | %-16s | %-8s |\n", "", col.name.c_str(),
               col.dataType.c_str());
      }
    }
    std::cout << "+------------------+------------------+----------+\n";
  }
}

} // namespace MiniSQL
