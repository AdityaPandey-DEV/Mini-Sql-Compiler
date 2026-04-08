/**
 * Mini SQL Compiler for Query Validation
 * =======================================
 * File: lexer.h
 * Description: Lexical Analyzer Header
 * Team Member: Member 1
 *
 * The Lexical Analyzer (Lexer/Scanner) is the first phase of the compiler.
 * It reads the input SQL query character by character and groups them into
 * meaningful units called TOKENS.
 *
 * Responsibilities:
 * - Tokenize SQL keywords (SELECT, FROM, WHERE, AND, OR)
 * - Identify identifiers (table names, column names)
 * - Recognize operators (=, <, >, ,, *, ;)
 * - Handle numeric and string literals
 * - Track line and column positions for error reporting
 * - Skip whitespace and comments
 */

#ifndef LEXER_H
#define LEXER_H

#include "common.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace MiniSQL {

class Lexer {
private:
  std::string source;                // Input SQL query
  std::vector<Token> tokens;         // Output token stream
  std::vector<CompilerError> errors; // Lexical errors encountered

  size_t start;    // Start position of current token
  size_t current;  // Current position in source
  int line;        // Current line number
  int column;      // Current column number
  int startColumn; // Column at start of current token

  // Keyword lookup table
  static std::unordered_map<std::string, TokenType> keywords;

  // Helper methods
  bool isAtEnd() const;
  char advance();
  char peek() const;
  char peekNext() const;
  bool match(char expected);

  // Token scanning methods
  void scanToken();
  void scanIdentifier();
  void scanNumber();
  void scanString();
  void skipWhitespace();

  // Token creation
  void addToken(TokenType type);
  void addToken(TokenType type, const std::string &value);

  // Error reporting
  void reportError(const std::string &message);

public:
  /**
   * Constructor
   * @param source The SQL query string to tokenize
   */
  explicit Lexer(const std::string &source);

  /**
   * Perform lexical analysis and generate token stream
   * @return Vector of tokens
   */
  std::vector<Token> tokenize();

  /**
   * Check if lexical analysis had errors
   * @return true if errors occurred
   */
  bool hasErrors() const;

  /**
   * Get all lexical errors
   * @return Vector of compiler errors
   */
  std::vector<CompilerError> getErrors() const;

  /**
   * Print token stream to console (for demonstration)
   */
  void printTokens() const;
};

} // namespace MiniSQL

#endif // LEXER_H
