/**
 * Mini SQL Compiler for Query Validation
 * =======================================
 * File: symbol_table.h
 * Description: Symbol Table Header
 * Team Member: Member 3
 *
 * The Symbol Table is a data structure used by the compiler to store
 * information about identifiers (variables, table names, column names).
 *
 * THEORY:
 * -------
 * In a real database system, the symbol table would contain:
 * - Table definitions (names, schemas)
 * - Column definitions (names, data types, constraints)
 * - Index information
 *
 * For our mini SQL compiler, we simulate a simple schema to demonstrate
 * semantic validation. The symbol table stores:
 * - Known table names
 * - Columns belonging to each table
 *
 * This allows us to validate:
 * - Whether a referenced table exists
 * - Whether columns exist in the specified table
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace MiniSQL {

// Column information
struct ColumnInfo {
  std::string name;
  std::string dataType; // "INT", "VARCHAR", "FLOAT"

  ColumnInfo(const std::string &n, const std::string &t)
      : name(n), dataType(t) {}
};

// Table information
struct TableInfo {
  std::string name;
  std::vector<ColumnInfo> columns;

  TableInfo() = default;
  TableInfo(const std::string &n) : name(n) {}

  void addColumn(const std::string &colName, const std::string &type) {
    columns.push_back(ColumnInfo(colName, type));
  }

  bool hasColumn(const std::string &colName) const {
    for (const auto &col : columns) {
      if (col.name == colName)
        return true;
    }
    return false;
  }
};

class SymbolTable {
private:
  std::unordered_map<std::string, TableInfo> tables;

public:
  /**
   * Constructor - Initialize with sample database schema
   */
  SymbolTable();

  /**
   * Add a table to the symbol table
   */
  void addTable(const TableInfo &table);

  /**
   * Check if a table exists
   */
  bool tableExists(const std::string &tableName) const;

  /**
   * Check if a column exists in a table
   */
  bool columnExists(const std::string &tableName,
                    const std::string &columnName) const;

  /**
   * Get table information
   */
  const TableInfo *getTable(const std::string &tableName) const;

  /**
   * Get all table names
   */
  std::vector<std::string> getTableNames() const;

  /**
   * Print the symbol table (for debugging/demonstration)
   */
  void print() const;
};

} // namespace MiniSQL

#endif // SYMBOL_TABLE_H
