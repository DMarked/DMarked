#!/usr/bin/env bash

# Debian 包构建脚本
# 需要安装 devscripts 包: sudo apt install devscripts

set -e

if ! type debuild >/dev/null 2>&1; then
   echo "错误: 未安装 debuild"
   echo "请运行: sudo apt install devscripts"
   exit 1
fi

# 初始化子模块
if [ ! -f "3rdparty/FakeVim/CMakeLists.txt" ] || [ ! -f "3rdparty/qmarkdowntextedit/CMakeLists.txt" ]; then
    echo "初始化 Git 子模块..."
    git submodule init
    git submodule update
fi

echo "开始构建 Debian 包..."
# -uc: 不签名 .changes 文件
# -us: 不签名源码包
# -tc: 构建后清理
# -b: 只构建二进制包
debuild -uc -us -tc -b

echo ""
echo "构建完成！生成的包文件在上级目录中。"
