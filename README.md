# Mini SQL Compiler for Query Validation

A Compiler Design academic project that validates SQL queries by implementing core compiler phases.

## Project Overview

This project demonstrates the implementation of a mini compiler that validates SQL SELECT queries without executing them on a real database. It covers all major phases of compilation:

1. **Lexical Analysis** - Tokenizes SQL queries
2. **Syntax Analysis** - Validates grammar using recursive descent parsing
3. **Semantic Analysis** - Validates identifiers against a symbol table
4. **Error Handling** - Generates meaningful error messages

## Team Members

| Member | Responsibility | Files |
|--------|----------------|-------|
| Member 1 | Lexical Analyzer | `lexer.h`, `lexer.cpp` |
| Member 2 | Syntax Analyzer | `parser.h`, `parser.cpp` |
| Member 3 | Semantic Analyzer & Symbol Table | `semantic.h/cpp`, `symbol_table.h/cpp` |
| Member 4 | Error Handler, Testing, Documentation | `error_handler.h/cpp`, `main.cpp` |

## Building the Project

### Prerequisites
- G++ compiler with C++17 support
- Make utility

### Build Commands

```bash
# Build the project
make

# Build and run
make run

# Run demo mode with sample queries
make demo

# Run automated tests
make test

# Clean build artifacts
make clean
```

## Usage

### Interactive Mode
```bash
./sql_compiler
```

Then type SQL queries at the `sql>` prompt:
```sql
sql> SELECT * FROM employees;
sql> SELECT name, age FROM users WHERE age > 25;
sql> exit
```

### Demo Mode
```bash
./sql_compiler --demo
```

### Pipe Mode
```bash
echo "SELECT * FROM employees;" | ./sql_compiler
```

## Supported SQL Syntax

```sql
SELECT column1, column2, ... | *
FROM table_name
[WHERE column operator value]
;
```

### Supported Operators
- `=` (equality)
- `<` (less than)
- `>` (greater than)

### Available Tables (Simulated Schema)

| Table | Columns |
|-------|---------|
| employees | id, name, age, salary, department |
| departments | id, name, budget |
| users | id, username, email, age, status |
| products | id, name, price, quantity |

## Project Structure

```
compiler/
├── include/
│   ├── common.h          # Shared types and structures
│   ├── lexer.h           # Lexer declarations
│   ├── parser.h          # Parser declarations
│   ├── semantic.h        # Semantic analyzer declarations
│   ├── symbol_table.h    # Symbol table declarations
│   └── error_handler.h   # Error handler declarations
├── src/
│   ├── main.cpp          # Main driver program
│   ├── lexer.cpp         # Lexer implementation
│   ├── parser.cpp        # Parser implementation
│   ├── semantic.cpp      # Semantic analyzer implementation
│   ├── symbol_table.cpp  # Symbol table implementation
│   └── error_handler.cpp # Error handler implementation
├── tests/
│   ├── valid_queries.txt
│   └── invalid_queries.txt
├── docs/
│   ├── PROJECT_REPORT.md
│   ├── VIVA_QUESTIONS.md
│   └── SAMPLE_OUTPUTS.md
├── Makefile
└── README.md
```

## Sample Output

### Valid Query
```
sql> SELECT name, age FROM employees WHERE age > 25;

========================================
   PHASE 1: LEXICAL ANALYSIS
========================================
Input Query: SELECT name, age FROM employees WHERE age > 25;

Lexical Analysis: SUCCESS
Total Tokens Generated: 11

========================================
   PHASE 2: SYNTAX ANALYSIS
========================================
Syntax Analysis: SUCCESS
Parse Tree constructed successfully.

========================================
   PHASE 3: SEMANTIC ANALYSIS
========================================
Semantic Analysis: SUCCESS
All identifiers resolved correctly.

╔═══════════════════════════════════════╗
║   QUERY VALIDATION: SUCCESSFUL        ║
╚═══════════════════════════════════════╝
```

## Error Examples

### Syntax Error
```
Input: SELECT * employees;
Error: Syntax Error at Line 1, Column 10: Expected 'FROM' keyword
```

### Semantic Error
```
Input: SELECT * FROM customers;
Error: Semantic Error: Table 'customers' does not exist
```

## License

This is an academic project for the Compiler Design course.
