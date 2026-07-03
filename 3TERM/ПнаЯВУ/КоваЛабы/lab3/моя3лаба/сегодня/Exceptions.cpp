#include "Exceptions.h"

void handleInvalidInput() {
    try {
        throw InvalidInputException("Invalid input. Please enter a valid value.");
    }
    catch (const InvalidInputException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void handleWatchNotFound(const std::string& watchId) {
    try {
        throw WatchNotFoundException("Watch with ID " + watchId + " not found.");
    }
    catch (const WatchNotFoundException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void handleWatchDeleteError(const std::string& watchId) {
    try {
        throw WatchDeleteException("Error deleting watch with ID " + watchId + ".");
    }
    catch (const WatchDeleteException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
