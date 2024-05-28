#include "State.h"

State::State(int width, int height, bool fullscreen):
    window({width, height, fullscreen}) {}

void State::handleInput(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
            window.active = !HIWORD(wParam);
            return;
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return;							    // Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		case WM_DESTROY:
        {
			PostQuitMessage(0);						// Send A Quit Message
			return;					    			// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
            input.keyPressed[wParam] = true;
			input.keyState[wParam] = TRUE;			
			return;
		}

		case WM_KEYUP:								// Has A Key Been Released?
		{
			input.keyState[wParam] = FALSE;			// If So, Mark It As FALSE
            //input.keyPressed[wParam] = FALSE;
			return;				    				// Jump Back
		}
	
        case WM_LBUTTONDOWN: 
        {
            SetCapture(window.hWnd); 
            input.mouseState = true;
            input.mousePressed = true;
            SetCapture(window.hWnd); 
            // Retrieve the screen coordinates of the client area, 
            // and convert them into client coordinates. 
            POINTS p = MAKEPOINTS(lParam);
            input.mouseX = p.x;
            input.mouseY = p.y;
            return;
        }

        case WM_LBUTTONUP:
        {
            input.mouseState = false;
            // input.mousePressed = false;
            ReleaseCapture(); 
            return;
        }

    }
}

bool State::isKeyPressed(int key) {
    if (input.keyPressed[key]) {
        input.keyPressed[key] = false;
        return true;
    }
    return false;
}

bool State::isMousePressed() {
    if (input.mousePressed) {
        input.mousePressed = false;
        return true;
    }
    return false;
}
