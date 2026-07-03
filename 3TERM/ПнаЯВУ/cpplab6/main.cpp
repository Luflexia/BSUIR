#include <iostream>
#include <string>

using namespace std;


class Plant
{
public:
    Plant()
    {

    }
    Plant(string name)
    {
        this->personal_name = name;
    }

    virtual void get_info() = 0;

    virtual ~Plant()
    {

    }
protected:
    string personal_name;
    virtual float some_calculation() = 0;
};

class Pomponella : public virtual Plant
{
public:
    Pomponella()
    {

    }
    Pomponella(string name) :Plant(name)
    {

    }

    void get_info() override
    {
        cout << "\n /// info about Plant ///\n\n class \"Pomponella\"\n plant name \"" << this->personal_name <<
             "\"\n";
        cout << " color \"" << this->color << "\"\n average plant height in centimeters \"" << this->average_height <<
             "\"\n average plant height in meters \"" << this->some_calculation() << "\"\n";
    }
    ~Pomponella() override
    {

    }
protected:
    string color = "pink";
    int average_height = 150;
    float some_calculation() override
    {
        return float(this->average_height) / 100;
    }
};

class Flammentanz : public  Pomponella
{
public:
    Flammentanz()
    {

    }
    Flammentanz(string name) : Pomponella(name)
    {
        this->personal_name = name;
    }

    void get_info() override
    {
        cout << "\n /// info about Plant ///\n\n class \"Pomponella\"\n plant variety \"Flammentanz\"\n plant name \"" << this->personal_name <<
             "\"\n";
        cout << " color \"" << this->color << "\"\n average plant height \"" << this->average_height << "\"\n" << " Average rate /100 :  " <<
             this->rate << "/100\"\n"  << " Average rate /10  :  " <<
             this->some_calculation() << "/10\"\n";
    }
    ~Flammentanz() override
    {

    }
protected:
    int rate = 76;
    float some_calculation() override
    {
        return float(this->rate) / 10;
    }
};

class Golden_Gate : public virtual Plant
{
public:
    Golden_Gate()
    {

    }
    Golden_Gate(string name) : Plant(name)
    {

    }

    void get_info() override
    {
        cout << "\n /// info about Plant ///\n\n class \"Golden_Gate\"\n plant name \"" << this->personal_name <<
             "\"\n";
        cout << " color \"" << this->color << "\"\n average plant height in centimeters \"" << this->average_height <<
             "\"\n average plant height in meters \"" << this->some_calculation() << "\"\n";
    }
    ~Golden_Gate() override
    {

    }
protected:
    string color = "yellow";
    int average_height = 250;
    float some_calculation() override
    {
        return float(this->average_height) / 100;
    }
};


int main()
{
    int num;



    cout << "Enter integer value: ";
    cin >> num;

    Plant** ptr_plants = new Plant * [num];

    for (int i = 0; i < num; i++) {
        int x = 3;
        string name;
        cout << "choose one \n 1 - Golden_Gate \n 2 - Pomponella \n 3 - Flammentanz" << endl;
        cin >> x;
        cout << "enter plant name" << endl;
        cin >> name;

        switch (x)
        {
            case 1:
            {
                Golden_Gate* ptr_plant = new Golden_Gate;
                Golden_Gate my_plant(name);
                *ptr_plant = my_plant;
                ptr_plants[i] = ptr_plant;
            }
            case 2:
            {
                Pomponella* ptr_plant = new Pomponella;
                Pomponella my_plant(name);
                *ptr_plant = my_plant;
                ptr_plants[i] = ptr_plant;

            }
            case 3:
            {
                Flammentanz* ptr_plant = new Flammentanz;
                Flammentanz my_plant(name);
                *ptr_plant = my_plant;
                ptr_plants[i] = ptr_plant;
            }
        }

    }
    for (int i = 0; i < num; i++)
    {
        cout << " Info about element " << i << endl;
        ptr_plants[i]->get_info();
    }
    for (int i = 0; i < num; i++)
        delete (ptr_plants[i]);
    delete[] ptr_plants;
}