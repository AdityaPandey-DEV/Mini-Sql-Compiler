# Mini SQL Compiler — Complete Grammar Specification

## 1. Overview

This document defines the **formal grammar** of the Mini SQL Compiler. The compiler supports 4 SQL statement types — **SELECT**, **INSERT**, **UPDATE**, and **DELETE** — parsed using a **Recursive Descent Parser** (top-down, LL(1)).

---

## 2. Formal Grammar (BNF Notation)

BNF = Backus-Naur Form — standard way to define programming language grammar.

- `::=` means "is defined as"
- `|` means "or" (alternative)
- `[ ]` means optional (0 or 1 times)
- `{ }` means repetition (0 or more times)
- **Bold** = terminal symbols (actual tokens)
- `<angle_brackets>` = non-terminal symbols (grammar rules)

```bnf
<query>          ::= <select_query>
                   | <insert_query>
                   | <update_query>
                   | <delete_query>

<select_query>   ::= SELECT <select_list> FROM <table_name> [ <where_clause> ] ;

<insert_query>   ::= INSERT INTO <table_name> ( <column_list> ) VALUES ( <value_list> ) ;

<update_query>   ::= UPDATE <table_name> SET <assignment> [ <where_clause> ] ;

<delete_query>   ::= DELETE FROM <table_name> [ <where_clause> ] ;

<select_list>    ::= *
                   | <column_name> { , <column_name> }

<column_list>    ::= <column_name> { , <column_name> }

<value_list>     ::= <value> { , <value> }

<assignment>     ::= <column_name> = <value>

<where_clause>   ::= WHERE <condition> { ( AND | OR ) <condition> }

<condition>      ::= <column_name> <rel_operator> <value>

<rel_operator>   ::= =  |  !=  |  <  |  <=  |  >  |  >=

<value>          ::= NUMBER | STRING_LITERAL | IDENTIFIER

<table_name>     ::= IDENTIFIER
<column_name>    ::= IDENTIFIER
```

---

## 3. Lexical Grammar (Token Definitions)

The **Lexer** (Phase 1) converts raw characters into tokens. Here are all token types:

### 3.1 Keywords (13 total)

| Token Type | Keyword | Purpose |
|---|---|---|
| `KEYWORD_SELECT` | `SELECT` | Start of SELECT query |
| `KEYWORD_FROM` | `FROM` | Specify source table |
| `KEYWORD_WHERE` | `WHERE` | Start filter condition |
| `KEYWORD_AND` | `AND` | Logical AND in conditions |
| `KEYWORD_OR` | `OR` | Logical OR in conditions |
| `KEYWORD_INSERT` | `INSERT` | Start of INSERT query |
| `KEYWORD_INTO` | `INTO` | Used with INSERT |
| `KEYWORD_VALUES` | `VALUES` | Start of value list |
| `KEYWORD_UPDATE` | `UPDATE` | Start of UPDATE query |
| `KEYWORD_SET` | `SET` | Assignment in UPDATE |
| `KEYWORD_DELETE` | `DELETE` | Start of DELETE query |
| `KEYWORD_CREATE` | `CREATE` | Reserved for future use |
| `KEYWORD_TABLE` | `TABLE` | Reserved for future use |

> **Note:** Keywords are **case-insensitive** — `select`, `SELECT`, `Select` are all valid.

### 3.2 Operators (11 total)

| Token Type | Symbol | Purpose |
|---|---|---|
| `OP_EQUALS` | `=` | Equality comparison |
| `OP_NOT_EQUALS` | `!=` | Not-equal comparison |
| `OP_LESS_THAN` | `<` | Less-than comparison |
| `OP_LESS_EQUALS` | `<=` | Less-than-or-equal |
| `OP_GREATER_THAN` | `>` | Greater-than comparison |
| `OP_GREATER_EQUALS` | `>=` | Greater-than-or-equal |
| `OP_COMMA` | `,` | Separator in lists |
| `OP_STAR` | `*` | Select all columns |
| `OP_SEMICOLON` | `;` | Query terminator |
| `OP_LPAREN` | `(` | Open parenthesis |
| `OP_RPAREN` | `)` | Close parenthesis |

### 3.3 Literals & Identifiers

