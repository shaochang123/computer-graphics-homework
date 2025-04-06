<h2>如何运行</h2><br>
把 <b>src/Experi</b> 里面相应实验的代码贴到main.cpp上，再在项目主文件夹终端输入<code>make run</code>即可。<br>
如果输入<code>make run</code>后提示没有make命令，改为输入<code>mingw32-make run</code>。<br>

<h2>窗口操作</h2>
按<code>ctrl+z</code>code>可以撤销上一次操作<br>
按<code>ctrl+s</code>可以将图像保存到当前目录下<br>
目前图形支持一下几种颜色：
<code>ctrl+r</code>红色颜色
<code>ctrl+g</code>绿色颜色
<code>ctrl+b</code>蓝色颜色
<code>ctrl+y</code>黄色颜色
<code>ctrl+c</code>青色颜色
<code>ctrl+m</code>洋红色颜色
<code>ctrl+k</code>黑色颜色
<code>ctrl+w</code>白色颜色
<h2>one.cpp</h2> <br>
绘制直线和圆弧，窗口启动之后，可以按键盘上的l或者c来切换直线或者圆弧模式<br>
线宽设置：按数字键1,2,3,4,5可以调整线宽模式，数字越大线越宽。<br>
直线模式：鼠标左键设置起始点，此时直线另一端会随鼠标移动，再次按下左键完成直线的绘制。<br>
圆弧模式：鼠标左键设置圆心，再次按下左键固定半径，此时会以<b>顺时针角度</b>绘制圆弧，再次按下左键完成绘制。<br>
<img src="image.png"></img>
<h2>two.cpp</h2>
填充：键盘按下F，对一块4连通区域鼠标左键双击，可利用种子填充算法+bfs填充该区域。<br>
多边形扫描线：按下P进入扫描线模式，左键选择点，右键完成创建。
