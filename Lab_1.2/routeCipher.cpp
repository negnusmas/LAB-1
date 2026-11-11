#include "routeCipher.h"
#include <cwctype>
#include <vector>

//Вспомогательные методы

RouteCipher::RouteCipher(size_t skey) {
    if (skey < 2)
        throw cipher_error("Invalid key: number of columns must be >= 2");
    if (skey > 1000)
        throw cipher_error("Invalid key: too many columns");
    key = skey;
}

std::wstring RouteCipher::getValidText(const std::wstring& s) {
    std::wstring result;
    for (wchar_t c : s) {
        if (std::iswalpha(c)) {              // оставляем только буквы
            result.push_back(std::iswlower(c) ? std::towupper(c) : c);
        }
    }
    if (result.empty())
        throw cipher_error("Empty text");
    return result;
}

std::wstring RouteCipher::formatTable(const std::wstring& text,
                                      size_t rows, size_t cols) const {
    std::wstring table(rows * cols, L'~');   // ~ — padding
    for (size_t i = 0; i < text.size(); i++)
        table[i] = text[i];
    return table;
}

// ==================== Зашифрование ====================

std::wstring RouteCipher::encrypt(const std::wstring& open_text) {
    std::wstring text = getValidText(open_text);

    size_t n    = text.size();
    size_t cols = key;
    size_t rows = (n + cols - 1) / cols;

    std::wstring table = formatTable(text, rows, cols);

    // Маршрут считывания: сверху вниз, справа налево
    std::wstring cipher;
    cipher.reserve(rows * cols);
    for (size_t c = cols; c-- > 0; ) {
        for (size_t r = 0; r < rows; r++)
            cipher.push_back(table[r * cols + c]);
    }
    return cipher;                            // длина сохраняется
}

// ==================== Расшифрование ====================

std::wstring RouteCipher::decrypt(const std::wstring& cipher_text) {
    std::wstring text = getValidText(cipher_text);

    size_t n    = text.size();
    size_t cols = key;
    size_t rows = (n + cols - 1) / cols;

    if (rows * cols != n)
        throw cipher_error("Invalid cipher text length for this key");

    // Заполняем таблицу по маршруту считывания
    std::wstring table(rows * cols, L'~');
    size_t idx = 0;
    for (size_t c = cols; c-- > 0; ) {
        for (size_t r = 0; r < rows; r++)
            table[r * cols + c] = text[idx++];
    }

    // Читаем по строкам слева направо
    std::wstring result;
    result.reserve(rows * cols);
    for (size_t i = 0; i < rows * cols; i++)
        result.push_back(table[i]);

    size_t last = result.find_last_not_of(L'~');
    if (last == std::wstring::npos)
        throw cipher_error("Invalid cipher text");
    result.erase(last + 1);
    return result;
}
