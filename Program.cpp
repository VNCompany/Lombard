#include "pch.h"
#include <CommCtrl.h>
#include <cwchar>

#include "Commands.h"

#pragma comment(lib, "comctl32.lib")

// Components

HWND Edit_Search;

HWND ListView_User;
HWND ListView_Lombard;

// <<< Components

VOID InitializeComponent(HWND);

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
		InitializeComponent(hWnd);
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

VOID InitializeListViews(HWND hWnd);

VOID InitializeComponent(HWND hWnd)
{
	// Search Textbox
	Edit_Search = CreateWindowExW(0L, TEXT("EDIT"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 15, 15, 200, 20, hWnd, NULL, NULL, NULL);
	// Search Button
	CreateWindowExW(0L, TEXT("BUTTON"), TEXT("Поиск"), WS_CHILD | WS_VISIBLE | WS_BORDER, 220, 15, 80, 20, hWnd, (HMENU)IDC_BUTTON_SEARCH, NULL, NULL);

	InitializeListViews(hWnd);

	CreateWindowExW(0L, TEXT("BUTTON"), TEXT("Покупка"), WS_CHILD | WS_VISIBLE | WS_BORDER, 435, 15, 80, 20, hWnd, (HMENU)IDC_BUTTON_SEARCH, NULL, NULL);

	CreateWindowExW(0L, TEXT("BUTTON"), TEXT("Выкуп"), WS_CHILD | WS_VISIBLE | WS_BORDER, 15, 480, 80, 20, hWnd, (HMENU)IDC_BUTTON_SEARCH, NULL, NULL);
}

VOID InitializeListViews(HWND hWnd)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_FMT;
	// User ListView
	ListView_User = CreateWindowExW(0L, WC_LISTVIEW, TEXT(""), WS_CHILD | WS_VISIBLE | LVS_REPORT, 15, 60, 400, 400, hWnd, NULL, NULL, NULL);
	
	lvc.iSubItem = 0;
	lvc.pszText = (LPWSTR)TEXT("Название");
	lvc.cx = 200;
	ListView_InsertColumn(ListView_User, 0, &lvc);
	
	lvc.iSubItem = 1;
	lvc.pszText = (LPWSTR)TEXT("Цена");
	lvc.cx = 80;
	ListView_InsertColumn(ListView_User, 1, &lvc);
	
	lvc.iSubItem = 2;
	lvc.pszText = (LPWSTR)TEXT("Дата");
	lvc.cx = 150;
	ListView_InsertColumn(ListView_User, 2, &lvc);

	// Lombard ListView
	ListView_Lombard = CreateWindowExW(0L, WC_LISTVIEW, TEXT(""), WS_CHILD | WS_VISIBLE | LVS_REPORT, 435, 60, 400, 400, hWnd, NULL, NULL, NULL);

	lvc.iSubItem = 0;
	lvc.pszText = (LPWSTR)TEXT("Название");
	lvc.cx = 250;
	ListView_InsertColumn(ListView_Lombard, 0, &lvc);

	lvc.iSubItem = 1;
	lvc.pszText = (LPWSTR)TEXT("Цена");
	lvc.cx = 80;
	ListView_InsertColumn(ListView_Lombard, 1, &lvc);
}