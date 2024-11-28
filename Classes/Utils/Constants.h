 
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

namespace GameConstants {
    // 游戏规则
    const int MAX_HAND_SIZE = 10;
    const int MAX_FIELD_SIZE = 7;
    const int MAX_MANA = 10;
    const int STARTING_HAND_SIZE = 3;
    const int TURN_TIME_LIMIT = 90;

    // UI常量
    const float CARD_SCALE = 0.8f;
    const float CARD_SPACING = 80.0f;
    const float FIELD_SPACING = 100.0f;
    const float ANIMATION_DURATION = 0.3f;

    // 网络常量
    const int RECONNECT_ATTEMPTS = 3;
    const float RECONNECT_DELAY = 2.0f;
    const int NETWORK_TIMEOUT = 5000;

    // 资源路径
    namespace Resources {
        const char* const CARD_FRAME = "cards/frame.png";
        const char* const CARD_BACK = "cards/card_back.png";
        const char* const BUTTON_NORMAL = "buttons/button_normal.png";
        const char* const BUTTON_PRESSED = "buttons/button_pressed.png";
        const char* const BACKGROUND = "backgrounds/game_bg.png";
        const char* const FONT_PATH = "fonts/arial.ttf";
    }

    // 动画常量
    namespace Animation {
        const float CARD_DRAW_DURATION = 0.5f;
        const float CARD_PLAY_DURATION = 0.3f;
        const float ATTACK_DURATION = 0.4f;
        const float DAMAGE_DURATION = 0.2f;
        const float HEAL_DURATION = 0.3f;
    }

    // 音效名称
    namespace Sound {
        const char* const CARD_DRAW = "sounds/card_draw.mp3";
        const char* const CARD_PLAY = "sounds/card_play.mp3";
        const char* const ATTACK = "sounds/attack.mp3";
        const char* const DAMAGE = "sounds/damage.mp3";
        const char* const VICTORY = "sounds/victory.mp3";
        const char* const DEFEAT = "sounds/defeat.mp3";
        // UI音效
        const char* const BUTTON_CLICK = "sounds/button_click.mp3";    // 添加按钮点击音效
        const char* const BUTTON_HOVER = "sounds/button_hover.mp3";    // 可选：添加按钮悬停音效
        const char* const MENU_OPEN = "sounds/menu_open.mp3";         // 可选：添加菜单打开音效
        const char* const MENU_CLOSE = "sounds/menu_close.mp3";       // 可选：添加菜单关闭音效

        // 背景音乐
        const char* const MENU_BGM = "music/menu_bgm.mp3";           // 主菜单背景音乐
        const char* const GAME_BGM = "music/game_bgm.mp3";           // 游戏背景音乐
    }
}

#endif // __CONSTANTS_H__