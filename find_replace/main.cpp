#include "GenJson/gen_json.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <clocale>

using namespace std;

void find_in_file(const string& sub_to_found) {
    int i = 1;
    bool founded = false;
    long long int count_find = 0;
    while (true) {
        string filename = "data" + to_string(i) + ".json";
        ifstream fin(filename);
        if (!fin.is_open()) break;
        string line;
        while (getline(fin, line)) {
            size_t pos_start_name = line.find("\"name\":\"");
            if (pos_start_name == string::npos) continue;
            pos_start_name += 8;
            size_t pos_end_name = line.find("\",\"", pos_start_name);
            if (pos_end_name == string::npos) continue;
            string name = line.substr(pos_start_name, pos_end_name - pos_start_name);
            size_t pos_start_substr = line.find("\"content\":\"");
            if (pos_start_substr == string::npos) continue;
            pos_start_substr += 11;
            size_t pos_end_substr = line.find("\"", pos_start_substr);
            if (pos_end_substr == string::npos) continue;
            string sub = line.substr(pos_start_substr, pos_end_substr - pos_start_substr);
            if (sub.find(sub_to_found) != string::npos) {
                cout << filename << "   " << name << "   " << sub << endl;
                count_find +=1;
                founded = true;
            }
        }

        i++;
    }
    cout << "Всего было найдено " << count_find << " совпадений.";
    if (founded == false){
        cout << "Подстрока " << sub_to_found << " не была найдена ни в одном файле.";
    }
}

void replace_in_file(string name_json, string name_file, string substr, string substr_replaced){
    ifstream fin(name_json+".json");
    ofstream fout(name_json+"edited.json");
    if (!fin.is_open()){
        cout<<"Не удалось найди данный файл.";
    }
    string line;
    size_t count_replace = 0;
    while(getline(fin, line)){
        if (line.find(name_file) != string :: npos){
            while(line.find(substr) != string :: npos){
                size_t pos = line.find(substr);
                line.replace(pos, substr.size(), substr_replaced);
            }
            fout << line << endl;
            count_replace +=1;
        } else {
            fout << line << endl;
        }
    }
    cout << "Всего было совершено " << count_replace << " замен.";
    fin.close();
    fout.close();
}

void replace_all(string name_json, string name_file, string substr, string substr_replaced){
    ifstream fin(name_json+".json");
    ofstream fout(name_json+"edited.json");
    if (!fin.is_open()){
        cout<<"Не удалось найди данный файл.";
    }
    string line;
    size_t count_replace = 0;
    while(getline(fin, line)){
        if (line.find(name_file) != string :: npos){
            while(line.find(substr) != string :: npos){
                size_t pos = line.find(substr);
                line.replace(pos, substr.size(), substr_replaced);
            }
            fout << line << endl;
            count_replace +=1;
        } else {
            fout << line << endl;
        }
    }
    cout << "Всего было совершено " << count_replace << " замен.";
    fin.close();
    fout.close();
}

int main(){
    setlocale(LC_ALL, "Russian");
    GenerateJSON(400000, 500000); // генерация джейсон файлов перед запуском, после работы они удаляются и при новом запуске генерируются новые
    string command;
    cout << "Файлы json были успешно сгенерированы, введите необходимую комманду, или \"--help\" для получения списка комманд." << endl;
    while (true){
        cin >> command;
        if (command == "--help"){
            cout << "Список доступных комманд: " << 
            "\nПолучение информации о доступных коммандах - \"--help\"" <<
            "Поиск слова в содержании файлов - \"--find\"" << 
            "Поиск и замена содержимого файла - \"--replace\"" << 
            "Режим \"ТОЛЬКО ПОИСК\" - \"--dry-run\"";
        } else if (command == "--find"){
            cout << "\nВведите искомую подстроку: ";
            string sub_found;
            cin >> sub_found;
            cout << "  Файл json  Файл с подстрокой  Содержание";
            find_in_file(sub_found);
        } else if (command == "--replace"){
            cout << "Для поиска подстроки введите по порядку: \nназвание json файла -> название файла содержания -> что заменить -> на что заменить.";


        }
    }
    return 0;
}