/**
 * Mini SQL Compiler for Query Validation
 * =======================================
 * File: lexer.cpp
 * Description: Lexical Analyzer Implementation
 * Team Member: Member 1
 *
 * THEORY:
 * -------
 * Lexical Analysis is the first phase of compilation. It reads the source
 * code as a stream of characters and converts it into a stream of tokens.
 *
 * A TOKEN is the smallest meaningful unit in the source code.
 * Examples: Keywords (SELECT), Identifiers (employee_name), Operators (=)
 *
 * The lexer uses a technique called "scanning" where it:
 * 1. Reads characters one by one
 * 2. Groups them based on patterns (using finite automata concepts)
 * 3. Classifies each group as a specific token type
 * 4. Records position information for error reporting
 *
 * PATTERN RECOGNITION:
 * - Keywords: Predefined reserved words (SELECT, FROM, WHERE)
 * - Identifiers: Start with letter/underscore, followed by alphanumeric
 * - Numbers: Sequence of digits (with optional decimal point)
 * - Strings: Characters enclosed in single quotes
 * - Operators: Single-character symbols (=, <, >, *, etc.)
 */

#include "../include/lexer.h"
#include <algorithm>
#include <cctype>
#include <iostream>

namespace MiniSQL {

// Initialize keyword lookup table
std::unordered_map<std::string, TokenType> Lexer::keywords = {
    {"SELECT", TokenType::KEYWORD_SELECT},
    {"FROM", TokenType::KEYWORD_FROM},
    {"WHERE", TokenType::KEYWORD_WHERE},
    {"AND", TokenType::KEYWORD_AND},
    {"OR", TokenType::KEYWORD_OR},
    {"INSERT", TokenType::KEYWORD_INSERT},
    {"INTO", TokenType::KEYWORD_INTO},
    {"VALUES", TokenType::KEYWORD_VALUES},
    {"UPDATE", TokenType::KEYWORD_UPDATE},
    {"SET", TokenType::KEYWORD_SET},
    {"DELETE", TokenType::KEYWORD_DELETE},
    {"CREATE", TokenType::KEYWORD_CREATE},
    {"TABLE", TokenType::KEYWORD_TABLE}};

// Constructor
Lexer::Lexer(const std::string &source)
    : source(source), start(0), current(0), line(1), column(1), startColumn(1) {
}

// ============================================================================
// MAIN TOKENIZATION METHOD
// ============================================================================
std::vector<Token> Lexer::tokenize() {
  tokens.clear();
  errors.clear();

  std::cout << "\n========================================\n";
  std::cout << "   PHASE 1: LEXICAL ANALYSIS\n";
  std::cout << "========================================\n";
  std::cout << "Input Query: " << source << "\n\n";

  while (!isAtEnd()) {
    // Mark the beginning of the next token
    start = current;
    startColumn = column;
    scanToken();
  }

  // Add end-of-input token
  tokens.push_back(Token(TokenType::END_OF_INPUT, "", line, column));

  if (errors.empty()) {
    std::cout << "Lexical Analysis: SUCCESS\n";
    std::cout << "Total Tokens Generated: " << tokens.size() << "\n";
  } else {
    std::cout << "Lexical Analysis: FAILED with " << errors.size()
              << " error(s)\n";
  }

  return tokens;
}

// ============================================================================
// TOKEN SCANNING - Core scanning logic
// ============================================================================
void Lexer::scanToken() {
  char c = advance();

  switch (c) {
  // Skip whitespace
  case ' ':
  case '\r':
  case '\t':
    // Whitespace - ignore
    break;
  case '\n':
    line++;
    column = 1;
    break;

  // Single-character operators
  case '*':
    addToken(TokenType::OP_STAR);
    break;
  case ',':
    addToken(TokenType::OP_COMMA);
    break;
  case ';':
    addToken(TokenType::OP_SEMICOLON);
    break;
  case '(':
    addToken(TokenType::OP_LPAREN);
    break;
  case ')':
    addToken(TokenType::OP_RPAREN);
    break;
  case '=':
    addToken(TokenType::OP_EQUALS);
    break;
  case '!':
    if (match('=')) {
      addToken(TokenType::OP_NOT_EQUALS, "!=");
    } else {
      std::string msg = "Unexpected character '!', did you mean '!='?";
      reportError(msg);
    }
    break;
  case '<':
    if (match('=')) {
      addToken(TokenType::OP_LESS_EQUALS, "<=");
    } else {
      addToken(TokenType::OP_LESS_THAN);
    }
    break;
  case '>':
    if (match('=')) {
      addToken(TokenType::OP_GREATER_EQUALS, ">=");
    } else {
      addToken(TokenType::OP_GREATER_THAN);
    }
    break;

  // String literals
  case '\'':
    scanString();
    break;

  default:
    // Identifiers and keywords start with letter or underscore
    if (std::isalpha(c) || c == '_') {
      scanIdentifier();
    }
    // Numbers start with digit
    else if (std::isdigit(c)) {
      scanNumber();
    }
    // Unknown character - lexical error
    else {
      std::string msg = "Unexpected character '";
      msg += c;
      msg += "'";
      reportError(msg);
    }
    break;
  }
}

// ============================================================================
// IDENTIFIER SCANNING - Handles keywords and identifiers
// ============================================================================
void Lexer::scanIdentifier() {
  // Continue while we see alphanumeric characters or underscore
  while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
    advance();
  }

