#include <windows.h>
#include <time.h>

#include "resource.h"

enum LVL { SIMPLY, MIDDLE, HARD } level;
HWND a_hField[3][3];
TCHAR a_cRez[3][3];
TCHAR cSignComp[2] = TEXT("X"),
cSignHum[2] = TEXT("O");
short uNhod(0);

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
HWND compMove(LVL lvl);
TCHAR checkMove(HWND field, TCHAR sign);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst,
	LPSTR lpszCmdLine, int nCmdShow)
{
	MSG msg;

	HWND hDialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG4), NULL, DlgProc);

	ShowWindow(hDialog, nCmdShow);

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT message, WPARAM wp, LPARAM lp)
{
	LRESULT result;
	static TCHAR cRezMove;
	static bool bHod;
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		PostQuitMessage(0);
		return TRUE;

	case WM_INITDIALOG:
	{
		HANDLE hIco1 = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON1));
		HANDLE hIco2 = LoadIcon(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON2));
		SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIco1);
		SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIco1);

		CheckRadioButton(hWnd, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
	}
		return TRUE;

	case WM_COMMAND:
	{
		if (LOWORD(wp) == IDC_BUTTON4)
		{
			int it(0);
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
				{
				a_hField[i][j] = GetDlgItem(hWnd, IDC_BT1 + it++);
				EnableWindow(a_hField[i][j], 1);
				SetWindowText(a_hField[i][j], L"");
				a_cRez[i][j] = ' ';
				}
			result = SendDlgItemMessage(hWnd, IDC_CHECK1, BM_GETCHECK, 0, 0);
			if (result == BST_CHECKED)
			{
				bHod = 0;//Первым ходит пользователь
				lstrcpy(cSignHum, L"X");
				lstrcpy(cSignComp, L"O");
			}
			else
			{
				bHod = 1;
				lstrcpy(cSignComp, L"X");
				lstrcpy(cSignHum, L"O");
			}
			result = SendDlgItemMessage(hWnd, IDC_RADIO1, BM_GETCHECK, 0, 0);
			if (result == BST_CHECKED)
				level = SIMPLY;
			result = SendDlgItemMessage(hWnd, IDC_RADIO2, BM_GETCHECK, 0, 0);
			if (result == BST_CHECKED)
				level = MIDDLE;
			result = SendDlgItemMessage(hWnd, IDC_RADIO3, BM_GETCHECK, 0, 0);
			if (result == BST_CHECKED)
				level = HARD;
			uNhod = 0;
			if (bHod == 1)
			{
				HWND hBt = compMove(level);
				checkMove(hBt, cSignComp[0]);
				SetWindowText(hBt, cSignComp);
				EnableWindow(hBt, 0);
				uNhod++;
			}
		}

		if (LOWORD(wp) >= IDC_BT1 && LOWORD(wp) <= IDC_BT9)
		{
			HWND hBt;
			hBt = GetDlgItem(hWnd, LOWORD(wp));
			SetWindowText(hBt, cSignHum);
			EnableWindow(hBt, 0);
			cRezMove = checkMove(hBt, cSignHum[0]);
			uNhod++;
			if (uNhod == 9 && cRezMove == ' '){
				MessageBox(
					0,
					TEXT("Ничья!"),
					TEXT("Игра окончена!"),
					MB_OK | MB_ICONINFORMATION);
				return TRUE;
			}
			if (cRezMove != ' ')
			{
				MessageBox(
					0,
					TEXT("Вы выиграли, поздравляем!"),
					TEXT("Игра окончена!"),
					MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						EnableWindow(a_hField[i][j], 0);
				return TRUE;
			}
			hBt = compMove(level);
			SetWindowText(hBt, cSignComp);
			EnableWindow(hBt, 0);
			cRezMove = checkMove(hBt, cSignComp[0]);
			uNhod++;
			if (cRezMove != ' ')
			{
				MessageBox(
					0,
					TEXT("К сожалению, выиграл компьютер!"),
					TEXT("Игра окончена!"),
					MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
				for (int i = 0; i < 3; i++)
					for (int j = 0; j < 3; j++)
						EnableWindow(a_hField[i][j], 0);
			}
			if (uNhod == 9 && cRezMove == ' ')
				MessageBox(
				0,
				TEXT("Ничья!"),
				TEXT("Игра окончена!"),
				MB_OK | MB_ICONINFORMATION | MB_TASKMODAL);
		}
	}
		return TRUE;
	}
	return FALSE;
}

