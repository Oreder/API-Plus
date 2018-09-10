#include <Windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static char szAppName[] = "EX130509";
	HWND        hwnd;
	MSG         msg;
	WNDCLASSEX  wndclass;

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szAppName,      // window class name
		"API C++ Project",				// window caption
		WS_OVERLAPPEDWINDOW,			// window style
		CW_USEDEFAULT,					// initial x position
		CW_USEDEFAULT,					// initial y position
		CW_USEDEFAULT,					// initial x size
		CW_USEDEFAULT,					// initial y size
		NULL,							// parent window handle
		NULL,							// window menu handle
		hInstance,						// program instance handle
		NULL);							// creation parameters

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;

	static int	cxClient, cyClient, color;
	static bool flagTimer;
	static int	isLeftMouse;	// 1 - left, 2 - up, 3 - right, 4 - down
	static int	curX, curY;
	static int	delta;
	switch (iMsg)
	{
	case WM_CREATE:		// Constructor
	{
		// Mouse flag
		isLeftMouse = 0;

		// Paint move
		delta = 20;
		curX = 200;
		curY = 200;
		
		// Setting timer
		flagTimer = true;
		SetTimer(hwnd, 1, 200, NULL);
		return 0;
	}
	case WM_TIMER:		// OnTimer event
	{
		InvalidateRect(hwnd, 0, TRUE);
		return 0;
	}
	case WM_LBUTTONDOWN:	// OnLeftMouse event - do our stuff
	{
		// update left mouse in range [1, 4]
		if ((++isLeftMouse) == 5)
			isLeftMouse = 1;

		InvalidateRect(hwnd, 0, TRUE);
		return 0;
	}
	case WM_RBUTTONDOWN:	// OnRightMouse event - pause
	{
		flagTimer = !flagTimer;
		if (flagTimer)
			SetTimer(hwnd, 1, 200, NULL);
		else
			KillTimer(hwnd, 1);
		return 0;
	}
	case WM_SIZE:	// Change window's size
	{
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;
	}
	case WM_PAINT:	// Painting event
	{
		// Starting paint screen
		hdc = BeginPaint(hwnd, &ps);		

		// Setting pen
		char red, green, blue;
		color = rand() & 0xffffff;	// randomize pen's color
		red = color & 255;
		green = (color >> 8) & 255;
		blue = (color >> 16) & 255;
		
		// Creating pen
		HPEN hpen = CreatePen(PS_SOLID, 2, RGB(red, green, blue));
		SelectObject(hdc, hpen);

		HBRUSH hBrush = CreateSolidBrush(RGB(rand() & 255, rand() & 255, rand() & 255));
		SelectObject(hdc, hBrush);
		
		// Update current position of our figure
		switch (isLeftMouse)
		{
		case 1: curX -= delta; break;
		case 2: curY -= delta; break;
		case 3: curX += delta; break;
		case 4: curY += delta; break;
		}

		// Draw our stuff
		Rectangle(hdc, curX, curY, curX + 50, curY + 80);
		
		char buf[256];
		TextOut(hdc, 10, 10, buf, wsprintf(buf, "M: %d\tX: %d\t Y: %d", isLeftMouse, curX, curY));


		GetClientRect(hwnd, &rect);
		DeleteObject(hBrush);
		DeleteObject(hpen);

		ReleaseDC(hwnd, hdc);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:	// Destructor
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, iMsg, wParam, lParam);
}