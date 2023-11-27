#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

// Структура для представления блюда
struct Dish {
    string name;
    string type;
    double price;
};

// Функция для добавления блюда в файл
void addDishToFile(const Dish& dish, const string& filename) {
    ofstream file(filename, ios::app); // Открытие файла для добавления данных в конец
    if (file.is_open()) {
        file << dish.name << ',' << dish.type << ',' << dish.price << endl;
        cout << "Блюдо успешно добавлено в файл." << endl;
        file.close();
    } else {
        cerr << "Не удалось открыть файл для записи." << endl;
    }
}

// Функция для поиска блюда по названию в файле
void searchDishByName(const string& dishName, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(',');
            string name = line.substr(0, pos);
            if (name == dishName) {
                cout << "Найдено блюдо: " << line << endl;
                file.close();
                return;
            }
        }
        cout << "Блюдо с названием '" << dishName << "' не найдено." << endl;
        file.close();
    } else {
        cerr << "Не удалось открыть файл для чтения." << endl;
    }
}

// Функция для сортировки блюд в файле по типу или цене
void sortDishes(const string& sortBy, const string& filename) {
    vector<Dish> dishes;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.find(',', pos1 + 1);
            string name = line.substr(0, pos1);
            string type = line.substr(pos1 + 1, pos2 - pos1 - 1);
            double price = stod(line.substr(pos2 + 1));
            dishes.push_back({name, type, price});
        }
        file.close();

        if (sortBy == "type") {
            sort(dishes.begin(), dishes.end(), [](const Dish& a, const Dish& b) {
                return a.type < b.type;
            });
        } else if (sortBy == "price") {
            sort(dishes.begin(), dishes.end(), [](const Dish& a, const Dish& b) {
                return a.price < b.price;
            });
        }

        ofstream outFile("output.txt");
        if (outFile.is_open()) {
            for (const Dish& dish : dishes) {
                outFile << dish.name << ',' << dish.type << ',' << dish.price << endl;
            }
            cout << "Блюда успешно отсортированы и сохранены в файл 'output.txt'." << endl;
            outFile.close();
        } else {
            cerr << "Не удалось открыть файл для записи." << endl;
        }
    } else {
        cerr << "Не удалось открыть файл для чтения." << endl;
    }
}

// Функция для вывода данных о блюдах по запросу пользователя
void displayFilteredData(const string& query, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(',');
            string name = line.substr(0, pos);
            size_t pos2 = line.find(',', pos + 1);
            string type = line.substr(pos + 1, pos2 - pos - 1);
            double price = stod(line.substr(pos2 + 1));

            // Обработка запроса пользователя (пример: "price<=10")
            if (query == "all" || (query.substr(0, pos) == "type" && type == query.substr(pos + 1)) ||
                (query.substr(0, pos) == "price" && price <= stod(query.substr(pos + 2)))) {
                cout << "Название: " << name << ", Тип: " << type << ", Цена: " << price << endl;
            }
        }
        file.close();
    } else {
        cerr << "Не удалось открыть файл для чтения." << endl;
    }
}

int main() {
    string menuFileName = "menu.txt";

    while (true) {
        cout << "Меню:\n";
        cout << "1. Добавить блюдо\n";
        cout << "2. Поиск блюда\n";
        cout << "3. Сортировка блюд\n";
        cout << "4. Вывод данных по запросу\n";
        cout << "5. Выйти из программы\n";

        int choice;
        cout << "Выберите команду (1-5): ";
        cin >> choice;

        if (choice == 1) {
            Dish newDish;
            cin.ignore(); // Очистка буфера перед вводом строки
            cout << "Введите название блюда (пустая строка для завершения): ";
            getline(cin, newDish.name);

            if (newDish.name.empty()) {
                break; // Завершение ввода блюд
            }

            cout << "Введите тип блюда: ";
            getline(cin, newDish.type);

            cout << "Введите цену блюда: ";
            cin >> newDish.price;

            addDishToFile(newDish, menuFileName);
        } else if (choice == 2) {
            string searchName;
            cin.ignore();
            cout << "Введите название блюда для поиска: ";
            getline(cin, searchName);

            searchDishByName(searchName, menuFileName);
        } else if (choice == 3) {
            string sortBy;
            cout << "Выберите параметр сортировки (type/price): ";
            cin >> sortBy;

            sortDishes(sortBy, menuFileName);
        } else if (choice == 4) {
            string query;
            cout << "Введите запрос (type=Тип или price<=Цена): ";
            cin >> query;

            displayFilteredData(query, menuFileName);
        } else if (choice == 5) {
            break; // Выход из программы
        } else {
            cout << "Неверная команда. выберите от 1 до 5." << endl;
        }
    }

    return 0;
}
