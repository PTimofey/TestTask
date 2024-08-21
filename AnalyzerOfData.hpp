#include<Object.hpp>

class DataAnalyzer
{
public:
    virtual void grouping(std::vector<SomeObject> &objects);
};

class DistanceAnalyzer : public DataAnalyzer
{

};

class NameAnalyzer : public DataAnalyzer
{

};

class TimeAnalyzer : public DataAnalyzer
{

};

class TypeAnalyzer : public DataAnalyzer
{

};