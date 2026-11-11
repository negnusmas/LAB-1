#include "modAlphaCipher.h"
#include <cctype>


static std::vector<std::string> splitUtf8(const std::string& s) {
    std::vector<std::string> chars;
    for (size_t i = 0; i < s.size(); ) {
        unsigned char c = s[i];
        size_t len = 1;
        if      ((c & 0x80) == 0x00) len = 1;
        else if ((c & 0xE0) == 0xC0) len = 2;
        else if ((c & 0xF0) == 0xE0) len = 3;
        else if ((c & 0xF8) == 0xF0) len = 4;
        chars.push_back(s.substr(i, len));
        i += len;
    }
    return chars;
}

static std::string toUpperRu(const std::string& ch) {
    static const std::string lower = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    static const std::string upper = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    size_t pos = lower.find(ch);
    if (pos != std::string::npos) return std::string(1, '\0') .replace(0,1,"") + upper.substr(pos, 2);
    return ch;
}

static bool isUpperRu(const std::string& ch) {
    static const std::string upper = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    return ch.size() == 2 && upper.find(ch) != std::string::npos;
}

static bool isLowerRu(const std::string& ch) {
    static const std::string lower = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    return ch.size() == 2 && lower.find(ch) != std::string::npos;
}

std::string modAlphaCipher::getValidKey(const std::string& s) {
    if (s.empty())
        throw cipher_error("Empty key");
    std::vector<std::string> chars = splitUtf8(s);
    std::string tmp;
    for (const auto& c : chars) {
        if (isUpperRu(c)) {
            tmp += c;
        } else if (isLowerRu(c)) {
            tmp += toUpperRu(c);          // строчные переводим в заглавные
        } else {
            throw cipher_error("Invalid key " + s);   // цифры, пробелы, латиница
        }
    }
   
    int n = 0;
    for (auto e : key) if (e == 0) n++;
    if (2 * n > (int)key.size())
        throw cipher_error("WeakKey");
    return tmp;
}

std::string modAlphaCipher::getValidOpenText(const std::string& s) {
    std::vector<std::string> chars = splitUtf8(s);
    std::string tmp;
    for (const auto& c : chars) {
        if (isUpperRu(c)) {
            tmp += c;
        } else if (isLowerRu(c)) {
            tmp += toUpperRu(c);          // строчные в заглавные
        } 
    }
    if (tmp.empty())
        throw cipher_error("Empty open text");
    return tmp;
}

std::string modAlphaCipher::getValidCipherText(const std::string& s) {
    if (s.empty())
        throw cipher_error("Empty cipher text");
    for (const auto& c : splitUtf8(s)) {
        if (!isUpperRu(c))
            throw cipher_error("Invalid cipher text " + s);
    }
    return s;
}

//Преобразования
std::vector<int> modAlphaCipher::convert(const std::string& s) {
    std::vector<int> result;
    for (const auto& c : splitUtf8(s)) {
        auto it = alphaNum.find(c);
        if (it == alphaNum.end())
            throw cipher_error("Invalid character in text: " + c);
        result.push_back(it->second);
    }
    return result;
}

std::string modAlphaCipher::convert(const std::vector<int>& v) {
    std::string result;
    for (auto i : v)
        result += numAlpha.substr(i * 2, 2);  // каждая русская буква 
    return result;
}

//Конструктор
modAlphaCipher::modAlphaCipher(const std::string& skey) {
    for (size_t i = 0; i < numAlpha.size() / 2; i++)
        alphaNum[numAlpha.substr(i * 2, 2)] = i;

    // строим ключ
    key = convert(getValidKey(skey));
}

std::string modAlphaCipher::encrypt(const std::string& open_text) {
    std::vector<int> work = convert(getValidOpenText(open_text));
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % (int)(numAlpha.size() / 2);
    }
    return convert(work);
}

std::string modAlphaCipher::decrypt(const std::string& cipher_text) {
    std::vector<int> work = convert(getValidCipherText(cipher_text));
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + (int)(numAlpha.size() / 2) - key[i % key.size()])
                  % (int)(numAlpha.size() / 2);
    }
    return convert(work);
}
