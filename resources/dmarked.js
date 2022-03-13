var dmarked_content;
var dmarked_config;
var dmarked_isDark = false;
var dmarked_pangu = true;

/**************************************************************************/

function setMarkdownTheme(theme) {
  if (theme === 'none')
    document.getElementById('cssplaceholder').innerHTML = '';
  else
    document.getElementById('cssplaceholder').innerHTML = '<link rel=\"stylesheet\" type=\"text/css\" href=\"themes/' + theme + '.css\">';
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
function filepath2Absolute(base, relative) {
  var stack = base.split("/"),
      parts = relative.split("/");
  stack.pop(); // remove current file name (or empty string)
  // (omit if "base" is the current folder without trailing slash)
  for (var i = 0; i < parts.length; i++) {
    if (parts[i] === ".")
      continue;
    if (parts[i] === "..")
      stack.pop();
    else
      stack.push(parts[i]);
  }
  return stack.join("/");
}

var updateText = function () {
  var defaults = {
    html:        dmarked_config.html,             // Enable HTML tags in source
    xhtmlOut:    dmarked_config.xhtmlOut,         // Use '/' to close single tags (<br />)
    breaks:      dmarked_config.breaks,           // Convert '\n' in paragraphs into <br>
    langPrefix:  'language-',                     // CSS language prefix for fenced blocks
    linkify:     dmarked_config.linkify,          // autoconvert URL-like texts to links
    typographer: dmarked_config.typographer,      // Enable smartypants and other sweet transforms
  };

  var mermaidConfigs = {
    startOnLoad:    true,
    darkMode:       dmarked_isDark,
    theme:          dmarked_isDark ? 'dark' : 'default',
    securityLevel: 'strict'//'loose',
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
    } else if (lang === 'mermaid') {
      return mermaidChart(str)
    } else {
      return '<pre class="hljs"><code>' + esc(str) + '</code></pre>';
    }
  };

  var uslugify = function (s) {
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

  // https://github.com/markdown-it/markdown-it/blob/master/docs/architecture.md#renderer
  md.renderer.rules.image = function (tokens, idx, options, env, self) {
    var token = tokens[idx],
      aIndexSrc = token.attrIndex('src'),
      aIndexAlt = token.attrIndex('alt');

    // http or https
    if (token.attrs[aIndexSrc][1].substr(0, 4) === "http")
      return self.renderToken(tokens, idx, options, env, self);

    // relative file path
    if (token.attrs[aIndexSrc][1][0] !== '/')
      token.attrs[aIndexSrc][1] = filepath2Absolute(dmarked_content.path, token.attrs[aIndexSrc][1]);

    //console.log(path, token.attrs[aIndexSrc]);
    return '<p>' + '<img src="file://' + token.attrs[aIndexSrc][1] + '" alt="' + token.attrs[aIndexAlt][1] + '">' + '</p>';
  };

  if (dmarked_pangu)
    text = pangu.spacing(dmarked_content.text);
  else
    text = dmarked_content.text;
  document.getElementById('placeholder').innerHTML = md.render(text);
  mermaid.initialize(mermaidConfigs);
  dmarked_content.onMdLoadFinished();
}

const dmarked_setup = () => new QWebChannel(qt.webChannelTransport,
  function (channel) {
    dmarked_content = channel.objects.content;
    dmarked_config = channel.objects.config;
    updateText();
    dmarked_content.textChanged.connect(updateText);
  }
);
