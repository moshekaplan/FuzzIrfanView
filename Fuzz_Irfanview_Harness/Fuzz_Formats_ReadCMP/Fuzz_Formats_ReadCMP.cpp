#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadCMP(char *fname);

typedef LPCWSTR* (WINAPIV *ReadCMP_t)(char*, char*, char*);

HMODULE formats_dll_handle;
ReadCMP_t ReadCMP_handle;
char arg2[2048] = { 0 };
char arg3[2048] = { 0 };

__declspec(noinline) void call_ReadCMP(char *fname) {
	ReadCMP_handle(fname, arg2, arg3);
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

	ReadCMP_handle = (ReadCMP_t)(GetProcAddress(formats_dll_handle, "ReadCMP"));
	if (ReadCMP_handle == NULL) {
		printf("Failed to get address for ReadCMP_handle\n");
		return 1;
	}
	call_ReadCMP(argv[1]);
}