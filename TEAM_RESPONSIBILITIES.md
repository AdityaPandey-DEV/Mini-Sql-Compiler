# Team Responsibilities - Difficulty-Based Division

## Mini SQL Compiler Project

This document assigns work to 4 team members based on **difficulty level** - from easiest to hardest.

---

## 📊 Difficulty Overview

| Member | Role | Difficulty | Work Hours Est. |
|--------|------|------------|-----------------|
| **Member 4** | Error Handler & Testing | ⭐ Easy | 8-10 hrs |
| **Member 1** | Lexical Analyzer | ⭐⭐ Medium | 12-15 hrs |
| **Member 3** | Semantic Analyzer | ⭐⭐⭐ Medium-Hard | 15-18 hrs |
| **Member 2** | Syntax Analyzer | ⭐⭐⭐⭐ Hard | 18-22 hrs |

---

## Member 4: Error Handler & Testing (⭐ EASY)

### Why Easy?
- Straightforward string manipulation
- No complex algorithms
- Collects and formats errors from other modules
- Testing involves running predefined queries

### Files to Work On
- `include/error_handler.h`
- `src/error_handler.cpp`
- `src/main.cpp` (driver program)
- `tests/valid_queries.txt`
- `tests/invalid_queries.txt`
- All documentation files

### Key Concepts to Understand
1. Error structure (type, message, line, column)
2. String formatting for console output
3. Basic C++ classes and vectors
4. How to integrate with other modules

### Main Tasks
```
[ ] Create ErrorHandler class
[ ] Implement addError() and addErrors() methods
[ ] Format error messages with source line display
[ ] Create compilation summary with ASCII art
[ ] Write main.cpp driver integrating all phases
[ ] Create test query files
[ ] Write documentation (README, sample outputs)
```

### Code Complexity: LOW
- Simple data storage and retrieval
- Printf/cout formatting
- No parsing or complex logic

---

## Member 1: Lexical Analyzer (⭐⭐ MEDIUM)

### Why Medium?
- Character-by-character processing (systematic)
- Pattern recognition using switch-case
- Keyword table lookup (simple hash map)
- Well-defined token categories

### Files to Work On
- `include/common.h` (Token types)
- `include/lexer.h`
- `src/lexer.cpp`

### Key Concepts to Understand
1. **Tokens** - Smallest meaningful units
2. **Lexemes** - Actual character sequences
3. **Finite Automata** - State-based pattern matching
4. **Keyword Table** - Reserved word lookup

### Main Tasks
```
[ ] Define TokenType enum (SELECT, FROM, IDENTIFIER, etc.)
[ ] Define Token structure with position info
[ ] Implement character scanning (advance, peek)
[ ] Implement scanToken() switch-case logic
[ ] Implement scanIdentifier() for keywords/identifiers
[ ] Implement scanNumber() for numeric literals
[ ] Implement scanString() for string literals
[ ] Create tokenize() main method
[ ] Print token stream in tabular format
```

### Code Complexity: MEDIUM
- Sequential character processing
- State tracking (current position, line, column)
- Pattern matching with simple rules

---

## Member 3: Semantic Analyzer (⭐⭐⭐ MEDIUM-HARD)

### Why Medium-Hard?
- Requires understanding parse tree structure
- Symbol table design and lookup
- Multiple validation checks
- Must understand context (which table, which columns)

### Files to Work On
- `include/symbol_table.h`
- `src/symbol_table.cpp`
- `include/semantic.h`
- `src/semantic.cpp`

### Key Concepts to Understand
1. **Symbol Table** - Database of identifiers
2. **Name Resolution** - Finding if identifiers exist
3. **Type Checking** - Validating compatible operations
4. **Attribute Grammar** - Passing info through parse tree

### Main Tasks
```
[ ] Design TableInfo and ColumnInfo structures
[ ] Create SymbolTable class with lookup methods
[ ] Initialize sample database schema
[ ] Implement tableExists() and columnExists()
[ ] Create SemanticAnalyzer class
[ ] Validate FROM clause first (establish context)
[ ] Validate SELECT columns against table schema
[ ] Validate WHERE clause column and condition
[ ] Handle "*" (select all) case
[ ] Generate meaningful semantic error messages
```

