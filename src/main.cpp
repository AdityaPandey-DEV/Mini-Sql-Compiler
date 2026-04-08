/**
 * ============================================================================
 *                    MINI SQL COMPILER FOR QUERY VALIDATION & EXECUTION
 * ============================================================================
 *
 * Project: Compiler Design Academic Project
 * Subject: Compiler Design
 * Team Size: 4 Members
 *
 * Team Members and Responsibilities:
 * ----------------------------------
 * Member 1: Lexical Analyzer (lexer.h, lexer.cpp)
 * Member 2: Syntax Analyzer  (parser.h, parser.cpp)
 * Member 3: Semantic Analyzer & Symbol Table (semantic.h/cpp,
 *           symbol_table.h/cpp)
 * Member 4: Error Handler, Executor, Main Driver, Testing
 *           (error_handler.h/cpp, executor.h/cpp, data_store.h/cpp, main.cpp)
 *
 * Compilation Phases Implemented:
 * ------------------------------
 * 1. LEXICAL ANALYSIS:   Converts character stream to token stream
 * 2. SYNTAX ANALYSIS:    Validates grammar and builds parse tree
 * 3. SEMANTIC ANALYSIS:  Validates meaning and resolves identifiers
 * 4. QUERY EXECUTION:    Executes validated queries against data store
 * 5. ERROR HANDLING:     Reports errors from all phases
 *
 * Supported SQL:
 * --------------------
 * SELECT column1, column2, ... | * FROM table_name [WHERE condition];
 * INSERT INTO table (col1, col2) VALUES (val1, val2);
 * UPDATE table SET col = val WHERE condition;
 * DELETE FROM table [WHERE condition];
 *
 * Operators: =, !=, <, <=, >, >=
 *
 * Usage:
 * ------
 * ./sql_compiler                               (Interactive mode)
 * ./sql_compiler --demo                        (Demo mode)
 * ./sql_compiler --file queries.txt            (Batch mode)
 * echo "SELECT * FROM users;" | ./sql_compiler (Pipe mode)
 *
 * ============================================================================
 */

#include "../include/data_store.h"
#include "../include/error_handler.h"
#include "../include/executor.h"
#include "../include/json_output.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/semantic.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace MiniSQL;

// Global data store (persists across queries in a session)
static SymbolTable globalSymbolTable;
static DataStore globalDataStore(globalSymbolTable);

// Function prototypes
void printBanner();
void printHelp();
void compileAndExecute(const std::string &query);
void compileJson(const std::string &query);
void runInteractiveMode();
void runDemoMode();
void runBatchMode(const std::string &filePath);

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main(int argc, char *argv[]) {
  // Check for command line arguments
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "--json" && argc > 2) {
      // JSON output mode for web integration (no banner)
      std::string query = argv[2];
      compileJson(query);
      return 0;
    } else if (arg == "--json-schema") {
      // Output schema as JSON (no banner)
      std::cout << JsonOutput::serializeSchema(globalSymbolTable) << std::endl;
      return 0;
    }
  }

  printBanner();

  // Check for other command line arguments
  if (argc > 1) {
    std::string arg = argv[1];
    if (arg == "--help" || arg == "-h") {
      printHelp();
      return 0;
    } else if (arg == "--demo") {
      runDemoMode();
      return 0;
    } else if (arg == "--file" && argc > 2) {
      runBatchMode(argv[2]);
      return 0;
    }
  }

  // Interactive mode
  runInteractiveMode();

  return 0;
}

// ============================================================================
// BANNER AND HELP
// ============================================================================
void printBanner() {
  std::cout << "\n";
  std::cout
      << "╔══════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║       MINI SQL COMPILER - QUERY VALIDATION & EXECUTION      ║\n";
  std::cout
      << "║                   Compiler Design Project                    ║\n";
  std::cout
      << "╠══════════════════════════════════════════════════════════════╣\n";
  std::cout
      << "║  Team Members:                                               ║\n";
  std::cout
      << "║    Member 1: Lexical Analyzer                                ║\n";
  std::cout
      << "║    Member 2: Syntax Analyzer                                 ║\n";
  std::cout
      << "║    Member 3: Semantic Analyzer                               ║\n";
  std::cout
      << "║    Member 4: Executor & Error Handler                        ║\n";
  std::cout
      << "╚══════════════════════════════════════════════════════════════╝\n";
}

