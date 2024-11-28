#include "Audio/AudioManager.h"
#include "Utils/GameLogger.h"

AudioManager* AudioManager::_instance = nullptr;

AudioManager* AudioManager::getInstance() {
    if (!_instance) {
        _instance = new AudioManager();
    }
    return _instance;
}

AudioManager::AudioManager() 
    : _backgroundMusicId(-1)
    , _musicVolume(1.0f)
    , _effectsVolume(1.0f)
    , _isMuted(false) {
    init();
}

void AudioManager::init() {
    cocos2d::experimental::AudioEngine::lazyInit();
}

int AudioManager::playEffect(const std::string& filename, bool loop) {
    if (_isMuted) return -1;
    
    float volume = getAdjustedVolume(_effectsVolume);
    int soundId = cocos2d::experimental::AudioEngine::play2d(filename, loop, volume);
    
    if (soundId != -1) {
        _effectsMap[filename] = soundId;
    }
    
    return soundId;
}

void AudioManager::playBackgroundMusic(const std::string& filename, bool loop) {
    if (_backgroundMusicId != -1) {
        stopBackgroundMusic();
    }
    
    float volume = getAdjustedVolume(_musicVolume);
    _backgroundMusicId = cocos2d::experimental::AudioEngine::play2d(filename, loop, volume);
}

void AudioManager::stopBackgroundMusic() {
    if (_backgroundMusicId != -1) {
        cocos2d::experimental::AudioEngine::stop(_backgroundMusicId);
        _backgroundMusicId = -1;
    }
}

float AudioManager::getAdjustedVolume(float volume) const {
    return _isMuted ? 0.0f : volume;
}
int AudioManager::play2d(const std::string& filename, bool loop, float volume) {
    if (getInstance()->_isMuted) return -1;

    float adjustedVolume = getInstance()->getAdjustedVolume(volume);
    int audioId = cocos2d::experimental::AudioEngine::play2d(filename, loop, adjustedVolume);

    if (audioId != -1) {
        // 使用 _effectsMap 而不是 _audioCache
        getInstance()->_effectsMap[filename] = audioId;
    }

    return audioId;
}