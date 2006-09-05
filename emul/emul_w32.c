#include <windows.h>
#include "GUI/guiserv.h"
#include "GUI/graphics.h"

unsigned char VideoRam[SCREEN_TOTAL];

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPTSTR    lpCmdLine,
		int       nCmdShow)
{
	MSG msg;
	struct CEventDescriptor event;

	InitGUIServer();
	ClearScreen();
	event.Type = EVENT_NULL;
	while (!ProcessGUIServer(&event))
		;

	MyRegisterClass(hInstance);
	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int) msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(NULL, (LPCTSTR)IDI_APPLICATION);
	wcex.hCursor		= LoadCursor(NULL, IDC_HAND);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= "EMUL_WINDOW";
	wcex.hIconSm		= LoadIcon(NULL, (LPCTSTR)IDI_APPLICATION);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	RECT rcWnd;

	hWnd = CreateWindow("EMUL_WINDOW", "Emul", WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, 320, 240, NULL, NULL, hInstance, NULL);

	if (!hWnd)
		return FALSE;

	if (GetClientRect(hWnd, &rcWnd))
	{
		rcWnd.right = SCREEN_WIDTH;
		rcWnd.bottom = SCREEN_HEIGHT;

		if (AdjustWindowRectEx(&rcWnd, WS_OVERLAPPEDWINDOW, FALSE, 0))
		{
			UINT uFlags = SWP_NOZORDER | SWP_NOMOVE;
			SetWindowPos(hWnd, NULL, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top, uFlags);
		}
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	PAINTSTRUCT ps;
	HDC hdc;
	struct CEventDescriptor event;

	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		for (y = 0; y < SCREEN_HEIGHT; ++y)
			for (x = 0; x < SCREEN_WIDTH; ++x)
				if (IsPixelSet(x, y))
					SetPixel(hdc, x, y, 0);
		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:	/* ESC key */
			DestroyWindow(hWnd);
			return 0;
		case VK_LEFT:
			event.key = K_LEFT;
			break;
		case VK_RIGHT:
			event.key = K_RIGHT;
			break;
		case VK_UP:
			event.key = K_UP;
			break;
		case VK_DOWN:
			event.key = K_DOWN;
			break;
		case '0':
		case VK_NUMPAD0:
			event.key = K_0;
			break;
		case '1':
		case VK_NUMPAD1:
			event.key = K_1;
			break;
		case '2':
		case VK_NUMPAD2:
			event.key = K_2;
			break;
		case '3':
		case VK_NUMPAD3:
			event.key = K_3;
			break;
		case '4':
		case VK_NUMPAD4:
			event.key = K_4;
			break;
		case '5':
		case VK_NUMPAD5:
			event.key = K_5;
			break;
		case '6':
		case VK_NUMPAD6:
			event.key = K_6;
			break;
		case '7':
		case VK_NUMPAD7:
			event.key = K_7;
			break;
		case '8':
		case VK_NUMPAD8:
			event.key = K_8;
			break;
		case '9':
		case VK_NUMPAD9:
			event.key = K_9;
			break;
		case VK_END:
			event.key = K_PGDOWN;
			break;
		case VK_HOME:
			event.key = K_PGUP;
			break;
		case VK_ADD:
			event.key = K_INC;
			break;
		case VK_SUBTRACT:
			event.key = K_DEC;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		event.Type = EVENT_KEYBOARD;
		while (!ProcessGUIServer(&event))
			;
		event.Type = EVENT_NULL;
		while (!ProcessGUIServer(&event))
		;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_LBUTTONDOWN:
		event.Type = EVENT_TOUCHSCREEN;
		event.x = LOWORD(lParam);
		event.y = HIWORD(lParam);
		while (!ProcessGUIServer(&event))
			;
		event.Type = EVENT_NULL;
		while (!ProcessGUIServer(&event))
			;
		InvalidateRect(hWnd, NULL, TRUE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

