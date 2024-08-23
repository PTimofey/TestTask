#include<Object.hpp>



class DataAnalyzer
{
public:
    DataAnalyzer(const std::vector<std::shared_ptr<SomeObject>> objcts, sf::Font& f) : Objects(objcts), show(false)
    {
        font=std::make_shared<sf::Font>(f);
        if (!font->loadFromFile("arial.ttf")) 
        {
            exit(1);
        }
    }  

    virtual void Grouping()=0;
    virtual void SortingGroup()=0;
    
    
    std::shared_ptr<sf::Font> font;
    std::vector<std::shared_ptr<SomeObject>> Objects;
    std::vector<sf::Text> textObjects;
    bool show;
};

class DistanceAnalyzer : public DataAnalyzer
{
public:
    using DataAnalyzer::DataAnalyzer;
    
};


class NameAnalyzer : private DataAnalyzer
{
public:
    using DataAnalyzer::DataAnalyzer;

    void Grouping()override
    {
        for(auto& i : Objects)
        {
            wchar_t FirstChar = i->NameOfObject[0];
            if (std::isdigit(FirstChar) || !std::isalpha(FirstChar)) 
            {
                groupedNames['#'].push(i);
            }
            else
            {
                groupedNames[FirstChar].push(i);
            }
        }
    }

    void SortingGroup() override
    {   
        
        int i=0;
        for(auto iter{groupedNames.begin()}; iter!=groupedNames.end(); iter++)
        {
            i++;
            while(!iter->second.empty())
            {
                i++;
                //sf::Text text(iter->second.top()->NameOfObject, *font, 24);
                //text.setFillColor(sf::Color::White);
                //text.setPosition(300, 200 + i * 30);
                //textObjects.push_back(text);
            }
        }   
    }
private:
struct CompareByName 
    {
        bool operator()(const std::shared_ptr<SomeObject> LeftObj, const std::shared_ptr<SomeObject> RightObj){return LeftObj->NameOfObject > RightObj->NameOfObject;};    
    };
    std::map<wchar_t, std::priority_queue<std::shared_ptr<SomeObject>>, CompareByName> groupedNames;

};

class TimeAnalyzer : private DataAnalyzer
{

};

class TypeAnalyzer : private DataAnalyzer
{

};

