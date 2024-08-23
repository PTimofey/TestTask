#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<sstream>
#include <iomanip>
#include <ctime>
#include<memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include<map>
#include<queue>
#include<functional>
#include<stack>
#include <locale>
#include <codecvt>


std::wstring convertTimestampToWString(double timestamp) {
    // Разделение на целую и дробную части
    std::time_t seconds = static_cast<std::time_t>(timestamp);
    double fractional_part = timestamp - static_cast<double>(seconds);

    // Конвертация целой части в структуру tm
    std::tm* tm_ptr = std::gmtime(&seconds); // Конвертируем в UTC время

    // Форматированный вывод даты и времени
    std::wostringstream woss;
    woss << std::put_time(tm_ptr, L"%Y-%m-%d %H:%M:%S");

    // Добавление дробной части секунд с точностью до 4 знаков
    woss << L"." << std::fixed << std::setprecision(4) << fractional_part * 10000;

    return woss.str();
}


std::string wstringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(wstr);
}



class SomeObject
{
public:
    SomeObject(std::wstring name, double X, double Y, std::wstring type, long double time) : NameOfObject(name), CoordX(X), CoordY(Y), TypeOfObject(type), TimeOfCreation(time)
    {}
    
    std::wstring NameOfObject;
    double CoordX;
    double CoordY;
    std::wstring TypeOfObject;
    double TimeOfCreation;

    const std::wstring getString()
    {
        std::wstringstream s;
        s<<NameOfObject<<" "<<CoordX<<" "<<CoordY<<" "<<TypeOfObject<<" "<<convertTimestampToWString(TimeOfCreation)<<"\n";
        return s.str();
    }
    


};