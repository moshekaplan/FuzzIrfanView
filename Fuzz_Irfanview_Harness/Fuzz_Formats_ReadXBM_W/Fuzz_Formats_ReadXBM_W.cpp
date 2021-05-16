#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadXBM_W(wchar_t *fname);

typedef LPCWSTR* (WINAPIV *ReadXBM_W_t)(LPCWSTR, wchar_t*);

HMODULE formats_dll_handle;
ReadXBM_W_t ReadXBM_W_handle;
wchar_t arg2[2048] = { 0 };

__declspec(noinline) void call_ReadXBM_W(wchar_t *fname) {
	ReadXBM_W_handle(fname, arg2);
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

	ReadXBM_W_handle = (ReadXBM_W_t)(GetProcAddress(formats_dll_handle, "ReadXBM_W"));
	if (ReadXBM_W_handle == NULL) {
		printf("Failed to get address for ReadXBM_W\n");
		return 1;
	}
	call_ReadXBM_W(argv[1]);
}