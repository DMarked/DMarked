var updateText = function (text) {
  var defaults = {
    html: true,                // Enable HTML tags in source
    xhtmlOut: false,            // Use '/' to close single tags (<br />)
    breaks: false,              // Convert '\n' in paragraphs into <br>
    langPrefix: 'language-',    // CSS language prefix for fenced blocks
    linkify: true,              // autoconvert URL-like texts to links
    typographer: true,          // Enable smartypants and other sweet transforms
  };

  // 参考 https://github.com/yansenlei/markdown-it-mermaid
  const mermaidChart = (code) => {
    try {
      // mermaid.parse(code)
      // return `<div class="mermaid">${code}</div>`
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
    .use(window.markdownitEmoji)
    .use(window.markdownitFootnote)
    .use(texmath, {
      engine: katex,
      delimiters: 'dollars',
      katexOptions: { macros: { "\\RR": "\\mathbb{R}" } }
    })
    .use(window.markdownitContainer, 'spoiler', {
      validate: function(params) {
        return params.trim().match(/^spoiler\s+(.*)$/);
      },
      render: function (tokens, idx) {
        var m = tokens[idx].info.trim().match(/^spoiler\s+(.*)$/);
        if (tokens[idx].nesting === 1) {
          // opening tag
          return '<details><summary>' + md.utils.escapeHtml(m[1]) + '</summary>\n';
        } else {
          // closing tag
          return '</details>\n';
        }
      }
    })
    .use(window.markdownItAnchor, {
      permalink: window.markdownItAnchor.permalink.linkAfterHeader({
        style: 'visually-hidden',
        assistiveText: title => `Permalink to “${title}”`,
        visuallyHiddenClass: 'visually-hidden',
      })
   })  
   .use(window.markdownItTocDoneRight, { slugify: uslugify });
   //.use(require('MarkdownItImsize'),  { autofill: true });

  document.getElementById('placeholder').innerHTML = md.render(text);
  mermaid.initialize({ startOnLoad: true });
}

const dmarked_setup = () => new QWebChannel(qt.webChannelTransport,
  function (channel) {
    var content = channel.objects.content;
    updateText(content.text);
    content.textChanged.connect(updateText);
  }
);
