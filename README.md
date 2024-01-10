# chinesechess
Qt版本的中国象棋程序
本项目旨在作为C++Qt项目练手，作为一名中国象棋爱好者，自己写一个具有网络对战，人机对战的中国象棋程序，是非常激动人心的。
## 程序演示
![](https://github.com/mei-lun/chinesechess/blob/main/images/chinesechessimage.png)
## 运行环境
本项目基于vscode + qmake + MSVC2019 + Qt-6.6.0开发
1. 安装对应版本的Qt https://www.qt.io/download-qt-installer
2. 配置vscode的开发环境
## 关于版本
* 工程内使用了少量C++11的语法特性，请使用相应的版本运行环境
* 关于Qt版本，只使用Qt的基础功能和语法如果有的版本编译不通过，涉及接口的修改，自行修改代码运行即可
## 打包发布
使用Qt自带的windeployqt工具打包Windows平台的应用程序，可以找到当前exe所需要的所有动态链接库，并拷贝到当前文件夹
1. 把目标程序复制到新创建的文件夹
2. 运行系统内带C++ Qt环境的控制台，`windeployqt "aim app full path"`
这样就可以生成一个绿色在其他机器上就可以运行的应用程序
## TODO
1. 完成基础版本的AI人机对战，使用基础的人工智能算法(已完成)
2. 升级AI的智能，测试其水平(当前水平很低，算法效率需要提升)
3. 接入现有的适合的人工智能算法，使其变为一个专家系统
