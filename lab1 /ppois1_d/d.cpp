#include "d.h"
#include <fstream>
#include <iostream>
#include <utility>

D::D() : root(nullptr), wordCount(0) {}

void D::addWord(const std::string& english, const std::string& russian) {
    root = insert(root, english, russian);
}

void D::removeWord(const std::string& english) {
    root = remove(root, english);
}

std::string D::getTranslation(const std::string& english) const {
    auto node = find(root, english);
    return node ? node->russian : "";
}

bool D::updateTranslation(const std::string& english, const std::string& russian) {
    auto node = find(root, english);
    if (node) {
        node->russian = russian;
        return true;
    }
    return false;
}

bool D::contains(const std::string& english) const {
    return find(root, english) != nullptr;
}

size_t D::getWordCount() const {
    return wordCount;
}

bool D::isEmpty() const {
    return root == nullptr;
}

// Загрузка из файла (формат: английское_слово русский_перевод)
bool D::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string english, russian;
    while (file >> english >> russian) {
        addWord(english, russian);
    }

    file.close();
    return true;
}

// Операторы
D& D::operator+=(const std::pair<const char*, const char*>& words) {
    addWord(std::string(words.first), std::string(words.second));
    return *this;
}

D& D::operator+=(const std::pair<std::string, std::string>& words) {
    addWord(words.first, words.second);
    return *this;
}

D& D::operator-=(const std::string& english) {
    removeWord(english);
    return *this;
}

std::string D::operator[](const std::string& english) const {
    return getTranslation(english);
}

std::string& D::operator[](const std::string& english) {
    auto node = find(root, english);
    if (!node) {
        // Если слова нет, добавляем с пустым переводом
        addWord(english, "");
        node = find(root, english);
    }
    return node->russian;
}

// Существующие приватные методы остаются без изменений
std::shared_ptr<D::Node> D::insert(std::shared_ptr<Node> node, const std::string& english, const std::string& russian) {
    if (!node) {
        wordCount++;
        return std::make_shared<Node>(english, russian);
    }

    if (english < node->english) {
        node->left = insert(node->left, english, russian);
    } else if (english > node->english) {
        node->right = insert(node->right, english, russian);
    } else {
        node->russian = russian;
    }

    return node;
}

std::shared_ptr<D::Node> D::remove(std::shared_ptr<Node> node, const std::string& english) {
    if (!node) return nullptr;

    if (english < node->english) {
        node->left = remove(node->left, english);
    } else if (english > node->english) {
        node->right = remove(node->right, english);
    } else {
        if (!node->left) {
            wordCount--;
            return node->right;
        } else if (!node->right) {
            wordCount--;
            return node->left;
        }

        auto minNode = findMin(node->right);
        node->english = minNode->english;
        node->russian = minNode->russian;
        node->right = remove(node->right, minNode->english);
    }

    return node;
}

std::shared_ptr<D::Node> D::find(std::shared_ptr<Node> node, const std::string& english) const {
    if (!node || node->english == english) {
        return node;
    }

    if (english < node->english) {
        return find(node->left, english);
    }

    return find(node->right, english);
}

std::shared_ptr<D::Node> D::findMin(std::shared_ptr<Node> node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}