HWND compMove(LVL lvl)
{
	srand(time(NULL));
	short i(0), j(0);
	if (lvl == SIMPLY)
	{
		do
		{
			i = rand() % 3;
			j = rand() % 3;
		} while (a_cRez[i][j] != ' ');
		return a_hField[i][j];
	}
	else if (lvl == MIDDLE)//middle level---------------------------
	{
		short count(0);
		TCHAR cPro[2];
		lstrcpy(cPro, cSignComp);
		i = j = -1;
		if (a_cRez[1][1] == ' ')
			return a_hField[1][1];
		else
			while (count < 2)//Check line & diagonals for win or close
			{
			if (a_cRez[0][0] == a_cRez[0][1] &&
				a_cRez[0][0] == cPro[0] &&
				a_cRez[0][2] == ' ')
			{
				i = 0; j = 2;
			}
			else if (a_cRez[0][1] == a_cRez[0][2] &&
				a_cRez[0][1] == cPro[0] &&
				a_cRez[0][0] == ' ')
			{
				i = 0; j = 0;
			}
			else if (a_cRez[0][0] == a_cRez[0][2] &&
				a_cRez[0][0] == cPro[0] &&
				a_cRez[0][1] == ' ')
			{
				i = 0; j = 1;
			}

			else if (a_cRez[1][0] == a_cRez[1][1] &&
				a_cRez[1][0] == cPro[0] &&
				a_cRez[1][2] == ' ')
			{
				i = 1; j = 2;
			}
			else if (a_cRez[1][1] == a_cRez[1][2] &&
				a_cRez[1][1] == cPro[0] &&
				a_cRez[1][0] == ' ')
			{
				i = 1; j = 0;
			}
			else if (a_cRez[1][0] == a_cRez[1][2] &&
				a_cRez[1][0] == cPro[0] &&
				a_cRez[1][1] == ' ')
			{
				i = 1; j = 1;
			}

			else if (a_cRez[2][0] == a_cRez[2][1] &&
				a_cRez[2][0] == cPro[0] &&
				a_cRez[2][2] == ' ')
			{
				i = 2; j = 2;
			}
			else if (a_cRez[2][1] == a_cRez[2][2] &&
				a_cRez[2][1] == cPro[0] &&
				a_cRez[2][0] == ' ')
			{
				i = 2; j = 0;
			}
			else if (a_cRez[2][0] == a_cRez[2][2] &&
				a_cRez[2][0] == cPro[0] &&
				a_cRez[2][1] == ' ')
			{
				i = 2; j = 1;
			}

			else if (a_cRez[0][0] == a_cRez[1][0] &&
				a_cRez[0][0] == cPro[0] &&
				a_cRez[2][0] == ' ')
			{
				i = 2; j = 0;
			}
			else if (a_cRez[1][0] == a_cRez[2][0] &&
				a_cRez[1][0] == cPro[0] &&
				a_cRez[0][0] == ' ')
			{
				i = 0; j = 0;
			}
			else if (a_cRez[0][0] == a_cRez[2][0] &&
				a_cRez[0][0] == cPro[0] &&
				a_cRez[1][0] == ' ')
			{
				i = 1; j = 0;
			}

			else if (a_cRez[0][1] == a_cRez[1][1] &&
				a_cRez[0][1] == cPro[0] &&
				a_cRez[2][1] == ' ')
			{
				i = 2; j = 1;
			}
			else if (a_cRez[1][1] == a_cRez[2][1] &&
				a_cRez[1][1] == cPro[0] &&
				a_cRez[0][1] == ' ')
			{
				i = 0; j = 1;
			}
			else if (a_cRez[0][1] == a_cRez[2][1] &&
				a_cRez[0][1] == cPro[0] &&
				a_cRez[1][1] == ' ')
			{
				i = 1; j = 1;
			}

			else if (a_cRez[0][2] == a_cRez[1][2] &&
				a_cRez[0][2] == cPro[0] &&
				a_cRez[2][2] == ' ')
			{
				i = 2; j = 2;
			}
			else if (a_cRez[1][2] == a_cRez[2][2] &&
				a_cRez[1][2] == cPro[0] &&
				a_cRez[0][2] == ' ')
			{
				i = 0; j = 2;
			}
			else if (a_cRez[0][2] == a_cRez[2][2] &&
				a_cRez[0][2] == cPro[0] &&
				a_cRez[1][2] == ' ')
			{
				i = 1; j = 2;
			}

			else if (a_cRez[0][0] == a_cRez[1][1] &&
				a_cRez[0][0] == cPro[0] &&
				a_cRez[2][2] == ' ')
			{
				i = 2; j = 2;
			}
			else if (a_cRez[1][1] == a_cRez[2][2] &&
				a_cRez[1][1] == cPro[0] &&
				a_cRez[0][0] == ' ')
			{
				i = 0; j = 0;
			}
			else if (a_cRez[0][0] == a_cRez[2][2] &&
				a_cRez[0][0] == cPro[0] &&
				a_cRez[1][1] == ' ')
			{
				i = 1; j = 1;
			}

			else if (a_cRez[2][0] == a_cRez[1][1] &&
				a_cRez[2][0] == cPro[0] &&
				a_cRez[0][2] == ' ')
			{
				i = 0; j = 2;
			}
			else if (a_cRez[1][1] == a_cRez[0][2] &&
				a_cRez[1][1] == cPro[0] &&
				a_cRez[2][0] == ' ')
			{
				i = 2; j = 0;
			}
			else if (a_cRez[2][0] == a_cRez[0][2] &&
				a_cRez[2][0] == cPro[0] &&
				a_cRez[1][1] == ' ')
			{
				i = 1; j = 1;
			}
			if (i >= 0)
				break;
			count++;
			lstrcpy(cPro, cSignHum);
			}
		if (i >= 0)
			return a_hField[i][j];
		else
			compMove(SIMPLY);
	}
	else if (lvl == HARD)//hard level------------------------------------------------------
	{
		i = j = -1;

		if (uNhod == 0)//if computer begin close angle
			if (a_cRez[0][0] == ' ')
			{
			i = 0; j = 0;
			}
			else if (a_cRez[0][2] == ' ')
			{
				i = 0; j = 2;
			}
			else if (a_cRez[2][2] == ' ')
			{
				i = 2; j = 2;
			}
			else if (a_cRez[2][0] == ' ')
			{
				i = 2; j = 0;
			}

			if (uNhod == 1)//if human begin close centre or angle
				if (a_cRez[1][1] == ' ')
				{
				i = 1; j = 1;
				}
				else if (a_cRez[0][0] == ' ')
				{
					i = 0; j = 0;
				}
				else if (a_cRez[0][2] == ' ')
				{
					i = 0; j = 2;
				}
				else if (a_cRez[2][2] == ' ')
				{
					i = 2; j = 2;
				}
				else if (a_cRez[2][0] == ' ')
				{
					i = 2; j = 0;
				}

				if (uNhod == 2)//if computer begin play after enemy progress
					if (a_cRez[0][2] == cSignComp[0])
					{
					if (a_cRez[0][1] == cSignHum[0])
					{
						i = 1; j = 1;
					}
					else if (a_cRez[0][0] == cSignHum[0])
					{
						i = 1; j = 2;
					}
					else if (a_cRez[1][0] == cSignHum[0] ||
						a_cRez[2][0] == cSignHum[0])
					{
						i = 1; j = 1;
					}
					else if (a_cRez[1][1] == cSignHum[0])
					{
						i = 2; j = 0;
					}
					}
					else if (a_cRez[2][2] == cSignComp[0])
					{
						if (a_cRez[1][2] == cSignHum[0])
						{
							i = 1; j = 1;
						}
						else if (a_cRez[0][2] == cSignHum[0])
						{
							i = 2; j = 1;
						}
						else if (a_cRez[0][1] == cSignHum[0] ||
							a_cRez[0][0] == cSignHum[0])
						{
							i = 1; j = 1;
						}
						else if (a_cRez[1][1] == cSignHum[0])
						{
							i = 0; j = 0;
						}
					}
					else if (a_cRez[2][0] == cSignComp[0])
					{
						if (a_cRez[2][1] == cSignHum[0])
						{
							i = 1; j = 1;
						}
						else if (a_cRez[2][2] == cSignHum[0])
						{
							i = 1; j = 0;
						}
						else if (a_cRez[1][2] == cSignHum[0] ||
							a_cRez[0][2] == cSignHum[0])
						{
							i = 1; j = 1;
						}
						else if (a_cRez[1][1] == cSignHum[0])
						{
							i = 0; j = 2;
						}
					}
					else if (a_cRez[0][0] == cSignComp[0])
					{
						if (a_cRez[1][0] == cSignHum[0])
						{
							i = 1; j = 1;
						}
						else if (a_cRez[2][0] == cSignHum[0])
						{
							i = 0; j = 1;
						}
						else if (a_cRez[2][1] == cSignHum[0] ||
							a_cRez[2][2] == cSignHum[0])
						{
							i = 1; j = 1;
						}
						else if (a_cRez[1][1] == cSignHum[0])
						{
							i = 2; j = 2;
						}

						if (a_cRez[1][1] == cSignComp[0] &&
							a_cRez[0][0] == cSignHum[0] &&
							a_cRez[2][2] == ' ')
						{
							i = 2; j = 2;
						}
						else if (a_cRez[1][1] == cSignComp[0] &&
							a_cRez[2][2] == cSignHum[0] &&
							a_cRez[0][0] == ' ')
						{
							i = 0; j = 0;
						}
						else if (a_cRez[1][1] == cSignComp[0] &&
							a_cRez[2][0] == cSignHum[0] &&
							a_cRez[0][2] == ' ')
						{
							i = 0; j = 2;
						}
						else if (a_cRez[1][1] == cSignComp[0] &&
							a_cRez[0][2] == cSignHum[0] &&
							a_cRez[2][0] == ' ')
						{
							i = 2; j = 0;
						}
					}

					if (uNhod == 3)//if human begin, close opposite angle
					{
						if (a_cRez[1][1] == cSignComp[0])//answers for non-logic progress of human
						{
							if (a_cRez[0][0] == cSignComp[0] &&
								(a_cRez[0][1] == cSignHum[0] ||
								a_cRez[1][0] == cSignHum[0]))
							{
								if (a_cRez[1][0] == ' ')
								{
									i = 1; j = 0;
								}
								else if (a_cRez[0][1] == ' ')
								{
									i = 0; j = 1;
								}
							}
							else if (a_cRez[0][2] == cSignComp[0] &&
								(a_cRez[1][2] == cSignHum[0] ||
								a_cRez[0][1] == cSignHum[0]))
							{
								if (a_cRez[1][2] == ' ')
								{
									i = 1; j = 2;
								}
								else if (a_cRez[0][1] == ' ')
								{
									i = 0; j = 1;
								}
							}
							else if (a_cRez[2][2] == cSignComp[0] &&
								(a_cRez[1][2] == cSignHum[0] ||
								a_cRez[2][1] == cSignHum[0]))
							{
								if (a_cRez[1][2] == ' ')
								{
									i = 1; j = 2;
								}
								else if (a_cRez[2][1] == ' ')
								{
									i = 2; j = 1;
								}
							}
							else if (a_cRez[2][0] == cSignComp[0] &&
								(a_cRez[1][0] == cSignHum[0] ||
								a_cRez[2][1] == cSignHum[0]))
							{
								if (a_cRez[1][0] == ' ')
								{
									i = 1; j = 0;
								}
								else if (a_cRez[2][1] == ' ')
								{
									i = 2; j = 1;
								}
							}
						}
					}

					if (i >= 0)
						return a_hField[i][j];
					else
						compMove(MIDDLE);
	}
}

