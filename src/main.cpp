#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Heap.cpp"

int main() {
    // Resources
    sf::Font buttonfont;
    sf::Font welcomefont;
    sf::Font listfont;
    sf::Texture backgroundTexture;
    backgroundTexture.setRepeated(true);
    sf::Image icon;
    sf::Sprite backgroundSprite1(backgroundTexture);
    sf::Sprite backgroundSprite2(backgroundTexture);
    sf::Music music;
    sf::RectangleShape easyButton;
    sf::RectangleShape intermediateButton;
    sf::RectangleShape hardButton;
    sf::RectangleShape backButton;
    sf::RectangleShape maxHeap;
    sf::RectangleShape minHeap;
    sf::Text easyText("Easy", buttonfont, 55);
    sf::Text intermediateText("Intermediate", buttonfont, 55);
    sf::Text hardText("Hard", buttonfont, 55);
    sf::Text backText("Back", buttonfont, 24);
    sf::Text welcomeText("Welcome to MidiFi!", welcomefont, 135);
    sf::Text explanationText("Please select which Midi", welcomefont, 75);
    sf::Text explanatitonText2("difficulty you would like to look at", welcomefont, 70);
    sf::Text maxHeapText("Highest to Lowest (Max Heap)", buttonfont, 50);
    sf::Text minHeapText("Lowest to Highest (Min Heap)", buttonfont, 50);
    if (!music.openFromFile("../resources/snowflake-relaxing-piano-music-269243.ogg")) {
        std::cout << "Error opening music file" << std::endl;
        return 1;
    }
    if (!backgroundTexture.loadFromFile("../resources/Texture_512x512_39.png")) {
        std::cerr << "Failed to load background texture" << std::endl;
        return 1;
    }
    if (!buttonfont.loadFromFile("../resources/KGMechanicallyMechanical.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }
    if (!welcomefont.loadFromFile("../resources/Ketchup Manis Demo.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }
    if (!icon.loadFromFile("../resources/1175795.png")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    // Window settings
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "MidiFi");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Background panning variable initialization
    sf::Vector2f backgroundPosition(0, 0);
    sf::Vector2f nextBackgroundPosition(static_cast<float>(backgroundTexture.getSize().x), 0);
    float panSpeed = 50.0f;

    // Generates Heap and Midi files
    std::string midiDir = "../resources/DATABASE";
    MidiCollection midiCollection(midiDir);
    Heap heap(midiDir);

    heap.insertIntoHeap("Easy");
    heap.buildMinHeap();
    std::string EasyMinHeap = heap.getAllSongInfo();
    heap.clearMinHeap();

    heap.insertIntoHeap("Easy");
    heap.buildMaxHeap();
    std::string EasyMaxHeap = heap.getAllSongInfo();
    heap.clearMaxHeap();

    heap.insertIntoHeap("Intermediate");
    heap.buildMinHeap();
    std::string IntermediateMinHeap = heap.getAllSongInfo();
    heap.clearMinHeap();

    heap.insertIntoHeap("Intermediate");
    heap.buildMaxHeap();
    std::string IntermediateMaxHeap = heap.getAllSongInfo();
    heap.clearMaxHeap();

    heap.insertIntoHeap("Hard");
    heap.buildMinHeap();
    std::string HardMinHeap = heap.getAllSongInfo();
    heap.clearMinHeap();

    heap.insertIntoHeap("Hard");
    heap.buildMaxHeap();
    std::string HardMaxHeap = heap.getAllSongInfo();
    heap.clearMaxHeap();

    // States
    enum class State { Menu, Easy, Intermediate, Hard, EasyMaxHeap, EasyMinHeap, IntermediateMinHeap, IntermediateMaxHeap, HardMaxHeap, HardMinHeap };
    State currentState = State::Menu;

    // Window dimensions
    const float WINDOW_WIDTH = window.getSize().x;
    const float WINDOW_HEIGHT = window.getSize().y;

    // Button dimensions (as percentages of window size)
    const float BUTTON_WIDTH = WINDOW_WIDTH * 0.16f;
    const float BUTTON_HEIGHT = WINDOW_HEIGHT * 0.12f;
    const float INTBUTTON_WIDTH = WINDOW_WIDTH * 0.25f;

    // Calculate positions for buttons
    float windowCenterX = WINDOW_WIDTH / 2.0f;
    float buttonY = WINDOW_HEIGHT * 0.6f;
    float buttonSpacing = WINDOW_WIDTH * 0.15f;

    // Set button positions
    easyButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    intermediateButton.setSize(sf::Vector2f(INTBUTTON_WIDTH, BUTTON_HEIGHT));
    hardButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    maxHeap.setSize(sf::Vector2f(WINDOW_WIDTH * 0.52f, WINDOW_HEIGHT * 0.09f));
    minHeap.setSize(sf::Vector2f(WINDOW_WIDTH * 0.52f, WINDOW_HEIGHT * 0.09f));
    backButton.setSize(sf::Vector2f(WINDOW_WIDTH * 0.05f, WINDOW_HEIGHT * 0.05f));

    easyButton.setPosition(windowCenterX - buttonSpacing - BUTTON_WIDTH, buttonY);
    intermediateButton.setPosition(windowCenterX - INTBUTTON_WIDTH / 2, buttonY);
    hardButton.setPosition(windowCenterX + buttonSpacing, buttonY);
    maxHeap.setPosition(WINDOW_WIDTH * 0.24f, WINDOW_HEIGHT * 0.37f);
    minHeap.setPosition(WINDOW_WIDTH * 0.24f, WINDOW_HEIGHT * 0.51f);
    backButton.setPosition(WINDOW_WIDTH * 0.01f, WINDOW_HEIGHT * 0.02f);

    // Set text sizes relative to window height
    easyText.setCharacterSize(static_cast<unsigned int>(WINDOW_HEIGHT * 0.05f));
    intermediateText.setCharacterSize(static_cast<unsigned int>(WINDOW_HEIGHT * 0.05f));
    hardText.setCharacterSize(static_cast<unsigned int>(WINDOW_HEIGHT * 0.05f));
    backText.setCharacterSize(static_cast<unsigned int>(WINDOW_HEIGHT * 0.02f));
    welcomeText.setCharacterSize(static_cast<unsigned int>(WINDOW_HEIGHT * 0.125f));
    explanationText.setCharacterSize(static_cast<unsigned int>(WINDOW_HEIGHT * 0.07f));
    explanatitonText2.setCharacterSize(static_cast<unsigned int>(WINDOW_HEIGHT * 0.065f));
    maxHeapText.setCharacterSize(static_cast<unsigned int>(WINDOW_HEIGHT * 0.046f));
    minHeapText.setCharacterSize(static_cast<unsigned int>(WINDOW_HEIGHT * 0.046f));

    // Set text positions
    sf::Text songText;
    songText.setPosition(
        window.getSize().x / 2.0f - songText.getLocalBounds().width / 2.0f,
        window.getSize().y / 2.0f - songText.getLocalBounds().height / 2.0f
    );

    // Set text positions
    easyText.setPosition(
        easyButton.getPosition().x + easyButton.getSize().x / 2 - easyText.getLocalBounds().width / 2,
        easyButton.getPosition().y + easyButton.getSize().y / 2 - easyText.getLocalBounds().height / 2
    );
    intermediateText.setPosition(
        intermediateButton.getPosition().x + intermediateButton.getSize().x / 2 - intermediateText.getLocalBounds().width / 2,
        intermediateButton.getPosition().y + intermediateButton.getSize().y / 2 - intermediateText.getLocalBounds().height / 2
    );
    hardText.setPosition(
        hardButton.getPosition().x + hardButton.getSize().x / 2 - hardText.getLocalBounds().width / 2,
        hardButton.getPosition().y + hardButton.getSize().y / 2 - hardText.getLocalBounds().height / 2
    );
    backText.setPosition(
        backButton.getPosition().x + backButton.getSize().x / 2 - backText.getLocalBounds().width / 2,
        backButton.getPosition().y + backButton.getSize().y / 2 - backText.getLocalBounds().height / 2
    );
    welcomeText.setPosition(WINDOW_WIDTH * 0.19f, WINDOW_HEIGHT * 0.09f);
    explanationText.setPosition(WINDOW_WIDTH * 0.26f, WINDOW_HEIGHT * 0.32f);
    explanatitonText2.setPosition(WINDOW_WIDTH * 0.20f, WINDOW_HEIGHT * 0.42f);
    maxHeapText.setPosition(
        maxHeap.getPosition().x + maxHeap.getSize().x / 2 - maxHeapText.getLocalBounds().width / 2,
        maxHeap.getPosition().y + maxHeap.getSize().y / 2 - maxHeapText.getLocalBounds().height / 2
    );
    minHeapText.setPosition(
        minHeap.getPosition().x + minHeap.getSize().x / 2 - minHeapText.getLocalBounds().width / 2,
        minHeap.getPosition().y + minHeap.getSize().y / 2 - minHeapText.getLocalBounds().height / 2
    );

    // Text styles
    easyText.setFillColor(sf::Color::Black);
    intermediateText.setFillColor(sf::Color::Black);
    hardText.setFillColor(sf::Color::Black);
    backText.setFillColor(sf::Color::Black);
    maxHeapText.setFillColor(sf::Color::Black);
    minHeapText.setFillColor(sf::Color::Black);

    // Button colors
    easyButton.setFillColor(sf::Color::White);
    intermediateButton.setFillColor(sf::Color::White);
    hardButton.setFillColor(sf::Color::White);
    backButton.setFillColor(sf::Color::White);
    maxHeap.setFillColor(sf::Color::White);
    minHeap.setFillColor(sf::Color::White);

    // Music settings
    music.play();
    music.setLoop(true);
    music.setVolume(100);

    // Scrolling
    float scrollOffset = 0.0f;
    const float scrollSpeed = 30.0f;

    // Main loop
    sf::Clock clock;
    while (window.isOpen()) {
        if (music.getStatus() != sf::Music::Status::Playing) {
            std::cout << "Failed to play music" << std::endl;
        }

        sf::Time deltaTime = clock.restart();

        // Background Panning
        backgroundPosition.x += panSpeed * deltaTime.asSeconds();
        nextBackgroundPosition.x += panSpeed * deltaTime.asSeconds();

        if (backgroundPosition.x >= backgroundTexture.getSize().x) {
            backgroundPosition.x = 0;
        }
        if (nextBackgroundPosition.x >= backgroundTexture.getSize().x) {
            nextBackgroundPosition.x = 0;
        }

        backgroundSprite1.setTextureRect(sf::IntRect(
            static_cast<int>(backgroundPosition.x),
            static_cast<int>(backgroundPosition.y),
            static_cast<int>(window.getSize().x),
            static_cast<int>(window.getSize().y)
        ));

        backgroundSprite2.setTextureRect(sf::IntRect(
            static_cast<int>(nextBackgroundPosition.x),
            static_cast<int>(nextBackgroundPosition.y),
            static_cast<int>(window.getSize().x),
            static_cast<int>(window.getSize().y)
        ));

        backgroundSprite1.setPosition(0, 0);
        backgroundSprite2.setPosition(static_cast<float>(window.getSize().x), 0);

        window.clear();
        window.draw(backgroundSprite1);
        window.draw(backgroundSprite2);

        // Event Handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));

                // Update button sizes relative to the new window size
                const float BUTTON_WIDTH = event.size.width * 0.16f;
                const float BUTTON_HEIGHT = event.size.height * 0.12f;
                const float INTBUTTON_WIDTH = event.size.width * 0.25f;

                easyButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
                intermediateButton.setSize(sf::Vector2f(INTBUTTON_WIDTH, BUTTON_HEIGHT));
                hardButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
                maxHeap.setSize(sf::Vector2f(event.size.width * 0.52f, event.size.height * 0.09f));
                minHeap.setSize(sf::Vector2f(event.size.width * 0.52f, event.size.height * 0.09f));
                backButton.setSize(sf::Vector2f(event.size.width * 0.05f, event.size.height * 0.05f));

                // Update positions based on new window size
                float windowCenterX = event.size.width / 2.0f;
                float buttonY = event.size.height * 0.6f;
                float buttonSpacing = event.size.width * 0.15f;

                easyButton.setPosition(windowCenterX - buttonSpacing - BUTTON_WIDTH, buttonY);
                intermediateButton.setPosition(windowCenterX - INTBUTTON_WIDTH / 2, buttonY);
                hardButton.setPosition(windowCenterX + buttonSpacing, buttonY);
                maxHeap.setPosition(event.size.width * 0.24f, event.size.height * 0.37f);
                minHeap.setPosition(event.size.width * 0.24f, event.size.height * 0.51f);
                backButton.setPosition(event.size.width * 0.01f, event.size.height * 0.02f);

                // Recalculate text positions after resizing
                easyText.setPosition(
                    easyButton.getPosition().x + easyButton.getSize().x / 2 - easyText.getLocalBounds().width / 2,
                    easyButton.getPosition().y + easyButton.getSize().y / 2 - easyText.getLocalBounds().height / 2
                );
                intermediateText.setPosition(
                    intermediateButton.getPosition().x + intermediateButton.getSize().x / 2 - intermediateText.getLocalBounds().width / 2,
                    intermediateButton.getPosition().y + intermediateButton.getSize().y / 2 - intermediateText.getLocalBounds().height / 2
                );
                hardText.setPosition(
                    hardButton.getPosition().x + hardButton.getSize().x / 2 - hardText.getLocalBounds().width / 2,
                    hardButton.getPosition().y + hardButton.getSize().y / 2 - hardText.getLocalBounds().height / 2
                );
                backText.setPosition(
                    backButton.getPosition().x + backButton.getSize().x / 2 - backText.getLocalBounds().width / 2,
                    backButton.getPosition().y + backButton.getSize().y / 2 - backText.getLocalBounds().height / 2
                );
                // Recalculate positions for text elements
                welcomeText.setPosition(
                    (event.size.width - welcomeText.getLocalBounds().width) / 2,
                    event.size.height * 0.09f
                );

                explanationText.setPosition(
                    (event.size.width - explanationText.getLocalBounds().width) / 2,
                    event.size.height * 0.32f
                );

                explanatitonText2.setPosition(
                    (event.size.width - explanatitonText2.getLocalBounds().width) / 2,
                    event.size.height * 0.42f
                );
                maxHeapText.setPosition(
                    maxHeap.getPosition().x + maxHeap.getSize().x / 2 - maxHeapText.getLocalBounds().width / 2,
                    maxHeap.getPosition().y + maxHeap.getSize().y / 2 - maxHeapText.getLocalBounds().height / 2
                );
                minHeapText.setPosition(
                    minHeap.getPosition().x + minHeap.getSize().x / 2 - minHeapText.getLocalBounds().width / 2,
                    minHeap.getPosition().y + minHeap.getSize().y / 2 - minHeapText.getLocalBounds().height / 2
                );
                sf::Text& currentSongText = songText;
                currentSongText.setPosition(
                    event.size.width / 2.0f - currentSongText.getLocalBounds().width / 2.0f,
                    event.size.height / 2.0f - currentSongText.getLocalBounds().height / 2.0f + scrollOffset);
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Global back button handling
                if (backButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    currentState = State::Menu;
                }

                // Main menu buttons
                if (currentState == State::Menu) {
                    if (easyButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::Easy;
                    } else if (intermediateButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::Intermediate;
                    } else if (hardButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::Hard;
                    }
                }

                // Difficulty-specific heap buttons ---------
                if (currentState == State::Easy) {
                    if (minHeap.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::EasyMinHeap;
                    } else if (maxHeap.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::EasyMaxHeap;
                    }
                } else if (currentState == State::Intermediate) {
                    if (minHeap.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::IntermediateMinHeap;
                    } else if (maxHeap.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::IntermediateMaxHeap;
                    }
                } else if (currentState == State::Hard) {
                    if (minHeap.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::HardMinHeap;
                    } else if (maxHeap.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::HardMaxHeap;
                    }
                }
            }
            if (event.type == sf::Event::MouseWheelScrolled) {

                if (event.mouseWheelScroll.delta > 0) {
                    scrollOffset += scrollSpeed;
                } else if (event.mouseWheelScroll.delta < 0) {
                    scrollOffset -= scrollSpeed;
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    scrollOffset += scrollSpeed;
                } else if (event.key.code == sf::Keyboard::Down) {
                    scrollOffset -= scrollSpeed;
                }
            }
        }

        // Constrain the scroll offset
        float contentHeight = songText.getLocalBounds().height;
        float windowHeight = window.getSize().y;
        float maxScrollOffset = std::max(0.0f, contentHeight - windowHeight + 200);

        scrollOffset = std::clamp(scrollOffset, -maxScrollOffset, 0.0f);

        // Drawing Logic
        window.clear(sf::Color::White);
        if (currentState == State::Menu) {
            songText.setString("");
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(welcomeText);
            window.draw(explanationText);
            window.draw(explanatitonText2);
            window.draw(easyButton);
            window.draw(intermediateButton);
            window.draw(hardButton);
            window.draw(easyText);
            window.draw(intermediateText);
            window.draw(hardText);
        }
        else if (currentState == State::EasyMinHeap) {
            // EASY MIN HEAP LOGIC
            songText.setString(EasyMinHeap);
            songText.setCharacterSize(40);
            songText.setFont(buttonfont);
            songText.setFillColor(sf::Color::White);
            songText.setStyle(sf::Text::Bold);
            songText.setPosition(500, 150 + scrollOffset);
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(backButton);
            window.draw(backText);
            window.draw(songText);
        }
        else if (currentState == State::EasyMaxHeap) {
            // EASY MAX HEAP LOGIC
            songText.setString(EasyMaxHeap);
            songText.setCharacterSize(40);
            songText.setFont(buttonfont);
            songText.setFillColor(sf::Color::White);
            songText.setStyle(sf::Text::Bold);
            songText.setPosition(500, 150 + scrollOffset);

            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(backButton);
            window.draw(backText);
            window.draw(songText);
        }
        else if (currentState == State::IntermediateMinHeap) {
            // INTERMEDIATE MIN HEAP LOGIC
            songText.setString(IntermediateMinHeap);
            songText.setCharacterSize(40);
            songText.setFont(buttonfont);
            songText.setFillColor(sf::Color::White);
            songText.setStyle(sf::Text::Bold);
            songText.setPosition(500, 150 + scrollOffset);

            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(backButton);
            window.draw(backText);
            window.draw(songText);
        }
        else if (currentState == State::IntermediateMaxHeap) {
            // INTERMEDIATE MAX HEAP LOGIC
            songText.setString(IntermediateMaxHeap);
            songText.setCharacterSize(40);
            songText.setFont(buttonfont);
            songText.setFillColor(sf::Color::White);
            songText.setStyle(sf::Text::Bold);
            songText.setPosition(500, 150 + scrollOffset);

            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(backButton);
            window.draw(backText);
            window.draw(songText);
        }
        else if (currentState == State::HardMinHeap) {
            // HARD MIN HEAP LOGIC
            songText.setString(HardMinHeap);
            songText.setCharacterSize(40);
            songText.setFont(buttonfont);
            songText.setFillColor(sf::Color::White);
            songText.setStyle(sf::Text::Bold);
            songText.setPosition(window.getSize().x / 2.0f - songText.getLocalBounds().width / 2.0f, window.getSize().y / 2.0f - songText.getLocalBounds().height / 2.0f + scrollOffset);

            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(backButton);
            window.draw(backText);
            window.draw(songText);
        }
        else if (currentState == State::HardMaxHeap) {
            // HARD MAX HEAP LOGIC
            songText.setString(HardMaxHeap);
            songText.setCharacterSize(40);
            songText.setFont(buttonfont);
            songText.setFillColor(sf::Color::White);
            songText.setStyle(sf::Text::Bold);
            songText.setPosition(window.getSize().x / 2.0f - songText.getLocalBounds().width / 2.0f, window.getSize().y / 2.0f - songText.getLocalBounds().height / 2.0f + scrollOffset);
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(backButton);
            window.draw(backText);
            window.draw(songText);
        }
        else {
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(maxHeap);
            window.draw(minHeap);
            window.draw(maxHeapText);
            window.draw(minHeapText);
            window.draw(backButton);
            window.draw(backText);

            // Switches between states in menu
            std::string difficulty;
            switch (currentState) {
                case State::Easy: difficulty = "Easy"; break;
                case State::Intermediate: difficulty = "Intermediate"; break;
                case State::Hard: difficulty = "Hard"; break;
                default: break;
            }
        }

        window.display();
    }

    return 0;
}
