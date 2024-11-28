#include "AppDelegate.h"
#include "Scene/MenuScene.h"  // �滻 HelloWorldScene.h

// �����ʹ�� cocos2d-x ��Ԥ����ͷ�ļ�
#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(2048, 1024);  // �޸���Ʒֱ���
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
    // ���� OpenGL context ����
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };
    GLView::setGLContextAttrs(glContextAttrs);
}
bool AppDelegate::applicationDidFinishLaunching() {
    // ��ʼ������
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

    // ������ʾFPS
    director->setDisplayStats(false);  // ��Ϊ false������ʾFPS

    // ����FPS
    director->setAnimationInterval(1.0f / 60);

    // ������Ʒֱ���
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height,
        ResolutionPolicy::NO_BORDER);

    // ������Դ����·��
    std::vector<std::string> searchPaths;
    searchPaths.push_back("res");
    FileUtils::getInstance()->setSearchPaths(searchPaths);

    // ��������
    auto scene = MenuScene::createScene();  // ʹ�� MenuScene �滻 HelloWorld
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