#pragma once
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

/** @brief Шифрование методом Гронсфельда (русский алфавит)
 *  @details Ключ устанавливается в конструкторе.
 *  Для зашифровывания и расшифровывания предназначены методы encrypt и decrypt.
 *  @warning Реализация только для русского языка (33 буквы, включая Ё)
 */
class modAlphaCipher {
private:
    std::string numAlpha = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; // алфавит по порядку
    std::map <std::string, int> alphaNum;  // "номер по символу" (символ — UTF-8 строка)
    std::vector <int> key;                 // ключ в числовом виде

    std::vector<int> convert(const std::string& s);   // строка -> вектор
    std::string convert(const std::vector<int>& v);   // вектор -> строка

    std::string getValidKey(const std::string& s);
    std::string getValidOpenText(const std::string& s);
    std::string getValidCipherText(const std::string& s);

public:
    modAlphaCipher() = delete;                          // запрет конструктора без параметров
    modAlphaCipher(const std::string& skey);            // конструктор для установки ключа
    std::string encrypt(const std::string& open_text);  // зашифрование
    std::string decrypt(const std::string& cipher_text);// расшифрование
};

class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg) : std::invalid_argument(what_arg) {}
};