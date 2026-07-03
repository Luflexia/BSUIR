#include <iostream>
#include <fstream>
#include <utility>
#include <map>

class Exception {
    std::string errorString;
public:
    Exception(std::string msg) { errorString = "Error: " + msg; }

    ~Exception() = default;

    std::string what() { return errorString; }
};

const std::string initFileName = "input.txt";

class SomeClass {
public:
    int id = -1;
    int dimensionX{};
    int dimensionY{};
    double scale{};
    std::string licenseID;
};

class InitFile {
    std::string inputFile;
    const std::string outputFile = "output.txt";
public:
    InitFile(std::string inputFilePath = "input.txt") {
        inputFile = std::move(inputFilePath);
    }

    ~InitFile() = default;

    void run() {
        SomeClass sc;
        std::map<std::string, std::string> myMap;

        std::fstream in;
        std::cout << "[PROC] Trying to open \"" << inputFile << "\"..." << std::endl;
        in.open(this->inputFile);

        if (!in.is_open())
            throw Exception("Can`t open the file.");
        std::cout << "[PROC] Success." << std::endl;

        std::cout << "[PROC] Reading \"" << inputFile << "\" and initializing the Map class." << std::endl;
        std::string lineStr;
        int lines = 1;
        while (std::getline(in, lineStr)) {
            std::cout << "[READ] Read line #" << lines++ << std::endl;
            myMap.emplace(lineStr.substr(0, lineStr.find(' ')),
                          lineStr.substr(lineStr.find(' ') + 1, lineStr.find('\n')));
            std::cout << "[READ] Success." << std::endl;
        }

        setParameter(myMap, sc);

        std::cout << "[PROC] The class has been initialized." << std::endl;
        std::cout << "[PROC] Closing \"" << inputFile << "\"." << std::endl;
        in.close();

        std::cout << "[Example] Class after initialization:" << std::endl;
        std::cout << "\t======= MAP =======" << std::endl;
        std::cout << "\tID:        " << sc.id << std::endl
                  << "\tLicense:   " << sc.licenseID << std::endl
                  << "\tScale:     " << sc.scale << std::endl
                  << "\tX:         " << sc.dimensionX << std::endl
                  << "\tY:         " << sc.dimensionY << std::endl;
        std::cout << "\t===================" << std::endl;
        resultFile(sc.id < 0);
    }


    void setParameter(const std::map<std::string, std::string> &myMap, SomeClass &sc) {
        std::cout << "[INIT] Trying to initialize..." << std::endl;
        using namespace std;
        try {
            sc.id = stoi((myMap.find("id"))->second);
            sc.dimensionX = stoi((myMap.find("dimensionX"))->second);
            sc.dimensionY = stoi((myMap.find("dimensionY"))->second);
            sc.scale = stod((myMap.find("scale"))->second);
            sc.licenseID = (myMap.find("licenseID"))->second;
        } catch (...) {
            cout << "[INIT] Unknown parameter found." << endl;
            return;
        }
        std::cout << "[INIT] Success." << std::endl;
    };

    void resultFile(bool fail) {
        std::ofstream out;
        std::cout << "[PROC] Trying to open \"" << outputFile << "\"..." << std::endl;
        out.open(this->outputFile);
        if (!out.is_open())
            throw Exception("Can`t open the file.");
        std::cout << "[PROC] Success." << std::endl;
        std::cout << "[PROC] Write result to file." << std::endl;
        std::cout << "[PROC] Success." << std::endl;
        out << (fail ? "Fail" : "Success");
        std::cout << "[PROC] Closing \"" << outputFile << "\"." << std::endl;
        out.close();
    }
};

int main() {
    //  Waste disposal
    try {
        const char *fn = "output.txt";
        remove(fn);
    } catch (...) {}

    InitFile initFile(initFileName);
    try {
        initFile.run();
    } catch (Exception exception) {
        initFile.resultFile(true);
        std::cout << exception.what();
    } catch (...) {
        initFile.resultFile(true);
        std::unexpected();
    }
    return 0;
}
