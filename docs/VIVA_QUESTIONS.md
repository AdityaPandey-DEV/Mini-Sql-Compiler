# Viva Questions and Answers
## Mini SQL Compiler Project

---

## Section 1: Lexical Analysis (15 Questions)

### Q1. What is lexical analysis?
**Answer:** Lexical analysis (scanning) is the first phase of compilation that reads the source code character by character and groups them into meaningful units called **tokens**. It removes whitespace and comments, tracks line/column positions for error reporting, and produces a token stream for the parser.

### Q2. What is a token?
**Answer:** A token is the smallest meaningful unit in the source code. It consists of:
- **Token Type:** Category (keyword, identifier, operator, etc.)
- **Token Value:** Actual text (e.g., "SELECT", "employee")
- **Position:** Line and column number

Example: In `SELECT * FROM users`, the tokens are: `KEYWORD_SELECT`, `OP_STAR`, `KEYWORD_FROM`, `IDENTIFIER(users)`.

### Q3. What is the difference between a keyword and an identifier?
**Answer:** 
- **Keywords** are reserved words with predefined meaning (SELECT, FROM, WHERE). They cannot be used as variable names.
- **Identifiers** are user-defined names for tables, columns, etc. They follow naming rules (start with letter/underscore).

### Q4. How does the lexer recognize keywords?
**Answer:** The lexer first scans an identifier pattern, then looks it up in a **keyword table** (hash map). If found, it returns the keyword token type; otherwise, it returns IDENTIFIER.

```cpp
auto it = keywords.find(upperText);
if (it != keywords.end()) return it->second;  // Keyword
else return TokenType::IDENTIFIER;
```

### Q5. What is a finite automaton and how is it used in lexical analysis?
**Answer:** A finite automaton (FA) is a mathematical model with states and transitions used to recognize patterns. In lexical analysis:
- States represent progress through a token pattern
- Transitions occur on input characters
- Accept states indicate valid tokens

Example: Recognizing numbers: Start → (digit) → Number State → (digit) → loop

### Q6. How are string literals handled in the lexer?
**Answer:** String literals are scanned by:
1. Detecting the opening quote `'`
2. Consuming all characters until the closing quote
3. If EOF is reached without closing quote → error (unterminated string)
4. Extracting content between quotes as the token value

### Q7. What errors can occur during lexical analysis?
**Answer:**
- **Invalid characters:** Characters not part of the language (`@`, `$`, `#`)
- **Unterminated strings:** Missing closing quote
- **Malformed numbers:** Invalid numeric formats

### Q8. Why do we track line and column numbers?
**Answer:** To provide precise error location for users. When an error occurs, we can point exactly to where in the source code the problem exists, making debugging easier.

---

## Section 2: Syntax Analysis (15 Questions)

### Q9. What is syntax analysis?
**Answer:** Syntax analysis (parsing) is the second phase that verifies the token sequence follows the grammatical rules of the language. It builds a **parse tree** representing the hierarchical structure of the input.

### Q10. What is a Context-Free Grammar (CFG)?
**Answer:** A CFG is a formal grammar with four components (G = V, T, P, S):
- **V:** Non-terminals (syntactic categories)
- **T:** Terminals (tokens)
- **P:** Production rules
- **S:** Start symbol

Example rule: `<query> ::= SELECT <column_list> FROM <table_name>`

### Q11. What is recursive descent parsing?
**Answer:** Recursive descent is a top-down parsing technique where:
- Each non-terminal has a corresponding parsing function
- Functions call each other recursively following grammar rules
- The parser predicts which rule to apply based on the current token (lookahead)

```cpp
ParseTree parseQuery() {
    parseSelectClause();  // Call for SELECT
    parseFromClause();    // Call for FROM
    parseWhereClause();   // Optional WHERE
}
```

### Q12. What is the difference between top-down and bottom-up parsing?
**Answer:**
- **Top-down:** Starts from start symbol, derives input (recursive descent, LL parsers)
- **Bottom-up:** Starts from input, reduces to start symbol (shift-reduce, LR parsers)

