#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadWBMP_W(wchar_t *fname);

typedef LPCWSTR* (WINAPIV *ReadWBMP_W_t)(LPCWSTR, wchar_t*);

HMODULE formats_dll_handle;
ReadWBMP_W_t ReadWBMP_W_handle;
wchar_t arg2[2048] = { 0 };

__declspec(noinline) void call_ReadWBMP_W(wchar_t *fname) {
	ReadWBMP_W_handle(fname, arg2);
	// Note: Files must end with extension .wbmp to trigger
	// the wbmp handler in Irfanview
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

	ReadWBMP_W_handle = (ReadWBMP_W_t)(GetProcAddress(formats_dll_handle, "ReadWBMP_W"));
	if (ReadWBMP_W_handle == NULL) {
		printf("Failed to get address for ReadWBMP_W\n");
		return 1;
	}
	call_ReadWBMP_W(argv[1]);
}