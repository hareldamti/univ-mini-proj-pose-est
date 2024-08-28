#pragma once
#include "utils.h"

typedef struct WindowState {
    int     width = 800;
    int     height = 600;
    bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
    bool    shouldQuit = FALSE; // Determines if the window should terminate
    bool	fullscreen = FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default
    HDC			hDC = NULL;		// Private GDI Device Context
    HGLRC		hRC = NULL;		// Permanent Rendering Context
    HWND		hWnd = NULL;	// Holds Our Window Handle
    HINSTANCE	hInstance;		// Holds The Instance Of The Application
} WindowState;

typedef struct InputState {
    bool	keyState[256];			// Array Used For The Keyboard Routine
    bool    keyPressed[256];
    bool    mouseState;
    bool    mousePressed;
    int     mouseX;
    int     mouseY;
} InputState;

class AnimationHandlerGenericWrapper {
    public:
        virtual bool animate(int currentTime) = 0;
        virtual ~AnimationHandlerGenericWrapper() {};
};

template <typename T>
class AnimationHandler : public AnimationHandlerGenericWrapper {
    private:
        T* ptr;
        int startTime;
        std::vector<T> keyframeValues;
        std::vector<float> keyframeTimes;
    public:
        AnimationHandler(int currentTime, T* ptr, std::vector<T> keyframeValues, std::vector<float> keyframeTimes);
        bool animate(int currentTime) override;
};

class State {
    private:
        int startTime;
        int currTime;
        std::vector<AnimationHandlerGenericWrapper*> animations;
    public:
        State(int width, int height, bool fullscreen = false);
        void init(int width, int height, bool fullscreen = false);
        WindowState window;
        InputState input;
        void handleInput(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        bool isKeyPressed(int key);
        bool isMousePressed();
        void init();
        void update();
        template <typename T>
        void createAnimation(T* ptr, std::vector<T>& keyframeValues, std::vector<float>& keyframeTimes);
        template <typename T>
        void createAnimation(T* ptr, std::vector<T>&& keyframeValues, std::vector<float>&& keyframeTimes);
        int getMillis();

};

