把 **src/Experi** 里面的代码贴到main.cpp上，再在项目主文件夹终端输入```make run```即可。

##one.cpp
绘制直线和圆，窗口启动之后，可以按键盘上的l或者c来切换直线或者圆模式，按```ctrl+z```可以撤销上一次操作。
直线模式：鼠标左键设置起始点，此时直线另一端会随鼠标移动，再次按下左键完成直线的绘制
圆模式：鼠标左键设置圆心，此时程序会以鼠标和圆心的距离为半径绘制圆形，再次按下左键完成直线的绘制。
注意：绘制图形不能超过窗口，否则会卡死