### Code Complexity: MEDIUM-HIGH
- Tree traversal logic
- Context management (current table)
- Multiple validation rules
- Coordination with parser output

---

## Member 2: Syntax Analyzer (⭐⭐⭐⭐ HARD)

### Why Hard?
- Must understand Context-Free Grammar (CFG)
- Recursive descent implementation
- Parse tree construction
- Error recovery handling
- Most theoretical knowledge required

### Files to Work On
- `include/common.h` (Parse tree nodes)
- `include/parser.h`
- `src/parser.cpp`

### Key Concepts to Understand
1. **Context-Free Grammar (CFG)** - Production rules
2. **Recursive Descent Parsing** - Top-down technique
3. **Parse Tree / AST** - Tree representation of syntax
4. **Lookahead** - Predicting which rule to apply
5. **FIRST and FOLLOW Sets** - Grammar analysis

### SQL Grammar to Implement
```
<query>        ::= SELECT <column_list> FROM <table_name> [<where_clause>] ;
<column_list>  ::= * | <column_name> { , <column_name> }*
<column_name>  ::= IDENTIFIER
<table_name>   ::= IDENTIFIER
<where_clause> ::= WHERE <condition>
<condition>    ::= <column_name> <rel_op> <value>
<rel_op>       ::= = | < | >
<value>        ::= IDENTIFIER | NUMBER | STRING_LITERAL
```

### Main Tasks
```
[ ] Understand and design the SQL grammar
[ ] Define ParseTreeNode structure
[ ] Implement token navigation (peek, advance, match, check)
[ ] Implement parseQuery() - top-level rule
[ ] Implement parseSelectClause() - SELECT keyword
[ ] Implement parseColumnList() - columns or *
[ ] Implement parseFromClause() - FROM + table
[ ] Implement parseWhereClause() - optional WHERE
[ ] Implement parseCondition() - column op value
[ ] Build parse tree during parsing
[ ] Implement error recovery (synchronize)
[ ] Implement printParseTree() for visualization
```

### Code Complexity: HIGH
- Recursive function calls matching grammar
- Tree construction during parsing
- Must handle all grammar cases
- Error recovery is tricky

---

## 📈 Learning Curve by Member

```
Member 4 (Easy)      ████░░░░░░░░░░░░░░░░ 20%
Member 1 (Medium)    ████████░░░░░░░░░░░░ 40%
Member 3 (Med-Hard)  ████████████░░░░░░░░ 60%
Member 2 (Hard)      ████████████████████ 100%
```

---

## 🔗 Dependency Order

Work should proceed in this order:

```
1. Member 1 (Lexer)     ─┬─→ Can start immediately
2. Member 4 (Errors)    ─┤   Can start in parallel
                         │
3. Member 2 (Parser)    ←┘   Needs tokens from Lexer
                         │
4. Member 3 (Semantic)  ←────Needs parse tree from Parser
```

### Integration Timeline
| Week | Member 1 | Member 2 | Member 3 | Member 4 |
|------|----------|----------|----------|----------|
| 1 | Lexer done | Grammar study | Symbol table | Error class |
| 2 | Testing | Parser impl | Wait | Main driver |
| 3 | Integration | Parse tree | Semantic impl | Testing |
| 4 | All: Final testing and documentation |

---

## 🎯 Viva Focus Areas by Member

**Member 4:** Error types, error recovery, testing methodologies
**Member 1:** Tokens, lexemes, finite automata, keyword recognition
**Member 3:** Symbol table, name resolution, type checking
**Member 2:** CFG, recursive descent, parse trees, grammar rules

---

## Summary

