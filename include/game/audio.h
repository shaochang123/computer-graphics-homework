#ifndef GAME_AUDIO_H_INCLUDED
#define GAME_AUDIO_H_INCLUDED
#include <Windows.h>
#include <string>
#include <iostream>

class Audio {
public:
    // 播放WAV音效（同步方式）
    static void playSound(const std::string& filePath, bool async = true) {
        DWORD flags = SND_FILENAME;  // 指定文件路径
        if (async) {
            flags |= SND_ASYNC;      // 异步播放（不阻塞）
        }
        
        if (!PlaySound(filePath.c_str(), NULL, flags)) {
            std::cerr << "音效播放失败: " << filePath << std::endl;
        }
    }
    
    // 播放背景音乐（使用mciSendString - 支持MP3等格式）
    static void playBackgroundMusic(const std::string& filePath, bool loop = true) {
        // 停止当前播放的背景音乐
        stopBackgroundMusic();
        
        // 打开文件
        std::string openCommand = "open \"" + filePath + "\" type mpegvideo alias bgmusic";
        if (mciSendString(openCommand.c_str(), NULL, 0, NULL) != 0) {
            std::cerr << "无法打开背景音乐: " << filePath << std::endl;
            return;
        }
        
        // 播放音乐
        std::string playCommand = "play bgmusic";
        if (loop) {
            playCommand += " repeat";  // 循环播放
        }
        
        if (mciSendString(playCommand.c_str(), NULL, 0, NULL) != 0) {
            std::cerr << "无法播放背景音乐" << std::endl;
        }
    }
    
    // 停止背景音乐
    static void stopBackgroundMusic() {
        mciSendString("close bgmusic", NULL, 0, NULL);
    }
};

#endif // GAME_SIMPLE_AUDIO_H_INCLUDED