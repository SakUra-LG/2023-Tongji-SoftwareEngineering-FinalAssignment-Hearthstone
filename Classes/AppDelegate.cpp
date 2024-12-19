#include "AppDelegate.h"
#include "Scene/MenuScene.h"  // 替换 HelloWorldScene.h
#pragma execution_character_set("utf-8")
// 如果你使用 cocos2d-x 的预编译头文件
#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1920, 1080);  // 修改设计分辨率
static cocos2d::Size smallResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1920, 1080);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#endif
}
// AppDelegate.cpp
void AppDelegate::initGLContextAttrs() {
    // 设置 OpenGL context 属性
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}
bool AppDelegate::applicationDidFinishLaunching() {
    // 初始化导演
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("HearthStone",
            Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("HearthStone");
#endif
        director->setOpenGLView(glview);
    }

    // 设置显示FPS
    director->setDisplayStats(true);  // 改为 false，不显示FPS

    // 设置FPS
    director->setAnimationInterval(1.0f / 60);

    // 设置设计分辨率
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height,
        ResolutionPolicy::NO_BORDER);

    // 设置资源搜索路径
    std::vector<std::string> searchPaths;
    searchPaths.push_back("res");
    FileUtils::getInstance()->setSearchPaths(searchPaths);

    // 在这里添加
    FileUtils::getInstance()->setWritablePath("logs/");
    CCLOG("Log path: %s", FileUtils::getInstance()->getWritablePath().c_str());

    // 创建场景
    auto scene = MenuScene::createScene();  // 使用 MenuScene 替换 HelloWorld
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#endif
}