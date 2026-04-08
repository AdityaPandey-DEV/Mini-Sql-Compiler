/**
 * Mini SQL Compiler for Query Validation
 * =======================================
 * File: error_handler.cpp
 * Description: Error Handling Implementation
 * Team Member: Member 4
 *
 * This module provides formatted error output similar to professional
 * compilers like GCC or Clang.
 */

#include "../include/error_handler.h"
#include <iostream>
#include <sstream>

namespace MiniSQL {

ErrorHandler::ErrorHandler() : sourceQuery("") {}

void ErrorHandler::setSource(const std::string &source) {
  sourceQuery = source;
}

void ErrorHandler::addError(const CompilerError &error) {
  errors.push_back(error);
}

void ErrorHandler::addErrors(const std::vector<CompilerError> &errs) {
  for (const auto &err : errs) {
    errors.push_back(err);
  }
}

bool ErrorHandler::hasErrors() const { return !errors.empty(); }

int ErrorHandler::getErrorCount() const {
  return static_cast<int>(errors.size());
}

std::vector<CompilerError> ErrorHandler::getErrorsByType(ErrorType type) const {
  std::vector<CompilerError> filtered;
  for (const auto &err : errors) {
    if (err.type == type) {
      filtered.push_back(err);
    }
  }
  return filtered;
}

std::string ErrorHandler::getSourceLine(int lineNum) const {
  std::istringstream stream(sourceQuery);
  std::string line;
  int current = 0;

  while (std::getline(stream, line)) {
    current++;
    if (current == lineNum) {
      return line;
    }
  }
  return sourceQuery; // Return full query if single line
}

std::string ErrorHandler::getErrorPointer(int column) const {
  std::string pointer;
  for (int i = 1; i < column; i++) {
    pointer += ' ';
  }
  pointer += "^~~~";
  return pointer;
}

void ErrorHandler::printErrors() const {
  if (errors.empty())
    return;

  std::cout << "\n========================================\n";
  std::cout << "   ERROR REPORT\n";
  std::cout << "========================================\n";

  for (size_t i = 0; i < errors.size(); i++) {
    const auto &error = errors[i];

    std::cout << "\n[Error " << (i + 1) << "] " << error.toString() << "\n";

    // Show source context
    if (!sourceQuery.empty()) {
      std::string srcLine = getSourceLine(error.line);
      std::cout << "  " << error.line << " | " << srcLine << "\n";
      std::cout << "    | " << getErrorPointer(error.column) << "\n";
    }
  }

  std::cout << "\n";
}

void ErrorHandler::printSummary(bool syntaxValid, bool semanticValid) const {
  std::cout << "\n========================================\n";
  std::cout << "   COMPILATION SUMMARY\n";
  std::cout << "========================================\n";

  // Count errors by type
  int lexicalErrors = 0, syntaxErrors = 0, semanticErrors = 0;
  for (const auto &err : errors) {
    switch (err.type) {
    case ErrorType::LEXICAL_ERROR:
      lexicalErrors++;
      break;
    case ErrorType::SYNTAX_ERROR:
      syntaxErrors++;
      break;
    case ErrorType::SEMANTIC_ERROR:
      semanticErrors++;
      break;
    }
  }

  std::cout << "\n+-------------------+----------------+\n";
  std::cout << "| Phase             | Status         |\n";
  std::cout << "+-------------------+----------------+\n";

  // Lexical Analysis
  std::cout << "| Lexical Analysis  | ";
  if (lexicalErrors == 0) {
    std::cout << "PASSED         |\n";
  } else {
    printf("FAILED (%d err) |\n", lexicalErrors);
  }

  // Syntax Analysis
  std::cout << "| Syntax Analysis   | ";
  if (syntaxValid && syntaxErrors == 0) {
    std::cout << "PASSED         |\n";
  } else if (lexicalErrors > 0) {
    std::cout << "SKIPPED        |\n";
  } else {
    printf("FAILED (%d err) |\n", syntaxErrors);
  }

  // Semantic Analysis
  std::cout << "| Semantic Analysis | ";
  if (semanticValid && semanticErrors == 0) {
    std::cout << "PASSED         |\n";
  } else if (!syntaxValid || syntaxErrors > 0) {
    std::cout << "SKIPPED        |\n";
  } else {
    printf("FAILED (%d err) |\n", semanticErrors);
  }

  std::cout << "+-------------------+----------------+\n";

  // Overall result
  std::cout << "\n";
  if (errors.empty()) {
    std::cout << "╔═══════════════════════════════════════╗\n";
    std::cout << "║   QUERY VALIDATION: SUCCESSFUL        ║\n";
    std::cout << "║   The SQL query is valid!             ║\n";
    std::cout << "╚═══════════════════════════════════════╝\n";
  } else {
    std::cout << "╔═══════════════════════════════════════╗\n";
    std::cout << "║   QUERY VALIDATION: FAILED            ║\n";
    printf("║   Total errors found: %-16d ║\n", (int)errors.size());
    std::cout << "╚═══════════════════════════════════════╝\n";
  }
}

void ErrorHandler::clear() {
  errors.clear();
  sourceQuery = "";
}

} // namespace MiniSQL
