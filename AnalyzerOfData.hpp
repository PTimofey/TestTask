#include"Includes.hpp"
#include"Object.hpp"



class GroupingStrategy 
{
public:
    virtual void group(const std::vector<std::shared_ptr<SomeObject>>& objects) = 0;
    virtual std::vector<std::wstring> getGroupedStrings() const
    {
        std::vector<std::wstring> result;
        std::locale::global(std::locale(""));
        std::wofstream inputInFile("OutputFile");
        for (const auto& group : groups) 
        {
            result.push_back(group.first);
            inputInFile<<result.back()<<"\n";
            for (const auto& obj : group.second) 
            {
                result.push_back(obj->getString());
                inputInFile<<result.back();
            }
        }
        inputInFile.close();
        return result;
    }
    virtual ~GroupingStrategy() = default;
    
protected:
    std::map<std::wstring, std::vector<std::shared_ptr<SomeObject>>> groups;
    
    
};

class DistanceGroupingStrategy : public GroupingStrategy 
{
public:
    
    void group(const std::vector<std::shared_ptr<SomeObject>>& objects) override 
    {
        for (const auto& obj : objects) 
        {
            double distance = obj->getDistance();
            std::wstring groupKey;

            if (distance <= 100) 
            {
                groupKey = L"До 100 ед.";
            } 
            else if (distance <= 1000) 
            {
                groupKey = L"До 1000 ед.";
            } 
            else if (distance <= 10000) 
            {
                groupKey = L"До 10000 ед.";
            } 
            else 
            {
                groupKey = L"Слишком далеко";
            }

            groups[groupKey].push_back(obj);
        }

        // Сортируем внутри каждой группы по возрастанию дистанции
        for (auto& group : groups) {
            std::sort(group.second.begin(), group.second.end(), [](const std::shared_ptr<SomeObject>& a, const std::shared_ptr<SomeObject>& b) 
            {
                return a->getDistance() < b->getDistance();
            });
        }
    }

    std::vector<std::wstring> getGroupedStrings() const override 
    {
        std::vector<std::wstring> result;
        std::locale::global(std::locale(""));
        std::wofstream inputInFile("OutputFile");

        // Заполнение результирующего вектора
        for (const auto& group : groups) 
        {
            result.push_back(group.first);
            inputInFile<<result.back()<<'\n';
            for (const auto& obj : group.second) 
            {
                result.push_back(obj->getString());
                inputInFile<<result.back();
            }
        }
        inputInFile.close();
        return result;
    }
};

class NameGroupingStrategy : public GroupingStrategy {
public:
    void group(const std::vector<std::shared_ptr<SomeObject>>& objects) override 
    {
        for (const auto& obj : objects) 
        {
            wchar_t firstChar =obj->NameOfObject[0];

            // Определяем группу по первому символу
            std::wstring groupKey;
            if (iswdigit(firstChar) || iswpunct(firstChar) || iswalpha(firstChar) || iswlower(firstChar)) 
            {
                groupKey = L"#";
            } 
            else 
            {   
                groupKey = std::wstring(1, firstChar);
            }

            // Вставляем объект в соответствующую группу
            groups[groupKey].push_back(obj);
        }

        // Сортируем группы сразу после заполнения
        for (auto& group : groups) 
        {
            std::sort(group.second.begin(), group.second.end(), [](const std::shared_ptr<SomeObject>& a, const std::shared_ptr<SomeObject>& b) 
            {
                return a->NameOfObject < b->NameOfObject;
            });
        }
    }

    
};


// Grouping by creation time
class TimeGroupingStrategy : public GroupingStrategy {
public:
    void group(const std::vector<std::shared_ptr<SomeObject>>& objects) override {
        std::time_t now = std::time(0);
        std::tm now_tm = *std::localtime(&now);

        for (const auto& obj : objects) {
            
            // Converting double to time_t
            std::time_t obj_time = static_cast<std::time_t>(obj->TimeOfCreation);
            std::tm obj_tm = *std::localtime(&obj_time);

            

            // Grouping by time
            if (obj_tm.tm_year == now_tm.tm_year && obj_tm.tm_yday == now_tm.tm_yday) 
            {
                std::cout<<now_tm.tm_year<<"\n";
                groups[L"Сегодня"].push_back(obj);
            } 
            else if (obj_tm.tm_year == now_tm.tm_year && obj_tm.tm_yday == now_tm.tm_yday - 1) 
            {
                groups[L"Вчера"].push_back(obj);
            } 
            else if (obj_tm.tm_year == now_tm.tm_year && (now_tm.tm_yday - obj_tm.tm_yday) < 7) 
            {
                groups[L"На этой неделе"].push_back(obj);
            } 
            else if (obj_tm.tm_year == now_tm.tm_year && obj_tm.tm_mon == now_tm.tm_mon) 
            {
                groups[L"В этом месяце"].push_back(obj);
            } 
            else if (obj_tm.tm_year == now_tm.tm_year) 
            {
                groups[L"В этом году"].push_back(obj);
            } 
            else {
                groups[L"Ранее"].push_back(obj);
            }
        }

        // Sorting within each group by creation time
        for (auto& group : groups) 
        {
            std::sort(group.second.begin(), group.second.end(), [](const std::shared_ptr<SomeObject>& a, const std::shared_ptr<SomeObject>& b) 
            {
                return a->TimeOfCreation < b->TimeOfCreation;
            });
        }
    }

    
};



class TypeGroupingStrategy : public GroupingStrategy 
{
public:
    TypeGroupingStrategy(){typeThreshold=2;}

    void group(const std::vector<std::shared_ptr<SomeObject>>& objects) override 
    {
        std::map<std::wstring, std::vector<std::shared_ptr<SomeObject>>> typeGroups;

        for (const auto& obj : objects) 
        {
            typeGroups[obj->TypeOfObject].push_back(obj);
        }

        for (auto& typeGroup : typeGroups) {
            if (typeGroup.second.size() > typeThreshold) 
            {
                groups[typeGroup.first] = typeGroup.second;
            } 
            else 
            {
                groups[L"Разное"].insert(groups[L"Разное"].end(), typeGroup.second.begin(), typeGroup.second.end());
            }
        }

        // Сортировка внутри каждой группы по имени
        for (auto& group : groups) 
        {
            std::sort(group.second.begin(), group.second.end(), [](const std::shared_ptr<SomeObject>& a, const std::shared_ptr<SomeObject>& b) {return a->NameOfObject < b->NameOfObject;});
        }
    }

    

private:
    int typeThreshold;
};