We used top-down (recursive descent) for simplicity and intuitive implementation.

### Q13. What is a parse tree?
**Answer:** A parse tree is a hierarchical representation of the syntactic structure:
- Root = Start symbol
- Internal nodes = Non-terminals
- Leaves = Terminals (tokens)
- Shows derivation of input from grammar

### Q14. What is the role of lookahead in parsing?
**Answer:** Lookahead examines upcoming tokens without consuming them to decide which grammar rule to apply. Our parser uses 1-token lookahead (LL(1)):

```cpp
if (check(TokenType::KEYWORD_WHERE)) {
    parseWhereClause();  // Only parse if WHERE exists
}
```

### Q15. How does the parser handle optional clauses (like WHERE)?
**Answer:** By checking if the expected token is present before parsing:

```cpp
if (check(TokenType::KEYWORD_WHERE)) {
    auto whereClause = parseWhereClause();
    queryNode->addChild(whereClause);
}
// If not present, simply skip
```

### Q16. What is error recovery in parsing?
**Answer:** Error recovery allows the parser to continue after finding an error, enabling detection of multiple errors in one pass. Techniques include:
- **Panic mode:** Skip tokens until a synchronization point (e.g., semicolon)
- **Phrase-level recovery:** Insert/delete tokens locally

### Q17. What errors can occur during syntax analysis?
**Answer:**
- Missing keywords (e.g., no FROM after SELECT)
- Missing operators or punctuation
- Unexpected tokens
- Incomplete statements

---

## Section 3: Semantic Analysis (10 Questions)

### Q18. What is semantic analysis?
**Answer:** Semantic analysis checks the **meaning** of the program - aspects that cannot be expressed in grammar. It validates:
- Name resolution (do identifiers exist?)
- Type checking (are operations valid?)
- Scope rules (where are names visible?)

### Q19. What is a symbol table?
**Answer:** A symbol table is a data structure storing information about identifiers:
- **Tables:** Name, columns, constraints
- **Columns:** Name, data type, table membership

It's used for looking up and validating references during semantic analysis.

```cpp
class SymbolTable {
    std::unordered_map<std::string, TableInfo> tables;
    bool tableExists(const std::string& name);
    bool columnExists(const std::string& table, const std::string& col);
};
```

### Q20. What semantic checks are performed in this project?
**Answer:**
1. **Table existence:** Is the referenced table in the schema?
2. **Column existence:** Do selected columns belong to the table?
3. **WHERE column validation:** Is the condition column valid?
4. **Basic type compatibility:** Numeric columns with numeric values

### Q21. Why must FROM clause be validated before SELECT?
**Answer:** We need to establish the **table context** first. Only then can we validate whether the selected columns exist in that specific table. This is an **inherited attribute** - table information flows down to column validation.

### Q22. What is type checking?
**Answer:** Type checking ensures operations are performed on compatible types:
- Comparing INT column with numeric literal ✓
- Comparing VARCHAR column with string literal ✓
- Comparing INT column with string literal (warning)

### Q23. What errors can occur during semantic analysis?
**Answer:**
- Table doesn't exist in schema
- Column doesn't exist in the specified table
- Column used in wrong context
- Type mismatches in comparisons

---

## Section 4: Error Handling (5 Questions)

### Q24. Why is good error handling important?
**Answer:** Good error messages:
- Help users quickly identify and fix problems
- Reduce debugging time
- Improve user experience
- Allow detecting multiple errors in one pass

### Q25. What information should an error message contain?
**Answer:**
- **Error type:** Lexical, Syntax, or Semantic
- **Location:** Line and column number
- **Description:** What went wrong
- **Context:** Source line with error pointer
- **Suggestion:** How to fix (when possible)

### Q26. How are errors collected from different phases?
**Answer:** Each phase maintains its own error list. The ErrorHandler aggregates them:

