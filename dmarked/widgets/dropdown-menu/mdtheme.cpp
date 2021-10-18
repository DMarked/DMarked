#include "mdtheme.h"

const QStringList MdTheme::light_themes = { "apollo",        "infoq",      "markdown", "simple",  "wecatch-code"
                                          , "less",          "ocean",      "style",    "wecatch", "erye"
                                          , "list-writing",  "sanyuesha",  "typing",   "xiaolai"                };

const QStringList MdTheme::dark_themes = { "darkdown" };

QString MdTheme::light_current_theme = "apollo";
QString MdTheme::dark_current_theme = "darkdown";

