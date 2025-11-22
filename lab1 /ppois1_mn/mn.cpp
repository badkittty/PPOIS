#include "mn.h"
#include <stdexcept>
#include <cctype>
#include <sstream>

constexpr char MN::OPEN_BRACE;
constexpr char MN::CLOSE_BRACE;
constexpr char MN::SEPARATOR;
constexpr char MN::SPACE;

// Конструкторы
MN::MN(char element) {
    elements_.push_back(element);
}

MN::MN(const std::shared_ptr<MN>& subset) {
    elements_.push_back(subset);
}

MN::MN(std::initializer_list<Element> elements) : elements_(elements) {}

// Парсинг
void MN::skipSpaces(const std::string& str, size_t& pos) {
    while (pos < str.size() && std::isspace(str[pos])) {
        pos++;
    }
}

char MN::parseChar(const std::string& str, size_t& pos) {
    skipSpaces(str, pos);
    if (pos >= str.size() || !std::isalpha(str[pos])) {
        throw std::runtime_error("Expected letter");
    }
    return str[pos++];
}

MN::Element MN::parseElement(const std::string& str, size_t& pos) {
    skipSpaces(str, pos);

    if (pos < str.size() && str[pos] == OPEN_BRACE) {
        // Это вложенное множество
        MN subset = parseSet(str, pos);
        return std::make_shared<MN>(subset);
    } else {
        // Это символ
        char element = parseChar(str, pos);
        return element;
    }
}

MN MN::parseSet(const std::string& str, size_t& pos) {
    skipSpaces(str, pos);

    if (pos >= str.size() || str[pos] != OPEN_BRACE) {
        throw std::runtime_error("Expected '{'");
    }
    pos++;

    MN result;

    skipSpaces(str, pos);

    // Пустое множество
    if (pos < str.size() && str[pos] == CLOSE_BRACE) {
        pos++;
        return result;
    }

    while (pos < str.size() && str[pos] != CLOSE_BRACE) {
        // Парсим элемент (символ или вложенное множество)
        Element element = parseElement(str, pos);
        result.elements_.push_back(element);

        skipSpaces(str, pos);
        if (pos < str.size() && str[pos] == SEPARATOR) {
            pos++;
            skipSpaces(str, pos);
        }
    }

    if (pos >= str.size() || str[pos] != CLOSE_BRACE) {
        throw std::runtime_error("Expected '}'");
    }
    pos++;

    return result;
}

MN MN::fromString(const std::string& input) {
    if (input.empty()) {
        throw std::runtime_error("Empty input");
    }

    std::string trimmed = input;
    trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
    trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);

    if (trimmed.empty()) {
        throw std::runtime_error("Empty input after trimming");
    }

    size_t pos = 0;
    MN result = parseSet(trimmed, pos);

    skipSpaces(trimmed, pos);
    if (pos != trimmed.size()) {
        throw std::runtime_error("Extra characters after set definition");
    }

    return result;
}

// Основные операции
bool MN::contains(const Element& element) const {
    for (const auto& el : elements_) {
        if (areEqual(el, element)) {
            return true;
        }
    }
    return false;
}

bool MN::contains(char element) const {
    return contains(Element(element));
}

bool MN::contains(const MN& subset) const {
    auto subset_ptr = std::make_shared<MN>(subset);
    return contains(Element(subset_ptr));
}

void MN::addElement(char element) {
    if (!contains(element)) {
        elements_.push_back(element);
    }
}

void MN::addElement(const std::shared_ptr<MN>& subset) {
    if (!contains(Element(subset))) {
        elements_.push_back(subset);
    }
}

bool MN::removeElement(char element) {
    auto it = std::find_if(elements_.begin(), elements_.end(),
        [element](const Element& el) {
            return std::holds_alternative<char>(el) && std::get<char>(el) == element;
        });

    if (it != elements_.end()) {
        elements_.erase(it);
        return true;
    }
    return false;
}

bool MN::removeElement(const std::shared_ptr<MN>& subset) {
    auto it = std::find_if(elements_.begin(), elements_.end(),
        [&subset](const Element& el) {
            return std::holds_alternative<std::shared_ptr<MN>>(el) &&
                   *std::get<std::shared_ptr<MN>>(el) == *subset;
        });

    if (it != elements_.end()) {
        elements_.erase(it);
        return true;
    }
    return false;
}

size_t MN::cardinality() const {
    return elements_.size();
}

bool MN::isEmpty() const {
    return elements_.empty();
}

void MN::clear() {
    elements_.clear();
}

// Математические операции
MN MN::operator+(const MN& other) const {
    MN result = *this;
    for (const auto& element : other.elements_) {
        if (!result.contains(element)) {
            result.elements_.push_back(element);
        }
    }
    return result;
}