void printHelp() {
  std::cout << "\nUsage: ./sql_compiler [options]\n";
  std::cout << "\nOptions:\n";
  std::cout << "  --help, -h         Show this help message\n";
  std::cout << "  --demo             Run demo with sample queries\n";
  std::cout << "  --file <path>      Execute queries from a file\n";
  std::cout << "\nSupported SQL Syntax:\n";
  std::cout << "  SELECT col1, col2 | * FROM table [WHERE col op value];\n";
  std::cout << "  INSERT INTO table (col1, col2) VALUES (val1, val2);\n";
  std::cout << "  UPDATE table SET col = value [WHERE col op value];\n";
  std::cout << "  DELETE FROM table [WHERE col op value];\n";
  std::cout << "\nOperators: =, !=, <, <=, >, >=\n";
  std::cout << "\nAvailable Tables (with sample data):\n";
  std::cout << "  employees   (id, name, age, salary, department)\n";
  std::cout << "  departments (id, name, budget)\n";
  std::cout << "  users       (id, username, email, age, status)\n";
  std::cout << "  products    (id, name, price, quantity)\n";
  std::cout << "\nInteractive Commands:\n";
  std::cout << "  help       Show this help message\n";
  std::cout << "  tables     Show available tables and schema\n";
  std::cout << "  demo       Run demo queries\n";
  std::cout << "  clear      Clear screen\n";
  std::cout << "  save       Save data to CSV files (data/ directory)\n";
  std::cout << "  load       Load data from CSV files\n";
  std::cout << "  exit/quit  Exit the compiler\n";
}

// ============================================================================
// QUERY COMPILATION & EXECUTION - Main pipeline
// ============================================================================
void compileAndExecute(const std::string &query) {
  ErrorHandler errorHandler;
  errorHandler.setSource(query);

  bool syntaxValid = false;
  bool semanticValid = false;
  ParseTree parseTree = nullptr;

  std::cout << "\n══════════════════════════════════════════\n";
  std::cout << "Starting compilation of query...\n";
  std::cout << "══════════════════════════════════════════\n";

  // ========================================
  // PHASE 1: LEXICAL ANALYSIS (Member 1)
  // ========================================
  Lexer lexer(query);
  std::vector<Token> tokens = lexer.tokenize();

  // Display token stream
  lexer.printTokens();

  // Check for lexical errors
  if (lexer.hasErrors()) {
    errorHandler.addErrors(lexer.getErrors());
    errorHandler.printErrors();
    errorHandler.printSummary(false, false);
    return;
  }

  // ========================================
  // PHASE 2: SYNTAX ANALYSIS (Member 2)
  // ========================================
  Parser parser(tokens);
  parseTree = parser.parse();

  // Check for syntax errors
  if (parser.hasErrors()) {
    errorHandler.addErrors(parser.getErrors());
    errorHandler.printErrors();
    errorHandler.printSummary(false, false);
    return;
  }

  syntaxValid = (parseTree != nullptr);

  // Display parse tree
  if (parseTree) {
    std::cout << "\n--- Parse Tree (Intermediate Representation) ---\n";
    Parser::printParseTree(parseTree);
  }

  // ========================================
  // PHASE 3: SEMANTIC ANALYSIS (Member 3)
  // ========================================
  SemanticAnalyzer semanticAnalyzer;

  // Optionally print symbol table
  std::cout << "\n--- Available Schema for Validation ---\n";
  semanticAnalyzer.printSymbolTable();

  semanticValid = semanticAnalyzer.analyze(parseTree);

  // Check for semantic errors
  if (semanticAnalyzer.hasErrors()) {
    errorHandler.addErrors(semanticAnalyzer.getErrors());
  }

  // ========================================
  // PHASE 4: QUERY EXECUTION (Member 4)
  // ========================================
  if (syntaxValid && semanticValid && parseTree) {
    Executor executor(globalDataStore);
    QueryResult result = executor.execute(parseTree);
    executor.printResults(result);
  }

  // ========================================
  // PHASE 5: FINAL OUTPUT (Member 4)
  // ========================================
  if (errorHandler.hasErrors()) {
    errorHandler.printErrors();
  }

  errorHandler.printSummary(syntaxValid, semanticValid);
}

