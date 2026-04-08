/**
 * Compiler Visualization — Renders token tables, pipeline status, result tables, and summary
 */
const CompilerViz = (() => {

  /**
   * Update pipeline phase status with animation
   */
  function setPipelineStatus(phaseId, status) {
    const el = document.getElementById(`phase-${phaseId}`);
    if (!el) return;

    // Remove old state classes
    el.classList.remove('running', 'success', 'failed', 'skipped');

    // Set status badge
    const badge = el.querySelector('.status-badge');
    badge.className = `status-badge ${status}`;

    const labels = {
      idle: 'Idle', running: 'Running...', success: 'Passed',
      failed: 'Failed', skipped: 'Skipped'
    };
    badge.textContent = labels[status] || status;

    // Add state class to phase card
    if (status !== 'idle') {
      el.classList.add(status);
    }
  }

  /**
   * Update pipeline arrows between phases
   */
  function setArrowActive(index, active) {
    const arrows = document.querySelectorAll('.pipeline-arrow');
    if (arrows[index]) {
      arrows[index].classList.toggle('active', active);
    }
  }

  /**
   * Reset all pipeline phases to idle
   */
  function resetPipeline() {
    ['lexer', 'parser', 'semantic', 'execution'].forEach(p => setPipelineStatus(p, 'idle'));
    document.querySelectorAll('.pipeline-arrow').forEach(a => a.classList.remove('active'));
  }

  /**
   * Render token stream table
   */
  function renderTokens(tokens) {
    if (!tokens || tokens.length === 0) return '<p class="empty-state"><span>No tokens generated</span></p>';

    let html = '<table class="token-table"><thead><tr>';
    html += '<th>#</th><th>Type</th><th>Value</th><th>Position</th>';
    html += '</tr></thead><tbody>';

    tokens.forEach((token, i) => {
      if (token.type === 'END_OF_INPUT') return;

      const typeClass = getTokenTypeClass(token.type);
      const delay = i * 30;

      html += `<tr style="animation-delay: ${delay}ms">`;
      html += `<td class="token-pos">${i + 1}</td>`;
      html += `<td><span class="token-type ${typeClass}">${token.type}</span></td>`;
      html += `<td class="token-value">${escapeHtml(token.value)}</td>`;
      html += `<td class="token-pos">Ln ${token.line}, Col ${token.column}</td>`;
      html += '</tr>';
    });

    html += '</tbody></table>';
    return html;
  }

  /**
   * Render execution result table
   */
  function renderExecutionResult(execution) {
    if (!execution) return '<p class="empty-state"><span>Query was not executed</span></p>';

    let html = '';

    // Message
    html += `<div class="result-message">${escapeHtml(execution.message)}</div>`;

    // For non-SELECT (INSERT/UPDATE/DELETE)
    if (!execution.columns || execution.columns.length === 0) {
      if (execution.affectedRows > 0) {
        html += `<div class="result-meta">Affected rows: <span class="count">${execution.affectedRows}</span></div>`;
      }
      return html;
    }

    // SELECT result table
    html += '<table class="result-table"><thead><tr>';
    execution.columns.forEach(col => {
      html += `<th>${escapeHtml(col)}</th>`;
    });
    html += '</tr></thead><tbody>';

    if (execution.rows && execution.rows.length > 0) {
      execution.rows.forEach(row => {
        html += '<tr>';
        execution.columns.forEach(col => {
          html += `<td>${escapeHtml(row[col] || 'NULL')}</td>`;
        });
        html += '</tr>';
      });
    } else {
      html += `<tr><td colspan="${execution.columns.length}" style="text-align:center; color: var(--text-muted);">(no rows)</td></tr>`;
    }

    html += '</tbody></table>';
    html += `<div class="result-meta">${execution.rows ? execution.rows.length : 0} row(s) returned</div>`;

    return html;
  }

  /**
   * Render compilation summary
   */
  function renderSummary(result) {
    let html = '';

    // Phase results card
    html += '<div class="summary-card">';
    html += '<div class="summary-title">📊 Phase Results</div>';
    html += '<div class="summary-grid">';

    html += renderSummaryItem('Lexical Analysis', result.lexer.success);
    html += renderSummaryItem('Syntax Analysis', result.parser.success,
      !result.lexer.success ? 'skip' : null);
    html += renderSummaryItem('Semantic Analysis', result.semantic.success,
      !result.parser.success ? 'skip' : null);
    html += renderSummaryItem('Query Execution', result.execution !== null,
      !result.semantic.success ? 'skip' : null);

    html += '</div></div>';

    // Token count
    const tokenCount = result.lexer.tokens
      ? result.lexer.tokens.filter(t => t.type !== 'END_OF_INPUT').length
      : 0;

    html += '<div class="summary-card">';
    html += '<div class="summary-title">📝 Statistics</div>';
    html += '<div class="summary-grid">';
    html += `<div class="summary-item"><span class="label">Tokens Generated</span><span class="value">${tokenCount}</span></div>`;

    const totalErrors = (result.lexer.errors?.length || 0) +
      (result.parser.errors?.length || 0) +
      (result.semantic.errors?.length || 0);
    html += `<div class="summary-item"><span class="label">Total Errors</span><span class="value ${totalErrors > 0 ? 'fail' : 'pass'}">${totalErrors}</span></div>`;

    if (result.execution && result.execution.rows) {
      html += `<div class="summary-item"><span class="label">Rows Returned</span><span class="value">${result.execution.rows.length}</span></div>`;
    }
    if (result.execution && result.execution.affectedRows > 0) {
      html += `<div class="summary-item"><span class="label">Affected Rows</span><span class="value">${result.execution.affectedRows}</span></div>`;
    }

    html += '</div></div>';

    // Overall verdict
    const allPassed = result.lexer.success && result.parser.success && result.semantic.success;
    html += `<div class="summary-overall ${allPassed ? 'success' : 'failure'}">`;
    html += `<h3>${allPassed ? '✓ Query Valid & Executed' : '✗ Compilation Failed'}</h3>`;
    html += `<p>${allPassed
      ? 'All phases passed successfully.'
      : `Error in ${getFailedPhase(result)} phase.`}</p>`;
    html += '</div>';

    return html;
  }

  function renderSummaryItem(label, success, override) {
    let status, cls;
    if (override === 'skip') {
      status = 'SKIPPED';
      cls = 'skip';
    } else {
      status = success ? 'PASSED' : 'FAILED';
      cls = success ? 'pass' : 'fail';
    }
    return `<div class="summary-item"><span class="label">${label}</span><span class="value ${cls}">${status}</span></div>`;
  }

  function getFailedPhase(result) {
    if (!result.lexer.success) return 'Lexical Analysis';
    if (!result.parser.success) return 'Syntax Analysis';
    if (!result.semantic.success) return 'Semantic Analysis';
    return 'Execution';
  }

  function getTokenTypeClass(type) {
    if (type.startsWith('KEYWORD')) return 'keyword';
    if (type === 'IDENTIFIER') return 'identifier';
    if (type === 'NUMBER') return 'number';
    if (type === 'STRING_LITERAL') return 'string';
    if (type.startsWith('OP_')) return 'operator';
    return 'special';
  }

  function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text || '';
    return div.innerHTML;
  }

  return {
    setPipelineStatus, setArrowActive, resetPipeline,
    renderTokens, renderExecutionResult, renderSummary
  };
})();