| Difficulty | Member | Key Skill Required |
|------------|--------|-------------------|
| ⭐ Easy | Member 4 | Basic C++, string handling |
| ⭐⭐ Medium | Member 1 | Pattern matching, state machines |
| ⭐⭐⭐ Med-Hard | Member 3 | Data structures, tree traversal |
| ⭐⭐⭐⭐ Hard | Member 2 | Grammar theory, recursion |

---

# 🔧 Compiler Techniques, Approach & Algorithms (Simple Hinglish Explanation)

---

## 🎯 Overall Project Approach

**Humne kya kiya?**
Ek Mini SQL Compiler banaya jo SQL queries ko validate karta hai - bina actually database pe run kiye!

**Compiler ke 4 Phases implement kiye:**
```
SQL Query → Lexer → Parser → Semantic → Output
            (Phase 1) (Phase 2) (Phase 3)  (Phase 4)
```

---

## Member 1: Lexical Analyzer - Techniques & Algorithms

### 🔹 Technique Used: SCANNING / TOKENIZATION

**Simple Explanation:**
> Jaise hum sentence ko words mein todte hain, waise hi Lexer SQL query ko tokens mein todta hai.

**Example:**
```
Input:  "SELECT name FROM users;"
Output: [SELECT] [name] [FROM] [users] [;]
         ↓         ↓      ↓       ↓      ↓
      Keyword  Identifier Keyword Identifier Symbol
```

### 🔹 Algorithm: FINITE STATE MACHINE (FSM)

**Kya hai ye?**
> Ek state machine jo character-by-character padhti hai aur decide karti hai ki ye konsa token hai.

**States:**
```
START → (letter milaa) → IDENTIFIER_STATE → (letter/digit) → continue
                                          → (kuch aur)    → TOKEN COMPLETE!

START → (digit milaa) → NUMBER_STATE → (digit) → continue
                                     → (kuch aur) → TOKEN COMPLETE!

START → ('SELECT' milaa) → Check keyword table → KEYWORD token bana do
```

**Code Approach:**
```cpp
void scanToken() {
    char c = advance();  // Ek character padho
    
    switch(c) {
        case '*': addToken(OP_STAR); break;     // Star operator
        case ',': addToken(OP_COMMA); break;    // Comma
        case ';': addToken(OP_SEMICOLON); break; // Semicolon
        
        default:
            if (isAlpha(c)) scanIdentifier();   // Letter hai? Identifier scan karo
            else if (isDigit(c)) scanNumber();  // Digit hai? Number scan karo
            else error("Invalid character!");   // Kuch aur? Error!
    }
}
```

### 🔹 Data Structure: HASH TABLE (for Keywords)

**Kyun use kiya?**
> Keywords ko O(1) time mein lookup karne ke liye hash table use ki.

```cpp
keywords["SELECT"] = KEYWORD_SELECT;
keywords["FROM"]   = KEYWORD_FROM;
keywords["WHERE"]  = KEYWORD_WHERE;
```

**Kaam kaise karta hai?**
```
1. Identifier scan karo: "select"
2. Uppercase mein convert karo: "SELECT"  
3. Hash table mein check karo: keywords.find("SELECT")
4. Mila? → Return KEYWORD_SELECT
   Nahi mila? → Return IDENTIFIER
```

---

## Member 2: Syntax Analyzer - Techniques & Algorithms

### 🔹 Technique Used: RECURSIVE DESCENT PARSING

**Simple Explanation:**
> Har grammar rule ke liye ek function banao. Functions ek dusre ko call karte hain (recursion).

**Grammar humari:**
```
query        → SELECT column_list FROM table_name [WHERE condition] ;
column_list  → * | column_name (, column_name)*
condition    → column_name operator value
```

**Har rule ka function:**
```cpp
ParseTree parseQuery() {
    parseSelectClause();    // SELECT handle karo
    parseFromClause();      // FROM handle karo
    if (check(WHERE)) 
        parseWhereClause(); // Optional WHERE
    consume(SEMICOLON);     // ; expect karo
}
```

### 🔹 Algorithm: TOP-DOWN PARSING

**Kya hai ye?**
> Start symbol se shuru karo, input tak pahuncho by applying rules.

