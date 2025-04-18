#include<graph/Line.h>
#include<graph/Circle.h>
#include<graph/Fill.h>
#include<graph/Polygon.h>
#include<graph/Crop.h>
#include<graph/FullCircle.h>
// 鼠标点击回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Line_Mouse_Pressed(window, button, action);
    Circle_Mouse_Pressed(window, button, action);
    Full_Circle_Mouse_Pressed(window, button, action);
    Fill_Mouse_Pressed(window, button, action);
    Polygon_Mouse_Pressed(window, button, action);
}
// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Line_Keyboard_Pressed(key, action);
    Circle_Keyboard_Pressed(key, action);
    Full_Circle_Keyboard_Pressed(key, action);
    Fill_Keyboard_Pressed(key, action);
    Polygon_Keyboard_Pressed(key, action);
    Select(key,action);
    ChangeSelectedGraph(key,action);
    backup(key,mods,action);
    frontup(key,mods,action);
    saveImage(key,mods,action);
    ChangeColor(key,action);
    ChangeWidth(key, action);
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
        else if(mode==2) drawFullArc(window);//渲染整圆
        else if(mode==4) drawPolygon(window);//渲染填充
        else render();
        flushwindow();//刷新屏幕
        for(int i=0;i<graphics.size();i++){
            Color choosecolor;
            if(i==ChooseIdx){choosecolor=graphics[i].color;graphics[i].color=selectedColor;}
            if(graphics[i].mode==0){//直线
                drawLineBresenham(graphics[i].points[0],graphics[i].points[1],false, graphics[i].width,graphics[i].color);//最后的参数变为false，表示画实线
            }
            else if(graphics[i].mode==2){//整圆
                drawarc(graphics[i].points[0],sqrt((graphics[i].points[1].x-graphics[i].points[0].x)*(graphics[i].points[1].x-graphics[i].points[0].x)+(graphics[i].points[1].y-graphics[i].points[0].y)*(graphics[i].points[1].y-graphics[i].points[0].y)),0,2*M_PI,graphics[i].width,graphics[i].color);//起始角度和终止角度都为0和2π，表示画一个完整的圆
            }
            else if(graphics[i].mode==1){//圆弧
                // 计算半径
                double r=sqrt((graphics[i].points[1].x-graphics[i].points[0].x)*(graphics[i].points[1].x-graphics[i].points[0].x)+(graphics[i].points[1].y-graphics[i].points[0].y)*(graphics[i].points[1].y-graphics[i].points[0].y));//半径
                
                // 修改角度计算，与Circle.h中保持一致
                double startRad = atan2(graphics[i].points[0].y - graphics[i].points[1].y, graphics[i].points[1].x - graphics[i].points[0].x);
                double endRad = atan2(graphics[i].points[0].y - graphics[i].points[2].y, graphics[i].points[2].x - graphics[i].points[0].x);
                
                if(startRad < 0) startRad += 2*M_PI; // 保证起始角度也在0到2π之间
                if(endRad < 0) endRad += 2*M_PI; // 保证角度在0到2π之间
                
                drawarc(graphics[i].points[0], r, startRad, endRad,graphics[i].width,graphics[i].color);//画圆弧
            }
            else if(graphics[i].mode==3){//填充
                for(int j=0;j<graphics[i].points.size();j++){
                    setpixel(graphics[i].points[j].x,graphics[i].points[j].y,graphics[i].width,graphics[i].color);
                }
            }
            else if(graphics[i].mode==4){//多边形
                fillPolygon(graphics[i].points,graphics[i].width,graphics[i].color);
            }
            if(i==ChooseIdx)graphics[i].color=choosecolor;       }
        glfwSwapBuffers(window);//交换缓冲区
        glfwPollEvents();//处理所有未决事件
        if(mode!=-1)ChooseIdx = -1;
    
    }
    glfwTerminate();std::cout<<"bye\n"<<std::endl;//bye
    return 0;
}