MN MN::operator*(const MN& other) const {
    MN result;
    for (const auto& element : elements_) {
        if (other.contains(element)) {
            result.elements_.push_back(element);
        }
    }
    return result;
}

MN MN::operator-(const MN& other) const {
    MN result;
    for (const auto& element : elements_) {
        if (!other.contains(element)) {
            result.elements_.push_back(element);
        }
    }
    return result;
}

MN& MN::operator+=(const MN& other) {
    for (const auto& element : other.elements_) {
        if (!contains(element)) {
            elements_.push_back(element);
        }
    }
    return *this;
}

MN& MN::operator*=(const MN& other) {
    std::vector<Element> new_elements;
    for (const auto& element : elements_) {
        if (other.contains(element)) {
            new_elements.push_back(element);
        }
    }
    elements_ = new_elements;
    return *this;
}

MN& MN::operator-=(const MN& other) {
    std::vector<Element> new_elements;
    for (const auto& element : elements_) {
        if (!other.contains(element)) {
            new_elements.push_back(element);
        }
    }
    elements_ = new_elements;
    return *this;
}

// Сравнение
bool MN::operator==(const MN& other) const {
    if (elements_.size() != other.elements_.size()) {
        return false;
    }

    for (const auto& element : elements_) {
        if (!other.contains(element)) {
            return false;
        }
    }
    return true;
}

bool MN::operator!=(const MN& other) const {
    return !(*this == other);
}

// Принадлежность
bool MN::operator[](char element) const {
    return contains(element);
}

bool MN::operator[](const MN& subset) const {
    return contains(subset);
}

// Преобразование в строку
std::string MN::toString() const {
    if (isEmpty()) {
        return "{}";
    }

    std::string result = "{";
    for (size_t i = 0; i < elements_.size(); ++i) {
        result += elementToString(elements_[i]);
        if (i + 1 < elements_.size()) {
            result += ", ";
        }
    }
    result += "}";
    return result;
}

// Вспомогательные методы
bool MN::areEqual(const Element& a, const Element& b) {
    if (a.index() != b.index()) {
        return false;
    }

    if (std::holds_alternative<char>(a)) {
        return std::get<char>(a) == std::get<char>(b);
    } else {
        return *std::get<std::shared_ptr<MN>>(a) == *std::get<std::shared_ptr<MN>>(b);
    }
}

std::string MN::elementToString(const Element& element) {
    if (std::holds_alternative<char>(element)) {
        return std::string(1, std::get<char>(element));
    } else {
        return std::get<std::shared_ptr<MN>>(element)->toString();
    }
}

// Ввод/вывод
std::ostream& operator<<(std::ostream& os, const MN& set) {
    os << set.toString();
    return os;
}

std::istream& operator>>(std::istream& is, MN& set) {
    std::string input;
    if (!std::getline(is, input)) {
        throw std::runtime_error("Failed to read input");
    }

    if (input.empty()) {
        set = MN();
    } else {
        set = MN::fromString(input);
    }
    return is;
}

// Вспомогательный рекурсивный метод для построения булеана
MN MN::powerSetRecursive(const std::vector<Element>& current_set, size_t index) const {
    if (index >= elements_.size()) {
        // Базовый случай - возвращаем множество, содержащее текущее подмножество
        MN result;
        if (!current_set.empty()) {
            // Создаем новое множество из текущих элементов
            MN subset;
            for (const auto& element : current_set) {
                if (std::holds_alternative<char>(element)) {
                    subset.addElement(std::get<char>(element));
                } else {
                    subset.addElement(std::get<std::shared_ptr<MN>>(element));
                }
            }
            result.addElement(std::make_shared<MN>(subset));
        }
        return result;
    }

    // Рекурсивный случай: включаем текущий элемент и не включаем
    MN result;

    // Не включаем текущий элемент
    MN without_current = powerSetRecursive(current_set, index + 1);
    result += without_current;

    // Включаем текущий элемент
    std::vector<Element> new_set = current_set;
    new_set.push_back(elements_[index]);
    MN with_current = powerSetRecursive(new_set, index + 1);
    result += with_current;

    return result;
}

// Основной метод для построения булеана
MN MN::powerSet() const {
    if (isEmpty()) {
        // Булеан пустого множества - множество, содержащее пустое множество
        MN result;
        MN empty_set;
        result.addElement(std::make_shared<MN>(empty_set));
        return result;
    }

    std::vector<Element> current_set;
    MN result = powerSetRecursive(current_set, 0);

    // Добавляем пустое множество 
    MN empty_set;
    result.addElement(std::make_shared<MN>(empty_set));

    return result;
}