TCHAR checkMove(HWND field, TCHAR sign)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (a_hField[i][j] == field)
			{
		a_cRez[i][j] = sign;
		i = 3;
		break;
			}

	if (a_cRez[0][0] == a_cRez[0][1] &&
		a_cRez[0][1] == a_cRez[0][2] &&
		a_cRez[0][0] != ' ')
		return a_cRez[0][0];
	if (a_cRez[1][0] == a_cRez[1][1] &&
		a_cRez[1][1] == a_cRez[1][2] &&
		a_cRez[1][0] != ' ')
		return a_cRez[1][0];
	if (a_cRez[2][0] == a_cRez[2][1] &&
		a_cRez[2][1] == a_cRez[2][2] &&
		a_cRez[2][0] != ' ')
		return a_cRez[2][0];
	if (a_cRez[0][0] == a_cRez[1][0] &&
		a_cRez[1][0] == a_cRez[2][0] &&
		a_cRez[0][0] != ' ')
		return a_cRez[0][0];
	if (a_cRez[0][1] == a_cRez[1][1] &&
		a_cRez[1][1] == a_cRez[2][1] &&
		a_cRez[0][1] != ' ')
		return a_cRez[0][1];
	if (a_cRez[0][2] == a_cRez[1][2] &&
		a_cRez[1][2] == a_cRez[2][2] &&
		a_cRez[0][2] != ' ')
		return a_cRez[0][2];
	if (a_cRez[0][0] == a_cRez[1][1] &&
		a_cRez[1][1] == a_cRez[2][2] &&
		a_cRez[0][0] != ' ')
		return a_cRez[0][0];
	if (a_cRez[2][0] == a_cRez[1][1] &&
		a_cRez[1][1] == a_cRez[0][2] &&
		a_cRez[2][0] != ' ')
		return a_cRez[2][0];
	return ' ';
}