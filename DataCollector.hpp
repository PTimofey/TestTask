#include"Object.hpp"

class DataCollector
{
public:
    DataCollector& operator=(const DataCollector&)=delete;
    
    
    std::vector<std::shared_ptr<SomeObject>> CollectFromFile(std::string FileName)
    {
        std::vector<std::shared_ptr<SomeObject>> objects;
        std::ifstream file(FileName);
        std::string Line;
        std::string word;
        std::size_t LineNumber=0;

        while(std::getline(file,Line))
        {
            LineNumber++;
            std::vector<std::string> words;
            std::size_t start = 0;
            std::size_t end = 0;
            while ((end = Line.find(' ', start)) != std::string::npos) 
            {
                words.emplace_back(Line.data() + start, end - start);
                start = end + 1;
            }
            words.emplace_back(Line.data() + start, Line.size() - start);

            try 
            {
                std::shared_ptr<SomeObject> obj=std::make_shared<SomeObject>(words[0], std::stod(words[1]), std::stod(words[2]), words[3], std::stod(words[4]));
                objects.push_back(obj);
                
            } 
            catch (const std::invalid_argument& e) 
            {
                std::cerr << "Invalid argument: " << e.what() << "\nLine Number: "<<LineNumber << std::endl;
            } 
            catch (const std::out_of_range& e) 
            {
                std::cerr << "Out of range: " << e.what()<< "\nLine Number: "<<LineNumber << std::endl;
            }
        }

        return objects;
    }  
};