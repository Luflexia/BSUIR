#include <iostream>
#include <string>
#include <cmath>
#include <utility>

class Checker;

using namespace std;

class Task {
    friend Checker;

    friend void print(Task *task);

public:
    Task() {
        this->id = -1;
        this->name = "unknown";
        this->result = -1;
    }

    Task(int id, string name) {
        this->id = id;
        this->name = move(name);
    }

    ~Task() {

    }

private:
    int id;
    string name;
    float result = 0;
};

class Checker {
public:
    static void setPercentage(Task *task, int completed, int total) {
        if (completed > total || completed < 0) {
            cout << "Error: incorrect input." << endl;
            task->result = 0;
            return;
        }
        task->result = (float) round(((float) completed / (float) total * 100) * 10) /
                       10;    //  Округляет до двух знаков после запятой и возвращяет.
    }
};

void print(Task *task) {
    cout << "\nID:\t" << task->id << "\nName:\t" << task->name << "\nResult:\t" << task->result << " %" << endl;
}

int main() {
    static int id = 0;
    const int total = 128;

    Task t1(id++, "Steve");
    Checker::setPercentage(&t1, 98, total);
    print(&t1);


    Task t2(id++, "Alex");
    Checker::setPercentage(&t2, 111, total);
    print(&t2);

    srand(time(nullptr));

    Task task[4];
    for (int i = 0; i < 4; i++) {
        task[i] = Task(id++, ("Bot" + to_string(i + 1)));
        Checker::setPercentage(&task[i], (int) (rand() % total), total);
        print(&task[i]);
    }

    return 0;
}