#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdio.h>

extern "C" __declspec(dllexport) void call_ReadGLCD_W(wchar_t *fname);

//ReadGLCD_W(LPCWSTR param_1_fname,int param_2,undefined4 param_3,undefined4 param_4, ushort *param_5_out)
typedef LPCWSTR* (WINAPIV *ReadGLCD_W_t)(LPCWSTR, int, int, int, wchar_t*);

HMODULE formats_dll_handle;
ReadGLCD_W_t ReadGLCD_W_handle;
wchar_t arg5[2048] = { 0 };

__declspec(noinline) void call_ReadGLCD_W(wchar_t *fname) {
	// Only the 1st, second, and 5th parameter appear to be used
	//  if (0 < param_2) {
	//		FUN_1001db26_fseek((char **)pFVar2_filehandle, (char *)((uint)(local_8 >> 3) * (uint)uStack6 * param_2), 1);
	//  }
	// Note that I could not determine how to trigger this function from the
	// main IrfanView application, so this target may not even be worth fuzzing
	ReadGLCD_W_handle(fname, 0, NULL, NULL, arg5);
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

	ReadGLCD_W_handle = (ReadGLCD_W_t)(GetProcAddress(formats_dll_handle, "ReadGLCD_W"));
	if (ReadGLCD_W_handle == NULL) {
		printf("Failed to get address for ReadGLCD_W\n");
		return 1;
	}
	call_ReadGLCD_W(argv[1]);
}