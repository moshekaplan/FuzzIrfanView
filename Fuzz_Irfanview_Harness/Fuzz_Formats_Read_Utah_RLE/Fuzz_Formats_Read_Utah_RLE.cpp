#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_Read_Utah_RLE(char *fname);

typedef LPCWSTR* (WINAPIV *Read_Utah_RLE_t)(char*, wchar_t*);

HMODULE formats_dll_handle;
Read_Utah_RLE_t Read_Utah_RLE_handle;
wchar_t arg2[2048] = { 0 };
wchar_t arg3[2048] = { 0 };

__declspec(noinline) void call_Read_Utah_RLE(char *fname) {
	Read_Utah_RLE_handle(fname, arg2);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("USAGE: %s input-file\n", argv[0]);
		return 1;
	}

	formats_dll_handle = LoadLibraryA("FORMATS.DLL");
	if (formats_dll_handle == NULL) {
		printf("Failed to load FORMATS.DLL\n");
		return 1;
	}

	Read_Utah_RLE_handle = (Read_Utah_RLE_t)(GetProcAddress(formats_dll_handle, "Read_Utah_RLE"));
	if (Read_Utah_RLE_handle == NULL) {
		printf("Failed to get address for Read_Utah_RLE_handle\n");
		return 1;
	}
	call_Read_Utah_RLE(argv[1]);
}