#include "pch.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR cmdLine, INT nShowCmd)
{
	WNDCLASSW wndClass = { 0 };
	wndClass.hInstance = hInst;
	wndClass.hCursor = LoadCursorW(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wndClass.lpszClassName = TEXT("mainWindow");
	wndClass.lpfnWndProc = WndProc;

	if (!RegisterClassW(&wndClass))
		return -1;

	CreateWindowExW(0L, TEXT("mainWindow"), TEXT("Ломбард"), (WS_VISIBLE | WS_OVERLAPPEDWINDOW) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX, 30, 30, 900, 600, NULL, NULL, NULL, NULL);

	MSG msg;
	while (GetMessageW(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:

		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProcW(hWnd, msg, wParam, lParam);
		break;
	}
	return 0;
}