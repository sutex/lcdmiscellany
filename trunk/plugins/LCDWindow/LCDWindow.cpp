#include <Windows.h>
#include "plugin.h"

HINSTANCE ghInst;

LcdCallbacks *callback = 0;

struct WindowInfo : public LcdInfo {
	HWND hWnd;
	int realWidth;
	int realHeight;
	int initialized;
};

WindowInfo windows[2];

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int i = -1;
	if (windows[0].hWnd == hWnd) i = 0;
	else if (windows[1].hWnd == hWnd) i = 1;
	if (i >= 0) {
		switch (uMsg) {
			case WM_CLOSE:
				DestroyWindow(hWnd);
				break;
			case WM_DESTROY:
				windows[i].hWnd = 0;
				if (callback) {
					callback->DeviceChange(callback->id);
				}
				break;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*
typedef int (CALLBACK * lcdStart)(LcdInfo *info);

// Function specified by LCDInfo structure.
// Info contains what lcdEnum returned.  Not required.
// Currently always called on each device right before destroying
// it, but may allow for more advanved behavior later.
typedef void (CALLBACK * lcdStop)(LcdInfo *info);

// Function specified by LCDInfo structure.
// Info contains what lcdEnum returned.
// Final function called on info.  Not required.
typedef void (CALLBACK * lcdDestroy)(LcdInfo *info);
//*/

void CALLBACK Update(LcdInfo *info, BitmapInfo *bmp) {
	info = info;
	HDC hDC = GetDC(((WindowInfo*)info)->hWnd);
	BitBlt(hDC, 0, 0, bmp->width, bmp->height, bmp->hDC, 0, 0, SRCCOPY);
	ReleaseDC(((WindowInfo*)info)->hWnd, hDC);
}

void CALLBACK Destroy(LcdInfo *info) {
	((WindowInfo *)info)->initialized = 0;
}

void InitWindow(WindowInfo *win, char *name) {
	memset(win, 0, sizeof(WindowInfo));
	win->bpp = 24;
	win->refreshRate = 0;
	win->id = name;
	win->hWnd = CreateWindowEx(0, "LCD Window Class",
		name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, ghInst, 0);
	RECT r;
	GetClientRect(win->hWnd, &r);
	win->realWidth = r.right;
	win->realHeight = r.bottom;
	win->Update = Update;
	win->Destroy = Destroy;
	ShowWindow(win->hWnd, 1);
}

int registered = 0;
int CALLBACK lcdInit(const LcdCallbacks *callbacks) {
	callback = callbacks;
	WNDCLASS wndclass = {
		0,
		WndProc,
		0, 0,
		ghInst, 0,
		LoadCursor(NULL, IDC_ARROW),
		WHITE_BRUSH, 0,
		"LCD Window Class"
	};
	int test = RegisterClass(&wndclass);
	registered = 1;
	InitWindow(windows, "Window 1");
	InitWindow(windows+1, "Window 2");

	return 1;
}

void CALLBACK lcdUninit() {
	callback = 0;
	for (int i=0; i<2; i++) {
		if (windows[i].hWnd) DestroyWindow(windows[i].hWnd);
		windows[i].hWnd = 0;
	}
	if (registered) {
		UnregisterClass("LCD Window Class", ghInst);
		registered = 0;
	}
}

LcdInfo * CALLBACK lcdEnum() {
	for (int i=0; i<2; i++) {
		if (windows[i].hWnd && !windows[i].initialized) {
			windows[i].initialized = 1;
			windows[i].height = windows[i].realHeight;
			windows[i].width = windows[i].realWidth;
			return windows+i;
		}
	}
	return 0;
}


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, void* lpvReserved) {
	if (fdwReason == DLL_PROCESS_ATTACH) {
		ghInst = hInstance;
		DisableThreadLibraryCalls(hInstance);
	}
	else if (fdwReason == DLL_PROCESS_DETACH) {
		// Just in case.
		lcdUninit();
	}
    return 1;
}
