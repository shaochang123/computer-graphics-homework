#ifndef GAME_MENU_H_INCLUDED
#define GAME_MENU_H_INCLUDED
#ifndef GAME_INIT_H_INCLUDED
#include<game/init.h>
#endif
#ifndef GLFW_STB_IMAGE_H_INCLUDE
#define STB_IMAGE_IMPLEMENTATION
#include<GLFW/stb_image.h>
#endif
#include <string>
#include <unordered_map>
class Menu {
private:
    // 图像数据结构
    struct ImageData {
        unsigned char* data = nullptr;
        int width = 0;
        int height = 0;
        int channels = 0;
        
        ~ImageData() {
            if (data) {
                stbi_image_free(data);
                data = nullptr;
            }
        }
    }img;
    
public:
    Menu() = default;
    ~Menu() = default;
    void renderbar();
    void renderhp(int hp);
    void renderImage(const std::string& imagePath, int x, int y, int targetWidth = 0, int targetHeight = 0);
    void move(GLFWwindow* window, int key, int scancode, int action, int mods);
};
void Menu::renderhp(int hp){//渲染生命值
    for(int i=50;i<50+hp;i++){
        drawLineBresenham({i,77},{i,90},false,1,{1.0,1.0,0.0});
    }
    for(int i=50+hp;i<150;i++){
        drawLineBresenham({i,77},{i,90},false,1,{1.0,0.0,0.0});
    }
}
void Menu::renderbar(){//渲染菜单
    Point ld = {LMenu,DMenu},lu={LMenu,UMenu},rd={RMenu,DMenu},ru={RMenu,UMenu};
    drawLineBresenham(ld,lu,false,3,{1.0,1.0,1.0});
    drawLineBresenham(lu,ru,false,3,{1.0,1.0,1.0});
    drawLineBresenham(ru,rd,false,3,{1.0,1.0,1.0});
    drawLineBresenham(rd,ld,false,3,{1.0,1.0,1.0});
}
void Menu::renderImage(const std::string& imagePath, int x, int y, int targetWidth, int targetHeight) {//渲染贴图
    img.data = stbi_load(imagePath.c_str(), &img.width, &img.height, &img.channels, 0);
        
        // 如果未指定目标尺寸，使用原图尺寸
        if (targetWidth <= 0) targetWidth = img.width;
        if (targetHeight <= 0) targetHeight = img.height;
        
        // 计算缩放比例
        float scaleX = (float)img.width / targetWidth;
        float scaleY = (float)img.height / targetHeight;
        
        // 绘制缩放后的图像
        for (int j = 0; j < targetHeight; j++) {
            for (int i = 0; i < targetWidth; i++) {
                int posX = x + i;
                int posY = y + j;
                
                // 检查是否在屏幕范围内
                if (posX >= 0 && posX < maxn && posY >= 0 && posY < maxn) {
                    // 计算对应的原始图像坐标 (使用最近邻插值法)
                    int sourceI = (int)(i * scaleX);
                    int sourceJ = (int)(j * scaleY);
                    
                    // 确保不会越界
                    sourceI = std::min(sourceI, img.width - 1);
                    sourceJ = std::min(sourceJ, img.height - 1);
                    
                    // 计算像素在图像数据中的位置
                    int pixelIndex = (sourceJ * img.width + sourceI) * img.channels;
                    
                    Color pixelColor;
                    if (img.channels >= 3) {
                        pixelColor.r = img.data[pixelIndex] / 255.0f;
                        pixelColor.g = img.data[pixelIndex + 1] / 255.0f;
                        pixelColor.b = img.data[pixelIndex + 2] / 255.0f;
                    } else {
                        // 灰度图
                        float gray = img.data[pixelIndex] / 255.0f;
                        pixelColor.r = pixelColor.g = pixelColor.b = gray;
                    }
                    setpixel(posX, 2*y+img.height-posY, 1, pixelColor);
                }
            }
        }
}
void Menu::move(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key ==GLFW_KEY_LEFT){
        Idx--;
        if(Idx<0)Idx=0;
        audio.playSound("resource/switch.wav"); // 播放音效
        
    }
    if(key ==GLFW_KEY_RIGHT){
        Idx++;
        if(Idx>2)Idx=2;
        audio.playSound("resource/switch.wav"); // 播放音效
        
    }
}

Menu menu;//界面
#endif