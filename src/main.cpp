#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <fstream>
#define M_PI 3.14159265358979323846
struct Point {int x, y;}st,Arco,ArcA; // 定义点结构体，直线的初始点st，圆弧的圆心Arco和圆弧上的点ArcA
#define maxn 200 // 定义像素网格大小
int w=1;
uint8_t g[maxn][maxn],cnt=1,mode=0,ArcStep=0;//g是到时候显示在窗口上的像素网格，cnt是每个图形的时间戳（为了撤回），mode是模式，ArcStep是绘制圆弧的步骤
bool isMousePressed = false; // 鼠标是否按下，实际上只为直线模式服务。
// 保存图像函数
void saveImage(const char* filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }
    // 写入文件头
    file.put(0).put(0).put(2); // 无压缩的TGA格式
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(0).put(0);
    file.put(maxn & 0x00FF).put((maxn & 0xFF00) >> 8);
    file.put(maxn & 0x00FF).put((maxn & 0xFF00) >> 8);
    file.put(24); // 24位颜色深度
    file.put(0);  
}
// 转换函数，将窗口坐标转换为g数组的索引
void detectposition(GLFWwindow *window, double &xpos, double &ypos) {
    glfwGetCursorPos(window, &xpos, &ypos);
    // 将窗口坐标转换为OpenGL坐标
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    xpos = xpos / width * maxn;
    ypos = maxn - ypos / height * maxn;
}
// 渲染函数，将g数组的像素网格渲染到窗口上
void render(){
    glClear(GL_COLOR_BUFFER_BIT);//清空颜色缓冲区
    glMatrixMode(GL_MODELVIEW);//设置模型视图矩阵
    glLoadIdentity();//重置当前的模型视图矩阵

    // 遍历每个像素，绘制矩形
    for (int i = 0; i < maxn; i++) {
        for (int j = 0; j < maxn; j++) {
            // 设置颜色
            if (g[j][i]) {
                glColor3f(0.0f, 0.0f, 0.0f); // 黑色
            } else {
                glColor3f(1.0f, 1.0f, 1.0f); // 白色
            }
            // 绘制矩形（每个像素占1x1的正方形）
            glBegin(GL_QUADS);
            glVertex2f(j, i);        // 左下角
            glVertex2f(j+1, i);      // 右下角
            glVertex2f(j+1, i+1);    // 右上角
            glVertex2f(j, i+1);      // 左上角
            glEnd();
        }
    }
}
// Bresenham直线算法
void drawLineBresenham(Point start, Point end, bool arg=false) {//这里的arg是为了区分是否画虚线，为True时画虚线
    const int dx = abs(end.x - start.x), sx = start.x < end.x ? 1 : -1;
    const int dy = -abs(end.y - start.y), sy = start.y < end.y ? 1 : -1;
    int err = dx + dy, e2,mark=1;
    while (true) {
        if(start.x-1>=0&&start.y-1>=0&&start.x-1<maxn&&start.y-1<maxn&&g[start.x-1][start.y-1]==0&&mark%3!=0&&(mark+1)%3!=0)
        {g[start.x-1][start.y-1] = cnt;
        if(w==2){
            g[start.x-1][start.y-1] = cnt;
            g[start.x-2][start.y-1] = cnt;
            g[start.x-1][start.y-2] = cnt;
            g[start.x-2][start.y-2] = cnt;}
        if(w==3){
            g[start.x-1][start.y-1] = cnt;
            g[start.x-2][start.y-1] = cnt;
            g[start.x-1][start.y-2] = cnt;
            g[start.x-3][start.y-1] = cnt;
            g[start.x-1][start.y-3] = cnt;
            g[start.x-3][start.y-2] = cnt;
            g[start.x-2][start.y-3] = cnt;
            g[start.x-2][start.y-2] = cnt;
            g[start.x-3][start.y-3] = cnt;}
        }//把对应的像素点“涂黑”
        //为何要加上 g[start.x-1][start.y-1] == 0？因为如果这个点已经被画过了，就不要再画了，否则在撤回时会导致另外一个图形出现断点
        if (start.x == end.x && start.y == end.y) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; start.x += sx; }
        if (e2 <= dx) { err += dx; start.y += sy; }
        if(arg)mark++;
    }
}
//直线中点算法
void middleLine(Point start, Point end) {
    int dx = abs(end.x - start.x);
    int dy = abs(end.y - start.y);
    int sx = start.x < end.x ? 1 : -1;
    int sy = start.y < end.y ? 1 : -1;

    bool steep = dy > dx; // 判断斜率是否大于1
    if (steep) {
        std::swap(dx, dy); // 交换dx和dy
    }

    int d = 2 * dy - dx; // 初始决策变量
    int x = start.x, y = start.y;

    for (int i = 0; i <= dx; i++) {
        // 绘制点
        if (x >= 0 && x < maxn && y >= 0 && y < maxn && g[x][y] == 0) {
            g[x][y] = cnt; // 将像素点涂黑
        }

        // 更新决策变量和坐标
        if (d > 0) {
            if (steep) {
                x += sx;
            } else {
                y += sy;
            }
            d -= 2 * dx;
        }
        if (steep) {
            y += sy;
        } else {
            x += sx;
        }
        d += 2 * dy;
    }
}
void drawLine(GLFWwindow *window){
    double xpos, ypos;
    detectposition(window, xpos, ypos);
    if(isMousePressed ==true){//如果已经按下鼠标（确定好了第一个点），那么就以按下的点和当前鼠标位置为直线的两个端点
        drawLineBresenham(st, {static_cast<int>(xpos), static_cast<int>(ypos)});
        // middleLine(st, {static_cast<int>(xpos), static_cast<int>(ypos)});
    }
    render();
}
// 圆弧绘制算法
void checkAndDraw(int px, int py, Point center, double startRad, double endRad) {
    if (px < 0 || px >= maxn || py < 0 || py >= maxn) return;
    int dx = px - center.x;
    int dy = center.y - py;
    double theta = atan2(dy, dx);
    if (theta < 0) theta += 2 * M_PI;
    bool inArc = (theta >= startRad && theta <= endRad) || //在起始角度和终止角度之间
                (startRad > endRad && (theta >= startRad || theta <= endRad));//起始角度到终止角度跨越了0度，就要特殊考虑
    
    if (inArc && g[px][py] == 0) g[px][py] = cnt;
}

