#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <algorithm>
#include "Heap.cpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Piano Song Difficulty Ranking");
    sf::Font font;
    if (!font.loadFromFile("../resources/ABeeZee-Regular.otf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    string midiDir = "../resources/DATABASE";
    MidiCollection midiCollection(midiDir);
    midiCollection.DifficultyLevel();

    std::string currentDifficulty = "Easy";

    // Create buttons
    sf::RectangleShape easyButton(sf::Vector2f(200, 50));
    sf::RectangleShape intermediateButton(sf::Vector2f(200, 50));
    sf::RectangleShape hardButton(sf::Vector2f(200, 50));

    easyButton.setPosition(50, 50);
    intermediateButton.setPosition(300, 50);
    hardButton.setPosition(550, 50);

    easyButton.setFillColor(sf::Color::Green);
    intermediateButton.setFillColor(sf::Color::Yellow);
    hardButton.setFillColor(sf::Color::Red);

    sf::Text easyText("Easy", font, 24);
    sf::Text intermediateText("Intermediate", font, 24);
    sf::Text hardText("Hard", font, 24);

    easyText.setPosition(110, 60);
    intermediateText.setPosition(330, 60);
    hardText.setPosition(620, 60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (easyButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentDifficulty = "Easy";
                } else if (intermediateButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentDifficulty = "Intermediate";
                } else if (hardButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentDifficulty = "Hard";
                }
            }
        }

        window.clear(sf::Color::White);

        // Draw buttons
        window.draw(easyButton);
        window.draw(intermediateButton);
        window.draw(hardButton);
        window.draw(easyText);
        window.draw(intermediateText);
        window.draw(hardText);

        // Display songs
        float yOffset = 150; // Starting position for song display
        if (midiCollection.difficultyLevel.find(currentDifficulty) != midiCollection.difficultyLevel.end()) {
            auto songs = midiCollection.difficultyLevel[currentDifficulty];
            std::sort(songs.begin(), songs.end(),
                [](const auto& a, const auto& b) { return a.second < b.second; });

            for (const auto& song : songs) {
                // Get tempo and MIDI key from Parsing class
                int tempo = midiCollection.getTempo(song.first); // Assume getTempo exists in MidiCollection

                std::string songInfo = song.first + " (Rank: " + std::to_string(song.second) +
                                       ", Tempo: " + std::to_string(tempo) + ")";

                sf::Text songText(songInfo, font, 20);
                songText.setFillColor(sf::Color::Black);
                songText.setPosition(50, yOffset);
                window.draw(songText);
                yOffset += 30;

                if (yOffset > 1000) break; // Prevent overflow
            }
        }

        window.display();
    }

    return 0;
}
