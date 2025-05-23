#ifndef GAME_AUDIO_H_INCLUDED
#define GAME_AUDIO_H_INCLUDED
#include <Windows.h>
#include <string>
#include <iostream>
#include <mmsystem.h>
#include <vector>
#pragma comment(lib, "winmm.lib")
#ifndef GAME_INIT_H_INCLUDED
#include<game/init.h>
#endif

class Audio {
private:
    static int soundId;
    static std::vector<std::string> activeAliases; // 跟踪活动别名
    
public:
    // 播放WAV音效（使用MCI实现真正的异步）
    static void playSound(const std::string& filePath, bool async = true, bool allowMultiple = true) {
        // 定期清理资源，防止累积
        static int cleanupCounter = 0;
        if (++cleanupCounter > 50) {
            cleanupFinishedSounds();
            cleanupCounter = 0;
        }
        
        // 限制同时播放的音效数量
        if (activeAliases.size() > 50) {
            return; // 跳过新音效，避免资源耗尽
        }
        
        // 保留原来的PlaySound作为备用方法
        if (!allowMultiple) {
            // 使用更轻量的播放方式
            DWORD flags = SND_FILENAME | SND_ASYNC | SND_NOWAIT;
            PlaySoundA(filePath.c_str(), NULL, flags);
            return;
        }
        
        // 使用MCI播放多音效
        std::string alias = "snd" + std::to_string(soundId++);
        // 确保ID不会无限增长
        if (soundId > 50) soundId = 0; // 更频繁地重置ID
        
        // 尝试关闭可能存在的同名别名
        mciSendStringA(("close " + alias).c_str(), NULL, 0, NULL);
        
        // 确定文件类型
        std::string fileType = "waveaudio";
        if (filePath.find(".mp3") != std::string::npos || 
            filePath.find(".ogg") != std::string::npos) {
            fileType = "mpegvideo";
        }
        
        // 打开并播放
        std::string openCmd = "open \"" + filePath + "\" type " + fileType + " alias " + alias;
        if (mciSendStringA(openCmd.c_str(), NULL, 0, NULL) == 0) {
            std::string playCmd = "play " + alias;
            if (mciSendStringA(playCmd.c_str(), NULL, 0, NULL) == 0) {
                activeAliases.push_back(alias);
            } else {
                mciSendStringA(("close " + alias).c_str(), NULL, 0, NULL);
            }
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
    
    // 清理已完成的音效资源
    static void cleanupFinishedSounds() {
        for (auto it = activeAliases.begin(); it != activeAliases.end();) {
            char status[64] = {0};
            std::string statusCmd = "status " + *it + " mode";
            
            if (mciSendStringA(statusCmd.c_str(), status, sizeof(status), NULL) != 0 ||
                strcmp(status, "stopped") == 0) {
                // 音效已停止，关闭资源
                mciSendStringA(("close " + *it).c_str(), NULL, 0, NULL);
                it = activeAliases.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    // 强制清理所有音效资源
    static void forceCleanup() {
        for (const auto& alias : activeAliases) {
            mciSendStringA(("close " + alias).c_str(), NULL, 0, NULL);
        }
        activeAliases.clear();
        soundId = 0;
    }
    
    // 清理所有音频资源（包括背景音乐）
    static void cleanup() {
        forceCleanup();
        stopBackgroundMusic();
    }
};

// 初始化静态成员变量
int Audio::soundId = 0;
std::vector<std::string> Audio::activeAliases;

Audio audio; // 音效实例
#endif // GAME_AUDIO_H_INCLUDED