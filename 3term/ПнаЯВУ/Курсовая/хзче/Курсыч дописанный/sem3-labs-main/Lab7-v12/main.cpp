#include <iostream>
#include <utility>
#include <vector>
#include <fstream>

using namespace std;

class Exception {
    string errorMsg;
public:
    explicit Exception(string msg) {
        errorMsg = "Error: " + std::move(msg);
    }

    ~Exception() = default;

    string what() {
        return errorMsg;
    }
};

const string fileNameT = "text_write.txt";
const string fileNameTB = "binary_write.txt";
const string fileNameB = "binary_write.bin";
const string tempFileName = "temp";

class Plant {
    string name;
    float age;
    bool ripe;
public:
    Plant(string name = "Unknown", float age = 0, bool ripe = false) {
        setName(std::move(name));
        setAge(age);
        setRipe(ripe);
    }

    ~Plant() = default;

    /// ================= FOR FILES ====================
    //  ------------------- write ----------------------
    void writeT() {
        ofstream o;
        o.open(fileNameT, ios::app);
        if (!o.is_open())
            throw Exception("Failed to open file");
        o << *this << "\n";
        o.close();
    }

    void writeTB() {
        ofstream o;
        o.open(fileNameTB, ios::app);
        if (!o.is_open())
            throw Exception("Failed to open file");
        o.write((char *) this, sizeof(Plant));
        o.close();
    }

    void writeB() {
        ofstream o;
        o.open(fileNameB, ios::app);
        if (!o.is_open())
            throw Exception("Failed to open file");
        o.write(reinterpret_cast<char *>(this), sizeof(Plant));
        o.close();
    }

    //  ------------------- read ----------------------
    static void readT() {
        ifstream file;
        file.open(fileNameT);
        if (!file.is_open())
            throw Exception("Failed to open file");

        string str;
        while (getline(file, str)) {
            cout << str << endl;
        }
        file.close();
    }

    static void readTB() {
        ifstream file;
        file.open(fileNameTB);
        if (!file.is_open())
            throw Exception("Failed to open file");
        Plant p;
        while (file.read((char *) &p, sizeof(Plant))) {
            cout << p << endl;
        }
        file.close();
    }

    static void readB() {
        ifstream file;
        file.open(fileNameB);
        if (!file.is_open())
            throw Exception("Failed to open file");
        Plant p;
        while (file.read(reinterpret_cast<char *>(&p), sizeof(Plant))) {
            cout << p << endl;
        }
        file.close();
    }

    //  ------------------- find ----------------------
    static bool findT(string name) {
        ifstream in;
        in.open(fileNameT);
        Plant plant;
        while (in >> plant) {
            if (plant.name == name) {
                cout << plant << endl;
                return true;
            }
        }
        return false;
    }

    static bool findTB(string name) {
        ifstream in;
        in.open(fileNameTB);
        Plant plant;
        while (in.read(reinterpret_cast<char *>(&plant), sizeof(plant))) {
            if (plant.name == name) {
                cout << plant << endl;
                return true;
            }
        }
        return false;
    }

    static bool findB(string name) {
        ifstream in;
        in.open(fileNameB);
        Plant plant;
        while (in.read(reinterpret_cast<char *>(&plant), sizeof(plant))) {
            if (plant.name == name) {
                cout << plant << endl;
                return true;
            }
        }
        return false;
    }

    //  ------------------- delete ---------------------
    static void deleteT(string name) {
        if (findT(name)) {
            ifstream in;
            ofstream out;
            in.open(fileNameT);
            out.open(tempFileName, ios::app);
            if (!in.is_open() || !out.is_open())
                throw Exception("Failed to open file");
            Plant plant;
            while (in >> plant) {
                if (plant.name != name) {
                    out << plant << "\n";
                }
            }
            in.close();
            out.close();

            const char *fileName = fileNameT.data();
            const char *oldFileName = tempFileName.data();
            remove(fileName);
            rename(oldFileName, fileName);
        } else
            throw Exception("Attempt to delete a non-existent object");
    }

    static void deleteTB(string name) {
        if (findTB(name)) {
            ifstream in;
            ofstream out;
            in.open(fileNameTB);
            out.open(tempFileName);
            if (!in.is_open() || !out.is_open())
                throw Exception("Failed to open file");
            Plant plant;
            while (in.read(reinterpret_cast<char *>(&plant), sizeof(plant))) {
                if (plant.name != name) {
                    out.write(reinterpret_cast<char *>(&plant), sizeof(plant));
                }
            }
            in.close();
            out.close();

            const char *fileName = fileNameTB.data();
            const char *oldFileName = tempFileName.data();
            remove(fileName);
            rename(oldFileName, fileName);
        } else
            throw Exception("Attempt to delete a non-existent object");
    }

    static void deleteB(string name) {
        if (findB(name)) {
            ifstream in;
            ofstream out;
            in.open(fileNameB);
            out.open(tempFileName);
            if (!in.is_open() || !out.is_open())
                throw Exception("Failed to open file");
            Plant plant;
            while (in.read(reinterpret_cast<char *>(&plant), sizeof(plant))) {
                if (plant.name != name) {
                    out.write(reinterpret_cast<char *>(&plant), sizeof(plant));
                }
            }
            in.close();
            out.close();

            const char *fileName = fileNameB.data();
            const char *oldFileName = tempFileName.data();
            remove(fileName);
            rename(oldFileName, fileName);
        } else
            throw Exception("Attempt to delete a non-existent object");
    }

    /// ==================== END =======================

    //  ------------------ out \ in --------------------

    friend ostream &operator<<(ostream &os, Plant &Plant) {
        os << Plant.name << " " << Plant.age << " " << Plant.ripe;
        return os;
    }

    friend istream &operator>>(istream &in, Plant &Plant) {
        in >> Plant.name >> Plant.age >> Plant.ripe;
        return in;
    }

    //  ------------------- basic ----------------------

    void printData() {
        string tmp = "false";
        if (ripe)
            tmp = "true";
        cout << "Name: " << name << " | Age: " << age << " | Ripe: " << tmp;
    }

    void setName(string name) {
        this->name = std::move(name);
    }

    string getName() {
        return name;
    }

    void setAge(float age) {
        this->age = age;
    }

    float getAge() {
        return age;
    }

    void setRipe(bool ripe) {
        this->ripe = ripe;
    }

    bool isRipe() {
        return ripe;
    }
};

int main() {
    //  Waste disposal
    try {
        const char *fn1 = fileNameT.data();
        remove(fn1);
        const char *fn2 = fileNameTB.data();
        remove(fn2);
        const char *fn3 = fileNameB.data();
        remove(fn3);
        const char *fn4 = tempFileName.data();
        remove(fn4);
    } catch (...) {
        unexpected();
    }

    vector<Plant> array{
            Plant("Potato", 0.43, false),
            Plant("Cabbage", 0.3, false),
            Plant("Wheat", 1.05, true),
            Plant("Tulips", 0.8, true),
            Plant("Oak", 54.8, false)
    };

    try {
        for (auto element: array) {
            element.writeT();
            element.writeTB();
            element.writeB();
        }

        cout << "\tText file text mode:" << endl;
        Plant::readT();
        cout << endl;

        cout << "\tTrying to delete..." << endl;
        Plant::deleteT(array[2].getName());
        cout << "\tSuccess." << endl;

        cout << endl << "\tText file text mode:" << endl;
        Plant::readT();
        cout << endl;

    } catch (Exception e) {
        cout << e.what();
        return 1;
    } catch (...) {
        unexpected();
    }

    return 0;
}