| Token Type | Pattern | Example |
|---|---|---|
| `IDENTIFIER` | `[a-zA-Z_][a-zA-Z0-9_]*` | `employees`, `salary`, `user_name` |
| `NUMBER` | `[0-9]+(\.[0-9]+)?` | `25`, `70000`, `99.99` |
| `STRING_LITERAL` | `'...'` (single quotes) | `'Rahul Sharma'`, `'Engineering'` |

### 3.4 Special Tokens

| Token Type | Purpose |
|---|---|
| `END_OF_INPUT` | Marks end of query |
| `UNKNOWN` | Invalid/unrecognized character |

---

## 4. Parse Tree Node Types

The **Parser** (Phase 2) builds a parse tree. Each node has one of these types:

| NodeType | Used In | Description |
|---|---|---|
| `QUERY` | SELECT | Root node for SELECT queries |
| `INSERT_QUERY` | INSERT | Root node for INSERT queries |
| `UPDATE_QUERY` | UPDATE | Root node for UPDATE queries |
| `DELETE_QUERY` | DELETE | Root node for DELETE queries |
| `SELECT_CLAUSE` | SELECT | Contains column selection |
| `COLUMN_LIST` | SELECT, INSERT | List of column names |
| `COLUMN` | All | Single column name |
| `FROM_CLAUSE` | SELECT, DELETE | Contains table reference |
| `TABLE_NAME` | All | Single table name |
| `WHERE_CLAUSE` | SELECT, UPDATE, DELETE | Contains filter conditions |
| `CONDITION` | WHERE | Single comparison condition |
| `OPERATOR` | WHERE | Comparison operator |
| `VALUE` | INSERT, UPDATE, WHERE | Literal value (number/string) |
| `VALUE_LIST` | INSERT | List of values for INSERT |
| `SET_CLAUSE` | UPDATE | Contains column assignment |
| `ASSIGNMENT` | UPDATE | Column = Value pair |

---

## 5. Grammar Rule → Parser Function Mapping

Each grammar rule maps directly to a C++ function in `parser.cpp`:

```
Grammar Rule            →  Parser Function        →  File Location
────────────────────────────────────────────────────────────────────
<query>                 →  parseQuery()            →  parser.cpp:64
  dispatches to:
    SELECT              →  parseQuery() (default)
    INSERT              →  parseInsert()           →  parser.cpp:312
    UPDATE              →  parseUpdate()           →  parser.cpp:393
    DELETE              →  parseDelete()           →  parser.cpp:455

<select_list>           →  parseColumnList()       →  parser.cpp:128
<column_list>           →  parseColumnList()       →  parser.cpp:128
<from_clause>           →  parseFromClause()       →  parser.cpp:158
<where_clause>          →  parseWhereClause()      →  parser.cpp:186
<condition>             →  parseCondition()        →  parser.cpp:208
<value_list>            →  parseValueList()        →  parser.cpp:487
```

---

## 6. Example Parse Trees

### 6.1 SELECT Query

```sql
SELECT name, salary FROM employees WHERE salary > 70000;
```

```
QUERY
├── SELECT_CLAUSE: "SELECT"
│   └── COLUMN_LIST
│       ├── COLUMN: "name"
│       └── COLUMN: "salary"
├── FROM_CLAUSE: "FROM"
│   └── TABLE_NAME: "employees"
└── WHERE_CLAUSE: "WHERE"
    └── CONDITION
        ├── COLUMN: "salary"
        ├── OPERATOR: ">"
        └── VALUE: "70000"
```

### 6.2 INSERT Query

```sql
INSERT INTO employees (id, name, age) VALUES (10, 'Kavita', 31);
```

```
INSERT_QUERY
├── TABLE_NAME: "employees"
├── COLUMN_LIST
│   ├── COLUMN: "id"
│   ├── COLUMN: "name"
│   └── COLUMN: "age"
└── VALUE_LIST
    ├── VALUE: "10"
    ├── VALUE: "Kavita"
    └── VALUE: "31"
```

### 6.3 UPDATE Query

```sql
UPDATE employees SET salary = 100000 WHERE id = 3;
```

```
UPDATE_QUERY
├── TABLE_NAME: "employees"
├── SET_CLAUSE: "SET"
│   └── ASSIGNMENT
│       ├── COLUMN: "salary"
│       └── VALUE: "100000"
└── WHERE_CLAUSE: "WHERE"
    └── CONDITION
        ├── COLUMN: "id"
        ├── OPERATOR: "="
        └── VALUE: "3"
```

