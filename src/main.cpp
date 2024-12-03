#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <SFML/Audio.hpp>

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
        std::cout << "Error opening music.ogg file" << std::endl;
        return 1;
    }
    if (!backgroundTexture.loadFromFile("../resources/Texture_512x512_39.png")) {
        std::cerr << "Failed to load background texture" << std::endl;
        return 1;
    }
    if (!font.loadFromFile("../resources/ABeeZee-Regular.otf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    // Initial background positions
    sf::Vector2f backgroundPosition(0, 0);
    sf::Vector2f nextBackgroundPosition(backgroundTexture.getSize().x, 0);
    float panSpeed = 50.0f;

    string midiDir = "../resources/DATABASE";
    MidiCollection midiCollection(midiDir);
    midiCollection.DifficultyLevel();

    enum class State { Menu, Easy, Intermediate, Hard };
    State currentState = State::Menu;

    // Create buttons
    sf::RectangleShape easyButton(sf::Vector2f(200, 100));
    sf::RectangleShape intermediateButton(sf::Vector2f(200, 100));
    sf::RectangleShape hardButton(sf::Vector2f(200, 100));
    sf::RectangleShape backButton(sf::Vector2f(100, 50));

    sf::Text easyText("Easy", font, 24);
    sf::Text intermediateText("Intermediate", font, 24);
    sf::Text hardText("Hard", font, 24);
    sf::Text backText("Back", font, 24);
    sf::Text welcomeText("Welcome", font, 24);

    easyText.setColor(sf::Color::Black);
    easyText.setStyle(sf::Text::Bold);
    intermediateText.setColor(sf::Color::Black);
    intermediateText.setStyle(sf::Text::Bold);
    hardText.setColor(sf::Color::Black);
    hardText.setStyle(sf::Text::Bold);

    // Calculate the center of the window
    float windowCenterX = window.getSize().x / 2.0f;
    float buttonY = 540;

    // Set button positions to center them
    easyButton.setPosition(windowCenterX - 470, buttonY);
    intermediateButton.setPosition(windowCenterX - 20, buttonY);
    hardButton.setPosition(windowCenterX + 430, buttonY);

    // Set colors for buttons
    easyButton.setFillColor(sf::Color::White);
    intermediateButton.setFillColor(sf::Color::White);
    hardButton.setFillColor(sf::Color::White);
    backButton.setFillColor(sf::Color::White);

    // Set text positions accordingly
    easyText.setPosition(windowCenterX - 410, buttonY + 35);
    intermediateText.setPosition(windowCenterX - 40, buttonY + 35);
    hardText.setPosition(windowCenterX + 450, buttonY + 35);

    music.play();
    music.setLoop(true);
    music.setVolume(100);

    sf::RectangleShape fadeOverlay(sf::Vector2f(window.getSize().x, window.getSize().y));
    sf::Texture fadeTexture;
    fadeTexture.create(window.getSize().x, window.getSize().y);

    // Create a gradient that fades from the edges with a smooth transition
    sf::Image fadeImage;
    fadeImage.create(window.getSize().x, window.getSize().y, sf::Color::Transparent);

    // Calculate distance from edges and use it to control fade effect
    for (unsigned int x = 0; x < window.getSize().x; ++x) {
        for (unsigned int y = 0; y < window.getSize().y; ++y) {
            // Calculate the distance to the nearest edge
            float distToEdge = std::min({ x, y, window.getSize().x - x, window.getSize().y - y });

            // Use a smoother transparency function to make the fade effect more balanced
            // Gradual fade effect based on distance from the edges
            float fadeFactor = std::min(1.0f, distToEdge / (std::min(window.getSize().x, window.getSize().y) * 0.25f));

            // Apply fadeFactor to the alpha value
            int alpha = static_cast<int>(255 * (1.0f - fadeFactor)); // Fade to black near edges

            fadeImage.setPixel(x, y, sf::Color(0, 0, 0, alpha));
        }
    }

    fadeTexture.loadFromImage(fadeImage);
    fadeOverlay.setTexture(&fadeTexture);

    sf::Clock clock;
    while (window.isOpen()) {
        if (music.getStatus() != sf::Music::Playing) {
            std::cout << "Failed to play music" << std::endl;
        }
        sf::Time deltaTime = clock.restart();

        // Update background positions for smooth scrolling
        backgroundPosition.x += panSpeed * deltaTime.asSeconds();
        nextBackgroundPosition.x += panSpeed * deltaTime.asSeconds();

        // If one background reaches the end, reset its position for seamless looping
        if (backgroundPosition.x >= backgroundTexture.getSize().x) {
            backgroundPosition.x = 0;
        }
        if (nextBackgroundPosition.x >= backgroundTexture.getSize().x) {
            nextBackgroundPosition.x = 0;
        }

        // Set textures for each sprite
        backgroundSprite1.setTextureRect(sf::IntRect(
            static_cast<int>(backgroundPosition.x),
            static_cast<int>(backgroundPosition.y),
            window.getSize().x,
            window.getSize().y
        ));

        backgroundSprite2.setTextureRect(sf::IntRect(
            static_cast<int>(nextBackgroundPosition.x),
            static_cast<int>(nextBackgroundPosition.y),
            window.getSize().x,
            window.getSize().y
        ));

        // Set sprite positions
        backgroundSprite1.setPosition(0, 0);
        backgroundSprite2.setPosition(window.getSize().x, 0);  // Position the second background right after the first one

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
                    if (easyButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        currentState = State::Easy;
                    } else if (intermediateButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        currentState = State::Intermediate;
                    } else if (hardButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        currentState = State::Hard;
                    }
                } else if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
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

            // Display songs for the selected difficulty
            float yOffset = 150;
            if (midiCollection.difficultyLevel.find(difficulty) != midiCollection.difficultyLevel.end()) {
                auto songs = midiCollection.difficultyLevel[difficulty];
                std::sort(songs.begin(), songs.end(),
                    [](const auto& a, const auto& b) { return a.second < b.second; });

                for (const auto& song : songs) {
                    int tempo = midiCollection.getTempo(song.first);
                    std::string songInfo = song.first + " (Rank: " + std::to_string(song.second) +
                                           ", Tempo: " + std::to_string(tempo) + ")";

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
