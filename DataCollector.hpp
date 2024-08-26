#include"Object.hpp"


class DataCollector
{
public:
    

    std::shared_ptr<SomeObject> ParseLine(const std::wstring& Line)
    {
        std::shared_ptr<SomeObject> obj;
        std::size_t start = 0;
        std::size_t end = 0;
        std::vector<std::wstring> words;
        while ((end = Line.find(' ', start)) != std::wstring::npos) 
        {
                words.emplace_back(Line.data() + start, end - start);
                start = end + 1;
        }
        words.emplace_back(Line.data() + start, Line.size() - start);

        try 
        {
            obj=std::make_shared<SomeObject>(words[0], std::stod(words[1]), std::stod(words[2]), words[3], std::stod(words[4]));
        } 
        catch (const std::invalid_argument& e) 
        {
            std::cerr << "Invalid argument: " << e.what() << std::endl;
        } 
        catch (const std::out_of_range& e) 
        {
            std::cerr << "Out of range: " << e.what() << std::endl;
        }
    
        return obj;
    }
    virtual void Collect(std::vector<std::shared_ptr<SomeObject>>& )=0;

    //virtual ~DataCollector(){}
    
};


class DataCollectorFromFile : public DataCollector
{
public:
    DataCollectorFromFile(std::string& file_name) : FileName(file_name){}
    
    void Collect(std::vector<std::shared_ptr<SomeObject>>& objects) override
    {
        std::wifstream file(FileName);
        file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
        std::wstring Line;
        while (std::getline(file, Line)) {
            objects.push_back(ParseLine(Line));
        }
    }
private:
    std::string FileName;
};



class DataCollectorFromInputField : public DataCollector
{
public:
    DataCollectorFromInputField(std::wstring& str) : Line(str)
    {

    }
    void Collect(std::vector<std::shared_ptr<SomeObject>>& objects) override
    {
        objects.push_back(ParseLine(Line));
    }
private:
    std::wstring Line;
};