### 6.4 DELETE Query

```sql
DELETE FROM employees WHERE id = 1;
```

```
DELETE_QUERY
├── FROM_CLAUSE: "FROM"
│   └── TABLE_NAME: "employees"
└── WHERE_CLAUSE: "WHERE"
    └── CONDITION
        ├── COLUMN: "id"
        ├── OPERATOR: "="
        └── VALUE: "1"
```

---

## 7. FIRST Sets (LL(1) Parsing)

The parser uses **one-token lookahead** (LL(1)). Here are the FIRST sets:

| Rule | FIRST Set | How Parser Decides |
|---|---|---|
| `<select_query>` | { `SELECT` } | First token is SELECT |
| `<insert_query>` | { `INSERT` } | First token is INSERT |
| `<update_query>` | { `UPDATE` } | First token is UPDATE |
| `<delete_query>` | { `DELETE` } | First token is DELETE |
| `<select_list>` | { `*`, `IDENTIFIER` } | `*` = all, else column list |
| `<where_clause>` | { `WHERE` } | Optional — present only if WHERE found |
| `<condition>` | { `IDENTIFIER` } | Column name starts condition |
| `<value>` | { `NUMBER`, `STRING_LITERAL`, `IDENTIFIER` } | Any literal type |

Since all FIRST sets are **disjoint** (no overlap), the grammar is unambiguous and LL(1)-parsable.

---

## 8. Semantic Rules

After parsing, the **Semantic Analyzer** (Phase 3) checks:

| Rule | Check | Error Example |
|---|---|---|
| Table exists | `symbolTable.tableExists(name)` | `SELECT * FROM customers;` → Table 'customers' does not exist |
| Column exists | `symbolTable.columnExists(table, col)` | `SELECT xyz FROM employees;` → Column 'xyz' does not exist |
| INSERT col/val count match | `columns.size() == values.size()` | Column count (3) ≠ value count (2) |
| UPDATE column exists | Column in SET must exist in table | `SET xyz = 5` → Column 'xyz' does not exist |
| WHERE column exists | Column in condition must exist | `WHERE xyz = 5` → Column 'xyz' does not exist |

---

## 9. Execution Rules

After semantic validation, the **Executor** (Phase 4) runs the query:

| Query Type | Execution | Output |
|---|---|---|
| `SELECT` | Fetch rows, apply WHERE filter, project columns | Result table with rows |
| `INSERT` | Add new row to table | "1 row inserted successfully" |
| `UPDATE` | Find matching rows, update column value | "N row(s) updated successfully" |
| `DELETE` | Find matching rows, remove them | "N row(s) deleted successfully" |

---

## 10. Available Tables & Schema

| Table | Columns | Sample Rows |
|---|---|---|
| `employees` | id (INT), name (VARCHAR), age (INT), salary (FLOAT), department (VARCHAR) | 8 rows |
| `departments` | id (INT), name (VARCHAR), budget (FLOAT) | 4 rows |
| `users` | id (INT), username (VARCHAR), email (VARCHAR), age (INT), status (VARCHAR) | 5 rows |
| `products` | id (INT), name (VARCHAR), price (FLOAT), quantity (INT) | 5 rows |

---

## 11. Compilation Pipeline Flowchart

```
Input Query (string)
      │
      ▼
┌─────────────────┐
│  PHASE 1: LEXER │  → Tokenize characters into Token stream
│  (lexer.cpp)    │  → Reports lexical errors (unknown chars)
└────────┬────────┘
         │ Token Stream
         ▼
┌──────────────────┐
│  PHASE 2: PARSER │  → Validate grammar using recursive descent
│  (parser.cpp)    │  → Build Parse Tree (AST)
└────────┬─────────┘  → Reports syntax errors
         │ Parse Tree
         ▼
┌────────────────────┐
│  PHASE 3: SEMANTIC │  → Validate table/column existence
│  (semantic.cpp)    │  → Check type compatibility
└────────┬───────────┘  → Reports semantic errors
         │ Validated Tree
         ▼
┌─────────────────────┐
│  PHASE 4: EXECUTOR  │  → Execute query against DataStore
│  (executor.cpp)     │  → Return results / affected rows
└─────────────────────┘
```
