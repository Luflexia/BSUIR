#include "Watch.h"
#include "MechanicalWatch.h"
#include "ElectronicWatch.h"
#include "SmartWatch.h"
#include "WristSmartWatch.h"
#include "TableClock.h"
#include "WristWatch.h"

int main() {
    // Пример использования объектов различных классов
    Watch* watches[] = {
        new MechanicalWatch(10, 30, 45),
        new ElectronicWatch(14, 45, 20),
        new TableClock(8, 0, 0, true),
        new WristWatch(12, 15, 30, true),
        new SmartWatch(18, 0, 0, true),
        new WristSmartWatch(22, 30, 15, true, false)
    };

    // Вывод информации о каждом объекте
    for (const auto& watch : watches) {
        watch->display();
        std::cout << "------------------------------------" << std::endl;
    }

    // Освобождение памяти
    for (const auto& watch : watches) {
        delete watch;
    }

    return 0;
}
