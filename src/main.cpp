#include<graph/Line.h>
#include<graph/Circle.h>
#include<graph/Fill.h>
#include<graph/Polygon.h>
#include<graph/Crop.h>
#include<graph/FullCircle.h>
#include<transform/translation.h>
#include<graph/bezier.h>
#include<graph/bspline.h>
// 鼠标点击回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        // 如果ImGui正在处理鼠标输入，不执行绘图操作
        return;
    }
    HandleDragTranslation(window, button, action, mods);
    Line_Mouse_Pressed(window, button, action);
    Circle_Mouse_Pressed(window, button, action);
    Full_Circle_Mouse_Pressed(window, button, action);
    Fill_Mouse_Pressed(window, button, action);
    Polygon_Mouse_Pressed(window, button, action);
    SelectGraphByClick(window, button, action);
    Bezier_Mouse_Pressed(window, button, action);
    Bezier_Edit_Mouse_Handler(window,button,action,mods);
    newcenter(window,button,action,mods);
    crop_mouse_pressed(window,button,action,mods);
    BSpline_Mouse_Pressed(window,button,action);
    BSpline_Edit_Mouse_Handler(window,button,action,mods);
}
// 键盘回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    ChangeSelectedGraph(key,action);
    backup(key,mods,action);
    frontup(key,mods,action);
    saveImage(key,mods,action);
    Translation(window);
}
void flushwindow();
int main() {
    const int width = 800, height = 800;//设置窗口大小
    // 创建窗口
    GLFWwindow* window = init(width, height);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    // 初始化ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // 设置ImGui风格
    ImGui::StyleColorsDark();

    // 初始化平台/渲染器后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // 再注册其他回调
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // 注册鼠标滚轮回调函数，用来旋转图形
    glfwSetScrollCallback(window, scroll_callback);
    // 注册键盘回调函数
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window)) {
        
        glfwPollEvents();
        flushwindow();//刷新屏幕
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame(); 
        ui();
        cursor_position(window);
        glClear(GL_COLOR_BUFFER_BIT); // 清空颜色缓冲区
        if(mode==-1&&ChooseIdx!=-1){
            // 应用变换到旋转中心点
            Point centerPoint = {static_cast<int>(graphics[ChooseIdx].cenx), 
                    static_cast<int>(graphics[ChooseIdx].ceny)};
            Point transformedCenter;
            if(graphics[ChooseIdx].isdefault){
                transformedCenter= applyTransform(centerPoint, graphics[ChooseIdx].transform);
            }
            else transformedCenter = centerPoint;
            // 绘制变换后的旋转中心
            setpixel(transformedCenter.x, transformedCenter.y, 2, {1.0, 0.0, 0.0});
        }
        if(mode == 0 || mode==7) drawLine(window);//渲染直线
        else if(mode==1) drawArc(window);//渲染圆弧
        else if(mode==2) drawFullArc(window);//渲染整圆
        else if(mode==4) drawPolygon(window);//渲染填充
        else if(mode==5||mode==8||mode==9)drawrec(window);
        else if(mode==6) drawBezier(window);//渲染贝塞尔曲线
        else if(mode==10)drawBSpline(window);//渲染B样条曲线
        else render();
        if(mode!=-1)ChooseIdx = -1;
        if(mode==-1&&ChooseIdx!=-1&&(graphics[ChooseIdx].mode==6||graphics[ChooseIdx].mode==10)&&selectedPointIndex!=-1){
            // 1. 获取鼠标位置
                detectposition(window, xpos, ypos);
                Point mousePos = {static_cast<int>(xpos), static_cast<int>(ypos)};

                // 2. 计算当前图形的逆变换矩阵
                Matrix3x3 invTransform = inverse(graphics[ChooseIdx].transform);

                // 3. 把鼠标坐标逆变换到原始坐标系
                Point newCtrlPoint = applyTransform(mousePos, invTransform);

                // 4. 更新控制点
                graphics[ChooseIdx].points[selectedPointIndex] = newCtrlPoint;
                if(graphics[ChooseIdx].isdefault==true)getcenposition(graphics[ChooseIdx]);
        }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
        glfwSwapBuffers(window);
        
    }
    // 清理ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();std::cout<<"bye\n"<<std::endl;//bye
    return 0;
}


