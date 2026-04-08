/**
 * Main Application — Orchestrates the SQL Compiler Web IDE
 * Handles API calls, phase animation, tab switching, schema/history drawers
 */
(function () {
  'use strict';

  // ===== STATE =====
  let isCompiling = false;
  let queryHistory = [];
  const MAX_HISTORY = 50;

  // ===== INIT =====
  document.addEventListener('DOMContentLoaded', () => {
    Editor.init();
    ErrorModal.init();
    initTabs();
    initCompileButton();
    initKeyboardShortcuts();
    initSchemaDrawer();
    initHistoryDrawer();
    loadExamples();
    loadHistory();
  });

  // ===== COMPILE =====
  function initCompileButton() {
    document.getElementById('btn-compile').addEventListener('click', compile);
    document.getElementById('btn-clear-results').addEventListener('click', clearResults);
  }

  function initKeyboardShortcuts() {
    document.addEventListener('keydown', (e) => {
      if ((e.ctrlKey || e.metaKey) && e.key === 'Enter') {
        e.preventDefault();
        compile();
      }
    });
  }

  async function compile() {
    if (isCompiling) return;

    const query = Editor.getValue().trim();
    if (!query) {
      ErrorModal.show([{
        type: 'Syntax Error',
        phase: 'Syntax Analysis',
        message: 'Please enter a SQL query before compiling.',
        line: 1, column: 1
      }], '');
      return;
    }

    isCompiling = true;
    const btn = document.getElementById('btn-compile');
    btn.innerHTML = '<span class="spinner"></span> Compiling...';
    btn.classList.add('btn-loading');
    btn.disabled = true;

    // Reset pipeline
    CompilerViz.resetPipeline();
    switchTab('tokens');

    try {
      // Animate pipeline phases sequentially
      await animateCompilation(query);
    } catch (err) {
      ErrorModal.show([{
        type: 'System Error',
        phase: 'System',
        message: err.message || 'Failed to connect to the compiler server.',
        line: 1, column: 1
      }], query);
    } finally {
      isCompiling = false;
      btn.innerHTML = '<svg width="16" height="16" viewBox="0 0 16 16" fill="currentColor"><path d="M4 2l10 6-10 6V2z"/></svg> Compile & Execute';
      btn.classList.remove('btn-loading');
      btn.disabled = false;
    }
  }

  async function animateCompilation(query) {
    // Phase 1: Lexer — show "running"
    CompilerViz.setPipelineStatus('lexer', 'running');
    await Animations.delay(300);

    // Call the API
    const response = await fetch('/api/compile', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ query })
    });

    if (!response.ok) {
      const err = await response.json().catch(() => ({ error: 'Server error' }));
      throw new Error(err.error || `HTTP ${response.status}`);
    }

    const result = await response.json();

    // Save to history
    addToHistory(query, result);

    // Phase 1 result
    if (result.lexer.success) {
      CompilerViz.setPipelineStatus('lexer', 'success');
      CompilerViz.setArrowActive(0, true);
    } else {
      CompilerViz.setPipelineStatus('lexer', 'failed');
      renderResults(result);
      showErrors(result, query);
      return;
    }

    // Render tokens immediately
    document.getElementById('pane-tokens').innerHTML = CompilerViz.renderTokens(result.lexer.tokens);

    // Phase 2: Parser
    await Animations.delay(250);
    CompilerViz.setPipelineStatus('parser', 'running');
    await Animations.delay(300);

    if (result.parser.success) {
      CompilerViz.setPipelineStatus('parser', 'success');
      CompilerViz.setArrowActive(1, true);
    } else {
      CompilerViz.setPipelineStatus('parser', 'failed');
      CompilerViz.setPipelineStatus('semantic', 'skipped');
      CompilerViz.setPipelineStatus('execution', 'skipped');
      renderResults(result);
      showErrors(result, query);
      return;
    }

    // Render parse tree
    document.getElementById('pane-parse-tree').innerHTML =
      ParseTreeRenderer.render(result.parser.parseTree);

    // Phase 3: Semantic
    await Animations.delay(250);
    CompilerViz.setPipelineStatus('semantic', 'running');
    await Animations.delay(300);

    if (result.semantic.success) {
      CompilerViz.setPipelineStatus('semantic', 'success');
      CompilerViz.setArrowActive(2, true);
    } else {
      CompilerViz.setPipelineStatus('semantic', 'failed');
      CompilerViz.setPipelineStatus('execution', 'skipped');
      renderResults(result);
      showErrors(result, query);
      return;
    }

    // Phase 4: Execution
    await Animations.delay(250);
    CompilerViz.setPipelineStatus('execution', 'running');
    await Animations.delay(300);

    if (result.execution && result.execution.success) {
      CompilerViz.setPipelineStatus('execution', 'success');
    } else {
      CompilerViz.setPipelineStatus('execution', 'failed');
    }

    // Render all results
    renderResults(result);

    // Switch to execution tab if we got results
    if (result.execution && result.execution.columns && result.execution.columns.length > 0) {
      switchTab('execution');
    }
  }

  function renderResults(result) {
    // Tokens
    if (result.lexer.tokens) {
      document.getElementById('pane-tokens').innerHTML =
        CompilerViz.renderTokens(result.lexer.tokens);
    }

    // Parse tree
    if (result.parser.parseTree) {
      document.getElementById('pane-parse-tree').innerHTML =
        ParseTreeRenderer.render(result.parser.parseTree);
    } else {
      document.getElementById('pane-parse-tree').innerHTML =
        '<div class="empty-state"><p>Parse tree not available (syntax error)</p></div>';
    }

    // Execution results
    document.getElementById('pane-execution').innerHTML =
      CompilerViz.renderExecutionResult(result.execution);

    // Summary
    document.getElementById('pane-summary').innerHTML =
      CompilerViz.renderSummary(result);
  }

  function showErrors(result, query) {
    // Collect all errors
    const allErrors = [
      ...(result.lexer.errors || []),
      ...(result.parser.errors || []),
      ...(result.semantic.errors || []),
    ];

    if (allErrors.length > 0) {
      ErrorModal.show(allErrors, query);
      switchTab('summary');
    }
  }

  function clearResults() {
    CompilerViz.resetPipeline();
    document.getElementById('pane-tokens').innerHTML = `
      <div class="empty-state">
        <svg width="48" height="48" viewBox="0 0 48 48" fill="none" stroke="currentColor" stroke-width="1.5" opacity="0.3"><circle cx="24" cy="24" r="20"/><path d="M16 20l4 4-4 4M24 28h8"/></svg>
        <p>Run a query to see the token stream</p>
      </div>`;
    document.getElementById('pane-parse-tree').innerHTML = `
      <div class="empty-state">
        <svg width="48" height="48" viewBox="0 0 48 48" fill="none" stroke="currentColor" stroke-width="1.5" opacity="0.3"><rect x="18" y="4" width="12" height="8" rx="2"/><rect x="4" y="36" width="12" height="8" rx="2"/><rect x="32" y="36" width="12" height="8" rx="2"/><line x1="24" y1="12" x2="24" y2="24"/><line x1="24" y1="24" x2="10" y2="36"/><line x1="24" y1="24" x2="38" y2="36"/></svg>
        <p>Run a query to see the parse tree</p>
      </div>`;
    document.getElementById('pane-execution').innerHTML = `
      <div class="empty-state">
        <svg width="48" height="48" viewBox="0 0 48 48" fill="none" stroke="currentColor" stroke-width="1.5" opacity="0.3"><rect x="4" y="8" width="40" height="32" rx="3"/><line x1="4" y1="16" x2="44" y2="16"/><line x1="16" y1="16" x2="16" y2="40"/></svg>
        <p>Run a query to see execution results</p>
      </div>`;
    document.getElementById('pane-summary').innerHTML = `
      <div class="empty-state">
        <svg width="48" height="48" viewBox="0 0 48 48" fill="none" stroke="currentColor" stroke-width="1.5" opacity="0.3"><path d="M24 4l6 12 14 2-10 10 2 14-12-6-12 6 2-14L4 18l14-2z"/></svg>
        <p>Run a query to see the compilation summary</p>
      </div>`;
    switchTab('tokens');
  }

  // ===== TABS =====
  function initTabs() {
    document.querySelectorAll('.tab').forEach(tab => {
      tab.addEventListener('click', () => switchTab(tab.dataset.tab));
    });
  }

  function switchTab(tabId) {
    // Update tab buttons
    document.querySelectorAll('.tab').forEach(t => {
      t.classList.toggle('active', t.dataset.tab === tabId);
    });

    // Update panes
    document.querySelectorAll('.tab-pane').forEach(p => {
      p.classList.toggle('active', p.id === `pane-${tabId}`);
    });
  }

  // ===== EXAMPLES =====
  async function loadExamples() {
    try {
      const res = await fetch('/api/examples');
      const data = await res.json();

      const select = document.getElementById('example-select');

      // Valid examples
      const validGroup = document.createElement('optgroup');
      validGroup.label = '✓ Valid Queries';
      data.valid.forEach(ex => {
        const opt = document.createElement('option');
        opt.value = ex.query;
        opt.textContent = ex.name;
        validGroup.appendChild(opt);
      });
      select.appendChild(validGroup);

      // Invalid examples
      const invalidGroup = document.createElement('optgroup');
      invalidGroup.label = '✗ Error Queries';
      data.invalid.forEach(ex => {
        const opt = document.createElement('option');
        opt.value = ex.query;
        opt.textContent = `${ex.name} [${ex.errorType}]`;
        invalidGroup.appendChild(opt);
      });
      select.appendChild(invalidGroup);

      select.addEventListener('change', () => {
        if (select.value) {
          Editor.setValue(select.value);
          select.value = '';
        }
      });
    } catch (err) {
      console.warn('Failed to load examples:', err);
    }
  }

  // ===== SCHEMA DRAWER =====
  function initSchemaDrawer() {
    const btn = document.getElementById('btn-schema');
    const overlay = document.getElementById('schema-drawer-overlay');
    const close = document.getElementById('btn-close-schema');

    btn.addEventListener('click', async () => {
      overlay.classList.remove('hidden');
      await loadSchema();
    });

    close.addEventListener('click', () => overlay.classList.add('hidden'));
    overlay.addEventListener('click', (e) => {
      if (e.target === overlay) overlay.classList.add('hidden');
    });
  }

  async function loadSchema() {
    const container = document.getElementById('schema-content');
    try {
      const res = await fetch('/api/schema');
      const schema = await res.json();

      let html = '';
      schema.forEach(table => {
        html += '<div class="schema-table-card">';
        html += `<div class="schema-table-name">📋 ${table.name}</div>`;
        table.columns.forEach(col => {
          html += `<div class="schema-col">`;
          html += `<span class="schema-col-name">${col.name}</span>`;
          html += `<span class="schema-col-type">${col.type}</span>`;
          html += `</div>`;
        });
        html += '</div>';
      });
      container.innerHTML = html;
    } catch (err) {
      container.innerHTML = '<p style="color: var(--text-muted)">Failed to load schema</p>';
    }
  }

  // ===== HISTORY DRAWER =====
  function initHistoryDrawer() {
    const btn = document.getElementById('btn-history');
    const overlay = document.getElementById('history-drawer-overlay');
    const close = document.getElementById('btn-close-history');

    btn.addEventListener('click', () => {
      overlay.classList.remove('hidden');
      renderHistory();
    });

    close.addEventListener('click', () => overlay.classList.add('hidden'));
    overlay.addEventListener('click', (e) => {
      if (e.target === overlay) overlay.classList.add('hidden');
    });
  }

  function addToHistory(query, result) {
    const allPassed = result.lexer.success && result.parser.success && result.semantic.success;
    queryHistory.unshift({
      query,
      success: allPassed,
      time: new Date().toLocaleTimeString()
    });
    if (queryHistory.length > MAX_HISTORY) queryHistory.pop();
    saveHistory();
  }

  function renderHistory() {
    const container = document.getElementById('history-content');
    if (queryHistory.length === 0) {
      container.innerHTML = '<p style="color: var(--text-muted); text-align: center; margin-top: 2rem;">No queries yet</p>';
      return;
    }

    let html = '';
    queryHistory.forEach((item, i) => {
      html += `<div class="history-item" data-index="${i}">`;
      html += `<div class="history-query">${escapeHtml(item.query)}</div>`;
      html += `<div class="history-time">${item.time}</div>`;
      html += `<span class="history-status ${item.success ? 'success' : 'error'}">${item.success ? 'PASSED' : 'FAILED'}</span>`;
      html += '</div>';
    });
    container.innerHTML = html;

    // Click to load
    container.querySelectorAll('.history-item').forEach(el => {
      el.addEventListener('click', () => {
        const idx = parseInt(el.dataset.index);
        Editor.setValue(queryHistory[idx].query);
        document.getElementById('history-drawer-overlay').classList.add('hidden');
      });
    });
  }

  function saveHistory() {
    try {
      localStorage.setItem('sql-compiler-history', JSON.stringify(queryHistory));
    } catch (e) { /* ignore */ }
  }

  function loadHistory() {
    try {
      const saved = localStorage.getItem('sql-compiler-history');
      if (saved) queryHistory = JSON.parse(saved);
    } catch (e) { /* ignore */ }
  }

  function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text || '';
    return div.innerHTML;
  }
})();
