#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadMosaic_W(wchar_t *fname);

typedef LPCWSTR* (WINAPIV *ReadMosaic_W_t)(LPCWSTR, wchar_t*);

HMODULE formats_dll_handle;
ReadMosaic_W_t ReadMosaic_W_handle;
wchar_t arg2[2048] = { 0 };

__declspec(noinline) void call_ReadMosaic_W(wchar_t *fname) {
	ReadMosaic_W_handle(fname, arg2);
	// Note: File extension must be .mos to trigger the
	// mosaic handler, per https://irfanview-forum.de/forum/program/support/10017-
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

	ReadMosaic_W_handle = (ReadMosaic_W_t)(GetProcAddress(formats_dll_handle, "ReadMosaic_W"));
	if (ReadMosaic_W_handle == NULL) {
		printf("Failed to get address for ReadMosaic_W\n");
		return 1;
	}
	call_ReadMosaic_W(argv[1]);
}