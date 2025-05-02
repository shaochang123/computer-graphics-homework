// 需要安装FreeType库
// 比如使用vcpkg: vcpkg install freetype
#ifndef GAME_FONT_H_INCLUDED
#define GAME_FONT_H_INCLUDED
#ifndef FREETYPE_FT2BUILD_H_INCLUDED
#include <freetype/ft2build.h>
#endif
#ifndef FREETYPE_FREETYPE_H_INCLUDED
#include<freetype/freetype.h>
#endif
#ifndef GRAPH_INIT_H_INCLUDED
#include<graph/init.h>
#endif
class FontRenderer {
private:
    FT_Library ft;
    FT_Face face;
    // 字符数据结构
    struct Character {
        unsigned char* bitmap;
        int width;
        int height;
        int bearingX;
        int bearingY;
        int advance;
        
        ~Character() {
            delete[] bitmap;
        }
    };
    std::map<char,Character> characters;
    
public:
    FontRenderer() : ft(nullptr), face(nullptr) {}
    
    ~FontRenderer() {
        // 释放资源
        for (auto& pair : characters) {
            // Character析构函数会释放bitmap
        }
        
        if (face) {
            FT_Done_Face(face);
        }
        
        if (ft) {
            FT_Done_FreeType(ft);
        }
    }
    
    bool init(const std::string& fontPath, int fontSize) {
        // 初始化FreeType库
        if (FT_Init_FreeType(&ft)) {
            std::cerr << "无法初始化FreeType库" << std::endl;
            return false;
        }
        
        // 加载字体
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            std::cerr << "无法加载字体: " << fontPath << std::endl;
            return false;
        }
        
        // 设置字体大小
        FT_Set_Pixel_Sizes(face, 0, fontSize);
        
        // 预加载基本ASCII字符
        preloadCharacters();
        
        return true;
    }
    
    void preloadCharacters() {
        // 加载ASCII字符
        for (unsigned char c = 32; c < 128; c++) {
            loadCharacter(c);
        }
    }
    
    bool loadCharacter(char c) {
        // 加载字符到字形槽
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            std::cerr << "无法加载字符: " << c << std::endl;
            return false;
        }
        
        // 创建新字符
        Character ch;
        ch.width = face->glyph->bitmap.width;
        ch.height = face->glyph->bitmap.rows;
        ch.bearingX = face->glyph->bitmap_left;
        ch.bearingY = face->glyph->bitmap_top;
        ch.advance = face->glyph->advance.x >> 6; // 转换为像素
        
        // 复制位图数据
        int size = ch.width * ch.height;
        ch.bitmap = new unsigned char[size];
        memcpy(ch.bitmap, face->glyph->bitmap.buffer, size);
        
        // 存储字符
        characters[c] = ch;
        return true;
    }
    
    void renderText(const std::string& text, int x, int y, Color color) {
        for (char c : text) {
            if (c == '\n') {
                y += (face->size->metrics.height >> 6) + 2; // 处理换行
                x = 0;
                continue;
            }
            
            // 如果字符未加载，尝试加载
            if (characters.find(c) == characters.end()) {
                if (!loadCharacter(c)) continue;
            }
            
            const Character& ch = characters[c];
            
            int posX = x + ch.bearingX;
            int posY = y - ch.bearingY;
            
            // 渲染字符位图到像素网格
            for (int j = 0; j < ch.height; j++) {
                for (int i = 0; i < ch.width; i++) {
                    int pixelIndex = j * ch.width + i;
                    float alpha = ch.bitmap[pixelIndex] / 255.0f;
                    
                    if (alpha > 0.0f) {
                        int drawX = posX + i;
                        int drawY = posY + j;
                        
                        if (drawX >= 0 && drawX < maxn && drawY >= 0 && drawY < maxn) {
                            // 应用字体颜色
                            Color pixelColor = {
                                color.r * alpha,
                                color.g * alpha,
                                color.b * alpha
                            };
                            g[drawY][drawX] = pixelColor;
                        }
                    }
                }
            }
            
            // 前进到下一个字符位置
            x += ch.advance;
        }
    }
};
#endif // GAME_FONT_H_INCLUDED