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

		case WM_KEYDOWN:							
		{
			input.keyState[wParam] = true;		
			return;
		}

		case WM_KEYUP:								
		{
			input.keyState[wParam] = false;			
            input.keyPressed[wParam] = false;
			return;				    				
		}
	
        case WM_LBUTTONDOWN: 
        {
            SetCapture(window.hWnd); 
            input.mouseState = true;
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
            input.mousePressed = false;
            ReleaseCapture(); 
            return;
        }
    }
}

bool State::isKeyPressed(int key) {
    if (!input.keyPressed[key] && input.keyState[key]) {
        input.keyPressed[key] = true;
        return true;
    }
    return false;
}

bool State::isMousePressed() {
    if (!input.mousePressed && input.mouseState) {
        input.mousePressed = true;
        return true;
    }
    return false;
}

int getMilliCount(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

void State::init() {
    startTime = getMilliCount();
}

int State::getMillis() {
	int nSpan = getMilliCount() - startTime;
	if(nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}