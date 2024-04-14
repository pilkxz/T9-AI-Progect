#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void LoadDictionary(const std::wstring& path);
void SaveDictionary(const std::wstring& path);
void AddWordToDictionary(const std::wstring& word);

// ���������� ���������� ������ ��� ���
HBRUSH lightBackgroundBrush = CreateSolidBrush(RGB(255, 255, 255));
HBRUSH darkBackgroundBrush = CreateSolidBrush(RGB(50, 50, 50));
COLORREF lightTextColor = RGB(0, 0, 0);
COLORREF darkTextColor = RGB(255, 255, 255);

// ������� ��� ��������� ���
std::vector<std::wstring> dictionary;
std::wstring defaultDictionaryPath = L"dictionary.txt";

// ������� ��� ��������� ���� �����������
void CreateSettingsWindow() {
    // ��������� ����� ����
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProcedure;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"SettingsWindowClass";
    RegisterClass(&wc);

    // ��������� ����
    HWND hwnd = CreateWindow(wc.lpszClassName, L"������������", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, nullptr, nullptr, wc.hInstance, nullptr);

    // ��������� �������� � ���� �����������

    // �������� ���� ��� �������� ����� �� ��������
    CreateWindowW(L"STATIC", L"���� �� ��������:", WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 20, 150, 20, hwnd, nullptr, nullptr, nullptr);
    HWND hDictionaryEdit = CreateWindowW(L"EDIT", defaultDictionaryPath.c_str(), WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 180, 20, 180, 20, hwnd, nullptr, nullptr, nullptr);
    SendMessage(hDictionaryEdit, EM_SETREADONLY, TRUE, 0);

    // ���� ���� ��������
    CreateWindowW(L"STATIC", L"���� ��������:", WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 60, 150, 20, hwnd, nullptr, nullptr, nullptr);
    HWND hThemeComboBox = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 180, 60, 180, 150, hwnd, (HMENU)1001, nullptr, nullptr, nullptr);
    SendMessageW(hThemeComboBox, CB_ADDSTRING, 0, (LPARAM)L"�����");
    SendMessageW(hThemeComboBox, CB_ADDSTRING, 0, (LPARAM)L"�����");

    // ���� ����
    CreateWindowW(L"STATIC", L"���� ��������:", WS_CHILD | WS_VISIBLE | SS_LEFT, 20, 100, 150, 20, hwnd, nullptr, nullptr, nullptr);
    HWND hLanguageComboBox = CreateWindowW(L"COMBOBOX", L"", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, 180, 100, 180, 150, hwnd, (HMENU)1002, nullptr, nullptr, nullptr);
    SendMessageW(hLanguageComboBox, CB_ADDSTRING, 0, (LPARAM)L"���������");
    SendMessageW(hLanguageComboBox, CB_ADDSTRING, 0, (LPARAM)L"���������");
    SendMessageW(hLanguageComboBox, CB_ADDSTRING, 0, (LPARAM)L"��������");

    // ��������� ��� ������ ������������� ����������� ���
    CreateWindowW(L"BUTTON", L"����������� ����������� ���", WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 20, 140, 200, 20, hwnd, nullptr, nullptr, nullptr);

    // ������ "��������"
    CreateWindowW(L"BUTTON", L"��������", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 150, 180, 100, 30, hwnd, (HMENU)1, nullptr, nullptr);

    // ����� ����
    ShowWindow(hwnd, SW_SHOW);
}

// ������� ��� ������������ �������� �� �����
void LoadDictionary(const std::wstring& path) {
    std::wifstream dictionaryFile(path);
    if (dictionaryFile.is_open()) {
        std::wstring word;
        while (std::getline(dictionaryFile, word)) {
            dictionary.push_back(word);
        }
        dictionaryFile.close();
    }
}

// ������� ��� ���������� �������� � ����
void SaveDictionary(const std::wstring& path) {
    std::wofstream dictionaryFile(path);
    if (dictionaryFile.is_open()) {
        for (const auto& word : dictionary) {
            dictionaryFile << word << std::endl;
        }
        dictionaryFile.close();
    }
}

// ������� ��� ��������� ����� �� ��������
void AddWordToDictionary(const std::wstring& word) {
    // ����������, �� ���� ����� �� �� ��������� � ��������
    auto it = std::find(dictionary.begin(), dictionary.end(), word);
    if (it == dictionary.end()) {
        dictionary.push_back(word);
    }
}

// ������� ��� ���� ���� ����
void ChangeTheme(HWND hwnd, bool isDarkTheme) {
    if (isDarkTheme) {
        SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)darkBackgroundBrush);
        SetTextColor(GetDC(hwnd), darkTextColor);
    }
    else {
        SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)lightBackgroundBrush);
        SetTextColor(GetDC(hwnd), lightTextColor);
    }
    RedrawWindow(hwnd, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
}

// ������� ������� ������� ���������� ����
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CTLCOLORSTATIC: { // ������� ����������� ��� ���� ������� ������ ��� ���������� ��������
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(0, 0, 0)); // ���� ������� ������
        SetBkMode(hdcStatic, TRANSPARENT); // �������� ���
        return (LRESULT)GetStockObject(NULL_BRUSH); // ��������� ������� ��� ��������� ����
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == 1) { // ID ������ "��������"
            // �������� ���� �� ��������
            WCHAR dictionaryPath[MAX_PATH];
            GetDlgItemText(hwnd, 1000, dictionaryPath, MAX_PATH);
            defaultDictionaryPath = dictionaryPath;

            // �������� �������
            SaveDictionary(defaultDictionaryPath);

            HWND hThemeComboBox = GetDlgItem(hwnd, 1001); // �������� ���������� ������ ����
            int themeIndex = SendMessageW(hThemeComboBox, CB_GETCURSEL, 0, 0); // �������� ������� ����
            bool isDarkTheme = (themeIndex == 1); // ���� ������ 1, ������ ����� ����

            // ��������� ������� ��� ���� ���� ����
            ChangeTheme(hwnd, isDarkTheme);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int main() {
    // ����������� �������
    LoadDictionary(defaultDictionaryPath);

    // ������ ������� ��� ��������� ���� �����������
    CreateSettingsWindow();

    // �������� ���� ������� ����������
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // ��������� �������
    DeleteObject(lightBackgroundBrush);
    DeleteObject(darkBackgroundBrush);

    return 0;
}
