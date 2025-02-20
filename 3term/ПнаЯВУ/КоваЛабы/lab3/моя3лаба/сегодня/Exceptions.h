#pragma once
#include "Watch.h"
#include <exception>
#include <string>

class InvalidInputException : public std::exception {
public:
    InvalidInputException(const std::string& message) : message(message) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
private:
    std::string message;
};

class WatchNotFoundException : public std::exception {
public:
    WatchNotFoundException(const std::string& message) : message(message) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
private:
    std::string message;
};

class WatchDeleteException : public std::exception {
public:
    WatchDeleteException(const std::string& message) : message(message) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
private:
    std::string message;
};

// Пользовательские функции обработки исключений
void handleInvalidInput();
void handleWatchNotFound(const std::string& watchId);
void handleWatchDeleteError(const std::string& watchId);
