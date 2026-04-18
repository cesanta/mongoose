document.addEventListener("DOMContentLoaded", () => {
  const tabs = Array.from(document.querySelectorAll(".header-left .nav-link[href^='#']"));
  const panels = Array.from(document.querySelectorAll(".tab-panel"));

  const activateTab = targetId => {
    const targetPanel = document.getElementById(targetId);
    if (!targetPanel) return;

    tabs.forEach(tab => {
      const isActive = tab.getAttribute("href") === `#${targetId}`;
      tab.classList.toggle("nav-link-active", isActive);
      tab.setAttribute("aria-selected", String(isActive));
      tab.setAttribute("tabindex", isActive ? "0" : "-1");
    });

    panels.forEach(panel => {
      const isActive = panel.id === targetId;
      panel.classList.toggle("tab-panel-active", isActive);
      panel.hidden = !isActive;
    });
  };

  tabs.forEach(tab => {
    tab.addEventListener("click", event => {
      event.preventDefault();
      const targetId = tab.getAttribute("href").slice(1);
      activateTab(targetId);
      window.history.replaceState(null, "", `#${targetId}`);
    });
  });

  const initialId = window.location.hash.slice(1) || "overview";
  activateTab(initialId);
});
