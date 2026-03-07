# DMarked

使用 DTK 开发的 Markdown 编辑器。

> **重要更新 v0.4.0**: DMarked 已迁移到 Qt6 和 DTK6。

项目地址: [DMarked (GitHub)](https://github.com/DMarked/DMarked) & [Gitee](https://gitee.com/rewine/DMarked)

介绍帖: [https://bbs.deepin.org/en/post/228829](https://bbs.deepin.org/en/post/228829)

## 功能亮点

- 双栏编辑与实时预览，支持阅读/编辑/预览模式切换
- Markdown 扩展能力完整: Mermaid、KaTeX、highlight.js、markdown-it
- 支持导出 PDF、HTML，并可通过命令行批量转换
- 提供主题切换、字符统计、光标位置显示
- 支持配置保存与设置面板（字体、快捷键、自动保存等）
- 已支持 Qt6 / DTK6

## 演示截图

主体界面

![主体界面](dist/dmarked_20260307_155010_997.png)

设置界面示例

![设置界面](dist/dmarked_20260307_155205_614.png)

## 界面与模式

底部的信息栏可以切换 Markdown 主题。

模式分为:

- 阅读模式: 只有右侧的预览窗口
- 编辑模式: 只有左侧的编辑窗口
- 预览模式: 一边编辑，一边预览
- 预览模式 S: 编辑窗口和预览窗口滑动时自动同步（算法待改进）
- 预览模式 N: 编辑窗口和预览窗口各自独立滑动

## 命令行操作

```bash
$ dmarked -h
用法: dmarked [选项] source [destination]

选项:
  -h, --help                显示这个此帮助。
  -v, --version             显示版本信息。
  -w, --write <file>        在 DMarked 中打开 markdown 文件
  -f, --format <pdf>        把 markdown 文件转换为 pdf/html
  -L, --use-landscape       设置为横向，默认纵向
  -s, --page-size <A4>      设置页面大小为 A4，Letter 等
  -l, --margin-left <10>    设置左边距(pt)
  -t, --margin-top <10>     设置上边距(pt)
  -r, --margin-right <10>   设置右边距(pt)
  -b, --margin-bottom <10>  设置下边距(pt)
  -d, --max-depth <1>       批量转换时文件的最大深度

参数:
  source                    需要转换的文件或其所在目录
  [destination]             转换后文件的位置
```

设计命令行模式的初衷是为了完成批量文件转换，目前基本达成目的。

下面命令把 `a.md` 转换成 `my.html`:

```bash
dmarked -f html a.md my.html
```

把当前目录以及 2 层子目录的所有 Markdown 文件转换成 PDF。`-s` 的具体选项可以在图形界面下拉框里查看，主题使用上次打开软件时的主题。

`destination` 不填的话，导出的 PDF 放在 md 文件同一路径下。

```bash
dmarked -f pdf -s A5 -t 20 -b 20 -d 3 .
```

把 `content` 目录下的所有 md 文件（不包括子目录）转换成 PDF 放进 `content_pdf` 文件夹里:

```bash
dmarked -f pdf content content_pdf
```

## 依赖

Debian/Deepin 可使用以下命令安装构建依赖:

```bash
sudo apt install qt6-base-dev qt6-svg-dev libdtk6widget-dev libdtk6gui-dev libdtk6core-dev qt6-webengine-dev qt6-5compat-dev cmake
```

## 获取源码

```bash
git clone --recurse-submodules git@github.com:DMarked/DMarked.git
cd DMarked
```

如果你已经 clone 过仓库，可补拉子模块:

```bash
git submodule update --init --recursive
```

如果访问 GitHub 不方便，可使用 Gitee 的依赖仓库:

- `https://gitee.com/DMarked/qmarkdowntextedit.git`
- `https://gitee.com/DMarked/FakeVim`

## 构建与安装 (CMake)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
sudo cmake --install build
```

## License

DMarked is licensed under GPLv3.

## 使用的第三方库

| 分类 | 项目 | License |
| --- | --- | --- |
| 核心库 | [qmarkdowntextedit](https://github.com/pbek/qmarkdowntextedit) | MIT |
| 核心库 | [markdown-it](https://github.com/markdown-it/markdown-it) | MIT |
| 核心库 | [And More](https://github.com/wineee/DMarked/tree/main/resources/3rdscripts) | - |
| 主题 | [wecatch/markdown-css](https://github.com/wecatch/markdown-css) | MIT |
| 主题 | [yrgoldteeth/darkdowncss](https://github.com/yrgoldteeth/darkdowncss) | - |
| 主题 | [kevinburke/markdown-css](https://bitbucket.org/kevinburke/markdowncss) | Apache V2.0 |
| 主题 | [CuteMarkEd-NG's Theme](https://github.com/Waqar144/CuteMarkEd-NG) ([LICENSE](https://github.com/Waqar144/CuteMarkEd-NG/blob/develop/LICENSE.md)) | - |
| 参考代码 | [deepin-editor](https://github.com/linuxdeepin/deepin-editor) | GPL-3.0 |
| 参考代码 | [WebEngine Markdown Editor Example](https://doc.qt.io/qt-5/qtwebengine-webenginewidgets-markdowneditor-example.html) | BSD |

Organization's profile picture @[RigoLigo](https://github.com/RigoLigoRLC)
