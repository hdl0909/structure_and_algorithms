#include <iostream>
#include <vector>
#include <windows.h>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>

using namespace std;

const int p = 31; // основание полинома
const int q = 9973; // большое простое число
					


vector<string> FuncArrayWords(const string& text) {
	vector<string> arrayWords;
	string word = "";
	for (char symbol : text) {
		if (!ispunct(symbol)) {
			word += symbol;
		}
		else if (symbol == ' ') continue;
		else if (!word.empty()){
			arrayWords.push_back(word);
			word.clear();
		}
	}
	if (!word.empty()) {
		arrayWords.push_back(word);
	}
	return arrayWords;
}

vector<string> FuncSearchArrayWords(vector<string> ArrayWords, const string& pattern) {
	vector<string> result;
	for (string word : ArrayWords) {
		if ((pattern.size() <= word.size()) && 
			(word.substr(word.size() - pattern.size(), word.size()) == pattern)) {
				result.push_back(word);
			}
	}
	return result;
}

void CreateFile() {
	ofstream file("text.txt");
	
	if (!file.is_open()) {
		cerr << "Не удалось открыть файл\n";
		return;
	}

	if (file.is_open()) {
		string pattern;
		cout << "Введите подстроку длиной не более 17 символов\n";
		cin >> pattern;
		while (pattern.size() > 17) {
			cout << "Подстрока превышает длину\nВведите подстроку длиной не более 17 символов\n";
			cin >> pattern;
		}
		cin.ignore();
		file << pattern << endl;

		int length;
		cout << "Введите длину текста\n";
		cin >> length;

		srand(time(0));
		for (size_t i = 0; i < length; i++) {
			char randomChar;
			int randomValue = rand() % 27; // так как 26 букв и пробел
			if (randomValue < 26) {
				randomChar = 'a' + randomValue;
				file << randomChar;
			}
			else {
				file << ' ';
			}
		}
	}
	file.close();
}

long long CalcHash(const string& str) {
	long long hash = 0;
	long long pow_p = 1;
	for (int i = str.size() - 1; i >= 0; i--) {
		if (str[i]  == ' ') continue;
		hash = (hash + ((str[i] - 'a' + 1) * pow_p) % q) % q;
		pow_p = (pow_p * p) % q;
	}
	return hash;
}



int CntSearchFileWord() {
	ifstream file("text.txt");
	if (!file.is_open()) {
		cerr << "Не удалось открыть файл\n";
		return -1;
	}
	int count = 0;
	string pattern;
	string text;
	getline(file, pattern);
	getline(file, text);

	if (pattern.size() > text.size()) {
		return 0;
	}

	long long hash_pattern = CalcHash(pattern); // Хеш шаблона
	long long current_hash = CalcHash(text.substr(0, pattern.size())); // Хеш первой подстроки

	long long pow_p_minus = 1; 
	for (int i = 1; i < pattern.size(); i++) {
		pow_p_minus = (pow_p_minus * p) % q;
	}

	if (current_hash == hash_pattern && text.substr(0, pattern.size()) == pattern) {
		count++;
	}

	for (int i = 1; i < text.size() - pattern.size() + 1; i++) {
		current_hash = (current_hash - ((text[i - 1] - 'a' + 1) * pow_p_minus) % q + q) % q;
		current_hash = (current_hash * p) % q;
		current_hash = (current_hash + (text[i + pattern.size() - 1] - 'a' + 1)) % q;

		// Проверка совпадения хешей и строки
		if (current_hash == hash_pattern && text.substr(i, pattern.size()) == pattern) {
			count++;
		}
	}

	file.close();

	return count;
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	/*string text;

	while (true) {
		cout << "Введите текст\n";
		getline(cin, text);

		if (text == "exit") break;

		vector<string> arrayWords = FuncArrayWords(text);

		string pattern;
		cout << "Введите подстроку\n";
		getline(cin, pattern);
		vector<string> result = FuncSearchArrayWords(arrayWords, pattern);

		if (result.empty()) {
			cout << "Ничего не найдено\n";
		}
		else {
			for (string word : result) {
				cout << word << " ";
			}
		}
		cout << '\n';
	} */

	CreateFile();
	cout << "Количество вхождений в тексте " << CntSearchFileWord() << endl;

	return 0;
}