```
Step 1: query → ?
Step 2: query → SELECT column_list FROM table ...
Step 3: column_list → name, age
Step 4: ... continue until input matches
```

**Diagram:**
```
                    QUERY (Start)
                   /      \      \
           SELECT_CLAUSE  FROM_CLAUSE  WHERE_CLAUSE
              |              |              |
         COLUMN_LIST    TABLE_NAME     CONDITION
           /    \           |          /   |   \
       "name"  "age"    "employees"  col  ">"  25
```

### 🔹 Key Concept: LOOKAHEAD

**Kya hai?**
> Aage wala token dekho (bina consume kiye) aur decide karo kaunsa rule apply karna hai.

```cpp
// Agar * hai to SELECT * hai, warna column names hain
if (check(OP_STAR)) {
    // SELECT * case
} else {
    // SELECT col1, col2 case
}
```

### 🔹 Data Structure: PARSE TREE (Tree Structure)

**Kyun banaya?**
> Query ki structure ko represent karne ke liye tree banaya.

```cpp
struct ParseTreeNode {
    NodeType type;      // QUERY, SELECT_CLAUSE, etc.
    string value;       // "employees", "name", etc.
    vector<children>;   // Child nodes
};
```

---

## Member 3: Semantic Analyzer - Techniques & Algorithms

### 🔹 Technique Used: SYMBOL TABLE MANAGEMENT

**Simple Explanation:**
> Ek database banao jo schema information store kare - tables aur columns.

```cpp
Symbol Table:
┌─────────────┬──────────────────────────────┐
│ Table Name  │ Columns                      │
├─────────────┼──────────────────────────────┤
│ employees   │ id, name, age, salary, dept  │
│ users       │ id, username, email, age     │
│ products    │ id, name, price, quantity    │
└─────────────┴──────────────────────────────┘
```

### 🔹 Algorithm: NAME RESOLUTION

**Kya karta hai?**
> Check karo ki jo table/column use hua hai, wo actually exist karta hai ya nahi.

```cpp
bool validateTable(string tableName) {
    if (symbolTable.find(tableName) == symbolTable.end()) {
        error("Table '" + tableName + "' does not exist!");
        return false;
    }
    return true;  // Table mila!
}
```

**Process:**
```
Query: SELECT name FROM employees;

Step 1: "employees" table exist karta hai? → Check symbol table → YES ✓
Step 2: "name" column employees table mein hai? → Check → YES ✓
Step 3: All validated! → SUCCESS
```

### 🔹 Algorithm: TREE TRAVERSAL (Depth-First)

**Kyun?**
> Parse tree ko traverse karna padta hai validation ke liye.

```cpp
void validateQuery(ParseTree node) {
    // Pehle FROM clause dhundho (table context set karo)
    for (child in node->children) {
        if (child->type == FROM_CLAUSE) {
            validateFromClause(child);  // Table set karo
        }
    }
    
    // Phir SELECT columns validate karo
    for (child in node->children) {
        if (child->type == SELECT_CLAUSE) {
            validateSelectClause(child);  // Columns check karo
        }
    }
}
```

### 🔹 Data Structure: HASH MAP

**Symbol Table implementation:**
```cpp
unordered_map<string, TableInfo> tables;

// O(1) lookup time
bool tableExists(string name) {
    return tables.find(name) != tables.end();
}
```

---

## Member 4: Error Handler - Techniques & Algorithms

### 🔹 Technique Used: ERROR COLLECTION & REPORTING

**Simple Explanation:**
> Saare errors collect karo (sabhi phases se) aur ek jagah report karo.

```cpp
class ErrorHandler {
    vector<CompilerError> errors;  // Saare errors yahan store
    
    void addError(CompilerError e) {
        errors.push_back(e);
    }
    
    void printAll() {
        for (error : errors) {
            cout << error.toString();
        }
    }
};
```

### 🔹 Algorithm: ERROR LOCALIZATION

**Kya karta hai?**
> Error ka exact position batao (line number, column number).

