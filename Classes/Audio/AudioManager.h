// Audio/AudioManager.h
#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__
#pragma execution_character_set("utf-8")
#include "cocos2d.h"
#include "audio/include/AudioEngine.h"
#include <string>
#include <map>

/**
 * @brief 音频管理器类
 * 
 * 负责管理游戏中的所有音频，包括背景音乐、音效等
 * 使用单例模式实现
 */
class AudioManager {
public:
    static AudioManager* getInstance();
    // 播放音效
    static int play2d(const std::string& filename, bool loop = false, float volume = 1.0f);
    // 音乐控制
    void playBackgroundMusic(const std::string& filename, bool loop = true);
    void stopBackgroundMusic();
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    
    // 音效控制
    int playEffect(const std::string& filename, bool loop = false);
    void stopEffect(int soundId);
    void stopAllEffects();
    
    // 音量控制
    void setBackgroundMusicVolume(float volume);  // 0.0f to 1.0f
    void setEffectsVolume(float volume);          // 0.0f to 1.0f
    float getBackgroundMusicVolume() const;
    float getEffectsVolume() const;

    // 状态查询
    bool isBackgroundMusicPlaying() const;
    
    // 预加载
    void preloadBackgroundMusic(const std::string& filename);
    void preloadEffect(const std::string& filename);
    
    // 静音控制
    void setMuted(bool muted);
    bool isMuted() const;

private:
    AudioManager();
    ~AudioManager();
    static AudioManager* _instance;

    // 当前状态
    int _backgroundMusicId;
    float _musicVolume;
    float _effectsVolume;
    bool _isMuted;
    
    // 缓存音效ID
    std::map<std::string, int> _effectsMap;
    std::map<std::string, int> _audioCache;  // 添加这行
    // 内部方法
    void init();
    void cleanup();
    float getAdjustedVolume(float volume) const;
};

/*
使用示例：

// 获取音频管理器实例
auto audio = AudioManager::getInstance();

// 播放背景音乐
audio->playBackgroundMusic("music/background.mp3", true);

// 播放音效
audio->playEffect("sounds/card_play.mp3");

// 调整音量
audio->setBackgroundMusicVolume(0.8f);
audio->setEffectsVolume(0.6f);

// 静音控制
audio->setMuted(true);
*/

#endif // __AUDIO_MANAGER_H__