#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

struct HuffmanNode {
	char data;
	int freq;
	HuffmanNode* left;
	HuffmanNode* right;

	HuffmanNode(char d, int f) : data(d), freq(f), left(nullptr), right(nullptr) {}
};

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

void generateHuffmanCodes(HuffmanNode* node, const string& str, map<char, string>& codes) {
	if (!node) return;

	if (!node->left && !node->right) {
		codes[node->data] = str;
	}

	generateHuffmanCodes(node->left, str + "0", codes);
	generateHuffmanCodes(node->right, str + "1", codes);

}

void buildHuffmanTree(vector<pair<char, pair<int, double>>>& sortedChar, map<char, string>& codes) {
	vector <HuffmanNode*> nodes;

	for (const auto& character : sortedChar) {
		nodes.push_back(new HuffmanNode(character.first, character.second.first));
	}

	while (nodes.size() > 1) {
		sort(nodes.begin(), nodes.end(), [](HuffmanNode* left, HuffmanNode* right) {
			return left->freq < right->freq;
			});

		HuffmanNode* left = nodes[0];
		HuffmanNode* right = nodes[1];

		HuffmanNode* newNode = new HuffmanNode('\0', left->freq + right->freq);
		newNode->left = left;
		newNode->right = right;

		nodes.erase(nodes.begin());
		nodes.erase(nodes.begin());
		nodes.push_back(newNode);
	}

	HuffmanNode* root = nodes[0];

	generateHuffmanCodes(root, "", codes);
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

void calculateCompression(const map<char, string>& codes, const map<char, pair<int, double>>& mapCntChar, const string& text) {
	int totalBitsCompressed = 0;
	int totalBitsASCII = text.size() * 8;

	int uniformCodeLength = ceil(log2(mapCntChar.size()));
	int totalBitsUniform = text.size() * uniformCodeLength;

	double avgCodeLength = 0.0;

	for (const auto& item : mapCntChar) {
		char symbol = item.first;
		int cntSymbol = item.second.first;
		double prob = item.second.second;
		int codeLength = codes.at(symbol).size();

		avgCodeLength += prob * codeLength;

		totalBitsCompressed += cntSymbol * codeLength;
	}

	double dispersy = 0.0;

	for (const auto& item : mapCntChar) {
		char c = item.first;
		double prob = item.second.second;
		int codeLength = codes.at(c).size();

		dispersy += (prob * pow(codeLength - avgCodeLength, 2));
	}


	double compressionASCII = static_cast<double>(totalBitsCompressed) / totalBitsASCII;
	double compressionUniform = static_cast<double>(totalBitsCompressed) / totalBitsUniform;

	cout << "Коэффициент сжатия относительно ASCII: " << compressionASCII << endl;
	cout << "Коэффициент сжатия относительно равномерного кода: " << compressionUniform << endl;
	cout << "Средняя длина кода: " << avgCodeLength << endl;
	cout << "Дисперсия длины кода: " << dispersy << endl;
}

int main() {
	setlocale(LC_ALL, "ru");
	string textBeforeCompression = readFile("Huffman.txt");
	map<char, pair<int, double>> mapCntChar;
	funCntChar(textBeforeCompression, mapCntChar);
	vector<pair<char, pair<int, double>>> sortedCntChar(mapCntChar.begin(), mapCntChar.end());
	funSortedCntChar(sortedCntChar);

	map<char, string> codes;
	buildHuffmanTree(sortedCntChar, codes);

	for (map<char, string>::iterator it = codes.begin(); it != codes.end(); ++it) {
		cout << it->first << " : " << it->second << endl;
	}

	string compressedText = compressText(codes, textBeforeCompression);
	cout << "Сжатый текст: " << compressedText << endl;

	map<string, char> reverseCodes;
	for (auto& item : codes) {
		reverseCodes[item.second] = item.first;
	}
	string decompressedText = decompressText(compressedText, reverseCodes);
	cout << "Восстановленный текст: " << decompressedText << endl;

	calculateCompression(codes, mapCntChar, textBeforeCompression);
}