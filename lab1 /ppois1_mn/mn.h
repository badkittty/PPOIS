#ifndef MN_H
#define MN_H

#include <iostream>
#include <vector>
#include <string>

class MN {
private:
    bool is_element_;
    char element_;
    std::vector<MN> children_;

    // Константы для парсинга
    static constexpr char OPEN_BRACE = '{';
    static constexpr char CLOSE_BRACE = '}';
    static constexpr char SEPARATOR = ',';
    static constexpr char SPACE = ' ';

    // Вспомогательные методы для парсинга
    static void skipWhitespaces(const std::string& str, size_t& pos);
    static char parseSingleElement(const std::string& str, size_t& pos);
    static MN parseNestedSet(const std::string& str, size_t& pos);
    static void validateOpeningBrace(const std::string& str, size_t& pos);
    static void validateClosingBrace(const std::string& str, size_t& pos);
    static void parseSetContents(const std::string& str, size_t& pos, MN& result_set);
    static void handleElementSeparator(const std::string& str, size_t& pos);

public:
    MN();
    explicit MN(char element);
    MN(const MN& other);
    MN& operator=(const MN& other);

    static MN fromString(const std::string& input_string);

    bool operator==(const MN& other) const;
    bool operator!=(const MN& other) const;

    friend std::ostream& operator<<(std::ostream& os, const MN& set);
    friend std::istream& operator>>(std::istream& is, MN& set);
};

#endif