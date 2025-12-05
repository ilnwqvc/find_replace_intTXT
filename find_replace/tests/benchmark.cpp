#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <filesystem>
#include <random>

using namespace std;
namespace fs = std::filesystem;

string generate_line(int index, int content_length = 50) {
    static const string chars =
        "abcdefghijklmnopqrstuvwxyz";

    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(0, chars.size() - 1);

    string content;
    content.reserve(content_length);
    for (int i = 0; i < content_length; i++) {
        content.push_back(chars[dist(gen)]);
    }

    return "{\"name\":\"file_" + to_string(index) + ".csv\",\"content\":\"" + content + "\"}";
}


void generate_json(const string& filename, size_t lines) {
    ofstream out(filename);
    for (size_t i = 1; i <= lines; i++) {
        out << generate_line(i) << "\n";
    }
}


long long benchmark_find(const string& filename, const string& sub) {
    ifstream fin(filename);
    string line;

    auto start = chrono::high_resolution_clock::now();

    while (getline(fin, line)) {
        if (line.find(sub) != string::npos) {
        }
    }

    auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(end - start).count();
}


int main() {
    vector<size_t> sizes = {
        10, 100, 1000, 10000, 100000, 1000000, 10000000
    };

    string filename = "bench.json";
    string needle = "a"; 
    ofstream report("../docs/bench.md");
    report << "# Benchmark Report\n\n";
    report << "Тест: поиск подстроки `" << needle << "` в одном JSON-файле.\n\n";
    report << "| Кол-во строк | Время (ms) |\n";
    report << "|--------------|-------------|\n";

    for (size_t n : sizes) {
        cout << "Генерация " << n << " строк..." << endl;
        generate_json(filename, n);

        cout << "Бенчмарк поиска..." << endl;
        long long t = benchmark_find(filename, needle);

        cout << n << " строк -> " << t << " ms\n";

        report << "| " << n << " | " << t << " |\n";
    }

    report.close();
    cout << "\nБенчмарк завершён. Отчёт в docs/bench.md\n";

    return 0;
}
