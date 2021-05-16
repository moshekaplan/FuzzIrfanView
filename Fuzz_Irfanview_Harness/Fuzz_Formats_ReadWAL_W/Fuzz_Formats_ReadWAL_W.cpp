#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadWAL_W(wchar_t *fname);

typedef LPCWSTR* (WINAPIV *ReadWAL_W_t)(LPCWSTR, wchar_t*, wchar_t*);

HMODULE formats_dll_handle;
ReadWAL_W_t ReadWAL_W_handle;
wchar_t arg2[2048] = { 0 };
wchar_t arg3[2048] = { 0 };

__declspec(noinline) void call_ReadWAL_W(wchar_t *fname) {
	ReadWAL_W_handle(fname, arg2, arg3);
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

	ReadWAL_W_handle = (ReadWAL_W_t)(GetProcAddress(formats_dll_handle, "ReadWAL_W"));
	if (ReadWAL_W_handle == NULL) {
		printf("Failed to get address for ReadWAL_W\n");
		return 1;
	}
	call_ReadWAL_W(argv[1]);
}