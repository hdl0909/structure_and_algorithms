#include <iostream>
#include <vector>
#include <string>
#include <bitset>

using namespace std;

void bit_sort() { // Óïðàæíåíèå 2a
	vector<int> numbers;

	const int small_size = 8;
	const int big_size = 64;

	bool UseBigArray = false;

	unsigned char smallBitArray = 0; // Äëÿ íàáîðà äî 8-ìè ÷èñåë (ñî çíà÷åíèÿìè îò 0 äî 7)
	unsigned long long bigBitArray = 0; // Äëÿ íàáîðà áîëüøå 8-ìè ÷èñåë èëè çíà÷åíèÿìè áîëüøå 7

	cout << "Ââåäèòå ÷èñëî, ÷òîáû çàâåðøèòü ââîä, ââåäèòå -1" << endl;
	while (true) {
		int num;
		cin >> num;
		if (num == -1) {
			break;
		}
		if (num <= 7 && to_string(smallBitArray).size() <= 8) {
			if (!UseBigArray) smallBitArray |= (1 << num);
			else bigBitArray |= (1ULL << num);
		}
		else {
			if (!UseBigArray) {
				bigBitArray = smallBitArray;
				UseBigArray = true;
			}
			bigBitArray |= (1ULL << num);
		}
	}
	if (UseBigArray) {
		bitset<big_size> BigBitMask(bigBitArray);
		cout << "Ìàñêà: ";
		for (int i = 0; i < big_size; ++i) {
			cout << BigBitMask[i];  // Òåïåðü âûâîäèì ñ íóëåâîãî èíäåêñà ñïðàâà íàëåâî
		}
		cout << endl;
	}
	else {
		bitset<small_size> smallBitMask(smallBitArray);
		cout << "Ìàñêà: ";
		for (int i = 0; i < small_size; ++i) {
			cout << smallBitMask[i];  // Òåïåðü âûâîäèì ñ íóëåâîãî èíäåêñà ñïðàâà íàëåâî
		}
		cout << endl;
	}
}

void bit_sort_long() { // Óïðàæíåíèå 2b
	vector<unsigned long long> VectorBit; // Ìàññèâ äëÿ ÷èñåë ïðåâîñõîäÿùèå âîçìîæíîñòè ðàçðÿæíîé ñåòêè
	
	const int big_size = 64;
		
	bool UseArray = false;
	unsigned long long bigBitArray = 0; // Äëÿ ÷èñåë ìåíüøå 64

	cout << "Ââåäèòå ÷èñëî, ÷òîáû çàâåðøèòü ââîä, ââåäèòå -1" << endl;
	while (true) {
		int num;
		cin >> num;
		if (num == -1) {
			break;
		}
		if (num <= 63 && to_string(bigBitArray).size() <= 64) {
			if (!UseArray) bigBitArray |= (1ULL << num);
			else {
				VectorBit[0] |= (1ULL << num); // óñòàíàâëèâàåì áèò ñ íóæíîé ïîçèöèåé
			}
		}
		else {
			if (!UseArray) {
				VectorBit.push_back(bigBitArray);
				UseArray = true;
			}
			int index = num / big_size; // îïðåäåëÿåì èíäåêñ âåêòîðà

			if (index >= VectorBit.size()) {
				VectorBit.resize(index + 1, 0);
			}
			VectorBit[index] |= (1ULL << num); // óñòàíàâëèâàåì áèò ñ íóæíîé ïîçèöèåé
		}
	}
	if (UseArray) {
		cout << "Ìàñêà: ";
		for (int i = 0; i < VectorBit.size(); i++) {
			bitset<big_size> bigBitMask(VectorBit[i]);
			for (int j = 0; j < big_size; j++) {
				cout << bigBitMask[j];
			}
			cout << ":";
		}
		cout << endl;
	}
	else {
		bitset<big_size> bigBitMask(bigBitArray);
		cout << "Ìàñêà: ";
		for (int i = 0; i < big_size; i++) {
			cout << bigBitMask[i];
		}
		cout << endl;
	}
}

void bit_sort_long_char() { // Óïðàæíåíèå 2â
	vector<vector <unsigned char>> VectorBit; 

	const int big_size = 64;
	const int small_size = 8;

	bool UseArray = false;
	vector<unsigned char> bigBitArray(small_size, 0); // Ñîçäàåì âåêòîð, ñîäåðæàùèé 8 áàéòîâ è èíèöèëèçèðóåì 0

	cout << "Ââåäèòå ÷èñëî, ÷òîáû çàâåðøèòü ââîä, ââåäèòå -1" << endl;
	while (true) {
		int num;
		cin >> num;
		if (num == -1) {
			break;
		}
		if (num <= 63 && bigBitArray.size() * small_size <= 64) {
			if (!UseArray) {
				int index = num / small_size; // îïðåäåëÿåì èíäåêñ âíóòðè âåêòîðà
				int bit_position = num % small_size; // îïðeäåëÿåì ïîçèöèþ äëÿ óñòàíîâêè áèòà

				bigBitArray[index] |= (1 << bit_position);
			}
			else {
				int index = num / small_size;
				int bit_position = num % small_size;

				VectorBit[0][index] |= (1 << bit_position); // óñòàíàâëèâàåì áèò ñ íóæíîé ïîçèöèåé
			}
		}
		else {
			if (!UseArray) {
				VectorBit.push_back(bigBitArray);
				UseArray = true;
			}
			int index = num / big_size; // îïðåäåëÿåì èíäåêñ âåêòîðà
			int ins_index = (num % big_size) / small_size;// îïðåäåëÿåì èíäåêñ äëÿ ìàññèâà char
			int bit_position = num % small_size; // ïîçèöèÿ äëÿ óñòàíîâêè áèòà

			if (index >= VectorBit.size()) {
				VectorBit.resize(index + 1, vector <unsigned char>(small_size, 0));
			}
			VectorBit[index][ins_index] |= (1 << bit_position); // óñòàíàâëèâàåì áèò ñ íóæíîé ïîçèöèåé
		}
	}
	if (UseArray) {
		cout << "Ìàñêà: ";
		for (int i = 0; i < VectorBit.size(); i++) {
			for (int j = 0; j < small_size; j++) {
				bitset<small_size> bigBitMask(VectorBit[i][j]);
				for (int k = 0; k < small_size; k++) {
					cout << bigBitMask[k];
				}
			}
		}
		cout << endl;
	}
	else {
		cout << "Ìàñêà: ";
		for (int i = 0; i < small_size; i++) {
			bitset<small_size> bigBitMask(bigBitArray[i]);
			for (int j = 0; j < small_size; j++) {
				cout << bigBitMask[j];
			}
		}
		cout << endl;
	}
}

int main() {
	setlocale(LC_ALL, "ru");
	bit_sort();
	bit_sort_long();
	bit_sort_long_char();
	return 0;
}
