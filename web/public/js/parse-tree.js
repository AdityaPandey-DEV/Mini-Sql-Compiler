/**
 * Parse Tree Renderer — Interactive tree visualization from parse tree JSON
 */
const ParseTreeRenderer = (() => {

  /**
   * Render parse tree as nested HTML
   * @param {object} tree — Parse tree node { type, value, children }
   * @returns {string} HTML string
   */
  function render(tree) {
    if (!tree) return '<p class="empty-state"><span>No parse tree available</span></p>';

    let nodeIndex = 0;
    function renderNode(node, depth) {
      if (!node) return '';

      const idx = nodeIndex++;
      const delay = idx * 40;
      let html = '';

      html += `<div class="tree-node" style="--depth: ${depth}">`;
      html += `<div class="tree-node-label" style="animation-delay: ${delay}ms">`;
      html += `<span class="tree-node-type">${escapeHtml(node.type)}</span>`;

      if (node.value) {
        html += `<span class="tree-node-value">"${escapeHtml(node.value)}"</span>`;
      }
      html += `</div>`;

      if (node.children && node.children.length > 0) {
        node.children.forEach(child => {
          html += renderNode(child, depth + 1);
        });
      }

      html += '</div>';
      return html;
    }

    return '<div class="parse-tree-container">' + renderNode(tree, 0) + '</div>';
  }

  function escapeHtml(text) {
    const div = document.createElement('div');
    div.textContent = text || '';
    return div.innerHTML;
  }

  return { render };
})();
