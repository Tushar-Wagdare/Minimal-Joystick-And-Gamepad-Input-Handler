#include <windows.h>
#include <stdio.h>                                        
#include <stdlib.h> 
#include <Xinput.h>

#include "Window.h"
#include "Logger.h"


#define T_SENSITIVITY 500


//*** Globle Function Declarations ***
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); 
int initialize(void);
int getInput1(void);
int getInput2(void);


#pragma comment(lib, "Xinput.lib")
#pragma comment(lib, "Xinput9_1_0.lib")


//*** Globle Variable Declaration ***
FILE* gpFILE           = NULL;                                      
HWND ghwnd             = NULL;
BOOL gbActive          = FALSE; 
DWORD dwStyle          = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
BOOL gbFullscreen      = FALSE;
///Controller Variables
XINPUT_STATE myGamePadState;
XINPUT_GAMEPAD myGamepad;
INPUT myInput;
MOUSEINPUT myMouseInput;
XINPUT_KEYSTROKE keyPressed;
DWORD myControllerIndex = 0;
int coun = 0;
Logger::hConsole = NULL;


//*** Enrty-point Function ***
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	//*** Local Variable Declarations ***
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Tushar_Wagdare_Chi_Window");
	int iWinWidth, iWinHight;
	BOOL bDone = FALSE;
	int returnVal = 0;

	// create logger console for logging
	Logger::Init();


	iWinWidth = GetSystemMetrics(SM_CXSCREEN);                      
	iWinHight = GetSystemMetrics(SM_CYSCREEN);                      

	//*** Code ***
	gpFILE = fopen("Log.txt", "w");                                 
	if (gpFILE == NULL)
	{
		MessageBox(NULL, TEXT("Log Create Hou Shakat Nahi"), TEXT("Error"), MB_OK || MB_ICONERROR);
		exit(0);
	}
	fprintf(gpFILE, "Program Started Successfulle\n");            
    fprintf(gpFILE, "W = %d  H = %d\n", iWinWidth, iWinHight);

	//*** WNDCLASSEX INITIALIZATION ***
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON)); 
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	//*** REGISTER WNDCLASSEX ***
	RegisterClassEx(&wndclass);

	//*** CREATE WINDOW ***
	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Tushar Tulshiram Wagdare"),
		WS_OVERLAPPEDWINDOW,
		(iWinWidth / 2) - (800 / 2),                  
		(iWinHight / 2) - (600 / 2),                  
		800,                                         
		600,
		NULL,
		NULL,
		hInstance,
		NULL);

	ghwnd = hwnd;

	//*** SHOW THE WINDOW ***
	ShowWindow(hwnd, iCmdShow);

	//*** PAINT/REDROW THE WINDOW ***
	UpdateWindow(hwnd);

	//Initialize
	myControllerIndex = initialize();

	

	//*** Game LOOP ***
	while(bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))       
		{
			if (msg.message == WM_QUIT)
				bDone = TRUE;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//Render
			//getInput1();
			getInput2();
		}
	}

	return((int)msg.wParam);

}

//*** CALLBACK FUNCTION ***
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//*** Function Declaration ***
	void ToggleFullscreen(void);



	//*** Code ***
	switch (iMsg)
	{
	case WM_SETFOCUS:
		gbActive = TRUE;
		break;

	case WM_KILLFOCUS:
			gbActive = FALSE;
			break;

	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;


	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case 'F':
		case 'f':
			if (gbFullscreen == FALSE)
			{
				ToggleFullscreen();
				gbFullscreen = TRUE;
			}
			else
			{
				ToggleFullscreen();
				gbFullscreen = FALSE;
			}
			break;

		}
		break;

	case WM_DESTROY:
		if (gpFILE)
		{
			fprintf(gpFILE, "Program Ended Successfully\n");
			fclose(gpFILE);
			gpFILE = NULL;
		}
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}


void ToggleFullscreen(void)
{
	//*** Local Variable Declaration ****
	MONITORINFO mi = { sizeof(MONITORINFO) };

	//*** Code ***
	if (gbFullscreen == FALSE)
	{
		fprintf(gpFILE, "Window Is Not In Fullscreen Mode\n");

		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			fprintf(gpFILE, "Window Contains WS_OVERLAPPEDWINDOW\n");

			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);  

				fprintf(gpFILE, "By Getting Current Window Placement And Monitors Hight And Width We Removed WS_OVERLAPPEDWINDOW Succesffully\n");

			}
		}
		ShowCursor(FALSE);
	}
	else
	{
		fprintf(gpFILE, "Window Is Now Already In Fullscreen Mode\n");
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		fprintf(gpFILE, "Now Window Is Normal\n");
	}

}


