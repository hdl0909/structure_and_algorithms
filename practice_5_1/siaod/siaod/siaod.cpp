#include <iostream>
#include <cstdlib>
#include <bitset>
#include <Windows.h>

using namespace std;

void ex_1_a() {
	int x;
	
	cout << "Введите x: ";
	cin >> x;
	unsigned char maska = 1;
	
	x = x & (~ (maska << 4));
	cout << "Ответ после установки 5 бита в 0: " << x << endl;
}

void ex_1_b() {
	int x;

	cout << "Введите x: ";
	cin >> x;
	unsigned char maska = 1;

	x = x | (maska << 6);
	cout << "Ответ после установки 7 бита в 1: " << x << endl;
}

int main() { // ex_1_в
	setlocale(LC_ALL, "ru");
	ex_1_a();
	ex_1_a();
	ex_1_b();
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	unsigned char x = 25;
	const int n = sizeof(int) * 8;
	unsigned maska = (1 << n - 1);
	cout << "Начальный вид маски: " << bitset<n>(maska) << endl;
	cout << "Результат: ";
	for (int i = 1; i <= n; i++) {
		cout << ((x & maska) >> (n - i));
		maska = maska >> 1;
	}
	cout << endl;
	system("pause");
	return 0;
}