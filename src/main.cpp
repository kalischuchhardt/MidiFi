#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Heap.cpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Piano Song Difficulty Ranking");
    sf::Font font;
    sf::Texture backgroundTexture;
    backgroundTexture.setRepeated(true);
    sf::Sprite backgroundSprite1(backgroundTexture);
    sf::Sprite backgroundSprite2(backgroundTexture);
    sf::Music music;

    if (!music.openFromFile("../resources/snowflake-relaxing-piano-music-269243.ogg")) {
        std::cout << "Error opening music file" << std::endl;
        return 1;
    }

    if (!backgroundTexture.loadFromFile("../resources/Texture_512x512_39.png")) {
        std::cerr << "Failed to load background texture" << std::endl;
        return 1;
    }

    if (!font.loadFromFile("../resources/Milkyway.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    sf::Vector2f backgroundPosition(0, 0);
    sf::Vector2f nextBackgroundPosition(static_cast<float>(backgroundTexture.getSize().x), 0);
    float panSpeed = 50.0f;

    std::string midiDir = "../resources/DATABASE";
    MidiCollection midiCollection(midiDir);
    midiCollection.DifficultyLevel();

    enum class State { Menu, Easy, Intermediate, Hard };
    State currentState = State::Menu;

    // Window dimensions
    const float WINDOW_WIDTH = 1920.0f;
    const float WINDOW_HEIGHT = 1080.0f;

    // Button dimensions
    const float BUTTON_WIDTH = 310.0f;
    const float BUTTON_HEIGHT = 125.0f;
    const float INTBUTTON_WIDTH = 350.0f;

    // Calculate positions
    float windowCenterX = WINDOW_WIDTH / 2.0f;
    float buttonY = WINDOW_HEIGHT * 0.6f;
    float buttonSpacing = WINDOW_WIDTH * 0.15f;

    sf::RectangleShape easyButton;
    sf::RectangleShape intermediateButton;
    sf::RectangleShape hardButton;
    sf::RectangleShape backButton;

    sf::Text easyText("Easy", font, 55);
    sf::Text intermediateText("Intermediate", font, 55);
    sf::Text hardText("Hard", font, 55);
    sf::Text backText("Back", font, 24);
    sf::Text welcomeText("Welcome", font, 24);

    // Set button positions
    easyButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));
    intermediateButton.setSize(sf::Vector2f(INTBUTTON_WIDTH, BUTTON_HEIGHT));
    hardButton.setSize(sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT));

    easyButton.setPosition(windowCenterX - buttonSpacing - BUTTON_WIDTH, buttonY);
    intermediateButton.setPosition(windowCenterX - INTBUTTON_WIDTH / 2, buttonY);
    hardButton.setPosition(windowCenterX + buttonSpacing, buttonY);

    // Set text positions
    float textY = buttonY + BUTTON_HEIGHT / 2;
    easyText.setPosition(
        easyButton.getPosition().x + BUTTON_WIDTH / 2 - easyText.getLocalBounds().width / 2,
        textY - easyText.getLocalBounds().height / 2
    );
    intermediateText.setPosition(
        intermediateButton.getPosition().x + INTBUTTON_WIDTH / 2 - intermediateText.getLocalBounds().width / 2,
        textY - intermediateText.getLocalBounds().height / 2
    );
    hardText.setPosition(
        hardButton.getPosition().x + BUTTON_WIDTH / 2 - hardText.getLocalBounds().width / 2,
        textY - hardText.getLocalBounds().height / 2
    );

    // Back button positioning
    backButton.setSize(sf::Vector2f(100, 50));
    backButton.setPosition(20, 20);
    backText.setPosition(backButton.getPosition().x + backButton.getSize().x / 2 - backText.getLocalBounds().width / 2,
                         backButton.getPosition().y + backButton.getSize().y / 2 - backText.getLocalBounds().height / 2);

    easyText.setFillColor(sf::Color::Black);
    intermediateText.setFillColor(sf::Color::Black);
    hardText.setFillColor(sf::Color::Black);

    easyButton.setFillColor(sf::Color::White);
    intermediateButton.setFillColor(sf::Color::White);
    hardButton.setFillColor(sf::Color::White);
    backButton.setFillColor(sf::Color::White);

    music.play();
    music.setLoop(true);
    music.setVolume(100);

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

    sf::Clock clock;
    while (window.isOpen()) {
        if (music.getStatus() != sf::Music::Status::Playing) {
            std::cout << "Failed to play music" << std::endl;
        }

        sf::Time deltaTime = clock.restart();

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

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (currentState == State::Menu) {
                    if (easyButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::Easy;
                    } else if (intermediateButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::Intermediate;
                    } else if (hardButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                        currentState = State::Hard;
                    }
                } else if (backButton.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                    currentState = State::Menu;
                }
            }
        }

        window.clear(sf::Color::White);
        if (currentState == State::Menu) {
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(fadeOverlay);
            window.draw(easyButton);
            window.draw(intermediateButton);
            window.draw(hardButton);
            window.draw(easyText);
            window.draw(intermediateText);
            window.draw(hardText);
        } else {
            window.draw(backgroundSprite1);
            window.draw(backgroundSprite2);
            window.draw(fadeOverlay);
            window.draw(backButton);
            window.draw(backText);

            std::string difficulty;
            switch (currentState) {
                case State::Easy: difficulty = "Easy"; break;
                case State::Intermediate: difficulty = "Intermediate"; break;
                case State::Hard: difficulty = "Hard"; break;
                default: break;
            }

            float yOffset = 150;
            if (midiCollection.difficultyLevel.find(difficulty) != midiCollection.difficultyLevel.end()) {
                auto songs = midiCollection.difficultyLevel[difficulty];
                std::sort(songs.begin(), songs.end(), [](const auto& a, const auto& b) {
                    return a.second < b.second;
                });
                for (const auto& song : songs) {
                    int tempo = midiCollection.getTempo(song.first);
                    std::string songInfo = song.first + " (Rank: " + std::to_string(song.second) + ", Tempo: " + std::to_string(tempo) + ")";
                    sf::Text songText(songInfo, font, 20);
                    songText.setFillColor(sf::Color::Black);
                    songText.setPosition(50, yOffset);
                    window.draw(songText);
                    yOffset += 30;
                    if (yOffset > 1000) break;
                }
            }
        }

        window.display();
    }

    return 0;
}
