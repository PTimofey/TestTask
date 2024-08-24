#include"Includes.hpp"
#include"DataCollector.hpp"
#include"AnalyzerOfData.hpp"






class TextField {
private:
    sf::Text text;
    std::wstring input;  // Используем wstring для поддержки Unicode
    

public:
    TextField(sf::Font& font, int x, int y, int fontSize, sf::Color color) {
        text.setFont(font);
        text.setCharacterSize(fontSize);
        text.setFillColor(color);
        text.setPosition(x, y);
    }

    void handleInput(sf::Event& event) {
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b' && !input.empty()) {
                input.pop_back();  // Удаление символа при нажатии Backspace
            } else if (event.text.unicode < 128 || (event.text.unicode >= 1024 && event.text.unicode <= 1103)) {
                input += static_cast<wchar_t>(event.text.unicode);  // Добавление символов в строку
            }
            text.setString(input);
        }
    }

    void setInput(const std::wstring& str) {
        input = str;
        text.setString(input);
    }

    const std::wstring& getInput() const {
        return input;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(text);
    }
};

class Button {
private:
    sf::Text text;
    sf::RectangleShape buttonShape;

public:
    Button(const std::wstring& label, sf::Font& font, int x, int y, int width, int height, sf::Color textColor, sf::Color buttonColor) {
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(30);
        text.setFillColor(textColor);
        text.setPosition(x + 10, y + 10);

        buttonShape.setSize(sf::Vector2f(width, height));
        buttonShape.setFillColor(buttonColor);
        buttonShape.setPosition(x, y);
    }

    bool isClicked(sf::Vector2i mousePos) {
        return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(buttonShape);
        window.draw(text);
    }
};


// Класс для основного окна приложения
class Application {
private:
    sf::RenderWindow window;
    sf::Font font;
    std::vector<std::shared_ptr<SomeObject>> Objects;  // Используем вектор для хранения списка SomeObject
    std::vector<std::wstring> GroupedByName;
    std::vector<std::wstring> GroupedByDist;
    std::vector<std::wstring> GroupedByTime;
    std::vector<std::wstring> GroupedByType;
    std::shared_ptr<DataCollector> collector;
    

    // Основные элементы интерфейса
    Button btnManualInput;
    Button btnFileInput;
    Button btnShowObjects;
    Button btnAddObject;
    Button btnAddFromFile;
    Button btnBack;

    Button btnGroupDistance;
    Button btnGroupName;
    Button btnGroupTime;
    Button btnGroupType;

    TextField textField;
    bool showMainButtons;
    bool showObjectInput;
    bool showFileInput;
    bool showObjectList;
    bool showGroupName;
    bool showGroupDist;
    bool showGroupTime;
    bool showGroupType;
    
public:
    Application() : window(sf::VideoMode(1920, 1280), "Программа"),
                    btnGroupType(L"Группировать по типу", font, 300, 700, 400, 50, sf::Color::Black, sf::Color::Green),
                    btnGroupTime(L"Группировать по времени", font, 300, 600, 400, 50, sf::Color::Black, sf::Color::Green),
                    btnGroupDistance(L"Группировать по дистанции", font, 300, 500, 400, 50, sf::Color::Black, sf::Color::Green),
                    btnGroupName(L"Группировать по имени", font, 300, 400, 400, 50, sf::Color::Black, sf::Color::Green),
                    btnManualInput(L"Ввести объект вручную", font, 300, 100, 400, 50, sf::Color::Black, sf::Color::Green),
                    btnFileInput(L"Добавить из файла", font, 300, 200, 400, 50, sf::Color::Black, sf::Color::Green),
                    btnShowObjects(L"Показать список", font, 300, 300, 400, 50, sf::Color::Black, sf::Color::Green),
                    btnAddObject(L"Добавить", font, 1480, 260, 400, 50, sf::Color::Black, sf::Color::Green),
                    btnAddFromFile(L"Добавить", font, 1480, 260, 400, 50, sf::Color::Black, sf::Color::Green),
                    btnBack(L"Назад", font, 1480, 360, 400, 50, sf::Color::Black, sf::Color::Red),
                    textField(font, 300, 200, 30, sf::Color::Black),
                    showMainButtons(true), showObjectInput(false), showFileInput(false), showObjectList(false), showGroupName(false), showGroupDist(false), showGroupTime(false), showGroupType(false) 
                    {

                        if (!font.loadFromFile("arial.ttf")) {
                            std::cerr << "Ошибка загрузки шрифта" << std::endl;
                            exit(1);
                        }
                    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            render();
        }
    }

