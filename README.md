

[![GitHub stars](https://img.shields.io/github/stars/mayunxi/VideoParser_QT.svg)](https://github.com/mayunxi/VideoParser_QT)[![GitHub forks](https://img.shields.io/github/forks/mayunxi/VideoParser_QT.svg)](https://github.com/mayunxi/VideoParser_QT)

Stargazers over time  
[![Stargazers over time](https://starcharts.herokuapp.com/mayunxi/VideoParser_QT.svg)](https://starcharts.herokuapp.com/mayunxi/VideoParser_QT)




# VideoParser -- H.264/AVC H.265/HEVC码流分析工具

## 工程说明
基于Qt5.7.0开发，使用h264bitstream开源项目实现对H.264码流分析。<br>

## 功能
* 支持不同NAL的显示，包括VPS、SPS、PPS、SEI、AUD、Slice的解析。
* 支持显示NAL十六进制数据。
* 支持不同slice的着色显示，显示帧序号。
* 自动解析文件名。
* 支持文件名后缀：
    * H.264文件后缀名为.h264、.h264、.avc。
    * H.265文件后缀名为.h265、.h265、.hevc。
    * 如无上述后缀名，则根据内容自动识别。
* 支持播放H.264、H.265裸码流文件。
* 具备暂停、停止、逐帧播放功能。

## 用法
菜单`文件->打开`选项，工具会自动解析。单击某一项即可查看具体的NAL信息。<br>
点击“播放”菜单出现播放子窗口。

## 界面
V1.0版本界面：<br>



## 测试
本工程使用H264Visa、CodecVisa及HM工具对比测试。<br>
所用视频文件为x264/x265编码生成，另外使用H.265测试序列。<br>
本工具在Windows 10 64bit操作系统和ubuntu16.04 64bit中运行测试通过。<br>


## 协议

* 本工程源码使用LGPL协议。
* 可用于学习研究之目的，也可用于商业目的，但无义务保证程序功能完全可靠。

## 致谢
本工程基于[迟思堂工作室 李迟](http://www.latelee.org)的[https://github.com/latelee/H264BSAnalyzer](https://github.com/latelee/H264BSAnalyzer)MFC版本进行了移植，开发工具为Qt.

## 作者
52Hz
