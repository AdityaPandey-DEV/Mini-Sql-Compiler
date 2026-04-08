/**
 * Compiler Bridge — Spawns the C++ compiler binary and parses JSON output.
 * Provides a Promise-based API for the Express server.
 */

const { execFile } = require('child_process');
const path = require('path');

// Path to the compiled binary (relative to project root)
const COMPILER_PATH = path.resolve(__dirname, '../../sql_compiler');
const TIMEOUT_MS = 5000;

/**
 * Compile a SQL query using the C++ binary
 * @param {string} query - The SQL query to compile
 * @returns {Promise<object>} - Parsed JSON compilation result
 */
function compileQuery(query) {
  return new Promise((resolve, reject) => {
    // Input sanitization: only allow printable ASCII and common SQL chars
    const sanitized = query.replace(/[^\x20-\x7E\n\r\t]/g, '');
    
    if (!sanitized.trim()) {
      return reject(new Error('Empty query'));
    }

    if (sanitized.length > 2000) {
      return reject(new Error('Query too long (max 2000 characters)'));
    }

    execFile(
      COMPILER_PATH,
      ['--json', sanitized],
      {
        timeout: TIMEOUT_MS,
        maxBuffer: 1024 * 1024, // 1MB max output
        env: { ...process.env },
      },
      (error, stdout, stderr) => {
        if (error) {
          if (error.killed) {
            return reject(new Error('Compilation timed out (5 second limit)'));
          }
          // Try to parse output even on non-zero exit
          if (stdout && stdout.trim()) {
            try {
              const result = JSON.parse(stdout.trim());
              return resolve(result);
            } catch (parseErr) {
              return reject(new Error(`Compiler error: ${error.message}`));
            }
          }
          return reject(new Error(`Compiler error: ${error.message}`));
        }

        try {
          const result = JSON.parse(stdout.trim());
          resolve(result);
        } catch (parseErr) {
          reject(new Error(`Failed to parse compiler output: ${parseErr.message}`));
        }
      }
    );
  });
}

/**
 * Get the database schema from the compiler
 * @returns {Promise<Array>} - Array of table schemas
 */
function getSchema() {
  return new Promise((resolve, reject) => {
    execFile(
      COMPILER_PATH,
      ['--json-schema'],
      { timeout: TIMEOUT_MS },
      (error, stdout, stderr) => {
        if (error) {
          return reject(new Error(`Failed to get schema: ${error.message}`));
        }
        try {
          const schema = JSON.parse(stdout.trim());
          resolve(schema);
        } catch (parseErr) {
          reject(new Error(`Failed to parse schema: ${parseErr.message}`));
        }
      }
    );
  });
}

module.exports = { compileQuery, getSchema };