int initialize(void)
{
	DWORD dwResult;

	for (myControllerIndex = 0; myControllerIndex < XUSER_MAX_COUNT; myControllerIndex++)
	{
		ZeroMemory(&myGamePadState, sizeof(XINPUT_STATE));

		// Simply get the myGamePadState of the controller from XInput.
		dwResult = XInputGetState(myControllerIndex, &myGamePadState);

		if (dwResult == ERROR_SUCCESS)
		{
			char buff[64];

			// Controller is connected
			sprintf(buff, "Controller is connected at user index : %lu", myControllerIndex);
			MessageBox(ghwnd, buff, TEXT("SUCCESS"), MB_OK);
			break;
		}
		else if (myControllerIndex == (XUSER_MAX_COUNT-1))
		{
			MessageBox(ghwnd, TEXT("Controller not found !!!"), TEXT("ERROR"), MB_OK);
		}
	}

	///Initialize mouse movement 
	myMouseInput.dwFlags = MOUSEEVENTF_MOVE;
	myMouseInput.dx = 0;
	myMouseInput.dy = 0;
	myMouseInput.mouseData = 0;
	myMouseInput.dwExtraInfo = 0;
	myMouseInput.time = 0;

	myInput.type = INPUT_MOUSE;
	myInput.mi = myMouseInput;


	///
	myGamePadState.Gamepad = myGamepad;



	return(myControllerIndex);
}


