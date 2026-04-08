/**
 * Mini SQL Compiler for Query Validation & Execution
 * ====================================================
 * File: json_output.cpp
 * Description: JSON Output Serialization Implementation
 *
 * Produces structured JSON output from all compiler phases
 * for the web frontend to parse and visualize.
 */

#include "../include/json_output.h"
#include <algorithm>

namespace MiniSQL {

// ============================================================================
// JSON STRING ESCAPING
// ============================================================================
std::string JsonOutput::escapeJson(const std::string &s) {
  std::string result;
  result.reserve(s.length() + 10);
  for (char c : s) {
    switch (c) {
    case '"':
      result += "\\\"";
      break;
    case '\\':
      result += "\\\\";
      break;
    case '\n':
      result += "\\n";
      break;
    case '\r':
      result += "\\r";
      break;
    case '\t':
      result += "\\t";
      break;
    default:
      if (static_cast<unsigned char>(c) < 0x20) {
        char buf[8];
        snprintf(buf, sizeof(buf), "\\u%04x", (unsigned char)c);
        result += buf;
      } else {
        result += c;
      }
      break;
    }
  }
  return result;
}

// ============================================================================
// TOKEN SERIALIZATION
// ============================================================================
std::string JsonOutput::serializeTokens(const std::vector<Token> &tokens) {
  std::ostringstream ss;
  ss << "[";
  for (size_t i = 0; i < tokens.size(); i++) {
    if (i > 0)
      ss << ",";
    ss << "{";
    ss << "\"type\":\"" << escapeJson(tokenTypeToString(tokens[i].type))
       << "\",";
    ss << "\"value\":\"" << escapeJson(tokens[i].value) << "\",";
    ss << "\"line\":" << tokens[i].line << ",";
    ss << "\"column\":" << tokens[i].column;
    ss << "}";
  }
  ss << "]";
  return ss.str();
}

// ============================================================================
// PARSE TREE SERIALIZATION (recursive)
// ============================================================================
std::string JsonOutput::serializeParseTree(const ParseTree &node) {
  if (!node)
    return "null";

  std::ostringstream ss;
  ss << "{";
  ss << "\"type\":\"" << escapeJson(nodeTypeToString(node->type)) << "\",";
  ss << "\"value\":\"" << escapeJson(node->value) << "\",";
  ss << "\"children\":[";
  for (size_t i = 0; i < node->children.size(); i++) {
    if (i > 0)
      ss << ",";
    ss << serializeParseTree(node->children[i]);
  }
  ss << "]";
  ss << "}";
  return ss.str();
}

// ============================================================================
// ERROR SERIALIZATION
// ============================================================================
std::string
JsonOutput::serializeErrors(const std::vector<CompilerError> &errors) {
  std::ostringstream ss;
  ss << "[";
  for (size_t i = 0; i < errors.size(); i++) {
    if (i > 0)
      ss << ",";
    ss << "{";
    ss << "\"type\":\"" << escapeJson(errorTypeToString(errors[i].type))
       << "\",";

    // Map error type to phase name
    std::string phase;
    switch (errors[i].type) {
    case ErrorType::LEXICAL_ERROR:
      phase = "Lexical Analysis";
      break;
    case ErrorType::SYNTAX_ERROR:
      phase = "Syntax Analysis";
      break;
    case ErrorType::SEMANTIC_ERROR:
      phase = "Semantic Analysis";
      break;
    }
    ss << "\"phase\":\"" << escapeJson(phase) << "\",";
    ss << "\"message\":\"" << escapeJson(errors[i].message) << "\",";
    ss << "\"line\":" << errors[i].line << ",";
    ss << "\"column\":" << errors[i].column;
    ss << "}";
  }
  ss << "]";
  return ss.str();
}

// ============================================================================
// SCHEMA SERIALIZATION
// ============================================================================
std::string JsonOutput::serializeSchema(const SymbolTable &symbolTable) {
  std::ostringstream ss;
  ss << "[";
  auto names = symbolTable.getTableNames();

  // Sort for consistent output
  std::sort(names.begin(), names.end());

  for (size_t i = 0; i < names.size(); i++) {
    if (i > 0)
      ss << ",";
    const TableInfo *table = symbolTable.getTable(names[i]);
    ss << "{";
    ss << "\"name\":\"" << escapeJson(names[i]) << "\",";
    ss << "\"columns\":[";
    if (table) {
      for (size_t j = 0; j < table->columns.size(); j++) {
        if (j > 0)
          ss << ",";
        ss << "{";
        ss << "\"name\":\"" << escapeJson(table->columns[j].name) << "\",";
        ss << "\"type\":\"" << escapeJson(table->columns[j].dataType) << "\"";
        ss << "}";
      }
    }
    ss << "]";
    ss << "}";
  }
  ss << "]";
  return ss.str();
}

// ============================================================================
// QUERY RESULT SERIALIZATION
// ============================================================================
std::string JsonOutput::serializeQueryResult(const QueryResult &result) {
  std::ostringstream ss;
  ss << "{";
  ss << "\"success\":" << (result.success ? "true" : "false") << ",";
  ss << "\"message\":\"" << escapeJson(result.message) << "\",";
  ss << "\"affectedRows\":" << result.affectedRows << ",";

  // Column names
  ss << "\"columns\":[";
  for (size_t i = 0; i < result.columnNames.size(); i++) {
    if (i > 0)
      ss << ",";
    ss << "\"" << escapeJson(result.columnNames[i]) << "\"";
  }
  ss << "],";

  // Rows
  ss << "\"rows\":[";
  for (size_t i = 0; i < result.rows.size(); i++) {
    if (i > 0)
      ss << ",";
    ss << "{";
    bool first = true;
    for (const auto &pair : result.rows[i]) {
      if (!first)
        ss << ",";
      first = false;
      ss << "\"" << escapeJson(pair.first) << "\":\"" << escapeJson(pair.second)
         << "\"";
    }
    ss << "}";
  }
  ss << "]";

  ss << "}";
  return ss.str();
}

// ============================================================================
// FULL COMPILATION RESULT
// ============================================================================
std::string JsonOutput::serializeFullResult(
    const std::string &query, const std::vector<Token> &tokens,
    bool lexerSuccess, const std::vector<CompilerError> &lexerErrors,
    const ParseTree &parseTree, bool parserSuccess,
    const std::vector<CompilerError> &parserErrors, bool semanticSuccess,
    const std::vector<CompilerError> &semanticErrors,
    const QueryResult *execResult, const SymbolTable &symbolTable) {

  std::ostringstream ss;
  ss << "{";

  // Original query
  ss << "\"query\":\"" << escapeJson(query) << "\",";

  // Phase 1: Lexical Analysis
  ss << "\"lexer\":{";
  ss << "\"success\":" << (lexerSuccess ? "true" : "false") << ",";
  ss << "\"tokens\":" << serializeTokens(tokens) << ",";
  ss << "\"errors\":" << serializeErrors(lexerErrors);
  ss << "},";

  // Phase 2: Syntax Analysis
  ss << "\"parser\":{";
  ss << "\"success\":" << (parserSuccess ? "true" : "false") << ",";
  ss << "\"parseTree\":" << serializeParseTree(parseTree) << ",";
  ss << "\"errors\":" << serializeErrors(parserErrors);
  ss << "},";

  // Phase 3: Semantic Analysis
  ss << "\"semantic\":{";
  ss << "\"success\":" << (semanticSuccess ? "true" : "false") << ",";
  ss << "\"errors\":" << serializeErrors(semanticErrors);
  ss << "},";

  // Phase 4: Execution
  ss << "\"execution\":";
  if (execResult) {
    ss << serializeQueryResult(*execResult);
  } else {
    ss << "null";
  }
  ss << ",";

  // Schema info
  ss << "\"schema\":" << serializeSchema(symbolTable);

  ss << "}";
  return ss.str();
}

} // namespace MiniSQL