void flushwindow() {
    // 刷新屏幕
    for(int i=0; i<maxn; i++) {
        for(int j=0; j<maxn; j++) {
            g[i][j] = {1.0, 1.0, 1.0};
        }
    }
    for(int i=0; i<graphics.size(); i++) {
        Color choosecolor;
        if(i==ChooseIdx) {
            choosecolor = graphics[i].color;
            graphics[i].color = selectedColor;
        }
        
        // 获取当前变换矩阵
        Matrix3x3 transform = graphics[i].transform;
        
        if(graphics[i].mode == 0) { // 直线
            // 应用变换到端点
            Point p1 = applyTransform(graphics[i].points[0], transform);
            Point p2 = applyTransform(graphics[i].points[1], transform);
            
            drawLineBresenham(p1, p2, false, graphics[i].width, graphics[i].color,graphics[i].a,graphics[i].b);
        }
        else if(graphics[i].mode == 7) { // 中点直线
            // 应用变换到端点
            Point p1 = applyTransform(graphics[i].points[0], transform);
            Point p2 = applyTransform(graphics[i].points[1], transform);
            middleLine(p1, p2, graphics[i].width, graphics[i].color,graphics[i].a,graphics[i].b);
        }
        else if(graphics[i].mode == 2) { // 整圆
            Point center = applyTransform(graphics[i].points[0], transform); 
            Point edge = applyTransform(graphics[i].points[1], transform);
            
            // 计算变换后的半径
            float radius = sqrt(pow(edge.x - center.x, 2) + pow(edge.y - center.y, 2));
            
            drawarc(center, radius, 0, 2 * M_PI, graphics[i].width, graphics[i].color);
        }
        else if(graphics[i].mode == 1) { // 圆弧
            Point center = applyTransform(graphics[i].points[0], transform);
            Point start = applyTransform(graphics[i].points[1], transform);
            Point end = applyTransform(graphics[i].points[2], transform);
            
            double r = sqrt((start.x - center.x) * (start.x - center.x) + 
                         (start.y - center.y) * (start.y - center.y));
                      
            double startRad = atan2(-center.y + start.y, start.x - center.x);
            double endRad = atan2(-center.y + end.y, end.x - center.x);
            
            if (startRad < 0) startRad += 2 * M_PI;
            if (endRad < 0) endRad += 2 * M_PI;
            
            drawarc(center, r, startRad, endRad, graphics[i].width, graphics[i].color);
        }
        else if(graphics[i].mode == 3) { // 填充区域
            // 应用变换到所有填充点
            std::vector<Point> transformedPoints;
            for (const auto& point : graphics[i].points) {
                transformedPoints.push_back(applyTransform(point, transform));
            }
            
            for (const auto& point : transformedPoints) {
                setpixel(point.x, point.y, graphics[i].width, graphics[i].color);
            }
        }
        else if(graphics[i].mode == 4) { // 多边形
            // 应用变换到所有多边形顶点
            std::vector<Point> transformedPoints;
            for (const auto& point : graphics[i].points) {
                transformedPoints.push_back(applyTransform(point, transform));
            }
            
            fillPolygon(transformedPoints, graphics[i].width, graphics[i].color);
        }
        else if(graphics[i].mode == 6) { // Bezier曲线
           
            std::vector<Point> transformedPoints;
            for (const auto& point : graphics[i].points) {
                transformedPoints.push_back(applyTransform(point, transform));
            }
            for(int j=0;j<transformedPoints.size()-1;j++){
                if(graphics[i].key==0||graphics[i].key==1)drawLineBresenham(transformedPoints[j], transformedPoints[j+1], false, 1,{1.0f,0.0f,0.0f});
            }
            drawBezierCurve(transformedPoints,graphics[i].width,graphics[i].color);
        }
        else if(graphics[i].mode==10){
            // 使用reserved字段存储的阶数，如果没有则默认为4
            std::vector<Point> transformedPoints;
            for (const auto& point : graphics[i].points) {
                transformedPoints.push_back(applyTransform(point, transform));
            }
            for(int j=0;j<transformedPoints.size()-1;j++){
                if(graphics[i].key==0||graphics[i].key==1)drawLineBresenham(transformedPoints[j], transformedPoints[j+1], false, 1,{1.0f,0.0f,0.0f});
            }
            int order = graphics[i].reserved > 0 ? graphics[i].reserved : 4;
            drawBSplineCurve(transformedPoints, graphics[i].width, graphics[i].color, order);
        }
        if(i == ChooseIdx) {
            graphics[i].color = choosecolor;
        }
    }
}