int getInput1(void)
{
	unsigned int counter = 0;

	ZeroMemory(&keyPressed, sizeof(XINPUT_KEYSTROKE));

	if (XInputGetKeystroke(myControllerIndex, 0, &keyPressed) == ERROR_SUCCESS)
	{
		switch (keyPressed.VirtualKey)
		{
		case VK_PAD_A:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_A"), TEXT("Message"), MB_OK);
			
			break;

		case VK_PAD_B:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_B"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_X:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_X"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_Y:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_Y"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_RSHOULDER:
			if (keyPressed.Flags == XINPUT_KEYSTROKE_KEYDOWN)
			{
				myMouseInput.dwFlags = MOUSEEVENTF_RIGHTDOWN;
				myMouseInput.dx = 0;
				myMouseInput.dy = 0;
				myInput.mi = myMouseInput;
				SendInput(1, &myInput, sizeof(INPUT));
			}
			else
			{
				myMouseInput.dwFlags = MOUSEEVENTF_RIGHTUP;
				myMouseInput.dx = 0;
				myMouseInput.dy = 0;
				myInput.mi = myMouseInput;
				SendInput(1, &myInput, sizeof(INPUT));
			}
			break;

		case VK_PAD_LSHOULDER:
			if (keyPressed.Flags == XINPUT_KEYSTROKE_KEYDOWN)
			{
				myMouseInput.dwFlags = MOUSEEVENTF_LEFTDOWN;
				myMouseInput.dx = 0;
				myMouseInput.dy = 0;
				myInput.mi = myMouseInput;
				SendInput(1, &myInput, sizeof(INPUT));
			}
			else
			{
				myMouseInput.dwFlags = MOUSEEVENTF_LEFTUP;
				myMouseInput.dx = 0;
				myMouseInput.dy = 0;
				myInput.mi = myMouseInput;
				SendInput(1, &myInput, sizeof(INPUT));
			}
			break;

		case VK_PAD_LTRIGGER:
			//MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_LTRIGGER"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_RTRIGGER:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_RTRIGGER"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_DPAD_UP:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_DPAD_UP"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_DPAD_DOWN:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_DPAD_DOWN"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_DPAD_LEFT:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_DPAD_LEFT"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_DPAD_RIGHT:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_DPAD_RIGHT"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_START:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_START"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_BACK:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_BACK"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_LTHUMB_PRESS:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_LTHUMB_PRESS"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_RTHUMB_PRESS:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_RTHUMB_PRESS"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_LTHUMB_UP:
			counter = 0;

			while (keyPressed.Flags != XINPUT_KEYSTROKE_KEYUP)
			{
				counter++;
				XInputGetKeystroke(myControllerIndex, 0, &keyPressed);
				myMouseInput.dwFlags = MOUSEEVENTF_MOVE;

				myMouseInput.dx = 0;
				if (counter % T_SENSITIVITY == 0)
				{
					fprintf(gpFILE, "UP: %ld\n", counter);
					myMouseInput.dy = -1;
					myInput.mi = myMouseInput;

					
					SendInput(1, &myInput, sizeof(INPUT));
				}
			}
			break;

		case VK_PAD_LTHUMB_DOWN:
			counter = 0;

			while (keyPressed.Flags != XINPUT_KEYSTROKE_KEYUP)
			{
				counter++;

				XInputGetKeystroke(myControllerIndex, 0, &keyPressed);
				myMouseInput.dwFlags = MOUSEEVENTF_MOVE;

				myMouseInput.dx = 0;
				if (counter % T_SENSITIVITY == 0)
				{
					myMouseInput.dy = 1;
					myInput.mi = myMouseInput;


					SendInput(1, &myInput, sizeof(INPUT));
				}
			}
			break;

		case VK_PAD_LTHUMB_RIGHT:
			counter = 0;

			while (keyPressed.Flags != XINPUT_KEYSTROKE_KEYUP)
			{
				counter++;

				XInputGetKeystroke(myControllerIndex, 0, &keyPressed);
				myMouseInput.dwFlags = MOUSEEVENTF_MOVE;

				if (counter % T_SENSITIVITY == 0)
				{
					myMouseInput.dx = 1;
					myMouseInput.dy = 0;
					myInput.mi = myMouseInput;


					SendInput(1, &myInput, sizeof(INPUT));
				}
			}
			break;

		case VK_PAD_LTHUMB_LEFT:
			counter = 0;

			while (keyPressed.Flags != XINPUT_KEYSTROKE_KEYUP)
			{
				counter++;

				XInputGetKeystroke(myControllerIndex, 0, &keyPressed);
				myMouseInput.dwFlags = MOUSEEVENTF_MOVE;

				if (counter % T_SENSITIVITY == 0)
				{
					myMouseInput.dx = -1;
					myMouseInput.dy = 0;
					myInput.mi = myMouseInput;


					SendInput(1, &myInput, sizeof(INPUT));
				}
			}
			break;

		case VK_PAD_LTHUMB_UPLEFT:
			counter = 0;

			while (keyPressed.Flags != XINPUT_KEYSTROKE_KEYUP)
			{
				counter++;

				XInputGetKeystroke(myControllerIndex, 0, &keyPressed);
				myMouseInput.dwFlags = MOUSEEVENTF_MOVE;

				if (counter % T_SENSITIVITY == 0)
				{
					myMouseInput.dx = -1;
					myMouseInput.dy = -1;
					myInput.mi = myMouseInput;


					SendInput(1, &myInput, sizeof(INPUT));
				}
			}
			break;

		case VK_PAD_LTHUMB_UPRIGHT:
			counter = 0;

			while (keyPressed.Flags != XINPUT_KEYSTROKE_KEYUP)
			{
				counter++;

				XInputGetKeystroke(myControllerIndex, 0, &keyPressed);
				myMouseInput.dwFlags = MOUSEEVENTF_MOVE;

				if (counter % T_SENSITIVITY == 0)
				{
					myMouseInput.dx = 1;
					myMouseInput.dy = -1;
					myInput.mi = myMouseInput;


					SendInput(1, &myInput, sizeof(INPUT));
				}
			}
			break;

		case VK_PAD_LTHUMB_DOWNRIGHT:
			counter = 0;

			while (keyPressed.Flags != XINPUT_KEYSTROKE_KEYUP)
			{
				counter++;

				XInputGetKeystroke(myControllerIndex, 0, &keyPressed);
				myMouseInput.dwFlags = MOUSEEVENTF_MOVE;

				if (counter % T_SENSITIVITY == 0)
				{
					myMouseInput.dx = 1;
					myMouseInput.dy = 1;
					myInput.mi = myMouseInput;


					SendInput(1, &myInput, sizeof(INPUT));
				}
			}
			break;

		case VK_PAD_LTHUMB_DOWNLEFT:
			counter = 0;

			while (keyPressed.Flags != XINPUT_KEYSTROKE_KEYUP)
			{
				counter++;

				XInputGetKeystroke(myControllerIndex, 0, &keyPressed);
				myMouseInput.dwFlags = MOUSEEVENTF_MOVE;

				if (counter % T_SENSITIVITY == 0)
				{
					myMouseInput.dx = -1;
					myMouseInput.dy = 1;
					myInput.mi = myMouseInput;


					SendInput(1, &myInput, sizeof(INPUT));
				}
			}
			break;

		case VK_PAD_RTHUMB_UP:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_RTHUMB_UP"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_RTHUMB_DOWN:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_RTHUMB_DOWN"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_RTHUMB_RIGHT:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_RTHUMB_RIGHT"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_RTHUMB_LEFT:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_RTHUMB_LEFT"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_RTHUMB_UPLEFT:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_RTHUMB_UPLEFT"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_RTHUMB_UPRIGHT:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_RTHUMB_UPRIGHT"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_RTHUMB_DOWNRIGHT:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_RTHUMB_DOWNRIGHT"), TEXT("Message"), MB_OK);
			break;

		case VK_PAD_RTHUMB_DOWNLEFT:
			MessageBox(ghwnd, TEXT("KEY PRESSED : VK_PAD_RTHUMB_DOWNLEFT"), TEXT("Message"), MB_OK);
			break;
		}
		
	}

	return(0);
}

