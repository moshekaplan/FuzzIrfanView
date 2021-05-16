#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadG32(char *fname);

typedef LPCSTR* (WINAPIV *ReadG32_t)(char*, char*, char*);

HMODULE formats_dll_handle;
ReadG32_t ReadG32_handle;
char arg2[2048] = { 0 };
char arg3[2048] = { 0 };

__declspec(noinline) void call_ReadG32(char *fname) {
	ReadG32_handle(fname, arg2, arg3);
	/*
	Irfan wrote to me on 2021-04-30:
	CMP is actually not supported in IrfanView ... this was only a test
	format for one user ;-)
	(same as G32 format)
	*/
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

	ReadG32_handle = (ReadG32_t)(GetProcAddress(formats_dll_handle, "ReadG32"));
	if (ReadG32_handle == NULL) {
		printf("Failed to get address for ReadG32_handle\n");
		return 1;
	}
	call_ReadG32(argv[1]);
}