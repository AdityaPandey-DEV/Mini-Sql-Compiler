/**
 * Error Modal — Shows compilation errors with phase info, source context, and caret
 */
const ErrorModal = (() => {
  const overlay = () => document.getElementById('error-modal-overlay');
  const modal = () => document.getElementById('error-modal');
  const title = () => document.getElementById('error-modal-title');
  const phaseBadge = () => document.getElementById('error-modal-phase');
  const body = () => document.getElementById('error-modal-body');
  const icon = () => document.getElementById('error-icon');

  function init() {
    document.getElementById('btn-close-modal').addEventListener('click', hide);
    document.getElementById('btn-dismiss-error').addEventListener('click', hide);
    overlay().addEventListener('click', (e) => {
      if (e.target === overlay()) hide();
    });
    document.addEventListener('keydown', (e) => {
      if (e.key === 'Escape' && !overlay().classList.contains('hidden')) {
        hide();
      }
    });
  }

  /**
   * Show errors in modal popup
   * @param {Array} errors — Array of error objects with: type, phase, message, line, column
   * @param {string} query — The original query text for source context
   */
  function show(errors, query) {
    if (!errors || errors.length === 0) return;

    const firstError = errors[0];
    const phaseClass = getPhaseClass(firstError.phase || firstError.type);

    // Set title
    title().textContent = errors.length === 1
      ? 'Compilation Error'
      : `${errors.length} Compilation Errors`;

    // Set phase badge
    const phase = firstError.phase || firstError.type;
    phaseBadge().textContent = phase;
    phaseBadge().className = `error-phase-badge ${phaseClass}`;

    // Set icon
    icon().className = `error-icon ${phaseClass}`;
    icon().textContent = phaseClass === 'lexical' ? '✗'
      : phaseClass === 'syntax' ? '⚠'
      : '◈';

    // Build error items
    let html = '';
    errors.forEach((err, idx) => {
      const cls = getPhaseClass(err.phase || err.type);
      html += `<div class="error-item ${cls}">`;
      html += `<div class="error-message">${escapeHtml(err.message)}</div>`;

      // Source code context
      if (query) {
        const lines = query.split('\n');
        const lineNum = err.line || 1;
        const col = err.column || 1;
        const sourceLine = lines[lineNum - 1] || query;

        html += `<div class="error-source">`;
        html += `<span class="line-num">${lineNum}</span>`;
        html += `<span class="source-text">${escapeHtml(sourceLine)}</span>`;
        html += `<br>`;
        html += `<span class="line-num"> </span>`;
        html += `<span class="error-caret">${' '.repeat(Math.max(0, col - 1))}^~~~</span>`;
        html += `</div>`;
        html += `<div class="error-location">Line ${lineNum}, Column ${col}</div>`;
      }

      html += `</div>`;
    });

    body().innerHTML = html;

    // Show modal
    overlay().classList.remove('hidden');
  }

  function hide() {
    overlay().classList.add('hidden');
  }

  function getPhaseClass(phase) {
    if (!phase) return 'lexical';
    const p = phase.toLowerCase();
    if (p.includes('lexical') || p.includes('lex')) return 'lexical';
    if (p.includes('syntax') || p.includes('syn')) return 'syntax';
    if (p.includes('semantic') || p.includes('sem')) return 'semantic';
    return 'lexical';
  }

  function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text;
    return div.innerHTML;
  }

  return { init, show, hide };
})();
