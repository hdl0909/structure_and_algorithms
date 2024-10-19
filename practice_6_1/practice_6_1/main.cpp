#include <stdlib.h>
#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <windows.h>

#if defined(max)
#undef max
#endif

using namespace std;

struct PhoneOwner {
	unsigned long long numberPhone;
	string adress;
};

struct HashTable {
	vector<PhoneOwner*> table;
	int size; // ������ ���-�������
	int count; // ���������� ��������� � ���-�������
	vector<bool>deleted;

	HashTable(int s) : size(s), count(0) {
		table.resize(size, nullptr);
		deleted.resize(size, false);
	}

	int hashFunction(unsigned  numberPhone, int size) {
		return numberPhone % size;
	}

	void insertItem(unsigned long long numberPhone, const string& adress) { // ������� ��� �������
		if ((float(count) / float(size)) > 0.75) {
			resizeTable();
		}

		
		int index = hashFunction(numberPhone, size);
		int i = 0;

		// ������������ ������������ ��� ���������� ��������
		while (table[index] != nullptr && i < size) {
			if (table[index]->numberPhone == numberPhone) {
				table[index]->adress = adress;
				cout << "������ ��������� �� ������ " << numberPhone << " ������: " << index << endl;
			}
			index = (index + i * i) % size;
			i++;
		}

		table[index] = new PhoneOwner{ numberPhone, adress };
		count++;
		deleted[index] = false;
		cout << "������ ���������, ����� ��������: " << numberPhone << " �����: " << adress << " ������: " << index << endl;
	}

	void resizeTable() { // ������� ��� ���������� �������
		cout << "���������� �������...\n";
		int newSize = size * 2;
		cout << "����� ������ �������: " << newSize << endl;
		vector<PhoneOwner*> newTable(newSize, nullptr);

		for (int i = 0; i < size; i++) {
			if (table[i] != nullptr) {
				int newIndex = hashFunction(table[i]->numberPhone, newSize);
				int j = 0;
				
				// ������������ ������������ ��� ���������� ��������
				while (newTable[newIndex] != nullptr && j < newSize) {
					newIndex = (newIndex + j * j) % newSize;
					j++;
				}

				newTable[newIndex] = table[i];
				cout << "������� � ������� ��������: " << table[i]->numberPhone << " ��������� �� ������: " << newIndex << endl;
			}
		}
		size = newSize;
		table = move(newTable);
		deleted.resize(newSize, false);

		cout << "������� ���������� ��������\n";
	}

	PhoneOwner* searchItem(unsigned long long numberPhone) {
		int index = hashFunction(numberPhone, size);
		int i = 0;

		while ((table[index] != nullptr || deleted[index]) && i < size) {
			if (!deleted[index] && table[index]->numberPhone == numberPhone) {
				return table[index];
			}

			index = (index + i * i) % size;
			i++;
		}

		cout << "������� �� ������\n";
		return nullptr;
	}

	void removeItem(unsigned long long numberPhone) {
		int index = hashFunction(numberPhone, size);
		int i = 0;

		while (table[index] != nullptr && i < size) {
			if (table[index]->numberPhone == numberPhone) {
				cout << "��������� ������� � ������� ��������: " << table[index]->numberPhone << " � �������: " << table[index]->adress << endl;
				delete table[index];
				table[index] = nullptr;
				deleted[index] = true;
				count--;
				cout << "������� ������\n";
				return;
			}

			index = (index + i * i) % size;
			i++;
		}

		cout << "������ �������� ����\n";
		return;
	}

	void print() { 
		cout << "���-�������:\n";
		for (int i = 0; i < size; i++) {
			if (table[i] != nullptr) {
				cout << "������: " << i << " ����� ��������: " << table[i]->numberPhone << " �����: " << table[i]->adress << endl;
			}
			else {
				cout << "������ " << i << " �����\n";
			}
		}
	}

	void autoInitialization() {
		insertItem(89220959431, "������, ��.������, 16");
		insertItem(89856588535, "������, 11-�� ��������, 36");
		insertItem(89686898990, "����������, 14 ���, 23");
		insertItem(89535667543, "������, ����� �������, 43");
		insertItem(89456676834, "����������, ������������ ��������, 33");
		insertItem(89356686443, "�������, ����� ��������, 35");
	}
};

void print_comand() {
	cout << "��������� �������\n";
	cout << "1. insert <����� ��������> <�����> - �������� ������� � ���-�������\n";
	cout << "2. search <����� ��������> - ����� ������� ���-������� �� �����\n";
	cout << "3. remove <����� ��������> - ������� ������� ���-������� �� �����\n";
	cout << "4. print - ������� ��� ������� ���-�������\n";
	cout << "5. help - ������� ������ ������\n";
	cout << "6. exit - ��������� ������ ���������\n";
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HashTable hashTable(15);
	hashTable.autoInitialization();

	print_comand();
	string comand;


	while (true) {
		cout << "������� �������\n";
		cin >> comand;
		if (comand == "insert") {
			unsigned long long numberPhone;
			cin >> numberPhone;
			cin.ignore();
			string adress;
			getline(cin, adress);
			hashTable.insertItem(numberPhone, adress);
		}
		else if (comand == "search") {
			unsigned long long numberPhone;
			cin >> numberPhone;
			cout << "����� ��������: " << hashTable.searchItem(numberPhone)->numberPhone << " �����: " << 
				hashTable.searchItem(numberPhone)->adress << endl;
		}
		else if (comand == "remove") {
			unsigned long long numberPhone;
			cin >> numberPhone;
			hashTable.removeItem(numberPhone);
		}
		else if (comand == "print") {
			hashTable.print();
		}
		else if (comand == "help") {
			print_comand();
		}
		else if (comand == "exit") {
			break;
		}
		else {
			cout << "������� �������� �������\n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	return 0;
}	
