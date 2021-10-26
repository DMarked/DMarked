# DMarked

使用 dtk 开发的 markdown 编辑器

#### Clone
- git clone https://github.com/wineee/DMarked.git
- git submodule init
- git submodule update

#### Build
- mkdir build
- cd build
- qmake ..
- make
- sudo make install

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
- [ ] 多文件，文件列表，文件树
- [ ] 完善对命令行支持，可以在命令行中打开文件，批量转换HTML，PDF
- [ ] 完善快捷键
- [ ] 图床工具 --picgo
- [X] 增加 markdown-it 引擎
- [X] highlight.js
- [X] 画图 Mermaid.js 
- [X] 数学 katex
- [ ] 使用配置文件保存配置
- [ ] 增加设置面板（字体，快捷键，自动保存等）
- [ ] 进行打包工作

（Need Help）
- [ ] 一个 logo
- [ ] 更好的编辑页面，预览页面的同步算法

#### 使用的第三方库

- [qmarkdowntextedit](https://github.com/pbek/qmarkdowntextedit)
- [marked](https://github.com/markedjs/marked) 

主题使用了：
- [wecatch/markdown-css](https://github.com/wecatch/markdown-css)
- [yrgoldteeth/darkdowncss](https://github.com/yrgoldteeth/darkdowncss)
- [kevinburke/markdown-css](https://bitbucket.org/kevinburke/markdowncss)
- [CuteMarkEd-NG](https://github.com/Waqar144/CuteMarkEd-NG)

代码部分参考了：
- [deepin-editor](https://github.com/linuxdeepin/deepin-editor)
- [WebEngine Markdown Editor Example](https://doc.qt.io/qt-5/qtwebengine-webenginewidgets-markdowneditor-example.html)