int getInput2(void)
{
	DWORD isStateChanged = myGamePadState.dwPacketNumber;
	

	coun = 0;

	if (XInputGetState(myControllerIndex, &myGamePadState) == ERROR_SUCCESS)
	{
		if (isStateChanged != myGamePadState.dwPacketNumber)
		{
			switch (myGamePadState.Gamepad.wButtons)
			{
			case XINPUT_GAMEPAD_DPAD_UP:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_DPAD_UP"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_DPAD_DOWN:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_DPAD_DOWN"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_DPAD_LEFT:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_DPAD_LEFT"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_DPAD_RIGHT:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_DPAD_RIGHT"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_START:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_START"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_BACK:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_BACK"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_LEFT_THUMB:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_LEFT_THUMB"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_RIGHT_THUMB:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_RIGHT_THUMB"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_LEFT_SHOULDER:
				if (XInputGetKeystroke(myControllerIndex, 0, &keyPressed) == ERROR_SUCCESS)
				{
					if (keyPressed.Flags != XINPUT_KEYSTROKE_KEYDOWN)
					{
						myMouseInput.dwFlags = MOUSEEVENTF_LEFTDOWN;
						myMouseInput.dx = 0;
						myMouseInput.dy = 0;
						myInput.mi = myMouseInput;
						SendInput(1, &myInput, sizeof(INPUT));
					}
					else
					{
						myMouseInput.dwFlags = MOUSEEVENTF_LEFTUP;
						myMouseInput.dx = 0;
						myMouseInput.dy = 0;
						myInput.mi = myMouseInput;
						SendInput(1, &myInput, sizeof(INPUT));
						fprintf(gpFILE, "count = %d\n", coun);
					}
				}
				
			

				break;

			case XINPUT_GAMEPAD_RIGHT_SHOULDER: 
				coun++;

				myMouseInput.dwFlags = MOUSEEVENTF_RIGHTDOWN;
				myMouseInput.dx = 0;
				myMouseInput.dy = 0;
				myInput.mi = myMouseInput;
				SendInput(1, &myInput, sizeof(INPUT));

				myMouseInput.dwFlags = MOUSEEVENTF_RIGHTUP;
				myMouseInput.dx = 0;
				myMouseInput.dy = 0;
				myInput.mi = myMouseInput;
				SendInput(1, &myInput, sizeof(INPUT));

				fprintf(gpFILE, "count = %d\n", coun);

				break;

			case XINPUT_GAMEPAD_A:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_A"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_B:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_B"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_X:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_X"), TEXT("Message"), MB_OK);

				break;

			case XINPUT_GAMEPAD_Y:
				MessageBox(ghwnd, TEXT("KEY PRESSED : XINPUT_GAMEPAD_Y"), TEXT("Message"), MB_OK);

				break;
			}

			if (myGamePadState.Gamepad.sThumbLX != 0)
			{
				myMouseInput.dwFlags = MOUSEEVENTF_MOVE;
				myMouseInput.dx = ((myGamePadState.Gamepad.sThumbLX * 10) / 32767);
				myMouseInput.dy = 0;
				myInput.mi = myMouseInput;
				SendInput(1, &myInput, sizeof(INPUT));
			}

			if (myGamePadState.Gamepad.sThumbLY != 0)
			{
				myMouseInput.dwFlags = MOUSEEVENTF_MOVE;
				myMouseInput.dx = 0;
				myMouseInput.dy = -((myGamePadState.Gamepad.sThumbLY * 10) / 32767);
				myInput.mi = myMouseInput;
				SendInput(1, &myInput, sizeof(INPUT));
			}
		}
	}

	return(0);
}
