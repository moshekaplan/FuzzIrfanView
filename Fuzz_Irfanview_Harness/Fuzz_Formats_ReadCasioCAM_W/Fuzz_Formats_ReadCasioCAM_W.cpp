#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadCasioCAM_W(wchar_t *fname);

//Function definition: char* (*ReadCasioCAM_W_handle)(LPCWSTR, unsigned short*, short*);
typedef LPCWSTR* (WINAPIV *ReadCasioCAM_W_t)(LPCWSTR, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadCasioCAM_W_t ReadCasioCAM_W_handle;
wchar_t arg2[2048] = { 0 };
wchar_t arg3[2048] = { 0 };

__declspec(noinline) void call_ReadCasioCAM_W(wchar_t *fname) {
	ReadCasioCAM_W_handle(fname, arg2, arg3);
}

int wmain(int argc, wchar_t **argv)
{
	if (argc != 2) {
		printf("USAGE: %ws input-file\n", argv[0]);
		return 1;
	}

	formats_dll_handle = LoadLibraryA("FORMATS.DLL");
	if (formats_dll_handle == NULL) {
		printf("Failed to load FORMATS.DLL\n");
		return 1;
	}

	ReadCasioCAM_W_handle = (ReadCasioCAM_W_t)(GetProcAddress(formats_dll_handle, "ReadCasioCAM_W"));
	if (ReadCasioCAM_W_handle == NULL) {
		printf("Failed to get address for ReadCasioCAM_W\n");
		return 1;
	}
	wcsncpy(arg3, L"None", 100);
	call_ReadCasioCAM_W(argv[1]);
}