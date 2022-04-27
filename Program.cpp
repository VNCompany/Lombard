#include "pch.h"
#include <CommCtrl.h>
#include <cwchar>
#include <algorithm>

#include "Commands.h"
#include "Data.h"

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

// Functions

INT GetSelectedIndex(HWND);
VOID AddItemUser(int, const Item&);
VOID AddItemLombard(int, const Item&);

// <<< Functions

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		InitializeComponent(hWnd);
		int increment = 0;
		for (const Item& item : GetItems())
		{
			if (item.IsForSale == L"true")
				AddItemLombard(increment++, item);
		}
		break;
	}

	case WM_COMMAND:
	{
		if (LOWORD(wParam) == IDC_BUTTON_SEARCH)
		{
			ListView_DeleteAllItems(ListView_User);

			WCHAR searchText[32];
			GetWindowTextW(Edit_Search, searchText, 32);

			int increment = 0;
			for (const Item& item : GetItems())
			{
				if (wcscmp(searchText, (LPWSTR)item.Id.c_str()) == 0
					&& item.IsForSale == L"false")
				{
					AddItemUser(increment++, item);
				}
			}

			if (increment == 0)
				MessageBoxW(hWnd, TEXT("Пользователь с указанным ID не найден."), TEXT("Ошибка"), MB_OK | MB_ICONERROR);
		}

		if (LOWORD(wParam) == IDC_BUTTON_BUY)
		{
			int index = GetSelectedIndex(ListView_Lombard);
			if (index != -1)
			{	
				{
					int i = 0;
					for (const Item& item : GetItems())
					{
						if (item.IsForSale == L"true")
						{
							if (i == index)
							{
								GetItems().erase(std::next(GetItems().begin(), i));
								break;
							}
							i++;
						}
					}
				}
				ListView_DeleteItem(ListView_Lombard, index);
			}
		}

		if (LOWORD(wParam) == IDC_BUTTON_BUYOUT)
		{
			int index = GetSelectedIndex(ListView_User);
			if (index != -1)
			{
				{
					int i = 0;
					for (const Item& item : GetItems())
					{
						if (item.IsForSale == L"false")
						{
							if (i == index)
							{
								GetItems().erase(std::next(GetItems().begin(), i));
								break;
							}
							i++;
						}
					}
				}
				ListView_DeleteItem(ListView_User, index);
			}
		}
		break;
	}

	case WM_DESTROY:
		SaveItems();
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

	CreateWindowExW(0L, TEXT("BUTTON"), TEXT("Покупка"), WS_CHILD | WS_VISIBLE | WS_BORDER, 435, 15, 80, 20, hWnd, (HMENU)IDC_BUTTON_BUY, NULL, NULL);

	CreateWindowExW(0L, TEXT("BUTTON"), TEXT("Выкуп"), WS_CHILD | WS_VISIBLE | WS_BORDER, 15, 480, 80, 20, hWnd, (HMENU)IDC_BUTTON_BUYOUT, NULL, NULL);
}

VOID InitializeListViews(HWND hWnd)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH | LVCF_FMT;
	// User ListView
	ListView_User = CreateWindowExW(0L, WC_LISTVIEW, TEXT(""), WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL, 15, 60, 400, 400, hWnd, NULL, NULL, NULL);
	
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
	ListView_Lombard = CreateWindowExW(0L, WC_LISTVIEW, TEXT(""), WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL, 435, 60, 400, 400, hWnd, NULL, NULL, NULL);

	lvc.iSubItem = 0;
	lvc.pszText = (LPWSTR)TEXT("Название");
	lvc.cx = 250;
	ListView_InsertColumn(ListView_Lombard, 0, &lvc);

	lvc.iSubItem = 1;
	lvc.pszText = (LPWSTR)TEXT("Цена");
	lvc.cx = 80;
	ListView_InsertColumn(ListView_Lombard, 1, &lvc);
}

INT GetSelectedIndex(HWND listView)
{
	INT index = ListView_GetNextItem(listView, -1, LVNI_SELECTED);
	return index;
}

VOID AddItemUser(int index, const Item& item)
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVFI_PARAM;
	lvi.iItem = index;
	lvi.iSubItem = 0;
	lvi.pszText = (LPWSTR)item.Name.c_str();
	lvi.lParam = 123;
	ListView_InsertItem(ListView_User, &lvi);

	lvi.iSubItem = 1;
	lvi.pszText = (LPWSTR)item.Price.c_str();
	ListView_SetItem(ListView_User, &lvi);

	lvi.iSubItem = 2;
	lvi.pszText = (LPWSTR)item.Date.c_str();
	ListView_SetItem(ListView_User, &lvi);
}

VOID AddItemLombard(int index, const Item& item)
{
	LVITEM lvi;
	lvi.mask = LVIF_TEXT | LVFI_PARAM;
	lvi.iItem = index;
	lvi.iSubItem = 0;
	lvi.pszText = (LPWSTR)item.Name.c_str();
	lvi.lParam = 123;
	ListView_InsertItem(ListView_Lombard, &lvi);

	lvi.iSubItem = 1;
	lvi.pszText = (LPWSTR)item.Price.c_str();
	ListView_SetItem(ListView_Lombard, &lvi);
}