// ============================================================================
// JSON COMPILATION MODE - For web API integration
// ============================================================================
void compileJson(const std::string &query) {
  bool lexerSuccess = false;
  bool parserSuccess = false;
  bool semanticSuccess = false;
  ParseTree parseTree = nullptr;
  QueryResult *execResultPtr = nullptr;
  QueryResult execResult;

  // Redirect cout to suppress normal output
  std::streambuf *oldCout = std::cout.rdbuf();
  std::ostringstream nullStream;
  std::cout.rdbuf(nullStream.rdbuf());

  // Phase 1: Lexical Analysis
  Lexer lexer(query);
  std::vector<Token> tokens = lexer.tokenize();
  lexerSuccess = !lexer.hasErrors();

  std::vector<CompilerError> parserErrors;
  std::vector<CompilerError> semanticErrors;

  if (lexerSuccess) {
    // Phase 2: Syntax Analysis
    Parser parser(tokens);
    parseTree = parser.parse();
    parserSuccess = !parser.hasErrors() && parseTree != nullptr;
    parserErrors = parser.getErrors();

    if (parserSuccess && parseTree) {
      // Phase 3: Semantic Analysis
      SemanticAnalyzer semanticAnalyzer;
      semanticSuccess = semanticAnalyzer.analyze(parseTree);
      semanticErrors = semanticAnalyzer.getErrors();

      if (semanticSuccess) {
        // Phase 4: Execution
        Executor executor(globalDataStore);
        execResult = executor.execute(parseTree);
        execResultPtr = &execResult;
      }
    }
  }

  // Restore cout
  std::cout.rdbuf(oldCout);

  // Output JSON
  std::cout << JsonOutput::serializeFullResult(
      query, tokens, lexerSuccess, lexer.getErrors(), parseTree, parserSuccess,
      parserErrors, semanticSuccess, semanticErrors, execResultPtr,
      globalSymbolTable)
            << std::endl;
}

// ============================================================================
// INTERACTIVE MODE
// ============================================================================
void runInteractiveMode() {
  std::cout << "\nEntering Interactive Mode...\n";
  std::cout << "Type SQL queries to validate and execute. Type 'exit' or "
               "'quit' to stop.\n";
  std::cout
      << "Type 'help' for syntax help, 'tables' to see available tables.\n";

  std::string line;
  std::string query;

  while (true) {
    std::cout << "\nsql> ";

    if (!std::getline(std::cin, line)) {
      break; // EOF
    }

    // Trim whitespace
    size_t start = line.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
      continue;
    line = line.substr(start);

    // Check for commands
    if (line == "exit" || line == "quit") {
      std::cout << "Goodbye!\n";
      break;
    }

    if (line == "help") {
      printHelp();
      continue;
    }

    if (line == "tables") {
      SemanticAnalyzer temp;
      temp.printSymbolTable();
      continue;
    }

    if (line == "demo") {
      runDemoMode();
      continue;
    }

    if (line == "clear") {
      system("clear");
      printBanner();
      continue;
    }

    if (line == "save") {
      system("mkdir -p data");
      globalDataStore.saveToFiles("data");
      std::cout << "Data saved to data/ directory.\n";
      continue;
    }

    if (line == "load") {
      globalDataStore.loadFromFiles("data");
      std::cout << "Data loaded from data/ directory.\n";
      continue;
    }

    // Accumulate query until semicolon
    query += line;

    if (query.find(';') != std::string::npos) {
      compileAndExecute(query);
      query.clear();
    } else {
      std::cout << "     (continue query, end with ';')\n";
    }
  }
}

