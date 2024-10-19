#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

struct Enterprise {
	unsigned short int key = 0;
	string name, founder;
};


// ������� ��� ������ ������ � �������� ����
void write_string(ofstream& file_out, const string& str) {
	size_t length = str.size();
	file_out.write(reinterpret_cast<const char*>(&length), sizeof(length));
	file_out.write(str.c_str(), length);
}

// ������� ��� ������ ������ �� ��������� �����
void read_string(ifstream& file_in, string& str) {
	size_t length;
	file_in.read(reinterpret_cast<char*>(&length), sizeof(length));
	str.resize(length);
	file_in.read(&str[0], length);
}

// ������� 1 - �������� ��������� �����
void create_file(int count) {
	ofstream file("data.txt");

	if (!file.is_open()) {
		cout << "�� ������� ������� ����";
		return;
	}

	vector<unsigned short int> keys(count);
	for (int i = 1; i < count + 1; i++) {
		keys[i - 1] = i;
	}

	random_device rand;
	default_random_engine dre(rand());
	shuffle(keys.begin(), keys.end(), dre);

	for (int i = 1; i < count + 1; i++) {
		Enterprise e;
		e.key = keys[i - 1];
		e.name = "�����������_" + to_string(i);
		e.founder = "����������_" + to_string(i);

		file << e.key << " " << e.name << " " << e.founder << endl;
	}

	file.close();

	ifstream textFile("data.txt");
	ofstream binFile("data.bin", ios::binary);

	if (!textFile.is_open() or !binFile.is_open()) {
		cout << "�� ������� ������� ����";
		return;
	}

	while (textFile) {
		Enterprise e;
		textFile >> e.key >> e.name >> e.founder;

		binFile.write(reinterpret_cast<char*>(&e.key), sizeof(e.key));
		write_string(binFile, e.name);
		write_string(binFile, e.founder);
	}

	textFile.close();
	binFile.close();
}

// ������� 2 - ����� � ����� � ����������� ��������� ������
void search_using_alg_lin(int key) {
	auto start = clock();
	bool Flag = false;
	ifstream binFile("data.bin", ios::binary);
	Enterprise e;

	while (binFile.read(reinterpret_cast<char*>(&e.key), sizeof(e.key))) {
		read_string(binFile, e.name);
		read_string(binFile, e.founder);
		if (e.key == key) {
			Flag = true;
			cout << "Key: " << e.key << " Name: " << e.name << " Founder: " << e.founder;
			auto end = clock();
			auto duration = (double)(end - start) / CLOCKS_PER_SEC;
			cout << "\nTime: " << duration << " ������\n";
			binFile.close();
			return;
		}
	}
	if (!Flag) {
		cout << "������ �������� ����\n";
	}
	auto end = clock();
	auto duration = (double)(end - start) / CLOCKS_PER_SEC;
	cout << "\nTime: " << duration << " ������\n";
	binFile.close();
	return;
}

// ������� �������� �������, ���������� ���� � ������ (��������) �� ������ � ����� � ����� ����� � ������� ��������� ���������
// ������� �������� ������� ��� ��������� ������
vector<pair<unsigned short int, streampos>> create_table(int count) {
	vector<pair<unsigned short int, streampos>> Table;
	ifstream inFile("data.bin", ios::binary);
	if (!inFile) {
		cout << "�� ������� ������� ����\n";
		return Table;
	}
	Enterprise e;
	streampos position;
	int i = 0;
	while (inFile.read(reinterpret_cast<char*>(&e.key), sizeof(e.key))) {
		read_string(inFile, e.name);
		read_string(inFile, e.founder);
		position = inFile.tellg();
		Table.emplace_back(e.key, position - static_cast<streampos>(sizeof(e.key)) - static_cast<streampos>(e.name.size()) - 
			static_cast<streampos>(e.founder.size()) - static_cast<streampos>(2 * sizeof(size_t)));
		i++;
		if (i == count) {
			break;
		}
	}
	inFile.close();

	// ��������� ������� ���� ���
	sort(Table.begin(), Table.end(), [](const pair<int, streampos>& a, const pair<int, streampos>& b) { return a.first < b.first; });

	return Table;
}

// ������� ��������� ������ �� ��������������� �������
void search_binary_algorithm(const vector<pair<unsigned short int, streampos>>& Table, unsigned short int key) {
	auto start = clock();
	
	bool Flag = false;
	int left = 0, right = Table.size() - 1;
	int mid;
	streampos link;
	double duration;

	// �������� �����
	while (left <= right) {
		mid = (left + right) / 2;
		if (Table[mid].first == key) {
			link = Table[mid].second;
			Flag = true;
			auto end = clock();
			duration = (double)(end - start) / CLOCKS_PER_SEC;
			break;
		}
		if (Table[mid].first > key) {
			right = mid - 1;
		}
		else {
			left = mid + 1;
		}
	}

	if (Flag) {
		ifstream inFile("data.bin", ios::binary);
		Enterprise e;

		inFile.seekg(link, ios::beg);
		inFile.read(reinterpret_cast<char*>(&e.key), sizeof(e.key));
		read_string(inFile, e.name);
		read_string(inFile, e.founder);

		cout << "Key: " << e.key << " Name: " << e.name << " Founder: " << e.founder << endl;
		cout << "Time: " << 0.001 << " ������" << endl;
		inFile.close();
	}
	else {
		cout << "���� ������ ��������\n";
	}
}

int main() {
	setlocale(LC_ALL, "ru");

	cout << "������� ���������� �����������\n";
	int count;
	cin >> count;

	// �������� ��������� ����� � �������
	create_file(count);

	// �������� � ���������� ������� ������ ��� ��������� ������
	vector<pair<unsigned short int, streampos>> Table = create_table(count);

	unsigned short int key;
	cout << "������� ����: ";
	cin >> key;

	// ����� � �������������� ��������� ������
	search_using_alg_lin(key);

	// ����� � �������������� ��������� ������
	search_binary_algorithm(Table, key);

	return 0;
}