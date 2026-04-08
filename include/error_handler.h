/**
 * Mini SQL Compiler for Query Validation
 * =======================================
 * File: error_handler.h
 * Description: Error Handling Module Header
 * Team Member: Member 4
 *
 * The Error Handler is responsible for:
 * - Collecting errors from all compilation phases
 * - Formatting error messages in a user-friendly manner
 * - Providing detailed location information (line, column)
 * - Generating a final compilation report
 *
 * THEORY:
 * -------
 * Good error handling is crucial for a compiler's usability.
 * Errors should be:
 * - Specific: Clearly identify what went wrong
 * - Localized: Point to exactly where the error occurred
 * - Helpful: Suggest how to fix the problem when possible
 *
 * Error Categories:
 * 1. Lexical Errors: Invalid characters, malformed tokens
 * 2. Syntax Errors: Grammar violations
 * 3. Semantic Errors: Logical inconsistencies
 */

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "common.h"
#include <string>
#include <vector>

namespace MiniSQL {

class ErrorHandler {
private:
  std::vector<CompilerError> errors;
  std::string sourceQuery;

  // Helper to get the line containing an error
  std::string getSourceLine(int lineNum) const;

  // Generate pointer to error location
  std::string getErrorPointer(int column) const;

public:
  /**
   * Constructor
   */
  ErrorHandler();

  /**
   * Set the source query for error context display
   */
  void setSource(const std::string &source);

  /**
   * Add a single error
   */
  void addError(const CompilerError &error);

  /**
   * Add multiple errors
   */
  void addErrors(const std::vector<CompilerError> &errs);

  /**
   * Check if there are any errors
   */
  bool hasErrors() const;

  /**
   * Get error count
   */
  int getErrorCount() const;

  /**
   * Get errors by type
   */
  std::vector<CompilerError> getErrorsByType(ErrorType type) const;

  /**
   * Print all errors to console with formatting
   */
  void printErrors() const;

  /**
   * Print final compilation summary
   */
  void printSummary(bool syntaxValid, bool semanticValid) const;

  /**
   * Clear all errors
   */
  void clear();
};

} // namespace MiniSQL

#endif // ERROR_HANDLER_H
