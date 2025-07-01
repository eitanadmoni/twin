#include <windows.h>
#include <mutex>
#include <thread>
#include <chrono>


std::mutex lock;


int main() {
	if (lock.try_lock()) {
		LPCSTR message = "MANAGEMENT PROGRAM IS UP";
		MessageBoxA(NULL, message, "message", MB_OK);
		std::this_thread::sleep_for(std::chrono::hours(1));
		lock.unlock();
		return 0;
	}
	return 1;
}