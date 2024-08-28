#include "State.h"

State::State(int width, int height, Params& params, bool fullscreen):
    window({width, height, fullscreen}), params(params), startTime(0) {}

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

void State::update() {
    int removed = 0;
    for (int i = 0; i < animations.size(); i++) {
        if (animations[i - removed]->animate(getMillis())) {
            delete animations[i - removed];
            animations.erase(animations.begin() + i - removed);
            removed++;
        }
    }
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
    if (startTime == 0) startTime = getMilliCount();
	int nSpan = getMilliCount() - startTime;
	if(nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}

template <typename T>
AnimationHandler<T>::AnimationHandler(int currentTime, T* ptr, std::vector<T> keyframeValues, std::vector<float> keyframeTimes):
    ptr(ptr),
    startTime(currentTime), 
    keyframeValues(keyframeValues),
    keyframeTimes(keyframeTimes)
    {
        if (keyframeTimes.size() != keyframeValues.size() || keyframeValues.size() == 0) ERROR_EXIT("Inconsistent keyframes in AnimationHandler: %d %d\n", keyframeTimes.size(), keyframeValues.size());
        if (keyframeTimes[0] != 0) ERROR_EXIT("AnimationHanlder keyframe times must start at 0: %f\n", keyframeTimes[0]);
    }

template <typename T>
bool AnimationHandler<T>::animate(int currentTime) {
    float elapsed = 1e-3f * (currentTime - startTime);
    int i;
    for (i = 0; i < keyframeTimes.size() - 1; i++) {
        if (keyframeTimes[i + 1] <= elapsed) continue; {
            float t = (elapsed - keyframeTimes[i]) / (keyframeTimes[i + 1] - keyframeTimes[i]);
            *ptr = keyframeValues[i + 1] * t + keyframeValues[i] * (1 - t);
            return false;
        }
    }
    *ptr = keyframeValues[i];
    return true;
}

template <typename T>
void State::createAnimation(T* ptr, std::vector<T>& keyframeValues, std::vector<float>& keyframeTimes){
    animations.push_back(new AnimationHandler<T>(getMillis(), ptr, keyframeValues, keyframeTimes));

}

template <typename T>
void State::createAnimation(T* ptr, std::vector<T>&& keyframeValues, std::vector<float>&& keyframeTimes){
    animations.push_back(new AnimationHandler<T>(getMillis(), ptr, keyframeValues, keyframeTimes));

}

// animation types instantiation
template void State::createAnimation(glm::vec4* ptr, std::vector<glm::vec4>&& keyframeValues, std::vector<float>&& keyframeTimes);
template void State::createAnimation(glm::vec4* ptr, std::vector<glm::vec4>&  keyframeValues, std::vector<float>&  keyframeTimes);
template void State::createAnimation(glm::mat4* ptr, std::vector<glm::mat4>&& keyframeValues, std::vector<float>&& keyframeTimes);
template void State::createAnimation(glm::mat4* ptr, std::vector<glm::mat4>&  keyframeValues, std::vector<float>&  keyframeTimes);