#include <iostream>
#include <string>
#include "modAlphaCipher.h"

using namespace std;

// ---------- Проверка «только заглавные русские буквы» ----------
// Работаем по UTF-8: каждая русская буква — 2 байта.
static bool isValidKey(const string& s) {
    if (s.empty()) return false;
    static const string upper = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    static const string lower = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = s[i];
        size_t len = 1;
        if      ((c & 0x80) == 0x00) len = 1;
        else if ((c & 0xE0) == 0xC0) len = 2;
        else if ((c & 0xF0) == 0xE0) len = 3;
        else if ((c & 0xF8) == 0xF0) len = 4;
        string ch = s.substr(i, len);
        if (!(ch.size() == 2 &&
              (upper.find(ch) != string::npos || lower.find(ch) != string::npos)))
            return false;  // цифра, пробел, знак, латиница — недопустимы
        i += len;
    }
    return true;

}

//Проверка «текст содержит хотя бы одну русскую букву»
static bool isValidText(const string& s) {
    static const string upper = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    static const string lower = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    if (s.empty()) return false;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = s[i];
        size_t len = 1;
        if      ((c & 0x80) == 0x00) len = 1;
        else if ((c & 0xE0) == 0xC0) len = 2;
        else if ((c & 0xF0) == 0xE0) len = 3;
        else if ((c & 0xF8) == 0xF0) len = 4;
        string ch = s.substr(i, len);
        if (ch.size() == 2 &&
            (upper.find(ch) != string::npos || lower.find(ch) != string::npos))
            return true;
        i += len;
    }
    return false;
}

//Точка входа
int main(int argc, char **argv) {
    string key;
    string text;
    unsigned op;

    cout << "Cipher ready. Input key: ";
    if (!(cin >> key)) return 1;

    if (!isValidKey(key)) {
        cerr << "key not valid\n";
        return 1;
    }
    cout << "Key loaded\n";

    try {
        modAlphaCipher cipher(key);   // ключ устанавливается один раз

        do {
            cout << "Cipher ready. Input operation (0-exit, 1-encrypt, 2-decrypt): ";
            if (!(cin >> op)) break;

            if (op > 2) {
                cout << "Illegal operation\n";
            } else if (op > 0) {
                cout << "Cipher ready. Input text: ";
                if (!(cin >> text)) break;

                if (!isValidText(text)) {
                    cout << "Operation aborted: invalid text\n";
                    continue;
                }

                if (op == 1) {
                    try {
                        string cipherText = cipher.encrypt(text);
                        cout << "Encrypted text: " << cipherText << endl;

                        // Демонстрация обратимости
                        string back = cipher.decrypt(cipherText);
                        cout << "Decrypted back: " << back << endl;
                    } catch (const cipher_error& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                } else { // op == 2
                    try {
                        string decryptedText = cipher.decrypt(text);
                        cout << "Decrypted text: " << decryptedText << endl;
                    } catch (const cipher_error& e) {
                        cerr << "Error: " << e.what() << endl;
                    }
                }
            }
        } while (op != 0);
    } catch (const cipher_error& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}