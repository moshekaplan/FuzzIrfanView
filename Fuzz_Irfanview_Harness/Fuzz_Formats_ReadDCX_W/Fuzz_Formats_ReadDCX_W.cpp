#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadDCX_W(wchar_t *fname);

typedef LPCWSTR* (WINAPIV *ReadDCX_W_t)(LPCWSTR, int, LPCWSTR, int, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadDCX_W_t ReadDCX_W_handle;
wchar_t fname2[2048] = { 0 };
wchar_t arg5[2048] = { 0 };
wchar_t arg_none[2048] = { 0 };

__declspec(noinline) void call_ReadDCX_W(wchar_t *fname) {
	ReadDCX_W_handle(fname, 0x0, fname2, 0x0, arg5, arg_none);
	ReadDCX_W_handle(fname, 0x1000, 0x0, 0x0, arg5, arg_none);
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

	ReadDCX_W_handle = (ReadDCX_W_t)(GetProcAddress(formats_dll_handle, "ReadDCX_W"));
	if (ReadDCX_W_handle == NULL) {
		printf("Failed to get address for ReadDCX_W\n");
		return 1;
	}
	wcsncpy(arg_none, L"None", 100);
	wcsncpy(fname2, L"None", 100);
	call_ReadDCX_W(argv[1]);
}