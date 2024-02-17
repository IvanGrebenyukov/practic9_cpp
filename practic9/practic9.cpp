

#include <iostream>
#include <windows.h>
#include <vector>
using namespace std;

bool isPrime(int num) {
    if (num <= 1) return false;
	if (num <= 3 || num == 5) return true;
	if (num % 2 == 0 || num % 3 == 0 || num % 5 == 0) return false;
	return true;
}
CRITICAL_SECTION cs;
DWORD WINAPI findPrimes(LPVOID range) {

	int* diaposon = (int*)range;
	int start = diaposon[0];
	int end = diaposon[1];
	for (int i = start; i <= end; ++i)
	{
		if (isPrime(i)) {
			EnterCriticalSection(&cs);
			cout <<  i << " - простое число" << endl;
			LeaveCriticalSection(&cs);
		}
	}
	cout << "Поток завершился" << endl;
	return 0;
}

int main()
{
	setlocale(LC_ALL, "ru");
	int start, end;
	cout << "Введите начало диапазона: ";
	cin >> start;
	cout << "Введите конец диапазона: ";
	cin >> end;

	InitializeCriticalSection(&cs);
	int middle = (start + end) / 2;
	int* firstDiaposon = new int[2] {start, middle};
	int* secondDiaposon = new int[2] {middle+1, end};

	HANDLE firstThread = CreateThread(NULL, 0, findPrimes, firstDiaposon, 0, NULL);
	HANDLE secondThread = CreateThread(NULL, 0, findPrimes, secondDiaposon, 0, NULL);
	if (firstThread == NULL || secondThread == NULL) {
		return GetLastError();
	}

	WaitForSingleObject(firstThread, INFINITE);
	WaitForSingleObject(secondThread, INFINITE);

	CloseHandle(firstThread);
	CloseHandle(secondThread);

	
	return 0;

}
