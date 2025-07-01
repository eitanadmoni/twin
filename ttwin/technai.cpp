#include <windows.h>
#include <mutex>
#include <thread>
#include <chrono>


std::mutex lock;
static bool first = TRUE;

void setRegistry() {
	HKEY hkey;
	PHKEY phkResult = &hkey;
	RegOpenKeyA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", phkResult);
	RegSetValueExA(hkey, "messageBox", 0, REG_SZ, (const BYTE*)"C:\\Users\\user\\source\\repos\\eitanadmoni\\twin\\ttwin\\technai.cpp", sizeof("C:\\Users\\user\\source\\repos\\eitanadmoni\\twin\\ttwin\\technai.cpp") + 1);
}

int main() {
	if (first) {
		setRegistry();
		first = FALSE;
	}
	if (lock.try_lock()) {
		LPCSTR message = "MANAGEMENT PROGRAM IS UP";
		MessageBoxA(NULL, message, "message", MB_OK);
		std::this_thread::sleep_for(std::chrono::hours(1));
		lock.unlock();
		return 0;
	}
	return 1;
}