  // Extract the identifier text
  std::string text = source.substr(start, current - start);

  // Convert to uppercase for keyword matching
  std::string upperText = text;
  std::transform(upperText.begin(), upperText.end(), upperText.begin(),
                 ::toupper);

  // Check if it's a keyword
  auto it = keywords.find(upperText);
  if (it != keywords.end()) {
    addToken(it->second, upperText);
  } else {
    // It's a regular identifier
    addToken(TokenType::IDENTIFIER, text);
  }
}

// ============================================================================
// NUMBER SCANNING - Handles integer and decimal numbers
// ============================================================================
void Lexer::scanNumber() {
  // Consume all digits
  while (!isAtEnd() && std::isdigit(peek())) {
    advance();
  }

  // Check for decimal point
  if (peek() == '.' && std::isdigit(peekNext())) {
    // Consume the dot
    advance();

    // Consume fractional part
    while (!isAtEnd() && std::isdigit(peek())) {
      advance();
    }
  }

  std::string numStr = source.substr(start, current - start);
  addToken(TokenType::NUMBER, numStr);
}

// ============================================================================
// STRING SCANNING - Handles string literals in single quotes
// ============================================================================
void Lexer::scanString() {
  int stringStartLine = line;
  int stringStartCol = startColumn;

  // Consume characters until closing quote or end
  while (!isAtEnd() && peek() != '\'') {
    if (peek() == '\n') {
      line++;
      column = 0;
    }
    advance();
  }

  // Check for unterminated string
  if (isAtEnd()) {
    errors.push_back(CompilerError(ErrorType::LEXICAL_ERROR,
                                   "Unterminated string literal",
                                   stringStartLine, stringStartCol));
    return;
  }

  // Consume closing quote
  advance();

  // Extract string value (without quotes)
  std::string value = source.substr(start + 1, current - start - 2);
  addToken(TokenType::STRING_LITERAL, value);
}

// ============================================================================
// HELPER METHODS
// ============================================================================

bool Lexer::isAtEnd() const { return current >= source.length(); }

char Lexer::advance() {
  column++;
  return source[current++];
}

char Lexer::peek() const {
  if (isAtEnd())
    return '\0';
  return source[current];
}

char Lexer::peekNext() const {
  if (current + 1 >= source.length())
    return '\0';
  return source[current + 1];
}

bool Lexer::match(char expected) {
  if (isAtEnd())
    return false;
  if (source[current] != expected)
    return false;
  current++;
  column++;
  return true;
}

void Lexer::addToken(TokenType type) {
  std::string text = source.substr(start, current - start);
  tokens.push_back(Token(type, text, line, startColumn));
}

void Lexer::addToken(TokenType type, const std::string &value) {
  tokens.push_back(Token(type, value, line, startColumn));
}

void Lexer::reportError(const std::string &message) {
  errors.push_back(
      CompilerError(ErrorType::LEXICAL_ERROR, message, line, startColumn));
}

bool Lexer::hasErrors() const { return !errors.empty(); }

std::vector<CompilerError> Lexer::getErrors() const { return errors; }

// ============================================================================
// DISPLAY TOKEN STREAM
// ============================================================================
void Lexer::printTokens() const {
  std::cout << "\n--- Token Stream ---\n";
  std::cout << "+-----------------------+------------------+------+-----+\n";
  std::cout << "| Token Type            | Value            | Line | Col |\n";
  std::cout << "+-----------------------+------------------+------+-----+\n";

  for (const auto &token : tokens) {
    printf("| %-21s | %-16s | %4d | %3d |\n",
           tokenTypeToString(token.type).c_str(), token.value.c_str(),
           token.line, token.column);
  }

  std::cout << "+-----------------------+------------------+------+-----+\n";
}

} // namespace MiniSQL
