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
	int size; // размер хэш-таблицы
	int count; // количество элементов в хэш-таблице
	vector<bool>deleted;

	HashTable(int s) : size(s), count(0) {
		table.resize(size, nullptr);
		deleted.resize(size, false);
	}

	int hashFunction(unsigned  numberPhone, int size) {
		return numberPhone % size;
	}

	void insertItem(unsigned long long numberPhone, const string& adress) { // функция для вставки
		if ((float(count) / float(size)) > 0.75) {
			resizeTable();
		}

		
		int index = hashFunction(numberPhone, size);
		int i = 0;

		// квадратичное пробирование для разрешения коллизий
		while (table[index] != nullptr && i < size) {
			if (table[index]->numberPhone == numberPhone) {
				table[index]->adress = adress;
				cout << "Запись обновлена по номеру " << numberPhone << " Индекс: " << index << endl;
			}
			index = (index + i * i) % size;
			i++;
		}

		table[index] = new PhoneOwner{ numberPhone, adress };
		count++;
		deleted[index] = false;
		cout << "Запись добавлена, Номер телефона: " << numberPhone << " Адрес: " << adress << " Индекс: " << index << endl;
	}

	void resizeTable() { // Функция для расширения таблицы
		cout << "Расширение таблицы...\n";
		int newSize = size * 2;
		cout << "Новый размер таблицы: " << newSize << endl;
		vector<PhoneOwner*> newTable(newSize, nullptr);

		for (int i = 0; i < size; i++) {
			if (table[i] != nullptr) {
				int newIndex = hashFunction(table[i]->numberPhone, newSize);
				int j = 0;
				
				// квадратичное пробирование для разрешения коллизий
				while (newTable[newIndex] != nullptr && j < newSize) {
					newIndex = (newIndex + j * j) % newSize;
					j++;
				}

				newTable[newIndex] = table[i];
				cout << "Элемент с номером телефона: " << table[i]->numberPhone << " Перемещен на индекс: " << newIndex << endl;
			}
		}
		size = newSize;
		table = move(newTable);
		deleted.resize(newSize, false);

		cout << "Процесс расширения завершен\n";
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

		cout << "Элемент не найден\n";
		return nullptr;
	}

	void removeItem(unsigned long long numberPhone) {
		int index = hashFunction(numberPhone, size);
		int i = 0;

		while (table[index] != nullptr && i < size) {
			if (table[index]->numberPhone == numberPhone) {
				cout << "Удаляется элемент с номером телефона: " << table[index]->numberPhone << " и адресом: " << table[index]->adress << endl;
				delete table[index];
				table[index] = nullptr;
				deleted[index] = true;
				count--;
				cout << "Элемент удален\n";
				return;
			}

			index = (index + i * i) % size;
			i++;
		}

		cout << "Такого элемента нету\n";
		return;
	}

	void print() { 
		cout << "Хеш-таблица:\n";
		for (int i = 0; i < size; i++) {
			if (table[i] != nullptr) {
				cout << "Индекс: " << i << " Номер телефона: " << table[i]->numberPhone << " Адрес: " << table[i]->adress << endl;
			}
			else {
				cout << "Индекс " << i << " пусто\n";
			}
		}
	}

	void autoInitialization() {
		insertItem(89220959431, "Москва, ул.Ленина, 16");
		insertItem(89856588535, "Москва, 11-ая Парковая, 36");
		insertItem(89686898990, "Губкинский, 14 мкр, 23");
		insertItem(89535667543, "Москва, Улица Баумана, 43");
		insertItem(89456676834, "Зеленоград, Георгиевский проспект, 33");
		insertItem(89356686443, "Туймазы, Улица Комарова, 35");
	}
};

void print_comand() {
	cout << "Доступные команды\n";
	cout << "1. insert <номер телефона> <адрес> - Добавить элемент в хеш-таблицу\n";
	cout << "2. search <номер телефона> - найти элемент хеш-таблицы по ключу\n";
	cout << "3. remove <номер телефона> - удалить элемент хеш-таблицы по ключу\n";
	cout << "4. print - вывести все элменты хеш-таблицы\n";
	cout << "5. help - вывести список команд\n";
	cout << "6. exit - завершить работу программы\n";
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	HashTable hashTable(15);
	hashTable.autoInitialization();

	print_comand();
	string comand;


	while (true) {
		cout << "Введите команду\n";
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
			cout << "Номер телефона: " << hashTable.searchItem(numberPhone)->numberPhone << " Адрес: " << 
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
			cout << "Введена неверная команда\n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
	return 0;
}	