// ============================================================================
// DEMO MODE - Run sample queries demonstrating all features
// ============================================================================
void runDemoMode() {
  std::cout << "\n";
  std::cout
      << "╔══════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║              DEMO MODE - Full SQL Execution Demo            ║\n";
  std::cout
      << "╚══════════════════════════════════════════════════════════════╝\n";

  std::vector<std::pair<std::string, std::string>> testCases = {
      {"SELECT * - Show all employees", "SELECT * FROM employees;"},

      {"SELECT with columns & WHERE",
       "SELECT name, salary FROM employees WHERE salary > 70000;"},

      {"SELECT from users table",
       "SELECT username, email FROM users WHERE age > 25;"},

      {"INSERT a new employee",
       "INSERT INTO employees (id, name, age, salary, department) VALUES (9, "
       "'Kavita Joshi', 31, 68000, 'Sales');"},

      {"SELECT to verify INSERT",
       "SELECT name, department FROM employees WHERE department = Sales;"},

      {"UPDATE an employee salary",
       "UPDATE employees SET salary = 100000 WHERE id = 3;"},

      {"SELECT to verify UPDATE",
       "SELECT name, salary FROM employees WHERE id = 3;"},

      {"DELETE an employee", "DELETE FROM employees WHERE id = 9;"},

      {"Syntax Error - Missing FROM", "SELECT * employees;"},

      {"Semantic Error - Bad table", "SELECT * FROM customers;"},

      {"Semantic Error - Bad column", "SELECT invalid_col FROM employees;"}};

  for (size_t i = 0; i < testCases.size(); i++) {
    std::cout << "\n";
    std::cout
        << "┌────────────────────────────────────────────────────────────┐\n";
    std::cout << "│ Test Case " << (i + 1) << ": " << testCases[i].first;
    for (size_t j = testCases[i].first.length() + 15; j < 60; j++)
      std::cout << " ";
    std::cout << "│\n";
    std::cout << "│ Query: " << testCases[i].second;
    for (size_t j = testCases[i].second.length() + 8; j < 60; j++)
      std::cout << " ";
    std::cout << "│\n";
    std::cout
        << "└────────────────────────────────────────────────────────────┘\n";

    compileAndExecute(testCases[i].second);

    std::cout << "\n--- Press Enter to continue ---";
    std::cin.get();
  }

  std::cout
      << "\n╔══════════════════════════════════════════════════════════════╗\n";
  std::cout
      << "║                     Demo Complete!                           ║\n";
  std::cout
      << "╚══════════════════════════════════════════════════════════════╝\n";
}

// ============================================================================
// BATCH MODE - Execute queries from a file
// ============================================================================
void runBatchMode(const std::string &filePath) {
  std::ifstream file(filePath);
  if (!file.is_open()) {
    std::cerr << "Error: Could not open file '" << filePath << "'\n";
    return;
  }

  std::cout << "\n--- Batch Mode: Processing file '" << filePath << "' ---\n";

  std::string line;
  std::string query;
  int queryCount = 0;

  while (std::getline(file, line)) {
    // Skip empty lines and comments
    size_t start = line.find_first_not_of(" \t");
    if (start == std::string::npos)
      continue;
    line = line.substr(start);

    if (line[0] == '#' || line.substr(0, 2) == "--")
      continue;

    query += line + " ";

    if (query.find(';') != std::string::npos) {
      queryCount++;
      std::cout << "\n╔════════════════════════════════════════╗\n";
      std::cout << "║  Query #" << queryCount << "\n";
      std::cout << "║  " << query << "\n";
      std::cout << "╚════════════════════════════════════════╝\n";

      compileAndExecute(query);
      query.clear();
    }
  }

  file.close();
  std::cout << "\n--- Batch Mode Complete: " << queryCount
            << " queries processed ---\n";
}
