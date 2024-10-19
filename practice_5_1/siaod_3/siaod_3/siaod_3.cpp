#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

const int MIN_NUMBER = 1000000;
const int MAX_NUMBER = 9999999;
const int SIZE = 1000000;
const int SIZE_BIT_ARRAY = (MAX_NUMBER - MIN_NUMBER + 1); // Размер для битового массива

int create_file() {
    vector<int> numbers;
    for (int i = MIN_NUMBER; i <= MAX_NUMBER; i++) {
        numbers.push_back(i);
    }
    unsigned int seed = static_cast<unsigned int>(time(nullptr));
    shuffle(numbers.begin(), numbers.end(), default_random_engine(seed));

//    numbers.resize(SIZE);

    ofstream output_file("D:/DAN/proga/структуры и алгосы/practice_5_1/siaod_3/test.txt");
    if (!output_file.is_open()) {
        cerr << "Не удалось открыть файл";
        return 1;
    }

    for (int num : numbers) {
        output_file << num << "\n";
    }
    output_file.close();
    cout << "Файл создан\n";

    return 0;
}

int sort_file() {
    vector<unsigned char> bit_array(SIZE_BIT_ARRAY / 8, 0);

    ifstream file_input("D:/DAN/proga/структуры и алгосы/practice_5_1/siaod_3/test.txt");
    if (!file_input.is_open()) {
        cerr << "Не удалось открыть файл";
        return 1;
    }

    unsigned int num;
    while (file_input >> num) {
        bit_array[(num - MIN_NUMBER) / 8] |= (1 << ((num - MIN_NUMBER) % 8));
    }
    file_input.close();

    ofstream file_output("D:/DAN/proga/структуры и алгосы/practice_5_1/siaod_3/test.txt");
    if (!file_output.is_open()) {
        cerr << "Не удалось открыть файл";
        return 1;
    }

    for (int i = 0; i < SIZE_BIT_ARRAY; ++i) {
        if (bit_array[i / 8] & (1 << (i % 8))) {
            file_output << (MIN_NUMBER + i) << '\n';
        }
    }
    file_output.close();

    //Определение объема памяти, занимаемым битовым массивом
    float memory_size = bit_array.size() * sizeof(unsigned char);
    cout << "Объем памяти, занимаемая битовым массивом " << memory_size / 1024 / 1024 << " МБ" << endl;

    return 0;
}

int main() {
    setlocale(LC_ALL, "ru");
    create_file();
    system("pause");
    float start_time = clock();
    sort_file();
    float end_time = clock();
    float search_time = end_time - start_time;
    cout << "Время выполнения программы: " << search_time / 1000 << " c" << endl;

    return 0;
}