private:
    void processEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (showObjectInput || showFileInput) {
                textField.handleInput(event);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                handleMouseClick(sf::Mouse::getPosition(window));
            }
        }
    }

    void handleMouseClick(sf::Vector2i mousePos) 
    {
        if (showMainButtons) 
        {
            
            if (btnManualInput.isClicked(mousePos)) 
            {
                showMainButtons = false;
                showObjectInput = true;
                textField.setInput(L"");
            }
            else if(btnGroupType.isClicked(mousePos))
            {
                showMainButtons = false;
                showGroupType=true;
            }
            else if(btnGroupTime.isClicked(mousePos))
            {
                showMainButtons = false;
                showGroupTime=true;
            }
            else if(btnGroupDistance.isClicked(mousePos))
            {
                showMainButtons = false;
                showGroupDist=true;
            }
            else if(btnGroupName.isClicked(mousePos))
            {
                showMainButtons = false;
                showGroupName = true;
            }
            else if (btnFileInput.isClicked(mousePos)) 
            {
                showMainButtons = false;
                showFileInput = true;
                textField.setInput(L"");
            } 
            else if (btnShowObjects.isClicked(mousePos)) 
            {
                showMainButtons = false;
                showObjectList = true;
            }
        } 
        else if (showObjectInput) 
        {
            if (btnAddObject.isClicked(mousePos)) 
            {
                std::wstring Line = textField.getInput();
                collector = std::make_shared<DataCollectorFromInputField>(Line);
                collector->Collect(Objects);
                textField.setInput(L"");
            } 
            else if (btnBack.isClicked(mousePos)) 
            {
                showObjectInput = false;
                showMainButtons = true;
            }
        } 
        else if (showFileInput) 
        {
            if (btnAddFromFile.isClicked(mousePos)) 
            {
                std::string filename = wstringToString(textField.getInput());
                collector = std::make_shared<DataCollectorFromFile>(filename);
                collector->Collect(Objects);
                textField.setInput(L"");
            } 
            else if (btnBack.isClicked(mousePos)) 
            {
                showFileInput = false;
                showMainButtons = true;
            }
        } 
        else if (showObjectList) 
        {
            if (btnBack.isClicked(mousePos)) 
            {
                showObjectList = false;
                showMainButtons = true;
            }
        }
        else if (showGroupName)
        {
            NameGroupingStrategy NameGroup;
            NameGroup.group(Objects);
            GroupedByName=NameGroup.getGroupedStrings();
            if (btnBack.isClicked(mousePos)) 
            {
                showGroupName = false;
                showMainButtons = true;
            }
        }
        else if (showGroupDist)
        {
            DistanceGroupingStrategy DistGroup;
            DistGroup.group(Objects);
            GroupedByDist=DistGroup.getGroupedStrings();
            
            if (btnBack.isClicked(mousePos)) 
            {
                showGroupDist = false;
                showMainButtons = true;
            }
        }
        else if (showGroupTime)
        {
            TimeGroupingStrategy TimeGroup;
            TimeGroup.group(Objects);
            GroupedByTime=TimeGroup.getGroupedStrings();
            
            if (btnBack.isClicked(mousePos)) 
            {
                showGroupTime = false;
                showMainButtons = true;
            }
        }

        else if (showGroupType)
        {
            TypeGroupingStrategy TypeGroup(2);
            TypeGroup.group(Objects);
            GroupedByType=TypeGroup.getGroupedStrings();
            
            if (btnBack.isClicked(mousePos)) 
            {
                showGroupTime = false;
                showMainButtons = true;
            }
        }
        
    }

    void render() 
    {
        window.clear(sf::Color::White);

        if (showMainButtons) 
        {
            btnGroupType.draw(window);
            btnGroupTime.draw(window);
            btnGroupDistance.draw(window);
            btnGroupName.draw(window);
            btnManualInput.draw(window);
            btnFileInput.draw(window);
            btnShowObjects.draw(window);
        } 
        else if (showObjectInput) 
        {
            textField.draw(window);
            btnAddObject.draw(window);
            btnBack.draw(window);
        } 
        else if (showFileInput) 
        {
            textField.draw(window);
            btnAddFromFile.draw(window);
            btnBack.draw(window);
        } 
        else if (showObjectList) 
        {
            int y = 100;
            for (const auto& obj : Objects) 
            {
                sf::Text ObjText;
                ObjText.setFont(font);
                ObjText.setString(obj->getString());
                ObjText.setCharacterSize(30);
                ObjText.setFillColor(sf::Color::Black);
                ObjText.setPosition(300, y);
                window.draw(ObjText);
                y += 40;
            }
            btnBack.draw(window);
        }
        else if(showGroupName)
        {
            int y = 100;
            
            for (const auto& str : GroupedByName) 
            {
                sf::Text ObjText;
                ObjText.setFont(font);
                ObjText.setString(str);
                ObjText.setCharacterSize(18);
                ObjText.setFillColor(sf::Color::Black);
                ObjText.setPosition(300, y);
                window.draw(ObjText);
                y += 20;
            }
            btnBack.draw(window);
        }
        else if(showGroupDist)
        {
            int y = 100;
            
            for (const auto& str : GroupedByDist) 
            {
                sf::Text ObjText;
                ObjText.setFont(font);
                ObjText.setString(str);
                ObjText.setCharacterSize(18);
                ObjText.setFillColor(sf::Color::Black);
                ObjText.setPosition(300, y);
                window.draw(ObjText);
                y += 20;
            }
            btnBack.draw(window);
        }

        else if(showGroupTime)
        {
            int y = 100;
            
            for (const auto& str : GroupedByTime) 
            {
                sf::Text ObjText;
                ObjText.setFont(font);
                ObjText.setString(str);
                ObjText.setCharacterSize(18);
                ObjText.setFillColor(sf::Color::Black);
                ObjText.setPosition(300, y);
                window.draw(ObjText);
                y += 20;
            }
            btnBack.draw(window);
        }

        else if(showGroupType)
        {
            int y = 100;
            
            for (const auto& str : GroupedByType) 
            {
                sf::Text ObjText;
                ObjText.setFont(font);
                ObjText.setString(str);
                ObjText.setCharacterSize(18);
                ObjText.setFillColor(sf::Color::Black);
                ObjText.setPosition(300, y);
                window.draw(ObjText);
                y += 20;
            }
            btnBack.draw(window);
        }


        window.display();
    }
};
