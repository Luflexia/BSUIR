#include <iostream>

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
    virtual void get_info()
    {
        cout << "\n /// info about Plant ///\n\n class \"Plant\"\n plant name \"" << this->personal_name << "\"";
    }
    ~Plant()
    {

    }
protected:
    string personal_name;
};

class Rose_Pomponella : public  virtual Plant
{
public:
    Rose_Pomponella()
    {

    }
    Rose_Pomponella(string name) :Plant(name)
    {

    }
    void get_info() override
    {
        cout << "\n\n /// info about Plant ///\n\n class \"Rose Pomponella\"\n plant name \"" << this->personal_name << "\"";
        cout << "\n color \"" << this->color << "\"\n height \"" << this->height << "\"\n";
    }
    ~Rose_Pomponella()
    {

    }
protected:
    string color = "hot pink";
    int height = 150;
};

class Rose_Golden_Gate : public  Rose_Pomponella
{
public:

    Rose_Golden_Gate(string name) : Rose_Pomponella(name)
    {
        this->personal_name = name;
    }
    void get_info() override
    {
        cout << "\n /// info about Plant ///\n\n class \"Rose Pomponella\"\n plant variety \"Golden_Gate\"\n plant name \"" << this->personal_name << "\"";
        cout << "\n color \"" << this->color << "\"\n height \"" << this->height << "\"\n" << " max height \"" <<
             this->height_max << "\"\n";
    }
    ~Rose_Golden_Gate()
    {

    }
protected:
    string color = "yellow";
    int height = 350;
    int height_max = 450;

};

class Rose_Flammentanz : public virtual Plant
{
public:
    Rose_Flammentanz()
    {

    }
    Rose_Flammentanz(string name) : Plant(name)
    {

    }
    void get_info() override
    {
        cout << "\n /// info about Plant ///\n\n class \"Rose Flammentanz\"\n plant name \"" << this->personal_name << "\"";
        cout << "\n color \"" << this->color << "\"\n height \"" << this->height << "\"\n";
    }
    ~Rose_Flammentanz()
    {

    }
protected:
    string color = "red";
    int height = 300;
};

class Rose_Blue_Moon : public Rose_Flammentanz
{
public:

    Rose_Blue_Moon(string name) : Rose_Flammentanz(name)
    {
        this->personal_name = name;
    }
    void get_info() override
    {
        cout << "\n /// info about Plant ///\n\n class \"Rose Flammentanz\"\n plant variety \"Critical Blue_Moon\"\n plant name \"" << this->personal_name << "\"";
        cout << "\n color \"" << this->color << "\"\n height \"" << this->height << "\"\n" << " height_max \"" <<
             this->height_max << "\"\n";
    }
    ~Rose_Blue_Moon()
    {

    }
protected:
    int height = 90;
    int height_max = 150;

};

class Rose_Polka : public Rose_Golden_Gate, public Rose_Blue_Moon
{
public:
    Rose_Polka(string name) : Rose_Golden_Gate(name), Rose_Blue_Moon(name)
    {

    }
    void get_info() override
    {
        cout << "\n /// info about Plant ///\n\n class \"Rose Pomponella\"\n plant variety \"Golden_Gate\"\n plant name \"" << this->personal_name << "\"";

        cout << endl;
        cout << "\n /// Subjective assessment ///" << endl;

        for (int i = 0; i < 3; i++)
            cout <<  i+1 << ". " << this->Subj_assessment[i] << endl;

        cout << endl;
        cout << " /// Objective assessment ///" << endl;

        for (int i = 0; i < 3; i++)
            cout <<  i+1 << ". " << this->Obj_assessment[i] << endl;

    }
    ~Rose_Polka()
    {

    }
protected:
    string Subj_assessment[3] = {"Smell - 9/10", "Beauty - 8/10", "Care - 4/10"};
    string Obj_assessment[3] = { "Smell - 6/10", "Beauty - 6/10", "Care - 7/10" };

};

int main()
{
    Plant Rose("my_Rose");
    Rose.get_info();

    Rose_Pomponella Pomponella("my_Pomponella");
    Pomponella.get_info();

    Rose_Flammentanz Flammentanz("my_Flammentanz");
    Flammentanz.get_info();

    Rose_Golden_Gate Golden_Gate("my_Golden_Gate");
    Golden_Gate.get_info();

    Rose_Blue_Moon Blue_Moon("my_Blue_Moon");
    Blue_Moon.get_info();

    Rose_Polka Polka("my_Polka");
    Polka.get_info();
}