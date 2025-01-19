// Populate the sidebar
//
// This is a script, and not included directly in the page, to control the total size of the book.
// The TOC contains an entry for each page, so if each page includes a copy of the TOC,
// the total size of the page becomes O(n**2).
class MDBookSidebarScrollbox extends HTMLElement {
    constructor() {
        super();
    }
    connectedCallback() {
        this.innerHTML = '<ol class="chapter"><li class="chapter-item expanded affix "><a href="introduction.html">Introduction</a></li><li class="chapter-item expanded affix "><a href="rfc.html">RFC</a></li><li class="chapter-item expanded affix "><li class="spacer"></li><li class="chapter-item expanded affix "><li class="part-title">Engine development</li><li class="chapter-item expanded "><a href="engine/building.html"><strong aria-hidden="true">1.</strong> Building</a></li><li class="chapter-item expanded "><a href="engine/server.html"><strong aria-hidden="true">2.</strong> Server</a></li><li class="chapter-item expanded "><a href="engine/client.html"><strong aria-hidden="true">3.</strong> Client</a></li><li class="chapter-item expanded "><a href="engine/networking.html"><strong aria-hidden="true">4.</strong> Networking</a></li><li class="chapter-item expanded "><a href="engine/nodeSystem.html"><strong aria-hidden="true">5.</strong> NodeEngine</a></li><li class="chapter-item expanded affix "><li class="spacer"></li><li class="chapter-item expanded affix "><li class="part-title">Game development</li><li class="chapter-item expanded "><a href="game/filesystem.html"><strong aria-hidden="true">6.</strong> Filesystem</a></li><li class="chapter-item expanded "><a href="game/luau.html"><strong aria-hidden="true">7.</strong> LUAU</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="game/libs.html"><strong aria-hidden="true">7.1.</strong> Libraries</a></li><li class="chapter-item expanded "><a href="game/ServerMethod.html"><strong aria-hidden="true">7.2.</strong> Server</a></li><li class="chapter-item expanded "><a href="game/ClientMethod.html"><strong aria-hidden="true">7.3.</strong> Client</a></li><li class="chapter-item expanded "><a href="game/common.html"><strong aria-hidden="true">7.4.</strong> Common</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="game/NodeLibrary.html"><strong aria-hidden="true">7.4.1.</strong> Node</a></li><li class="chapter-item expanded "><a href="game/Node2DLibrary.html"><strong aria-hidden="true">7.4.2.</strong> Node2D</a></li><li class="chapter-item expanded "><a href="game/Sprite2DLibrary.html"><strong aria-hidden="true">7.4.3.</strong> Sprite2D</a></li><li class="chapter-item expanded "><a href="game/Area2DLibrary.html"><strong aria-hidden="true">7.4.4.</strong> Area2D</a></li><li class="chapter-item expanded "><a href="game/RigidBody2DLibrary.html"><strong aria-hidden="true">7.4.5.</strong> RigidBody2D</a></li><li class="chapter-item expanded "><a href="game/StaticBody2DLibrary.html"><strong aria-hidden="true">7.4.6.</strong> StaticBody2D</a></li><li class="chapter-item expanded "><a href="game/CollisionShape2DLibrary.html"><strong aria-hidden="true">7.4.7.</strong> CollisionShape2D</a></li><li class="chapter-item expanded "><a href="game/LabelLibrary.html"><strong aria-hidden="true">7.4.8.</strong> Label</a></li><li class="chapter-item expanded "><a href="game/ParallaxLibrary.html"><strong aria-hidden="true">7.4.9.</strong> Parallax</a></li><li class="chapter-item expanded "><a href="game/BoxLibrary.html"><strong aria-hidden="true">7.4.10.</strong> Box</a></li></ol></li></ol></li><li class="chapter-item expanded "><li class="spacer"></li><li class="chapter-item expanded affix "><li class="part-title">Games</li><li class="chapter-item expanded "><a href="games/r-type.html"><strong aria-hidden="true">8.</strong> R-Type</a></li><li class="chapter-item expanded "><a href="games/pong.html"><strong aria-hidden="true">9.</strong> Pong</a></li></ol>';
        // Set the current, active page, and reveal it if it's hidden
        let current_page = document.location.href.toString();
        if (current_page.endsWith("/")) {
            current_page += "index.html";
        }
        var links = Array.prototype.slice.call(this.querySelectorAll("a"));
        var l = links.length;
        for (var i = 0; i < l; ++i) {
            var link = links[i];
            var href = link.getAttribute("href");
            if (href && !href.startsWith("#") && !/^(?:[a-z+]+:)?\/\//.test(href)) {
                link.href = path_to_root + href;
            }
            // The "index" page is supposed to alias the first chapter in the book.
            if (link.href === current_page || (i === 0 && path_to_root === "" && current_page.endsWith("/index.html"))) {
                link.classList.add("active");
                var parent = link.parentElement;
                if (parent && parent.classList.contains("chapter-item")) {
                    parent.classList.add("expanded");
                }
                while (parent) {
                    if (parent.tagName === "LI" && parent.previousElementSibling) {
                        if (parent.previousElementSibling.classList.contains("chapter-item")) {
                            parent.previousElementSibling.classList.add("expanded");
                        }
                    }
                    parent = parent.parentElement;
                }
            }
        }
        // Track and set sidebar scroll position
        this.addEventListener('click', function(e) {
            if (e.target.tagName === 'A') {
                sessionStorage.setItem('sidebar-scroll', this.scrollTop);
            }
        }, { passive: true });
        var sidebarScrollTop = sessionStorage.getItem('sidebar-scroll');
        sessionStorage.removeItem('sidebar-scroll');
        if (sidebarScrollTop) {
            // preserve sidebar scroll position when navigating via links within sidebar
            this.scrollTop = sidebarScrollTop;
        } else {
            // scroll sidebar to current active section when navigating via "next/previous chapter" buttons
            var activeSection = document.querySelector('#sidebar .active');
            if (activeSection) {
                activeSection.scrollIntoView({ block: 'center' });
            }
        }
        // Toggle buttons
        var sidebarAnchorToggles = document.querySelectorAll('#sidebar a.toggle');
        function toggleSection(ev) {
            ev.currentTarget.parentElement.classList.toggle('expanded');
        }
        Array.from(sidebarAnchorToggles).forEach(function (el) {
            el.addEventListener('click', toggleSection);
        });
    }
}
window.customElements.define("mdbook-sidebar-scrollbox", MDBookSidebarScrollbox);
