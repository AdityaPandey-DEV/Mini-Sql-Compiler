/**
 * Animations module — Phase transitions, token flow, success/fail effects
 */
const Animations = (() => {
  /**
   * Stagger-animate child elements with a delay between each
   */
  function staggerIn(parent, selector, delayMs = 30) {
    const items = parent.querySelectorAll(selector);
    items.forEach((item, i) => {
      item.style.animationDelay = `${i * delayMs}ms`;
    });
  }

  /**
   * Add a temporary class with auto-removal
   */
  function flashClass(element, className, durationMs = 600) {
    element.classList.add(className);
    setTimeout(() => element.classList.remove(className), durationMs);
  }

  /**
   * Smooth scroll to an element
   */
  function scrollIntoView(element) {
    element.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
  }

  /**
   * Typewriter effect for text
   */
  function typewriter(element, text, speedMs = 20) {
    return new Promise(resolve => {
      let i = 0;
      element.textContent = '';
      const interval = setInterval(() => {
        if (i < text.length) {
          element.textContent += text[i];
          i++;
        } else {
          clearInterval(interval);
          resolve();
        }
      }, speedMs);
    });
  }

  /**
   * Wait for a specified duration
   */
  function delay(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

  return { staggerIn, flashClass, scrollIntoView, typewriter, delay };
})();
