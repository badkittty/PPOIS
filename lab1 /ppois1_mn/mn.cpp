#include "mn.h"
#include <stdexcept>
#include <cctype>

// Константы
constexpr char MN::OPEN_BRACE;
constexpr char MN::CLOSE_BRACE;
constexpr char MN::SEPARATOR;
constexpr char MN::SPACE;

// Конструкторы
MN::MN() : is_element_(false), element_('\0') {}

MN::MN(char element) : is_element_(true), element_(element) {}

MN::MN(const MN& other)
    : is_element_(other.is_element_),
      element_(other.element_),
      children_(other.children_) {}

MN& MN::operator=(const MN& other) {
    if (this != &other) {
        is_element_ = other.is_element_;
        element_ = other.element_;
        children_ = other.children_;
    }
    return *this;
}

// Вспомогательные методы для парсинга
void MN::skipWhitespaces(const std::string& str, size_t& pos) {
    while (pos < str.size() && std::isspace(str[pos])) {
        ++pos;
    }
}

char MN::parseSingleElement(const std::string& str, size_t& pos) {
    if (pos >= str.size() || !std::isalpha(str[pos])) {
        throw std::runtime_error("Expected alphabetical element");
    }
    return str[pos++];
}

void MN::validateOpeningBrace(const std::string& str, size_t& pos) {
    skipWhitespaces(str, pos);
    if (pos >= str.size() || str[pos] != OPEN_BRACE) {
        throw std::runtime_error("Expected opening brace '{'");
    }
    ++pos;
}

void MN::validateClosingBrace(const std::string& str, size_t& pos) {
    skipWhitespaces(str, pos);
    if (pos >= str.size() || str[pos] != CLOSE_BRACE) {
        throw std::runtime_error("Expected closing brace '}'");
    }
    ++pos;
}

void MN::handleElementSeparator(const std::string& str, size_t& pos) {
    skipWhitespaces(str, pos);
    if (pos < str.size() && str[pos] == SEPARATOR) {
        ++pos;
        skipWhitespaces(str, pos);
    }
}

void MN::parseSetContents(const std::string& str, size_t& pos, MN& result_set) {
    skipWhitespaces(str, pos);

    while (pos < str.size() && str[pos] != CLOSE_BRACE) {
        MN current_element;

        if (str[pos] == OPEN_BRACE) {
            current_element = parseNestedSet(str, pos);
        } else {
            current_element = MN(parseSingleElement(str, pos));
        }

        result_set.children_.push_back(current_element);
        handleElementSeparator(str, pos);
    }
}

MN MN::parseNestedSet(const std::string& str, size_t& pos) {
    MN result_set;
    result_set.is_element_ = false;

    validateOpeningBrace(str, pos);
    parseSetContents(str, pos, result_set);
    validateClosingBrace(str, pos);

    return result_set;
}

// Основные публичные методы
MN MN::fromString(const std::string& input_string) {
    size_t position = 0;
    MN parsed_set = parseNestedSet(input_string, position);

    skipWhitespaces(input_string, position);
    if (position != input_string.size()) {
        throw std::runtime_error("Extra characters after set definition");
    }

    return parsed_set;
}

bool MN::operator==(const MN& other) const {
    if (is_element_ != other.is_element_) {
        return false;
    }

    if (is_element_) {
        return element_ == other.element_;
    }

    if (children_.size() != other.children_.size()) {
        return false;
    }

    for (size_t index = 0; index < children_.size(); ++index) {
        if (children_[index] != other.children_[index]) {
            return false;
        }
    }

    return true;
}

bool MN::operator!=(const MN& other) const {
    return !(*this == other);
}

// Операторы ввода/вывода
std::ostream& operator<<(std::ostream& output_stream, const MN& cantor_set) {
    if (cantor_set.is_element_) {
        output_stream << cantor_set.element_;
    } else {
        output_stream << MN::OPEN_BRACE;
        for (size_t index = 0; index < cantor_set.children_.size(); ++index) {
            output_stream << cantor_set.children_[index];
            if (index + 1 < cantor_set.children_.size()) {
                output_stream << MN::SEPARATOR << MN::SPACE;
            }
        }
        output_stream << MN::CLOSE_BRACE;
    }
    return output_stream;
}

std::istream& operator>>(std::istream& input_stream, MN& cantor_set) {
    std::string input_line;
    if (!std::getline(input_stream, input_line)) {
        throw std::runtime_error("Failed to read input stream");
    }

    if (input_line.empty()) {
        throw std::runtime_error("Empty input line");
    }

    cantor_set = MN::fromString(input_line);
    return input_stream;
}