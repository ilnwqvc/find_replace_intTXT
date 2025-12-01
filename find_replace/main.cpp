#include "GenJson/gen_json.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <clocale>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

void close_program() {
    for (const auto& entry : fs::directory_iterator(".")) {
        if (entry.path().extension() == ".json") {
            fs::remove(entry.path());
        }
    }
    cout << "Все JSON-файлы удалены. Программа завершена." << endl;
    exit(0);
}

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
    cout << "Всего было найдено " << count_find << " совпадений." << endl;
    if (founded == false){
        cout << "Подстрока " << sub_to_found << " не была найдена ни в одном файле." << endl;
    }
}

void replace_in_file(string name_json, string name_file, string substr, string substr_replaced){
    ifstream fin(name_json + ".json");
    ofstream fout(name_json + "edited.json");

    if (!fin.is_open()){
        cout << "Не удалось найти данный файл." << endl;
        return;
    }

    string line;
    size_t count_replace = 0;

    while (getline(fin, line)) {

        if (line.find(name_file) != string::npos && line.find("\"content\"") != string::npos) {

            while (line.find(substr) != string::npos) {
                size_t pos = line.find(substr);
                line.replace(pos, substr.size(), substr_replaced);
                count_replace++;
            }

            fout << line << endl;

        } else {
            fout << line << endl;
        }
    }

    cout << "Файл " << name_json + "edited.json создан." << endl;
    cout << "Всего было совершено " << count_replace << " замен." << endl;

    fin.close();
    fout.close();
}


void replace_all(string name_json, string substr, string substr_replaced){
    ifstream fin(name_json + ".json");
    ofstream fout(name_json + "edited.json");

    if (!fin.is_open()){
        cout << "Не удалось найти данный файл." << endl;
        return;
    }

    string line;
    size_t count_replace = 0;

    while (getline(fin, line)) {

        if (line.find("\"content\"") != string::npos &&
            line.find(substr) != string::npos) {

            while (line.find(substr) != string::npos) {
                size_t pos = line.find(substr);
                line.replace(pos, substr.size(), substr_replaced);
                count_replace++;
            }
        }

        fout << line << endl;
    }

    cout << "Файл " << name_json + "edited.json создан." << endl;
    cout << "Всего было совершено " << count_replace << " замен." << endl;

    fin.close();
    fout.close();
}



int main(){
    setlocale(LC_ALL, "Russian");
    GenerateJSON(400000, 500000);
    string command;
    cout << "Файлы json были успешно сгенерированы, введите необходимую команду, или \"--help\" для получения списка команд." << endl;

    while (true){
        cin >> command;
        
        if (command != "--dry-run"){
            if (command == "--help"){
                cout << "Список доступных команд: " << 
                "\nПолучение информации о доступных командах - \"--help\"" <<
                "\nПоиск слова в содержании файлов - \"--find\"" << 
                "\nЗамена содержимого во всех файлах - \"--replaceall\"" << 
                "\nЗамена содержимого только в одном файле - \"--replacein\"" << 
                "\nРежим \"ТОЛЬКО ПОИСК\" - \"--dry-run\"" << 
                "\nЗавершение работы и удаление json файлов - \"--exit\"" << endl;

            } else if (command == "--find"){
                cout << "Введите искомую подстроку: ";
                string sub_found;
                cin >> sub_found;
                cout << "Файл json  Файл с подстрокой  Содержание" << endl;
                find_in_file(sub_found);

            } else if (command == "--replaceall"){
                cout << "Для замены подстроки во всех файлах содержании введите по порядку: \nназвание json файла без расширения -> что заменить -> на что заменить." << endl;
                string name;
                string sub;
                string sub_r;
                cin >> name >> sub >> sub_r;
                replace_all(name, sub, sub_r);

            } else if (command == "--replacein"){
                cout << "Для замены подстроки в определенном файле содержании введите по порядку: \nназвание json файла без расширения -> название файла содержании -> что заменить -> на что заменить." << endl;
                string name_j;
                string name_f;
                string sub;
                string sub_r;
                cin >> name_j >> name_f >> sub >> sub_r;
                replace_in_file(name_j, name_f, sub, sub_r);
            } else if (command == "--exit"){
                close_program();
            } else {
                cout << "Введена неверная команда, попробуйте снова." << endl;
            }

        } else {
            cout << "Включен режим \"ТОЛЬКО ПОИСК\", чтобы посмотреть доступные команды введите --help." << endl;
            string in_dr_command;
            cin >> in_dr_command;

            if (in_dr_command == "--dry-end"){
                cout << "Режим dry-run успешно завершен." << endl;
                command = "";   

            } else {
                if (in_dr_command == "--help"){
                    cout << "Список доступных команд: " << 
                    "\nПолучение информации о доступных командах - \"--help\"" <<
                    "\nПоиск слова в содержании файлов - \"--find\"" << 
                    "\nЗавершить режим \"Только поиск\" - \"--dry-end\"" <<
                    "\nЗавершение работы и удаление json файлов - \"--exit\"" << endl;

                } else if (in_dr_command == "--find"){
                    cout << "\nВведите искомую подстроку: ";
                    string sub_found;
                    cin >> sub_found;
                    cout << "  Файл json  Файл с подстрокой  Содержание" << endl;
                    find_in_file(sub_found);
                } else if (in_dr_command == "--exit"){
                    close_program();
                } else {
                    cout << "Введена неверная команда, попробуйте снова." << endl;
                }
            }
        }
    } 
    return 0;
}
