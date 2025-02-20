#include <iostream>
#include <windows.h>
#include "interfaces/MainMenu.h"
#include "services/Input.h"

#include "services/ContactDetails.h"
#include "immovables/piece/Piece.h"
#include "immovables/piece/House.h"
#include "immovables/piece/Empty.h"
#include "immovables/flat/Flat.h"
#include "immovables/parking/Parking.h"
#include "immovables/commercial/Commercial.h"

static unsigned int id = 0;
using namespace std;

int main() {
    setlocale(LC_ALL,"ru");
    SetConsoleOutputCP(CP_UTF8);

    Input in;
//    ContactDetails cd("+375293332255", "emsobak@mail.ru");

    /// -----------------------------

//    Piece piece(id++, "+375293332255", "emsobak@mail.ru", 9800, 102000, "обл. Минск, п. Стрипиздикова, ул. Ленина, 7", true, true, true, true);
//    piece.printInfo();
//    cout << "------------------------------------" << endl;
    House house(id++, "+375293332255", "emsobak@mail.ru", 24500, 140200, "обл. Минск, г. Натальевск, ул. Ленина, 12", false, false, true, true, 2, 11, 8);
    house.printInfo();
//    cout << "------------------------------------" << endl;
    Empty em(id++, "+375293332255", "emsobak@mail.ru", 9800, 102000, "обл. Минск, п. Стрипиздикова, ул. Ленина, 7", true, true, false, true, true, false);
    em.printInfo();
//    cout << "------------------------------------" << endl;
    Flat flat(id++, "+375293332255", "emsobak@mail.ru", 12300, 320, "г. Минск, ул. Ленина, д. 34/2, кв. 16", false, 4, 3, true);
    flat.printInfo();

    Parking parking(id++, "+375293332255", "emsobak@mail.ru", 12300, 320, "г. Минск, ул. Ленина, д. 34/2, кв. 16", false, 2);
    parking.printInfo();

    Commercial commercial(id++, "+375293332255", "emsobak@mail.ru", 12300, 320, "г. Минск, ул. Ленина, д. 34/2, кв. 16", false, 1);
    commercial.printInfo();
    /// -----------------------------

//    string m;
//    m = in.inputMobile();
//    cout << m << endl;
//    string em = in.inputEmail();
//    cout << em << endl;

//    ContactDetails cd(m, em);
//    cout << cd.getContactDetails() << endl;

//    MainMenu menu;
//    menu.print();

    return 0;
}




/*
 *  Task:
 * Информационная система по продаже недвижимости
 *
 *  Hint:
 * Информационная система – совокупность базы данных и всего комплекса аппаратно-программных средств
 *  для ее хранения, изменения и поиска информации,
 *  для взаимодействия с пользователем.
 */

/**
  						 Дом, Коттедж
						 /
			  -> Участок
			 |	 		 \
			 | 			 Пустой
Недвижимость-|
			 |->	Квартира
			 |->	Гаражи и стоянки
			 |
			 |			 	  / Офис
			  -> Коммерческая -	Склад
						  	  \	Помещение
*/

/*
 *
 */