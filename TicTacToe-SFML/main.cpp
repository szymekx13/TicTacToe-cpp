#include <SFML/Graphics.hpp>
#include <array>
#include <optional>
#include <string>
#include <thread>
#include <chrono>

const int SIZE = 3;

// Funkcja sprawdzaj¹ca zwyciêstwo lub remis
char checkWin(const std::array<std::array<char, SIZE>, SIZE>& board)
{
    // Sprawdzenie wierszy i kolumn
    for (int i = 0; i < SIZE; ++i)
    {
        if (board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] != 0 && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }

    // Sprawdzenie przek¹tnych
    if (board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];

    // Sprawdzenie remisu
    bool full = true;
    for (auto& row : board)
        for (char c : row)
            if (c == 0) full = false;

    if (full) return 'D'; // D = Draw

    return 0; // Gra trwa
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 600, 600 }), "Tic Tac Toe");

    sf::Font font("C:/Users/szyme/source/repos/TicTacToe-SFML/External/Fonts/Roboto-Regular.ttf");

    std::array<std::array<char, SIZE>, SIZE> board{};
    char currentPlayer = 'X';

    sf::Text infoText(font, "Turn: X", 40);
    infoText.setFillColor(sf::Color::Black);
    infoText.setPosition({ 10.f, 10.f });

    while (window.isOpen())
    {
        // Obs³uga zdarzeñ
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    int col = mousePressed->position.x / 200;
                    int row = mousePressed->position.y / 200;

                    if (row < SIZE && col < SIZE && board[row][col] == 0)
                    {
                        board[row][col] = currentPlayer;

                        // Sprawdzenie stanu gry
                        char result = checkWin(board);
                        if (result == 'X' || result == 'O')
                        {
                            infoText.setString("Winner: " + std::string(1, result));
                            window.clear(sf::Color::White);
                            window.draw(infoText);
                            window.display();
                            std::this_thread::sleep_for(std::chrono::seconds(2));

                            // Reset planszy
                            board = {};
                            currentPlayer = 'X';
                            infoText.setString("Turn: X");
                            continue;
                        }
                        else if (result == 'D')
                        {
                            infoText.setString("It's a draw!");
                            window.clear(sf::Color::White);
                            window.draw(infoText);
                            window.display();
                            std::this_thread::sleep_for(std::chrono::seconds(2));

                            board = {};
                            currentPlayer = 'X';
                            infoText.setString("Turn: X");
                            continue;
                        }

                        // Zmiana gracza
                        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
                        infoText.setString("Turn: " + std::string(1, currentPlayer));
                    }
                }
            }
        }

        // Czyszczenie i rysowanie planszy
        window.clear(sf::Color::White);

        // Siatka
        for (int i = 1; i < SIZE; i++)
        {
            sf::RectangleShape line({ 600, 5 });
            line.setFillColor(sf::Color::Black);
            line.setPosition({ 0.f, static_cast<float>(i * 200) });
            window.draw(line);

            line.setSize({ 5, 600 });
            line.setPosition({ static_cast<float>(i * 200), 0.f });
            window.draw(line);
        }

        // X i O
        for (int r = 0; r < SIZE; r++)
            for (int c = 0; c < SIZE; c++)
                if (board[r][c] != 0)
                {
                    sf::Text text(font, std::string(1, board[r][c]), 100);
                    text.setFillColor(sf::Color::Black);
                    text.setPosition({ c * 200.f + 50.f, r * 200.f + 30.f });
                    window.draw(text);
                }

        // Tekst o turze / wyniku
        window.draw(infoText);

        window.display();
    }
}
