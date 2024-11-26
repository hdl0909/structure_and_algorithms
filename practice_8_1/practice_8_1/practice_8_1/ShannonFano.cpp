#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

string readFile(string file_name) {
	string text;
	ifstream file(file_name);
	if (file.is_open()) {
		getline(file, text);
		transform(text.begin(), text.end(), text.begin(), ::tolower);
		return text;
	}
	else {
		cout << "Не удалось открыть файл";
		exit(1);
	}
}

void funCntChar(const string& text, map<char, pair<int, double>>& mapCntChar) {
	int size = text.size();
	for (auto character : text) {
		mapCntChar[character].first++;
	}
	for (map<char, pair<int, double>>::iterator it = mapCntChar.begin(); it != mapCntChar.end(); ++it) {
		it->second.second = static_cast<double>(it->second.first) / size;
	}
}

void funSortedCntChar(vector<pair<char, pair<int, double>>>& sortedCntChar) {
	sort(sortedCntChar.begin(), sortedCntChar.end(), [](const auto& a, const auto& b) {
		return a.second.first > b.second.first;
	});
}

void BuildShannonFanoCode(vector<pair<char, pair<int, double>>>& sortedChar, map<char, string>& codes, int start, int end, const string& prefix) {
	if (start == end) {
		codes[sortedChar[start].first] = prefix;
		return;
	}

	double total = 0.0;
	for (int i = start; i <= end; i++) {
		total += sortedChar[i].second.second;
	}

	double half = 0.0;
	int mid = start;
	double minDiff = total; 
	for (int i = start; i < end; i++) {
		half += sortedChar[i].second.second;
		double diff = abs(total / 2 - half);
		if (diff < minDiff) {
			minDiff = diff;
			mid = i;
		}
	}
	mid++;

	for (int i = start; i < mid; i++) {
		codes[sortedChar[i].first] = prefix + "0";
	}
	for (int i = mid; i < end; i++) {
		codes[sortedChar[i].first] = prefix + "1";
	}

	BuildShannonFanoCode(sortedChar, codes, start, mid - 1, prefix + "0");
	BuildShannonFanoCode(sortedChar, codes, mid, end, prefix + "1");
}

string compressText(map<char, string>& codes, const string& text) {
	string compressed;
	for (char c : text) {
		if (codes.find(c) != codes.end()) {
			compressed += codes.at(c);
		}
	}
	return compressed;
}

string decompressText(const string& compressedText, const map<string, char>& reverseCode) {
	string decompress;
	string temp = "";
	for (char c : compressedText) {
		temp += c;
		if (reverseCode.find(temp) != reverseCode.end()) {
			decompress += reverseCode.at(temp);
			temp = "";
		}
	}
	return decompress;
}

int main() {
	setlocale(LC_ALL, "ru");
	string textBeforeCompression = readFile("ShannonFano.txt");
	map<char, pair<int, double>> mapCntChar;
	funCntChar(textBeforeCompression, mapCntChar);
	vector<pair<char, pair<int, double>>> sortedCntChar(mapCntChar.begin(), mapCntChar.end());
	funSortedCntChar(sortedCntChar);
	map<char, string> codes;
	BuildShannonFanoCode(sortedCntChar, codes, 0, sortedCntChar.size() - 1, "");
	
	map<string, char> reverseCode;
	for (auto& item : codes) {
		reverseCode[item.second] = item.first;
	}

	for (map<char, string>::iterator it = codes.begin(); it != codes.end(); ++it) {
		cout << it->first << " : " << it->second << endl;

	}

	string compressedText = compressText(codes, textBeforeCompression);
	cout << "Сжатый текст: " << compressedText << endl;

	string decompressedText = decompressText(compressedText, reverseCode);
	cout << "Восстановленный текст: " << decompressedText << endl;

	int originalSize = textBeforeCompression.size() * 8; 
	int compressedSize = compressedText.size(); 

	double compressionRate = (1.0 - static_cast<double>(compressedSize) / originalSize) * 100;

	cout << "Процент сжатия: " << compressionRate << "%" << endl;

}