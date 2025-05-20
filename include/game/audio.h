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
private:
    static int soundId; // 用于生成唯一音效ID
public:
    // 播放WAV音效（使用MCI实现真正的异步）
    static void playSound(const std::string& filePath, bool async = true, bool allowMultiple = true) {
        // 保留原来的PlaySound作为备用方法
        if (!allowMultiple) {
            PlaySound(NULL, 0, 0); // 如果不允许多个音效，则先停止所有音效
            
            DWORD flags = SND_FILENAME;
            if (async) flags |= SND_ASYNC;
            
            if (!PlaySoundA(filePath.c_str(), NULL, flags)) {
                std::cerr << "music failed: " << filePath << std::endl;
            }
            return;
        }
        
        // 使用MCI播放多音效
        std::string alias = "sound" + std::to_string(soundId++);
        
        // 确保ID不会无限增长
        if (soundId > 1000) soundId = 0;
        
        // 尝试关闭可能存在的同名别名
        mciSendStringA(("close " + alias).c_str(), NULL, 0, NULL);
        
        // 确定文件类型
        std::string fileType = "waveaudio";
        if (filePath.find(".mp3") != std::string::npos) fileType = "mpegvideo";
        else if (filePath.find(".ogg") != std::string::npos) fileType = "mpegvideo";
        
        // 打开文件
        std::string openCmd = "open \"" + filePath + "\" type " + fileType + " alias " + alias;
        if (mciSendStringA(openCmd.c_str(), NULL, 0, NULL) != 0) {
            std::cerr << "无法打开音效: " << filePath << std::endl;
            return;
        }
        
        // 播放音效
        std::string playCmd = "play " + alias;
        if (mciSendStringA(playCmd.c_str(), NULL, 0, NULL) != 0) {
            std::cerr << "无法播放音效: " << filePath << std::endl;
            mciSendStringA(("close " + alias).c_str(), NULL, 0, NULL);
        }
    }
    
    // 播放背景音乐（使用mciSendString - 支持MP3等格式）
    static void playBackgroundMusic(const std::string& filePath, bool loop = true) {
        // 停止当前播放的背景音乐
        stopBackgroundMusic();
        
        // 打开文件
        std::string openCommand = "open \"" + filePath + "\" type mpegvideo alias bgmusic";
        if (mciSendStringA(openCommand.c_str(), NULL, 0, NULL) != 0) {
            std::cerr << "can't open: " << filePath << std::endl;
            return;
        }
        
        // 播放音乐
        std::string playCommand = "play bgmusic";
        if (loop) {
            playCommand += " repeat";  // 循环播放
        }
        
        if (mciSendStringA(playCommand.c_str(), NULL, 0, NULL) != 0) {
            std::cerr << "can't play" << std::endl;
        }
    }
    
    // 停止背景音乐
    static void stopBackgroundMusic() {
        mciSendStringA("close bgmusic", NULL, 0, NULL);
    }
};

// 初始化静态成员变量
int Audio::soundId = 0;

Audio audio;//音效
#endif // GAME_SIMPLE_AUDIO_H_INCLUDED