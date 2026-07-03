#include <iostream>
 
using namespace std;
 
class Detail
{
public:
  friend class Control;
  Detail(int DetailId, string DetailName, double DetailMass)
  {
    id = DetailId;
    name = DetailName;
    mass = DetailMass;
  }
  void get_mass()
  {
    cout << mass << endl;
  }
private:
  int id;
  double mass;
  string name;
};

 
class Control
{
public:
 
  void setMass(Detail& some, double weight)
  {
    some.mass = weight;
  }
 
};
 
 
 
int main()
{
  Detail cog(1, "cog", 2.3);
 
  Control panel;
  cog.get_mass();
 
  panel.setMass(cog, 3.2);
  cog.get_mass();
 
 
 
}