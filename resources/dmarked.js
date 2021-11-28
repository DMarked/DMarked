var dmarked_isDark = false;
var dmarked_content;

/**************************************************************************/

function setMarkdownTheme(theme) {
  if (theme == 'none')
    document.getElementById('cssplaceholder').innerHTML = '';
  else
    document.getElementById('cssplaceholder').innerHTML = '<link rel=\"stylesheet\" type=\"text/css\" href=\"themes/' + theme + '.css\">';
  dmarked_content.onMdThemeChanged();
}
function setHighlightTheme(theme) {
    document.getElementById('highlightplaceholder').innerHTML =
      '<link rel=\"stylesheet\" type=\"text/css\" href=\"3rdscripts/highlight/' + theme + '.css\">';
}
function setScrollbarsTheme(theme) {
    document.getElementById('scrollbarsplaceholder').innerHTML =
      '<link rel=\"stylesheet\" type=\"text/css\" href=\"' + theme + '.css\">';
}
function setMarkedIsDark(isDark) {
    dmarked_isDark = isDark;
    setScrollbarsTheme(isDark ? "scrollbars_dark" : "scrollbars_light");
}
/**************************************************************************/

var updateText = function (text) {
  var defaults = {
    html:       true,          // Enable HTML tags in source
    xhtmlOut:   false,         // Use '/' to close single tags (<br />)
    breaks:     false,         // Convert '\n' in paragraphs into <br>
    langPrefix: 'language-',   // CSS language prefix for fenced blocks
    linkify:    true,          // autoconvert URL-like texts to links
    typographer:true,          // Enable smartypants and other sweet transforms
  };

  var mermaidConfigs = {
    startOnLoad:    true,
    darkMode:       dmarked_isDark,
    theme:          dmarked_isDark ? 'dark' : 'default',
    securityLevel:  'strict'//'loose',
  };

  // 参考 https://github.com/yansenlei/markdown-it-mermaid
  const mermaidChart = (code) => {
    try {
      var needsUniqueId = "render" + (Math.floor(Math.random() * 10000)).toString();
      mermaid.mermaidAPI.render(needsUniqueId, code, sc => { code = sc })
      return `<div class="mermaid">${code}</div>`
    } catch ({ str, hash }) {
      return `<pre>${str}</pre>`
    }
  }

  defaults.highlight = function (str, lang) {
    //console.log(str);console.log(lang);
    var esc = md.utils.escapeHtml;
    if (lang && hljs.getLanguage(lang)) {
      try {
        return '<pre class="hljs"><code>' +
          hljs.highlight(lang, str, true).value +
          '</code></pre>';
      } catch (__) { }
    } else if (lang == 'mermaid') {
      return mermaidChart(str)
    } else {
      return '<pre class="hljs"><code>' + esc(str) + '</code></pre>';
    }
  };

  var uslugify = function(s) {
    return window.uslug(s);
  }

  var md = window.markdownit(defaults)
    .use(window.markdownitEmoji).use(window.markdownitFootnote)
    .use(window.markdownitSub).use(window.markdownitSup)
    .use(window.markdownitMark).use(window.markdownitDeflist)
    .use(window.markdownitIns).use(window.markdownitAbbr)
    .use(texmath, {
      engine: katex,
      delimiters: 'dollars',
      katexOptions: { macros: { "\\RR": "\\mathbb{R}" } }
    })
    .use(window.markdownitContainer, "info")
    .use(window.markdownitContainer, "success")
    .use(window.markdownitContainer, "warning")
    .use(window.markdownitContainer, "danger")
    .use(window.markdownItAnchor, {
//      permalink: window.markdownItAnchor.permalink.linkAfterHeader({
//        style: 'visually-hidden',
//        assistiveText: title => `Permalink to “${title}”`,
//        visuallyHiddenClass: 'visually-hidden',
//      })
   })  
   .use(window.markdownItTocDoneRight, { slugify: uslugify });

  // Replace emoji codes with images
  //md.renderer.rules.emoji = function(token, idx) {
  //  return window.twemoji.parse(token[idx].content);
  //};

  document.getElementById('placeholder').innerHTML = md.render(text);
  mermaid.initialize(mermaidConfigs);
  dmarked_content.onMdLoadFinished();
}

const dmarked_setup = () => new QWebChannel(qt.webChannelTransport,
  function (channel) {
    dmarked_content = channel.objects.content;
    updateText(dmarked_content.text);
    dmarked_content.textChanged.connect(updateText);
  }
);
