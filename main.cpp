#include"DataCollector.hpp"

int main()
{
    std::vector<std::shared_ptr<SomeObject>> Objects;

    DataCollector data;
    Objects=data.CollectFromFile("TestFile");
    
    
}