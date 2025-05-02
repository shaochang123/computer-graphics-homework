#ifndef GAME_AUDIO_H_INCLUDED
#define GAME_AUDIO_H_INCLUDED
#include <Windows.h>
#include <string>
#include <iostream>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#ifndef GAME_INIT_H_INCLUDED
#include<game/init.h>
#endif
class Audio {
public:
    // 播放WAV音效（同步方式）
    static void playSound(const std::string& filePath, bool async = true) {
        // 先停止上一个音效，避免阻塞
        PlaySound(NULL, 0, 0);
    
        DWORD flags = SND_FILENAME;
        if (async) {
            flags |= SND_ASYNC;
        }
    
        if (!PlaySoundA(filePath.c_str(), NULL, flags)) {
            std::cerr << "音效播放失败: " << filePath << std::endl;
        }
    }
    
    // 播放背景音乐（使用mciSendString - 支持MP3等格式）
    static void playBackgroundMusic(const std::string& filePath, bool loop = true) {
        // 停止当前播放的背景音乐
        stopBackgroundMusic();
        
        // 打开文件
        std::string openCommand = "open \"" + filePath + "\" type mpegvideo alias bgmusic";
        if (mciSendStringA(openCommand.c_str(), NULL, 0, NULL) != 0) {
            std::cerr << "无法打开背景音乐: " << filePath << std::endl;
            return;
        }
        
        // 播放音乐
        std::string playCommand = "play bgmusic";
        if (loop) {
            playCommand += " repeat";  // 循环播放
        }
        
        if (mciSendStringA(playCommand.c_str(), NULL, 0, NULL) != 0) {
            std::cerr << "无法播放背景音乐" << std::endl;
        }
    }
    
    // 停止背景音乐
    static void stopBackgroundMusic() {
        mciSendStringA("close bgmusic", NULL, 0, NULL);
    }
};
Audio audio;//音效
#endif // GAME_SIMPLE_AUDIO_H_INCLUDED