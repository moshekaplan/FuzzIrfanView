#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadWPG_W(wchar_t *fname);

//Function definition: char* (*ReadPVR_W_handle)(LPCWSTR, unsigned short*, short*);
typedef LPCWSTR* (WINAPIV *ReadWPG_W_t)(LPCWSTR, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadWPG_W_t ReadWPG_W_handle;
wchar_t arg2[2048] = { 0 };
wchar_t arg3[2048] = { 0 };

__declspec(noinline) void call_ReadWPG_W(wchar_t *fname) {
	ReadWPG_W_handle(fname, arg2, arg3);
}

int wmain(int argc, wchar_t **argv)
{
	if (argc != 2) {
		printf("USAGE: %ws input-file\n", argv[0]);
		return 1;
	}

	formats_dll_handle = LoadLibraryA("WPG.DLL");
	if (formats_dll_handle == NULL) {
		printf("Failed to load WPG.DLL\n");
		return 1;
	}

	ReadWPG_W_handle = (ReadWPG_W_t)(GetProcAddress(formats_dll_handle, "ReadWPG_W"));
	if (ReadWPG_W_handle == NULL) {
		printf("Failed to get address for ReadWPG_W\n");
		return 1;
	}
	wcsncpy(arg3, L"None", 100);
	call_ReadWPG_W(argv[1]);
}