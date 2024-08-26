#include "Includes.hpp"
#include "DataCollector.hpp"
#include "AnalyzerOfData.hpp"

// Determine the possible states of the applicatio
enum class ApplicationState {
    MainMenu,
    ManualInput,
    FileInput,
    ShowObjects,
    GroupByName,
    GroupByDistance,
    GroupByTime,
    GroupByType
};

// A class for working with text fields
class TextField {
private:
    sf::Text text;
    std::wstring input;

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
                input.pop_back();
            } else if (event.text.unicode < 128 || (event.text.unicode >= 1024 && event.text.unicode <= 1103)) {
                input += static_cast<wchar_t>(event.text.unicode);
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

// A class for working with buttons
class Button {
private:
    sf::Text text;
    sf::RectangleShape buttonShape;

public:
    Button(){}
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

    // checking for button clicks
    bool isClicked(sf::Vector2i mousePos) {
        return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos));
    }
    // drawing a button
    void draw(sf::RenderWindow& window) {
        window.draw(buttonShape);
        window.draw(text);
    }
};

// A class for the main application window
class Application {
private:
    sf::RenderWindow window;
    sf::Font font;
    std::vector<std::shared_ptr<SomeObject>> objects;
    std::shared_ptr<DataCollector> collector;
    std::vector<std::wstring> StringObjects;
    ApplicationState currentState;

