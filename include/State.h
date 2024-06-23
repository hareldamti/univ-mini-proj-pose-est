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



class State {
    private:
        int startTime;
    public:
        State(int width, int height, bool fullscreen = false);
        WindowState window;
        InputState input;
        void handleInput(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        bool isKeyPressed(int key);
        bool isMousePressed();
        void init();
        int getMillis();
};

