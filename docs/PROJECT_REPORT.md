# Mini SQL Compiler for Query Validation & Execution
## Compiler Design Project Report

---

## 1. Project Overview

### 1.1 Introduction

This project implements a **Mini SQL Compiler** that validates and executes SQL queries by simulating the core phases of compiler design. The compiler checks queries for lexical, syntactic, and semantic correctness, then **executes them against an in-memory data store** with pre-loaded sample data.

### 1.2 Objectives

1. Implement a **Lexical Analyzer** to tokenize SQL queries
2. Develop a **Syntax Analyzer** using recursive descent parsing
3. Create a **Semantic Analyzer** with symbol table management
4. Build a **Query Execution Engine** with in-memory data storage
5. Design comprehensive **Error Handling** with meaningful messages
6. Generate **Parse Tree** as intermediate representation
7. Support **DML Statements**: INSERT, UPDATE, DELETE alongside SELECT

### 1.2 Objectives

1. Implement a **Lexical Analyzer** to tokenize SQL queries
2. Develop a **Syntax Analyzer** using recursive descent parsing
3. Create a **Semantic Analyzer** with symbol table management
4. Design comprehensive **Error Handling** with meaningful messages
5. Generate **Parse Tree** as intermediate representation

### 1.3 Team Structure

| Member | Role | Deliverables |
|--------|------|--------------|
| Member 1 | Lexical Analysis | Token definitions, Tokenizer |
| Member 2 | Syntax Analysis | Grammar design, Parser |
| Member 3 | Semantic Analysis | Symbol table, Validation |
| Member 4 | Executor, Error Handler | Execution engine, Data Store, Testing, Docs |

---

## 2. Compiler Phases

### 2.1 Lexical Analysis (Member 1)

**Theory:**
Lexical analysis is the first phase of compilation. It reads the source code character by character and groups them into **tokens** - the smallest meaningful units.

**Token Categories:**
| Category | Examples | Pattern |
|----------|----------|---------|
| Keywords | SELECT, FROM, WHERE, INSERT, UPDATE, DELETE, etc. | Reserved words (13 total) |
| Identifiers | employee, name, age | [a-zA-Z_][a-zA-Z0-9_]* |
| Numbers | 25, 100.5 | [0-9]+(.[0-9]+)? |
| String Literals | 'Rahul', 'Sales' | '...' |
| Operators | =, !=, <, <=, >, >=, *, ,, (, ) | Single/compound characters |

**Implementation:**
```cpp
class Lexer {
    std::string source;
    std::vector<Token> tokens;
    
    void scanToken();      // Main scanning logic
    void scanIdentifier(); // Handle keywords/identifiers
    void scanNumber();     // Handle numeric literals
    void scanString();     // Handle string literals
};
```

### 2.2 Syntax Analysis (Member 2)

**Theory:**
Syntax analysis verifies that the token sequence follows the grammatical rules of SQL. We use **Recursive Descent Parsing**, a top-down technique where each grammar rule has a corresponding function.

**SQL Grammar (BNF):**
```
<query>          ::= <select_query> | <insert_query> | <update_query> | <delete_query>
<select_query>   ::= SELECT <select_list> FROM <table_name> [<where_clause>] ;
<insert_query>   ::= INSERT INTO <table_name> ( <column_list> ) VALUES ( <value_list> ) ;
<update_query>   ::= UPDATE <table_name> SET <assignment> [<where_clause>] ;
<delete_query>   ::= DELETE FROM <table_name> [<where_clause>] ;
<select_list>    ::= * | <column_name> { , <column_name> }
<column_list>    ::= <column_name> { , <column_name> }
<value_list>     ::= <value> { , <value> }
<assignment>     ::= <column_name> = <value>
<where_clause>   ::= WHERE <condition> { (AND|OR) <condition> }
<condition>      ::= <column_name> <rel_op> <value>
<rel_op>         ::= = | != | < | <= | > | >=
<value>          ::= NUMBER | STRING_LITERAL | IDENTIFIER
```

> See `docs/GRAMMAR.md` for the full grammar specification with FIRST sets and parse tree examples.

**Parse Tree Construction:**
The parser builds a hierarchical tree structure representing the query's syntactic structure.

### 2.3 Semantic Analysis (Member 3)

**Theory:**
Semantic analysis checks the **meaning** of the program - aspects that cannot be expressed in a context-free grammar.

**Validations Performed:**
1. **Table Existence:** Does the referenced table exist?
2. **Column Existence:** Do columns belong to the specified table?
3. **Type Compatibility:** Are comparisons type-safe?

