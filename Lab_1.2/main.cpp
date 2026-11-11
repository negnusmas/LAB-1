#include <iostream>
#include <string>
#include <clocale>
#include "routeCipher.h"

using namespace std;

int main() {

    setlocale(LC_ALL, "");

    size_t key;
    wstring text;
    int op;


    wcout << L"Введите ключ (>=2): ";
    if (!(wcin >> key)) {
        wcerr << L"Ошибка: это не число" << endl;
        return 1;
    }

    try {

        RouteCipher cipher(key);


        do {
            wcout << L"\nВыберите операцию (0 - выход, 1 - зашифровать, 2 - расшифровать): ";
            if (!(wcin >> op)) {
                wcin.clear();
                wcin.ignore(32767, L'\n');
                op = -1; 
            }

            if (op == 1 || op == 2) {
                
                wcout << L"Введите текст (русский): ";
                wcin >> text; 

                if (op == 1) {
                    wstring cipher_text = cipher.encrypt(text);
                    wcout << L"Зашифрованный текст: " << cipher_text << endl;
                } 
                else if (op == 2) {
                    wstring decrypted_text = cipher.decrypt(text);
                    wcout << L"Расшифрованный текст: " << decrypted_text << endl;
                }
            } 
            else if (op != 0) {
                wcout << L"Неверная операция" << endl;
            }

        } while (op != 0);

    } 
    catch (const cipher_error& e) {
        wcerr << L"\n[ОШИБКА]: " << e.what() << endl;
    }
    catch (const exception& e) {
        wcerr << L"\n[СИСТЕМНАЯ ОШИБКА]: " << e.what() << endl;
    }

    wcout << L"Программа завершена." << endl;
    return 0;
}