 
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

namespace GameConstants {
    // ��Ϸ����
    const int MAX_HAND_SIZE = 10;        // �����������
    const int MAX_FIELD_SIZE = 7;        // ��������������
    const int MAX_MANA = 10;             // �����ֵ
    const int STARTING_HAND_SIZE = 3;    // ��ʼ��������
    const int TURN_TIME_LIMIT = 90;      // �غ�ʱ�����ƣ��룩

    // UI����
    const float CARD_SCALE = 0.8f;       // �������ű���
    const float CARD_SPACING = 80.0f;    // ���Ƽ��
    const float FIELD_SPACING = 100.0f;  // ���ؼ��
    const float ANIMATION_DURATION = 0.3f;// ��������ʱ��

    // ���糣��
    const int RECONNECT_ATTEMPTS = 3;     // �������Դ���
    const float RECONNECT_DELAY = 2.0f;   // �����ӳ�ʱ��
    const int NETWORK_TIMEOUT = 5000;     // ���糬ʱʱ�䣨���룩

    // ��Դ·��
    namespace Resources {
        const char* const CARD_FRAME = "cards/frame.png";    // ���ƿ��ͼƬ·��
        const char* const CARD_BACK = "cards/card_back.png"; // ���Ʊ���ͼƬ·��
        const char* const BUTTON_NORMAL = "buttons/button_normal.png";
        const char* const BUTTON_PRESSED = "buttons/button_pressed.png";
        const char* const BACKGROUND = "backgrounds/game_bg.png";
        const char* const FONT_PATH = "fonts/arial.ttf";
    }

    // ��������
    namespace Animation {
        const float CARD_DRAW_DURATION = 0.5f;  // �鿨����ʱ��
        const float CARD_PLAY_DURATION = 0.3f;  // ���ƶ���ʱ��
        const float ATTACK_DURATION = 0.4f;
        const float DAMAGE_DURATION = 0.2f;
        const float HEAL_DURATION = 0.3f;
    }

    // ��Ч����
    namespace Sound {
        const char* const CARD_DRAW = "sounds/card_draw.mp3";  // �鿨��Ч
        const char* const CARD_PLAY = "sounds/card_play.mp3";  // ������Ч
        const char* const ATTACK = "sounds/attack.mp3";
        const char* const DAMAGE = "sounds/damage.mp3";
        const char* const VICTORY = "sounds/victory.mp3";
        const char* const DEFEAT = "sounds/defeat.mp3";
        // UI��Ч
        const char* const BUTTON_CLICK = "sounds/button_click.mp3";    // ��Ӱ�ť�����Ч
        const char* const BUTTON_HOVER = "sounds/button_hover.mp3";    // ��ѡ����Ӱ�ť��ͣ��Ч
        const char* const MENU_OPEN = "sounds/menu_open.mp3";         // ��ѡ����Ӳ˵�����Ч
        const char* const MENU_CLOSE = "sounds/menu_close.mp3";       // ��ѡ����Ӳ˵��ر���Ч

        // ��������
        const char* const MENU_BGM = "music/menu_bgm.mp3";           // ���˵���������
        const char* const GAME_BGM = "music/game_bgm.mp3";           // ��Ϸ��������
    }
}

#endif // __CONSTANTS_H__