// 优化后的圆弧绘制算法
void drawarc(Point center, int r, double startRad, double endRad) {
    int x = 0, y = r;
    int d = 1 - r;
    
    while (x <= y) {
        //这里按照象限分了八个区域，更方便处理
        checkAndDraw(center.x + x, center.y + y, center, startRad, endRad);
        checkAndDraw(center.x - x, center.y + y, center, startRad, endRad);
        checkAndDraw(center.x + x, center.y - y, center, startRad, endRad);
        checkAndDraw(center.x - x, center.y - y, center, startRad, endRad);
        checkAndDraw(center.x + y, center.y + x, center, startRad, endRad);
        checkAndDraw(center.x - y, center.y + x, center, startRad, endRad);
        checkAndDraw(center.x + y, center.y - x, center, startRad, endRad);
        checkAndDraw(center.x - y, center.y - x, center, startRad, endRad);
        
        if (d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}
// 圆弧模式下绘制像素网格
void drawArc(GLFWwindow *window){
    double xpos, ypos;
    detectposition(window, xpos, ypos);
    if(ArcStep==1){
        drawLineBresenham(Arco, {static_cast<int>(xpos), static_cast<int>(ypos)},true);//最后的参数变为true，表示画虚线
    }
    else if(ArcStep==2){
        double r=sqrt((ArcA.x-Arco.x)*(ArcA.x-Arco.x)+(ArcA.y-Arco.y)*(ArcA.y-Arco.y));//半径
        double startRad = atan2(Arco.y - ArcA.y, ArcA.x - Arco.x);//起始角度
        double endRad = atan2(Arco.y - ypos, xpos - Arco.x);//终止角度
        if (startRad < 0) startRad += 2 * M_PI;//保证角度在0到2π之间
        if(endRad < 0) endRad += 2*M_PI;//保证角度在0到2π之间
        drawarc(Arco, r, startRad, endRad);
    }
    render();
}
// 鼠标点击回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {//左键按下
        if(mode==1){//如果是圆弧模式
            if(ArcStep==0){//确定圆心
                double xpos, ypos;
                detectposition(window, xpos, ypos);
                Arco.x=static_cast<int>(xpos);Arco.y=static_cast<int>(ypos);
            }
            else if(ArcStep==1){//确定圆弧上的一个点(固定半径)
                double xpos, ypos;
                detectposition(window, xpos, ypos);
                ArcA.x=static_cast<int>(xpos);ArcA.y=static_cast<int>(ypos);
            }
            else if(ArcStep==2){//圆弧画好后，cnt++
                drawArc(window);
                cnt++;
            } 
            ArcStep=(ArcStep+1)%3;//每次点击左键，ArcStep+1，画完了重置为0
            return;
        }
        if(isMousePressed == true){
            drawLine(window);
            //这里是防止画完直线后就被窗口刷新掉，所以在这里再画一次
            cnt++;
            isMousePressed =false;
            st.x=0;st.y=0;
            return;
        }
        
        isMousePressed = true;
        double xpos,ypos;
        detectposition(window, xpos, ypos);
        st.x=static_cast<int>(xpos);st.y=static_cast<int>(ypos);
        
    }
}

// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Z && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {//撤回操作
        // 将 g 上面为 cnt 的格子变为白色
        for (int i = 0; i < maxn; i++) {
            for (int j = 0; j < maxn; j++) {
                if (g[i][j] == cnt-1) {
                    g[i][j] = 0;
                }
            }
        }
        cnt--;//退时间戳
        if(cnt<1)cnt=1;//防止撤回过多
    }
    if(key == GLFW_KEY_L && action == GLFW_PRESS){//切换到直线模式
        isMousePressed = false;
        mode=0;
        std::cout<<"Line Mode"<<std::endl;

    }
    if(key == GLFW_KEY_C && action == GLFW_PRESS){//切换到圆弧模式
        ArcStep = 0;
        mode=1;
        std::cout<<"Circle Mode"<<std::endl;
    }
    if(key == GLFW_KEY_1 && action == GLFW_PRESS){//切换线宽1
        w=1;
        std::cout<<"width1"<<std::endl;
    }
    if(key == GLFW_KEY_2 && action == GLFW_PRESS){//切换线宽2
        w=2;
        std::cout<<"width2"<<std::endl;
    }
    if(key == GLFW_KEY_3 && action == GLFW_PRESS){//切换线宽3
        w=3;
        std::cout<<"width3"<<std::endl;
    }
    if (key == GLFW_KEY_S && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {//保存图像
        saveImage("output.tga");
    }
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // 调整视口大小
    glViewport(0, 0, width, height);
    // 重新设置正交投影
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();//重置当前的投影矩阵
    glOrtho(0.0, maxn, 0.0, maxn, -1.0, 1.0);//设置正交投影
}
int main() {
    if (!glfwInit()) return -1;
    const int width = 800, height = 800;
    GLFWwindow* window = glfwCreateWindow(width, height, "painting toolbox", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 注册窗口大小回调函数，这个函数可以在你拉伸窗口后调整图像大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 注册鼠标点击回调函数，用来鼠标点击画图
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    // 注册键盘回调函数
    glfwSetKeyCallback(window, key_callback);
    
    // 初始化投影
    framebuffer_size_callback(window, width, height);
    
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT); // 清空颜色缓冲区
        if(mode == 0){
            drawLine(window);//渲染直线
        }
        else{
            drawArc(window);//渲染圆弧
        }
        // 刷新屏幕
        for(int i=0;i<maxn;i++){
            for(int j=0;j<maxn;j++){
                if(g[i][j]==cnt)g[i][j]=0;
            }
        }
        // framebuffer_size_callback(window, maxn, maxn);
        glfwSwapBuffers(window);//交换缓冲区
        glfwPollEvents();//处理所有未决事件
    }

    glfwTerminate();
    std::cout<<"bye\n"<<std::endl;//bye
    return 0;
}



