# ============================================================================
# Mini SQL Compiler — Docker Build for Free Deployment
# ============================================================================
# Multi-stage build:
#   Stage 1: Compile C++ binary
#   Stage 2: Run Node.js server
# ============================================================================

# Stage 1: Build the C++ compiler
FROM node:20-slim AS builder

RUN apt-get update && apt-get install -y g++ make && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy C++ source and build
COPY include/ include/
COPY src/ src/
COPY Makefile .

RUN make

# Stage 2: Production image
FROM node:20-slim

RUN apt-get update && apt-get install -y libstdc++6 && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy compiled binary
COPY --from=builder /app/sql_compiler .

# Copy web application
COPY web/server/package.json web/server/
RUN cd web/server && npm install --production

COPY web/ web/

# Expose port
EXPOSE 3000

# Health check
HEALTHCHECK --interval=30s --timeout=5s --start-period=10s \
  CMD node -e "const http = require('http'); http.get('http://localhost:3000/api/health', (r) => { process.exit(r.statusCode === 200 ? 0 : 1); }).on('error', () => process.exit(1));"

# Start the server
CMD ["node", "web/server/server.js"]