    // The main elements of the interface
    std::map<std::string, Button> buttons;
    TextField textField;

public:
    Application() : window(sf::VideoMode(1920, 1280), "Программа"),
                    textField(font, 300, 200, 30, sf::Color::Black),
                    currentState(ApplicationState::MainMenu) {

        if (!font.loadFromFile("arial.ttf")) {
            std::cerr << "Ошибка загрузки шрифта" << std::endl;
            exit(1);
        }

        // Initializing buttons
        buttons["ManualInput"] = Button(L"Ввести объект вручную", font, 300, 100, 400, 50, sf::Color::Black, sf::Color::Green);
        buttons["FileInput"] = Button(L"Добавить из файла", font, 300, 200, 400, 50, sf::Color::Black, sf::Color::Green);
        buttons["ShowObjects"] = Button(L"Показать список", font, 300, 300, 400, 50, sf::Color::Black, sf::Color::Green);
        buttons["AddObject"] = Button(L"Добавить", font, 1480, 260, 400, 50, sf::Color::Black, sf::Color::Green);
        buttons["AddFromFile"] = Button(L"Добавить", font, 1480, 260, 400, 50, sf::Color::Black, sf::Color::Green);
        buttons["Back"] = Button(L"Назад", font, 1480, 360, 400, 50, sf::Color::Black, sf::Color::Red);
        buttons["GroupByName"] = Button(L"Группировать по имени", font, 300, 400, 400, 50, sf::Color::Black, sf::Color::Green);
        buttons["GroupByDistance"] = Button(L"Группировать по дистанции", font, 300, 500, 400, 50, sf::Color::Black, sf::Color::Green);
        buttons["GroupByTime"] = Button(L"Группировать по времени", font, 300, 600, 400, 50, sf::Color::Black, sf::Color::Green);
        buttons["GroupByType"] = Button(L"Группировать по типу", font, 300, 700, 400, 50, sf::Color::Black, sf::Color::Green);
    }
    // Main loop
    void run() {
        while (window.isOpen()) {
            render();
            processEvents();
            
        }
    }

private:
    // the event processing process
    void processEvents() 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (currentState == ApplicationState::ManualInput || currentState == ApplicationState::FileInput) {
                textField.handleInput(event);
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                handleMouseClick(sf::Mouse::getPosition(window));
            }
        }
    }

    // Handling mouse clicks
    void handleMouseClick(sf::Vector2i mousePos) {
        switch (currentState) 
        {
            case ApplicationState::MainMenu:
                handleMainMenuClick(mousePos);
                break;
            case ApplicationState::ManualInput:
                handleManualInputClick(mousePos);
                break;
            case ApplicationState::FileInput:
                handleFileInputClick(mousePos);
                break;
            case ApplicationState::ShowObjects:
                handleShowObjectsClick(mousePos);
                break;
            case ApplicationState::GroupByName:
                handleGroupClick<NameGroupingStrategy>(mousePos);
                break;
            case ApplicationState::GroupByDistance:
                handleGroupClick<DistanceGroupingStrategy>(mousePos);
                break;
            case ApplicationState::GroupByTime:
                handleGroupClick<TimeGroupingStrategy>(mousePos);
                break;
            case ApplicationState::GroupByType:
                handleGroupClick<TypeGroupingStrategy>(mousePos);
                break;
        }
    }

    // Handling buttons in the main menu
    void handleMainMenuClick(sf::Vector2i mousePos) 
    {
        if (buttons["ManualInput"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::ManualInput;
            textField.setInput(L"");
        } 
        else if (buttons["FileInput"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::FileInput;
            textField.setInput(L"");
        } 
        else if (buttons["ShowObjects"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::ShowObjects;
        } 
        else if (buttons["GroupByName"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::GroupByName;
        } 
        else if (buttons["GroupByDistance"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::GroupByDistance;
        } 
        else if (buttons["GroupByTime"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::GroupByTime;
        } 
        else if (buttons["GroupByType"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::GroupByType;
        }
    }

    // Handling manual input
    void handleManualInputClick(sf::Vector2i mousePos) 
    {
        if (buttons["AddObject"].isClicked(mousePos)) 
        {
            std::wstring line = textField.getInput();
            collector = std::make_shared<DataCollectorFromInputField>(line);
            collector->Collect(objects);
            textField.setInput(L"");
        } 
        else if (buttons["Back"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::MainMenu;
        }
    }
    
    //Handling input from file
    void handleFileInputClick(sf::Vector2i mousePos) 
    {
        if (buttons["AddFromFile"].isClicked(mousePos)) 
        {
            std::string filename = wstringToString(textField.getInput());
            collector = std::make_shared<DataCollectorFromFile>(filename);
            collector->Collect(objects);
            textField.setInput(L"");
        } 
        else if (buttons["Back"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::MainMenu;
        }
    }

    // Show Objects
    void handleShowObjectsClick(sf::Vector2i mousePos) 
    {

        if (buttons["Back"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::MainMenu;
        }
    }

    // Handling of various groupings
    template <typename GroupingStrategy>
    void handleGroupClick(sf::Vector2i mousePos) 
    {
        StringObjects.clear();
        GroupingStrategy strategy;
        strategy.group(objects);
        StringObjects=strategy.getGroupedStrings();
        
        if (buttons["Back"].isClicked(mousePos)) 
        {
            currentState = ApplicationState::MainMenu;
        }
    }


    void render() 
    {
        window.clear(sf::Color::White);

        switch (currentState) {
            case ApplicationState::MainMenu:
                renderMainMenu();
                break;
            case ApplicationState::ManualInput:
                renderManualInput();
                break;
            case ApplicationState::FileInput:
                renderFileInput();
                break;
            case ApplicationState::ShowObjects:
                renderShowObjects();
                break;
            case ApplicationState::GroupByName:
            case ApplicationState::GroupByDistance:
            case ApplicationState::GroupByTime:
            case ApplicationState::GroupByType:
                renderGroupedObjects();
                break;
        }

        window.display();
    }

    
    void renderMainMenu() {
        buttons["ManualInput"].draw(window);
        buttons["FileInput"].draw(window);
        buttons["ShowObjects"].draw(window);
        buttons["GroupByName"].draw(window);
        buttons["GroupByDistance"].draw(window);
        buttons["GroupByTime"].draw(window);
        buttons["GroupByType"].draw(window);
    }

    void renderManualInput() {
        textField.draw(window);
        buttons["AddObject"].draw(window);
        buttons["Back"].draw(window);
    }

    void renderFileInput() {
        textField.draw(window);
        buttons["AddFromFile"].draw(window);
        buttons["Back"].draw(window);
    }

    void renderShowObjects() {
        int y = 100;
        for (const auto& obj : objects) {
            sf::Text objText;
            objText.setFont(font);
            objText.setString(obj->getString());
            objText.setCharacterSize(30);
            objText.setFillColor(sf::Color::Black);
            objText.setPosition(300, y);
            window.draw(objText);
            y += 40;
        }
        buttons["Back"].draw(window);
    }
    
    void renderGroupedObjects() 
    {
        buttons["Back"].draw(window);
        int y = 100;
        for (const auto& str : StringObjects) {
            sf::Text objText;
            objText.setFont(font);
            objText.setString(str);
            objText.setCharacterSize(18);
            objText.setFillColor(sf::Color::Black);
            objText.setPosition(300, y);
            window.draw(objText);
            y += 20;
        }
        sf::Text alert;
        alert.setFont(font);
        alert.setString(L"Нажмите на мышку)");
        alert.setCharacterSize(30);
        alert.setFillColor(sf::Color::Black);
        alert.setPosition(1480, 260);
        window.draw(alert);
    }
};
