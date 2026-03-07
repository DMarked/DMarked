# DMarked

使用 dtk 开发的 markdown 编辑器

> **🎉 重要更新 v0.4.0**: DMarked 已迁移到 Qt6 和 DTK6！
>
> - ✅ 完全支持 Qt6 和 DTK6
> - ✅ 改进的构建系统（CMake）
> - ✅ 更好的性能和兼容性
> - 📖 详见 [Qt6迁移说明.md](Qt6迁移说明.md) 或 [QUICK_START.md](QUICK_START.md)

[介绍帖](https://bbs.deepin.org/en/post/228829) & [github](https://github.com/wineee/DMarked) & [gitee](https://gitee.com/rewine/DMarked)

#### Dependencies

In debian, use below command to install compile dependencies:

`sudo apt install qt6-base-dev qt6-svg-dev libdtk6widget-dev libdtk6gui-dev libdtk6core-dev qt6-webengine-dev qt6-5compat-dev cmake`

#### Clone
- git clone git@github.com:DMarked/DMarked.git
- cd DMarked
- git submodule init
- git submodule update

#### Build
- mkdir build
- cd build
- cmake ..
- make
- sudo make install

#### Nix

In Project Dir:

- nix develop

get develop shell

- nix build ".?submodules=1"

build package

- nix run ".?submodules=1"

just run dmarked


#### TODO
- [x] 新建，打开，另存为 Markdown 文件
- [x] 一边编辑，另一边预览
- [x] 编辑框文字更新后，预览界面自动更新
- [x] 可以切换编辑模式（只有编辑框），阅读模式（只有预览框），预览模式（都有）
- [x] 统计字符数，显示光标行列
- [x] 初步支持中英文
- [x] 导出成 pdf 文件
- [x] 导出成 html 文件
- [x] 可以切换 css 主题，分浅色，深色两类
- [X] 完善对命令行支持，可以在命令行中打开文件，批量转换HTML，PDF
- [ ] 图床工具 --picgo
- [X] 增加 markdown-it 引擎
- [X] 代码高亮 highlight.js
- [X] 画图 Mermaid.js
- [X] 数学公式 katex
- [X] 可保存配置
- [X] 增加设置面板（字体，快捷键，自动保存等）/ 完善快捷键
- [X] 进行打包工作

（Need Help）
- [ ] 更好的编辑页面，预览页面的同步算法
- [ ] 更好的 logo

#### [TODO(VERSION 2.0)](https://github.com/wineee/DMarked/projects/1)

#### License

DMarked is licensed under GPLv3.

#### 使用的第三方库

- [qmarkdowntextedit](https://github.com/pbek/qmarkdowntextedit) MIT
- [markdown-it](https://github.com/markdown-it/markdown-it) MIT
- [And More](https://github.com/wineee/DMarked/tree/main/resources/3rdscripts)

主题使用了：
- [wecatch/markdown-css](https://github.com/wecatch/markdown-css) MIT
- [yrgoldteeth/darkdowncss](https://github.com/yrgoldteeth/darkdowncss)
- [kevinburke/markdown-css](https://bitbucket.org/kevinburke/markdowncss) Apache V2.0
- [CuteMarkEd-NG's Theme](https://github.com/Waqar144/CuteMarkEd-NG) [LICENSE](https://github.com/Waqar144/CuteMarkEd-NG/blob/develop/LICENSE.md)

代码参考了：
- [deepin-editor](https://github.com/linuxdeepin/deepin-editor) GPL-3.0
- [WebEngine Markdown Editor Example](https://doc.qt.io/qt-5/qtwebengine-webenginewidgets-markdowneditor-example.html) BSD

Organization's profile picture @[RigoLigo](https://github.com/RigoLigoRLC)