```cpp
errorHandler.addErrors(lexer.getErrors());
errorHandler.addErrors(parser.getErrors());
errorHandler.addErrors(semantic.getErrors());
```

---

## Section 5: General Compiler Theory (10 Questions)

### Q27. What are the phases of a compiler?
**Answer:**
1. **Lexical Analysis:** Source → Tokens
2. **Syntax Analysis:** Tokens → Parse Tree
3. **Semantic Analysis:** Validate meaning
4. **Intermediate Code Generation:** IR code
5. **Code Optimization:** Improve efficiency
6. **Code Generation:** Target machine code

### Q28. What is the difference between a compiler and an interpreter?
**Answer:**
- **Compiler:** Translates entire program before execution, produces executable
- **Interpreter:** Executes line by line, no separate executable

Our project is a **front-end only compiler** that validates but doesn't generate code.

### Q29. What is intermediate representation (IR)?
**Answer:** IR is a machine-independent representation between source and target code. In our project, the **Parse Tree** serves as the intermediate representation of the query.

### Q30. What is the Front-End vs Back-End of a compiler?
**Answer:**
- **Front-End:** Lexical, Syntax, Semantic analysis (language-dependent)
- **Back-End:** Optimization, Code generation (machine-dependent)

Our project implements only the front-end.

### Q31. Why use C++ for this project?
**Answer:**
- Direct memory management and performance
- Object-oriented design for modularity
- Standard library (strings, vectors, maps)
- Industry-standard for compiler development

### Q32. What is the time complexity of lexical analysis?
**Answer:** O(n) where n is the input length - each character is examined once.

### Q33. What is the time complexity of recursive descent parsing?
**Answer:** O(n) for LL(1) grammars where n is the number of tokens - each token is examined once.

### Q34. How would you add AND/OR support to WHERE clause?
**Answer:** Modify the grammar:
```
<condition> ::= <simple_condition> { (AND | OR) <simple_condition> }*
```
And update `parseCondition()` to handle compound expressions recursively.

### Q35. What real-world systems use similar techniques?
**Answer:**
- MySQL/PostgreSQL query parsers
- GCC/Clang compilers
- JavaScript engines (V8, SpiderMonkey)
- SQL IDEs for syntax highlighting

---

## Section 6: Code-Specific Questions (5 Questions)

### Q36. Explain the Token structure.
**Answer:**
```cpp
struct Token {
    TokenType type;    // Category (KEYWORD, IDENTIFIER, etc.)
    std::string value; // Actual text
    int line, column;  // Position in source
};
```

### Q37. How does `advance()` work in the lexer?
**Answer:** It consumes and returns the current character, incrementing position counters:
```cpp
char Lexer::advance() {
    column++;
    return source[current++];
}
```

### Q38. Explain the parse tree node structure.
**Answer:**
```cpp
struct ParseTreeNode {
    NodeType type;           // QUERY, SELECT_CLAUSE, etc.
    std::string value;       // Associated value (e.g., column name)
    std::vector<ParseTree> children;  // Child nodes
};
```

### Q39. How is the symbol table initialized?
**Answer:** The constructor creates sample tables with columns:
```cpp
SymbolTable::SymbolTable() {
    TableInfo employees("employees");
    employees.addColumn("id", "INT");
    employees.addColumn("name", "VARCHAR");
    // ... more columns
    addTable(employees);
}
```

### Q40. How do compilation phases communicate?
**Answer:** Through data structures:
- Lexer → Parser: Token vector
- Parser → Semantic: Parse tree pointer
- All → ErrorHandler: Error vectors

---

## Quick Reference: Important Terms

| Term | Definition |
|------|------------|
| Token | Smallest meaningful unit in source code |
| Lexeme | Actual character sequence forming a token |
| Grammar | Set of rules defining valid syntax |
| Parse Tree | Hierarchical representation of syntax |
| Symbol Table | Database of identifier information |
| Lookahead | Examining tokens without consuming |
| Recursive Descent | Top-down parsing with one function per rule |
