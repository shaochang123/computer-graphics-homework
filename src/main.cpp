#include<graph/Line.h>
#include<graph/Circle.h>
#include<graph/Fill.h>
#include<graph/Polygon.h>
#include<graph/Crop.h>
#include<graph/FullCircle.h>
#include<transform/translation.h>
#include<graph/bezier.h>
// 鼠标点击回调函数
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);
    Line_Mouse_Pressed(window, button, action);
    Circle_Mouse_Pressed(window, button, action);
    Full_Circle_Mouse_Pressed(window, button, action);
    Fill_Mouse_Pressed(window, button, action);
    Polygon_Mouse_Pressed(window, button, action);
    SelectGraphByClick(window, button, action);
    Bezier_Mouse_Pressed(window, button, action);
    Bezier_Edit_Mouse_Handler(window,button,action,mods);
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
    ChangeWidth(key, action);
    Translation(window);
    Bezier_Keyboard_Pressed(key, action);
}

void flushwindow();
int main() {
    const int width = 600, height = 800;//设置窗口大小
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
        {
            // 获取窗口尺寸
    int width, height;
    glfwGetWindowSize(glfwGetCurrentContext(), &width, &height);
    // if (ImGui::Button("测试按钮")) {
    //     std::cout << "按钮被点击了！" << std::endl;
    // }
    
    // static float value = 0.5f;
    // if (ImGui::SliderFloat("测试滑块", &value, 0.0f, 1.0f)) {
    //     std::cout << "滑块值改变为: " << value << std::endl;
    // }
    // 设置ImGui窗口的位置和大小
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(width, 200), ImGuiCond_Always);
    
    // 移除窗口装饰(标题栏、调整大小控件等)
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
                           ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
                           ImGuiWindowFlags_NoBringToFrontOnFocus;
                           
    ImGui::Begin("Drawing Toolbox", nullptr, flags);
    
    // 模式选择按钮区域
    ImGui::Text("Drawing Mode:");
    if (ImGui::Button("Line Mode(Bresenham)")){mode=0;curpoints.clear();std::cout<<"Line(Bresenham) Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Line Mode(middleline)")){mode=7;curpoints.clear();std::cout<<"Line(middleline) Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Arc Mode")){mode=1;curpoints.clear();std::cout<<"Arc Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Crop Mode")){mode=5;curpoints.clear();std::cout<<"Crop Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Circle Mode")){mode=2;curpoints.clear();std::cout<<"Circle Mode"<<std::endl;}
    if (ImGui::Button("Fill Mode")) {mode=3;curpoints.clear();std::cout<<"Fill Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Polygon Mode")){mode=4;curpoints.clear();std::cout<<"Polygon Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Bezier Mode")){mode=6;curpoints.clear();std::cout<<"Bezier Mode"<<std::endl;}
    ImGui::SameLine();
    if (ImGui::Button("Select Mode")){mode=-1;curpoints.clear();std::cout<<"Select Mode"<<std::endl;}
    
    ImGui::Separator();
    
    // 颜色调整区域
    ImGui::Text("Color Control:");
    
    // 创建临时变量存储当前颜色值（值范围0.0-1.0）
    static float color[3] = {0.0f, 0.0f, 0.0f};
    
    // 根据模式和选择状态获取当前颜色
    if (mode != -1) {
        // 在绘图模式下，使用当前颜色
        color[0] = curcolor.r;
        color[1] = curcolor.g;
        color[2] = curcolor.b;
    } else if (ChooseIdx >= 0 && ChooseIdx < graphics.size()) {
        // 在选择模式下，使用选中图形的颜色
        color[0] = graphics[ChooseIdx].color.r;
        color[1] = graphics[ChooseIdx].color.g;
        color[2] = graphics[ChooseIdx].color.b;
    }
    
    // 显示滑块控制RGB颜色
    bool colorChanged = false;
    colorChanged |= ImGui::SliderFloat("Red", &color[0], 0.0f, 1.0f);
    colorChanged |= ImGui::SliderFloat("Green", &color[1], 0.0f, 1.0f);
    colorChanged |= ImGui::SliderFloat("Blue", &color[2], 0.0f, 1.0f);
    
    // 根据模式将更改应用到相应的颜色变量
    if (colorChanged) {
        if (mode != -1) {
            // 更新当前绘图颜色
            curcolor = {color[0], color[1], color[2]};
        } else if (ChooseIdx >= 0 && ChooseIdx < graphics.size()) {
            // 更新选中图形的颜色
            graphics[ChooseIdx].color = {color[0], color[1], color[2]};
        }
    }
    
    ImGui::Separator();
    
    // 线宽控制
    int wid = (mode != -1) ? curwidth : 
                (ChooseIdx >= 0 && ChooseIdx < graphics.size()) ? graphics[ChooseIdx].width : 1;
    if (ImGui::SliderInt("Line Width", &wid, 1, 10)) {
        if (mode != -1) {
            curwidth = wid;
        } else if (ChooseIdx >= 0 && ChooseIdx < graphics.size()) {
            graphics[ChooseIdx].width = wid;
        }
    }
    
    ImGui::Separator();
    
    // 显示当前模式
    const char* modeNames[] = {"Line(Bresenham)", "Arc", "Circle", "Fill", "Polygon", "Crop", "Bezier", "Line(middleline)", "Selection"};
    int displayMode = (mode == -1) ? 8 : mode;
    if (displayMode >= 0 && displayMode < 9) {
        ImGui::Text("Current Mode: %s", modeNames[displayMode]);
    }
    
    // 显示键盘快捷键信息
    ImGui::Separator();
    ImGui::Text("Keyboard Shortcuts:");
    ImGui::BulletText("Ctrl+S: Save image");
    ImGui::BulletText("Ctrl+Z: Undo");
    ImGui::BulletText("Ctrl+Y: Redo");
    
    ImGui::End();
        }
        
        glClear(GL_COLOR_BUFFER_BIT); // 清空颜色缓冲区
        if(mode == 0) drawLine(window);//渲染直线
        else if(mode==1) drawArc(window);//渲染圆弧
        else if(mode==2) drawFullArc(window);//渲染整圆
        else if(mode==4) drawPolygon(window);//渲染填充
        else if(mode==6) drawBezier(window);
        else render();
        
        if(mode!=-1)ChooseIdx = -1;
        if(mode==-1&&ChooseIdx!=-1&&graphics[ChooseIdx].mode==6&&selectedPointIndex!=-1){
            detectposition(window, xpos, ypos);
            graphics[ChooseIdx].points[selectedPointIndex] = {static_cast<int>(xpos), static_cast<int>(ypos)};
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
            
            drawLineBresenham(p1, p2, false, graphics[i].width, graphics[i].color);
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
                      
            double startRad = atan2(center.y - start.y, start.x - center.x);
            double endRad = atan2(center.y - end.y, end.x - center.x);
            
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
        if(i == ChooseIdx) {
            graphics[i].color = choosecolor;
        }
    }
}