**Symbol Table:**
```cpp
struct TableInfo {
    std::string name;
    std::vector<ColumnInfo> columns;
};

class SymbolTable {
    std::unordered_map<std::string, TableInfo> tables;
    
    bool tableExists(const std::string& name);
    bool columnExists(const std::string& table, const std::string& column);
};
```

### 2.4 Query Execution (Member 4)

**Theory:**
The execution phase bridges the gap between validation and actual results. After semantic validation passes, the executor traverses the parse tree and performs the corresponding data operation.

**Execution Engine:**
```cpp
class Executor {
    DataStore& dataStore;
    
    QueryResult executeSelect(const ParseTree& tree);
    QueryResult executeInsert(const ParseTree& tree);
    QueryResult executeUpdate(const ParseTree& tree);
    QueryResult executeDelete(const ParseTree& tree);
};
```

**Data Store:**
```cpp
class DataStore {
    std::map<std::string, TableData> tables;
    
    bool insertRow(table, columns, values);
    std::vector<Row> getFilteredRows(table, column, op, value);
    int updateRows(table, setCol, setVal, whereCol, whereOp, whereVal);
    int deleteRows(table, whereCol, whereOp, whereVal);
    void saveToFiles(directory);  // CSV persistence
    void loadFromFiles(directory);
};
```

### 2.5 Error Handling (Member 4)

**Error Types:**
| Type | Phase | Example |
|------|-------|---------|
| Lexical Error | Lexer | Invalid character '@' |
| Syntax Error | Parser | Missing 'FROM' keyword |
| Semantic Error | Semantic | Table 'customers' doesn't exist |

**Error Message Format:**
```
[Error Type] at Line X, Column Y: Description
  1 | SELECT @ FROM users;
    |        ^~~~
```

---

## 3. Implementation Details

### 3.1 Project Structure

```
compiler/
├── include/           # Header files
│   ├── common.h       # Shared types (TokenType, NodeType)
│   ├── lexer.h
│   ├── parser.h
│   ├── semantic.h
│   ├── symbol_table.h
│   ├── error_handler.h
│   ├── data_store.h   # [NEW] In-memory data storage
│   └── executor.h     # [NEW] Query execution engine
├── src/               # Source files
│   ├── main.cpp
│   ├── lexer.cpp
│   ├── parser.cpp
│   ├── semantic.cpp
│   ├── symbol_table.cpp
│   ├── error_handler.cpp
│   ├── data_store.cpp # [NEW] Data store implementation
│   └── executor.cpp   # [NEW] Executor implementation
├── tests/             # Test queries
├── docs/              # Documentation
│   ├── GRAMMAR.md     # [NEW] Full grammar specification
│   ├── FLOWCHART.md
│   ├── PROJECT_REPORT.md
│   ├── SAMPLE_OUTPUTS.md
│   ├── TEAM_RESPONSIBILITIES.md
│   └── VIVA_QUESTIONS.md
└── Makefile
```

### 3.2 Data Flow

```
                     ┌─────────────────────────────────────────┐
                     │             SQL Query                   │
                     │   "SELECT * FROM employees;"            │
                     └─────────────────┬───────────────────────┘
                                       │
                                       ▼
┌──────────────────────────────────────────────────────────────────────┐
│  PHASE 1: LEXICAL ANALYSIS                                          │
│  ─────────────────────────                                          │
│  Input: Character stream                                            │
│  Output: Token stream                                                │
│  [KEYWORD_SELECT, OP_STAR, KEYWORD_FROM, IDENTIFIER, OP_SEMICOLON]  │
└────────────────────────────────────┬─────────────────────────────────┘
                                     │
                                     ▼
┌──────────────────────────────────────────────────────────────────────┐
│  PHASE 2: SYNTAX ANALYSIS                                           │
│  ────────────────────────                                           │
│  Input: Token stream                                                 │
│  Output: Parse Tree                                                  │
│  QUERY → SELECT_CLAUSE → COLUMN_LIST → FROM_CLAUSE → TABLE_NAME     │
└────────────────────────────────────┬─────────────────────────────────┘
                                     │
                                     ▼
┌──────────────────────────────────────────────────────────────────────┐
│  PHASE 3: SEMANTIC ANALYSIS                                         │
│  ──────────────────────────                                         │
│  Input: Parse Tree + Symbol Table                                    │
│  Output: Validation Result                                           │
│  Checks: Table exists? Columns valid? Types match?                   │
└────────────────────────────────────┬─────────────────────────────────┘
                                     │
                                     ▼
┌──────────────────────────────────────────────────────────────────────┐
│  PHASE 4: QUERY EXECUTION                                            │
│  ────────────────────────                                            │
│  Input: Validated Parse Tree + DataStore                              │
│  Output: Query results or affected row count                          │
│  SELECT → Tabular results | INSERT/UPDATE/DELETE → Affected rows      │
└──────────────────────────────────────────────────────────────────────┘
```

