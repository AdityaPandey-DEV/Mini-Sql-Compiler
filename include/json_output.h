/**
 * Mini SQL Compiler for Query Validation & Execution
 * ====================================================
 * File: json_output.h
 * Description: JSON Output Serialization for Web Integration
 *
 * Provides structured JSON output of all compiler phases for
 * consumption by the web frontend. Lightweight implementation
 * with no external dependencies.
 */

#ifndef JSON_OUTPUT_H
#define JSON_OUTPUT_H

#include "common.h"
#include "data_store.h"
#include "executor.h"
#include "symbol_table.h"
#include <sstream>
#include <string>
#include <vector>

namespace MiniSQL {

class JsonOutput {
private:
  static std::string escapeJson(const std::string &s);

public:
  // Serialize token stream
  static std::string serializeTokens(const std::vector<Token> &tokens);

  // Serialize parse tree recursively
  static std::string serializeParseTree(const ParseTree &node);

  // Serialize errors with phase info
  static std::string serializeErrors(const std::vector<CompilerError> &errors);

  // Serialize symbol table / schema
  static std::string serializeSchema(const SymbolTable &symbolTable);

  // Serialize query execution results
  static std::string serializeQueryResult(const QueryResult &result);

  // Serialize full compilation result
  static std::string serializeFullResult(
      const std::string &query, const std::vector<Token> &tokens,
      bool lexerSuccess, const std::vector<CompilerError> &lexerErrors,
      const ParseTree &parseTree, bool parserSuccess,
      const std::vector<CompilerError> &parserErrors, bool semanticSuccess,
      const std::vector<CompilerError> &semanticErrors,
      const QueryResult *execResult, const SymbolTable &symbolTable);
};

} // namespace MiniSQL

#endif // JSON_OUTPUT_H
