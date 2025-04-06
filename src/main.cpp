#include<graph/Line.h>
#include<graph/Circle.h>
#include<graph/Fill.h>
#include<graph/Polygon.h>
#include<graph/Crop.h>
// 鼠标点击回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Line_Mouse_Pressed(window, button, action, mods);
    Circle_Mouse_Pressed(window, button, action, mods);
    Fill_Mouse_Pressed(window, button, action, mods);
    Polygon_Mouse_Pressed(window, button, action, mods);
}
// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ReturnToBack(key, mods, action);
    Save2Image(key, mods, action);
    Line_Keyboard_Pressed(key, mods, action);
    Circle_Keyboard_Pressed(key, mods, action);
    Fill_Keyboard_Pressed(key, mods, action);
    Polygon_Keyboard_Pressed(key, mods, action);
    ConvertWidth(key,mods,action);
    ConvertColor(key,mods,action);
}
int main() {
    const int width = 800, height = 800;//设置窗口大小
    GLFWwindow* window = init(width, height);
    // 注册鼠标点击回调函数，用来鼠标点击画图
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // 注册键盘回调函数
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // 清空颜色缓冲区
        if(mode == 0) drawLine(window);//渲染直线
        else if(mode==1) drawArc(window);//渲染圆弧
        else if(mode==3) drawPolygon(window);//渲染填充
        else render();
        flushwindow();//刷新屏幕
        glfwSwapBuffers(window);//交换缓冲区
        glfwPollEvents();//处理所有未决事件
    }
    glfwTerminate();std::cout<<"bye\n"<<std::endl;//bye
    return 0;
}