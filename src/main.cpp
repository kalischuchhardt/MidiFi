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

    // States
    enum class State { Menu, Easy, Intermediate, Hard, EasyMaxHeap, EasyMinHeap, IntermediateMinHeap, IntermediateMaxHeap, HardMaxHeap, HardMinHeap };
    State currentState = State::Menu;

    // Window dimensions
    const float WINDOW_WIDTH = 1920.0f;
    const float WINDOW_HEIGHT = 1080.0f;

    // Button dimensions
    const float BUTTON_WIDTH = 310.0f;
    const float BUTTON_HEIGHT = 125.0f;
    const float INTBUTTON_WIDTH = 480.0f;

    // Calculate positions for buttons
    float windowCenterX = WINDOW_WIDTH / 2.0f;
    float buttonY = WINDOW_HEIGHT * 0.6f;
    float buttonSpacing = WINDOW_WIDTH * 0.15f;

    // Set button positions
    easyButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    intermediateButton.setSize(sf::Vector2f(INTBUTTON_WIDTH, BUTTON_HEIGHT));
    hardButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    maxHeap.setSize(sf::Vector2f(1000,100));
    minHeap.setSize(sf::Vector2f(1000,100));
    backButton.setSize(sf::Vector2f(100, 50));
    backButton.setPosition(20, 20);
    backText.setPosition(backButton.getPosition().x + backButton.getSize().x / 2 - backText.getLocalBounds().width / 2, backButton.getPosition().y + backButton.getSize().y / 2 - backText.getLocalBounds().height / 2 - 10);

    easyButton.setPosition(windowCenterX - buttonSpacing - BUTTON_WIDTH, buttonY);
    intermediateButton.setPosition(windowCenterX - INTBUTTON_WIDTH / 2, buttonY);
    hardButton.setPosition(windowCenterX + buttonSpacing, buttonY);
    maxHeap.setPosition(450, 400);
    minHeap.setPosition(450, 553);

    // Set text positions
    easyText.setPosition(
        easyButton.getPosition().x + easyButton.getSize().x / 2 - easyText.getLocalBounds().width / 2,
        easyButton.getPosition().y + easyButton.getSize().y / 2 - easyText.getLocalBounds().height / 2 - 15
    );
    intermediateText.setPosition(
        intermediateButton.getPosition().x + intermediateButton.getSize().x / 2 - intermediateText.getLocalBounds().width / 2,
        intermediateButton.getPosition().y + intermediateButton.getSize().y / 2 - intermediateText.getLocalBounds().height / 2 - 15
    );
    hardText.setPosition(
        hardButton.getPosition().x + hardButton.getSize().x / 2 - hardText.getLocalBounds().width / 2,
        hardButton.getPosition().y + hardButton.getSize().y / 2 - hardText.getLocalBounds().height / 2 - 15
    );

    maxHeapText.setPosition(
        maxHeap.getPosition().x + maxHeap.getSize().x / 2 - maxHeapText.getLocalBounds().width / 2,
        maxHeap.getPosition().y + maxHeap.getSize().y / 2 - maxHeapText.getLocalBounds().height / 2 - 15
    );

    minHeapText.setPosition(
        minHeap.getPosition().x + minHeap.getSize().x / 2 - minHeapText.getLocalBounds().width / 2,
        minHeap.getPosition().y + minHeap.getSize().y / 2 - minHeapText.getLocalBounds().height / 2 - 15
    );

    welcomeText.setPosition(373, 100);
    explanationText.setPosition(501,350);
    explanatitonText2.setPosition(386,450);

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

    // Fade Overlay to make it pretty
    sf::RectangleShape fadeOverlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    sf::Texture fadeTexture;
    fadeTexture.create(window.getSize().x, window.getSize().y);
    sf::Image fadeImage;
    fadeImage.create(window.getSize().x, window.getSize().y, sf::Color::Transparent);
    for (unsigned int x = 0; x < window.getSize().x; ++x) {
        for (unsigned int y = 0; y < window.getSize().y; ++y) {
            float distToEdge = std::min({static_cast<float>(x), static_cast<float>(y),
                                         static_cast<float>(window.getSize().x - x),
                                         static_cast<float>(window.getSize().y - y)});
            float fadeFactor = std::min(1.0f, distToEdge / (std::min(window.getSize().x, window.getSize().y) * 0.25f));
            sf::Uint8 alpha = static_cast<sf::Uint8>(255 * (1.0f - fadeFactor));
            fadeImage.setPixel(x, y, sf::Color(0, 0, 0, alpha));
        }
    }
    fadeTexture.loadFromImage(fadeImage);
    fadeOverlay.setTexture(&fadeTexture);

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

                // Difficulty-specific heap buttons
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
        }

        // Drawing Logic
        window.clear(sf::Color::White);
        if (currentState == State::Menu) {
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(fadeOverlay);
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
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(fadeOverlay);
            window.draw(backButton);
            window.draw(backText);
        }
        else if (currentState == State::EasyMaxHeap) {
            // EASY MAX HEAP LOGIC
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(fadeOverlay);
            window.draw(backButton);
            window.draw(backText);
        }
        else if (currentState == State::IntermediateMinHeap) {
            // INTERMEDIATE MIN HEAP LOGIC
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(fadeOverlay);
            window.draw(backButton);
            window.draw(backText);
        }
        else if (currentState == State::IntermediateMaxHeap) {
            // INTERMEDIATE MAX HEAP LOGIC
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(fadeOverlay);
            window.draw(backButton);
            window.draw(backText);
        }
        else if (currentState == State::HardMinHeap) {
            // HARD MIN HEAP LOGIC
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(fadeOverlay);
            window.draw(backButton);
            window.draw(backText);
        }
        else if (currentState == State::HardMaxHeap) {
            // HARD MAX HEAP LOGIC
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(fadeOverlay);
            window.draw(backButton);
            window.draw(backText);
        }
        else {
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(maxHeap);
            window.draw(minHeap);
            window.draw(maxHeapText);
            window.draw(minHeapText);
            window.draw(fadeOverlay);
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
            //
            // float yOffset = 150;
            // if (midiCollection.difficultyLevel.find(difficulty) != midiCollection.difficultyLevel.end()) {
            //     auto songs = midiCollection.difficultyLevel[difficulty];
            //     std::sort(songs.begin(), songs.end(), [](const auto& a, const auto& b) {
            //         return a.second < b.second;
            //     });
            //     for (const auto& song : songs) {
            //         int tempo = midiCollection.getTempo(song.first);
            //         std::string songInfo = song.first + " (Rank: " + std::to_string(song.second) + ", Tempo: " + std::to_string(tempo) + ")";
            //         sf::Text songText(songInfo, buttonfont, 20);
            //         songText.setFillColor(sf::Color::Black);
            //         songText.setPosition(50, yOffset);
            //         window.draw(songText);
            //         yOffset += 30;
            //         if (yOffset > 1000) break;
            //     }
            // }
        }

        window.display();
    }

    return 0;
}
