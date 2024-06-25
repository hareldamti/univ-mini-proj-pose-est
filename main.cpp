#include <chrono>
#include <thread>
#include "State.h"
#include "Program.h"

State state(1200, 700, false);
Program program(state);

int initGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	// if (!LoadGLTextures())								// Jump To Texture Loading Routine
	// {
	// 	return FALSE;									// If Texture Didn't Load Return FALSE
	// }

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// This Will Clear The Background Color To Black
	glClearDepth(1.0);									// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);								// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glShadeModel(GL_SMOOTH);							// Enables Smooth Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	return TRUE;										// Initialization Went OK
}

void ResizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0) height = 1;										
	glViewport(0, 0, width, height);						
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	if (uMsg == WM_SIZE) ResizeGLScene(LOWORD(lParam),HIWORD(lParam));
	state.handleInput(hWnd, uMsg, wParam, lParam);
	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void killGLWindow()								// Properly Kill The Window
{
	if (state.window.fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (state.window.hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(state.window.hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		}
		state.window.hRC = NULL;										// Set RC To NULL
	}
	/*
	if (state.window.hDC && !ReleaseDC(state.window.hWnd, state.window.hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		state.window.hDC = NULL;										// Set DC To NULL
	}

	if (state.window.hWnd && !DestroyWindow(state.window.hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		state.window.hWnd = NULL;										// Set hWnd To NULL
	}
	*/
	if (!UnregisterClass("OpenGL", state.window.hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		state.window.hInstance=NULL;									// Set hInstance To NULL
	}
	
}

bool createGLWindow(const char* title, int width, int height, bool fullscreenflag = false, byte depth = 16)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = 0;			    // Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = 0;				    // Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	state.window.fullscreen=fullscreenflag;			// Set The Global Fullscreen Flag

	state.window.hInstance	= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style				= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc			= (WNDPROC) WndProc;					// WndProc Handles Messages
	wc.cbClsExtra			= 0;									// No Extra Window Data
	wc.cbWndExtra			= 0;									// No Extra Window Data
	wc.hInstance			= state.window.hInstance;							// Set The Instance
	wc.hIcon				= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor				= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground		= NULL;									// No Background Required For GL
	wc.lpszMenuName			= NULL;									// We Don't Want A Menu
	wc.lpszClassName		= (LPCSTR)"OpenGL";					// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL,(LPCSTR)"Failed To Register The Window Class.",(LPCSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}
	
	if (state.window.fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth	= width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight	= height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel	= depth;				// Selected Bits Per Pixel
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL,(LPCSTR)"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",(LPCSTR)"Noam&Harel",MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				state.window.fullscreen = false;
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL,(LPCSTR)"Program Will Now Close.",(LPCSTR)"ERROR",MB_OK|MB_ICONSTOP);
				return false;								
			}
		}
	}

	if (state.window.fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(state.window.hWnd=CreateWindowEx(	dwExStyle,				// Extended Style For The Window
								(LPCSTR)"OpenGL",					// Class Name
								(LPCSTR)title,						// Window Title
								dwStyle |							// Defined Window Style
								WS_CLIPSIBLINGS |					// Required Window Style
								WS_CLIPCHILDREN,					// Required Window Style
								(GetSystemMetrics(SM_CXSCREEN) - width) / 2,
                                (GetSystemMetrics(SM_CYSCREEN) - height) / 2,	// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								state.window.hInstance,				// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,(LPCSTR)"Window Creation Error.",(LPCSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}
	PIXELFORMATDESCRIPTOR pfd=				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		depth,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};
	
	if (!(state.window.hDC=GetDC(state.window.hWnd)))			// Did We Get A Device Context?
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,(LPCSTR)"Can't Create A GL Device Context.",(LPCSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(state.window.hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,(LPCSTR)"Can't Find A Suitable PixelFormat.",(LPCSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!SetPixelFormat(state.window.hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,(LPCSTR)"Can't Set The PixelFormat.",(LPCSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(state.window.hRC=wglCreateContext(state.window.hDC)))				// Are We Able To Get A Rendering Context?
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,(LPCSTR)"Can't Create A GL Rendering Context.",(LPCSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if(!(wglMakeCurrent(state.window.hDC,state.window.hRC) && gladLoadGL()))					// Try To Activate The Rendering Context
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,(LPCSTR)"Can't Activate The GL Rendering Context.",(LPCSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(state.window.hWnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(state.window.hWnd);						// Slightly Higher Priority
	SetFocus(state.window.hWnd);								// Sets Keyboard Focus To The Window
	ResizeGLScene(state.window.width, state.window.height);		// Set Up Our Perspective GL Screen
	if (!initGL())									// Initialize Our Newly Created GL Window
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL,(LPCSTR)"Initialization Failed.",(LPCSTR)"ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
    state.window.shouldQuit = !createGLWindow("Pose Estimation", state.window.width, state.window.height);
	program.wrapInit();
	state.init();
	MSG	msg;
    while (!state.window.shouldQuit) {
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)				
				state.window.shouldQuit = true;		
			else									
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		state.update();
		program.update();
		program.draw();
		Sleep(10);
		SwapBuffers(state.window.hDC);
    };
    // Shutdown
	killGLWindow();										// Kill The Window
	return (msg.wParam);								// Exit The Program
}