```cpp
struct CompilerError {
    ErrorType type;     // LEXICAL, SYNTAX, SEMANTIC
    string message;     // "Expected 'FROM' keyword"
    int line;          // Line 1
    int column;        // Column 10
};

// Output:
// Syntax Error at Line 1, Column 10: Expected 'FROM' keyword
//   1 | SELECT * employees;
//     |          ^~~~
```

### 🔹 Approach: PHASE-WISE ERROR HANDLING

```
Lexer errors   ──┐
                 ├──→ ErrorHandler ──→ Final Report
Parser errors  ──┤
                 │
Semantic errors──┘
```

**Code:**
```cpp
// Har phase apne errors deta hai
errorHandler.addErrors(lexer.getErrors());
errorHandler.addErrors(parser.getErrors());
errorHandler.addErrors(semantic.getErrors());

// Ek saath print
errorHandler.printSummary();
```

---

## 📚 Compiler Algorithms Summary Table

| Phase | Algorithm/Technique | Simple Explanation |
|-------|--------------------|--------------------|
| **Lexer** | Finite State Machine | Character-by-character scan karke tokens banao |
| **Lexer** | Hash Table Lookup | Keywords O(1) mein dhundo |
| **Parser** | Recursive Descent | Har grammar rule ka ek function |
| **Parser** | Top-Down Parsing | Start symbol se input tak jao |
| **Parser** | 1-Token Lookahead | Aage dekho, decide karo |
| **Semantic** | Symbol Table | Schema info store karo |
| **Semantic** | Name Resolution | Table/Column exist check |
| **Semantic** | Tree Traversal | Parse tree DFS se traverse |
| **Error** | Error Collection | Saare errors ek jagah |
| **Error** | Position Tracking | Line/Column info maintain |

---

## 🎓 Viva ke liye Important Points

### Member 1 (Lexer) - Yaad Rakho:
1. **Token vs Lexeme:** Token = category, Lexeme = actual text
2. **FSM:** States + Transitions + Accept states
3. **Keyword Table:** Hash table for O(1) lookup

### Member 2 (Parser) - Yaad Rakho:
1. **CFG:** G = (V, T, P, S) - Variables, Terminals, Productions, Start
2. **Recursive Descent:** One function per grammar rule
3. **Lookahead:** peek() function use karo

### Member 3 (Semantic) - Yaad Rakho:
1. **Symbol Table:** Identifier info store karta hai
2. **Name Resolution:** Kya ye naam exist karta hai?
3. **Type Checking:** Operations compatible hain?

### Member 4 (Error Handler) - Yaad Rakho:
1. **Error Types:** Lexical, Syntax, Semantic
2. **Error Recovery:** Ek error ke baad bhi continue karo
3. **Position Info:** Line + Column number important

---

## 🔄 Complete Working Flow (Step by Step)

```
1. User Input: "SELECT name FROM users WHERE age > 25;"

2. LEXER (Member 1):
   → Character by character scan
   → FSM se token type identify
   → Output: [SELECT][name][FROM][users][WHERE][age][>][25][;]

3. PARSER (Member 2):
   → Tokens ko grammar se match karo
   → Recursive functions call karo
   → Parse Tree banao:
        QUERY
        ├── SELECT → name
        ├── FROM → users  
        └── WHERE → age > 25

4. SEMANTIC (Member 3):
   → Symbol table mein "users" dhundo → FOUND ✓
   → "name" column check karo → FOUND ✓
   → "age" column check karo → FOUND ✓

5. ERROR HANDLER (Member 4):
   → Koi error nahi → SUCCESS message print karo
   → Error hai → Detailed report banao
```

---

## 💡 Pro Tips for Each Member

**Member 1:** Switch-case master karo, ye tumhara main weapon hai!
**Member 2:** Grammar rules paper pe likh lo, phir code mein convert karo
**Member 3:** Pehle FROM clause handle karo, phir baaki - ORDER matters!
**Member 4:** Formatting pe dhyan do, output beautiful hona chahiye
