#pragma once
#include <string>
#include <stdexcept>

/** @brief Исключение модуля маршрутной перестановки */
class cipher_error : public std::invalid_argument {
public:
    explicit cipher_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}
    explicit cipher_error(const char* what_arg) : std::invalid_argument(what_arg) {}
};

/** @brief Шифр табличной маршрутной перестановки
 *  @details Ключ — количество столбцов таблицы (устанавливается в конструкторе).
 *           Маршрут записи:  слева направо, сверху вниз (по строкам).
 *           Маршрут считывания: сверху вниз, справа налево (по столбцам).
 *  @warning Реализация для русского алфавита (буквы верхнего регистра).
 */
class RouteCipher {
private:
    size_t key;                             // число столбцов таблицы

    std::wstring getValidText(const std::wstring& s);
    std::wstring formatTable(const std::wstring& text,
                             size_t rows, size_t cols) const;

public:
    RouteCipher() = delete;                 // без ключа объект создать нельзя
    RouteCipher(size_t skey);               // установка ключа
    std::wstring encrypt(const std::wstring& open_text);   // зашифрование
    std::wstring decrypt(const std::wstring& cipher_text); // расшифрование
};