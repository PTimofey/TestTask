#include "Includes.hpp"

class SomeObject
{
public:
    SomeObject(std::string name, double X, double Y, std::string type, long double time) : NameOfObject(name), CoordX(X), CoordY(Y), TypeOfObject(type), TimeOfCreation(time)
    {}
    
    std::string NameOfObject;
    double CoordX;
    double CoordY;
    std::string TypeOfObject;
    double TimeOfCreation;  
};