---

## 4. Sample Tables (Schema)

| Table | Columns |
|-------|---------|
| employees | id (INT), name (VARCHAR), age (INT), salary (FLOAT), department (VARCHAR) |
| departments | id (INT), name (VARCHAR), budget (FLOAT) |
| users | id (INT), username (VARCHAR), email (VARCHAR), age (INT), status (VARCHAR) |
| products | id (INT), name (VARCHAR), price (FLOAT), quantity (INT) |

---

## 5. Sample Queries and Outputs

### 5.1 Valid Query Example

**Input:**
```sql
SELECT name, age FROM employees WHERE age > 25;
```

**Token Stream:**
| Token Type | Value |
|------------|-------|
| KEYWORD_SELECT | SELECT |
| IDENTIFIER | name |
| OP_COMMA | , |
| IDENTIFIER | age |
| KEYWORD_FROM | FROM |
| IDENTIFIER | employees |
| KEYWORD_WHERE | WHERE |
| IDENTIFIER | age |
| OP_GREATER_THAN | > |
| NUMBER | 25 |
| OP_SEMICOLON | ; |

**Parse Tree:**
```
QUERY
├── SELECT_CLAUSE: "SELECT"
│   └── COLUMN_LIST
│       ├── COLUMN: "name"
│       └── COLUMN: "age"
├── FROM_CLAUSE: "FROM"
│   └── TABLE_NAME: "employees"
└── WHERE_CLAUSE: "WHERE"
    └── CONDITION
        ├── COLUMN: "age"
        ├── OPERATOR: ">"
        └── VALUE: "25"
```

**Result:** Query is VALID ✓

### 5.2 Invalid Query Example (Syntax Error)

**Input:**
```sql
SELECT * employees;
```

**Error Output:**
```
Syntax Error at Line 1, Column 10: Expected 'FROM' keyword (found 'employees')
  1 | SELECT * employees;
    |          ^~~~
```

---

## 6. New Features (v2.0 Upgrade)

| Feature | Description |
|---------|-------------|
| **Query Execution** | Queries now return actual results instead of just validation |
| **INSERT Support** | `INSERT INTO table (cols) VALUES (vals);` |
| **UPDATE Support** | `UPDATE table SET col = val WHERE condition;` |
| **DELETE Support** | `DELETE FROM table [WHERE condition];` |
| **In-Memory Data** | 4 tables pre-loaded with sample data (22 rows total) |
| **CSV Persistence** | Save/load data to CSV files with `save`/`load` commands |
| **Batch Mode** | Execute queries from a file with `--file` flag |
| **New Operators** | `!=`, `<=`, `>=` in WHERE conditions |

---

## 7. Future Scope

1. **JOIN Operations:** Add support for table joins (INNER, LEFT, RIGHT)
2. **Aggregate Functions:** Implement COUNT, SUM, AVG, MIN, MAX
3. **Subqueries:** Support nested SELECT statements
4. **ORDER BY / GROUP BY:** Sorting and grouping results
5. **CREATE TABLE:** Dynamic schema creation
6. **GUI Interface:** Web-based query interface

---

## 8. Conclusion

This project demonstrates the implementation of a complete SQL processing pipeline — from tokenization to query execution. The modular design across 4 compilation phases allows easy extension and maintenance. The project provides hands-on experience with:

- Lexical analysis and tokenization
- Grammar design and recursive descent parsing
- Symbol table management and semantic validation
- Query execution against in-memory storage
- Compiler error handling and reporting

The Mini SQL Compiler serves as an educational tool for understanding how real database engines validate and execute queries.

---

## 9. References

1. Aho, A.V., Sethi, R., & Ullman, J.D. (2006). *Compilers: Principles, Techniques, and Tools* (2nd ed.)
2. Cooper, K.D., & Torczon, L. (2011). *Engineering a Compiler* (2nd ed.)
3. SQL Language Reference - Oracle Documentation
4. C++ ISO Standard (C++17)
