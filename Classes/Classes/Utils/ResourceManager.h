#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

#include "cocos2d.h"

class ResourceManager {
public:
    static ResourceManager* getInstance();

    // 资源预加载
    void preloadCardTextures();
    void preloadEffects();
    void preloadAudio();

    // 资源获取
    cocos2d::Texture2D* getCardTexture(int cardId);
    cocos2d::Animation* getEffectAnimation(const std::string& effectName);

    // 资源释放
    void purgeUnusedResources();

private:
    ResourceManager();
    ~ResourceManager();
    static ResourceManager* _instance;

    std::map<int, cocos2d::Texture2D*> _cardTextures;
    std::map<std::string, cocos2d::Animation*> _effectAnimations;
};

#endif // __RESOURCE_MANAGER_H__