# Mini SQL Compiler

**Multi-phase SQL compiler with lexer, parser, semantic analyzer, and web-based visualization — built in C++.**

![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Docker](https://img.shields.io/badge/Docker-2496ED?style=flat-square&logo=docker&logoColor=white)
![CI](https://img.shields.io/badge/CI-GitHub_Actions-2088FF?style=flat-square&logo=githubactions&logoColor=white)

---

## What It Does

A complete SQL compilation pipeline that parses, validates, and analyzes SQL statements — with a web interface for stepping through each phase.

**Compilation Phases:**
1. **Lexer** — tokenizes SQL into keywords, identifiers, literals, operators
2. **Parser** — builds an Abstract Syntax Tree from token streams
3. **Semantic Analyzer** — type checking, scope resolution, SQL validity
4. **Web Visualizer** — interactive UI to step through each phase

## Architecture

```
SQL Input → Lexer (Token Stream) → Parser (AST) → Semantic Analyzer (Validated AST)
                                                        → Web Visualizer
```

## Tech Stack

| Component | Technology |
|---|---|
| Compiler Core | C++17, Makefile |
| Web UI | HTML/CSS/JavaScript |
| Container | Docker |
| CI/CD | GitHub Actions |

## My Role

I designed the 3-phase compilation pipeline, defined the token categories and grammar rules, and planned the AST node hierarchy. Code generation was accelerated using AI tools; compiler design and testing are mine.

## Quick Start

```bash
git clone https://github.com/AdityaPandey-DEV/Mini-Sql-Compiler.git && cd Mini-Sql-Compiler
make build && ./mini-sql-compiler
# Or with Docker: docker build -t mini-sql . && docker run -p 8080:8080 mini-sql
```

---

<div align="center">

*Architected & built by [Aditya Pandey](https://github.com/AdityaPandey-DEV) — AI-augmented development*

</div>
