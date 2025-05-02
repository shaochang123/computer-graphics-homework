#ifndef GAME_MENU_H_INCLUDED
#define GAME_MENU_H_INCLUDED
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
#ifndef GRAPH_LINE_H_INCLUDED
#include<graph/line.h>
#endif
#ifndef GLFW_STB_IMAGE_H_INCLUDE
#define STB_IMAGE_IMPLEMENTATION
#include<GLFW/stb_image.h>
#endif
#include <string>
#include <unordered_map>
// #include<game/font.h>
// FontRenderer font;
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
    // void renderText() {
    //     font.init("resource/undertale.ttf", 16);
    //     font.renderText("HP:100", 10, 20, {1.0f, 1.0f, 1.0f});
    // }
    void renderbar();
    void renderhp(int hp);
    // 在指定位置以指定尺寸显示图像
    void renderImage(const std::string& imagePath, int x, int y, int targetWidth = 0, int targetHeight = 0) {
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
};
void Menu::renderhp(int hp){
    for(int i=50;i<50+hp;i++){
        drawLineBresenham({i,77},{i,90},false,1,{1.0,1.0,0.0});
    }
    for(int i=50+hp;i<150;i++){
        drawLineBresenham({i,77},{i,90},false,1,{1.0,0.0,0.0});
    }
}
void Menu::renderbar(){
    Point ld = {50,100},lu={50,250},rd={450,100},ru={450,250};
    drawLineBresenham(ld,lu,false,3,{1.0,1.0,1.0});
    drawLineBresenham(lu,ru,false,3,{1.0,1.0,1.0});
    drawLineBresenham(ru,rd,false,3,{1.0,1.0,1.0});
    drawLineBresenham(rd,ld,false,3,{1.0,1.0,1.0});
}
#endif