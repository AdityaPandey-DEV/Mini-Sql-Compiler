/**
 * SQL Editor — Line numbers, cursor tracking, and example loading
 */
const Editor = (() => {
  let textarea;
  let lineNumbers;
  let cursorInfo;

  function init() {
    textarea = document.getElementById('sql-editor');
    lineNumbers = document.getElementById('line-numbers');
    cursorInfo = document.getElementById('cursor-info');

    // Update line numbers on input
    textarea.addEventListener('input', updateLineNumbers);
    textarea.addEventListener('scroll', syncScroll);
    textarea.addEventListener('keyup', updateCursorInfo);
    textarea.addEventListener('click', updateCursorInfo);

    // Handle Tab key
    textarea.addEventListener('keydown', (e) => {
      if (e.key === 'Tab') {
        e.preventDefault();
        const start = textarea.selectionStart;
        const end = textarea.selectionEnd;
        textarea.value = textarea.value.substring(0, start) + '  ' + textarea.value.substring(end);
        textarea.selectionStart = textarea.selectionEnd = start + 2;
        updateLineNumbers();
      }
    });

    // Initial line numbers
    updateLineNumbers();
    updateCursorInfo();
  }

  function updateLineNumbers() {
    const lines = textarea.value.split('\n');
    let html = '';
    for (let i = 1; i <= Math.max(lines.length, 1); i++) {
      html += `<div>${i}</div>`;
    }
    lineNumbers.innerHTML = html;
  }

  function syncScroll() {
    lineNumbers.scrollTop = textarea.scrollTop;
  }

  function updateCursorInfo() {
    const val = textarea.value;
    const pos = textarea.selectionStart;
    const lines = val.substring(0, pos).split('\n');
    const line = lines.length;
    const col = lines[lines.length - 1].length + 1;
    cursorInfo.textContent = `Ln ${line}, Col ${col}`;
  }

  function getValue() {
    return textarea.value;
  }

  function setValue(text) {
    textarea.value = text;
    updateLineNumbers();
    textarea.focus();
  }

  function focus() {
    textarea.focus();
  }

  return { init, getValue, setValue, focus };
})();
