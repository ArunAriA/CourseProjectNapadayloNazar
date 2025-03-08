#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

// Функція налаштовує розмір ігрового поля.
void setSizeGrid(int& line, int& col, bool bigGrid = false)
{
    if (bigGrid)
    {
        line = 5;
        col = 6;
    }
    else
    {
        line = 4;
        col = 4;
    }
}

// Заповнює ігрове поле випадковими кольорами.
void fillGridWithColors(vector<vector<string>>& grid, int line, int col, bool bigGrid)
{
    vector<string> colors;

    if (!bigGrid)
        colors = { "\033[90mA\033[0m ", "\033[31mB\033[0m ", "\033[32mC\033[0m ", "\033[33mD\033[0m ", "\033[34mE\033[0m ", "\033[35mF\033[0m ", "\033[36mG\033[0m ", "\033[92mV\033[0m " };
    else
        colors = { "\033[90mA\033[0m ", "\033[31mB\033[0m ", "\033[32mC\033[0m ", "\033[33mD\033[0m ", "\033[34mE\033[0m ", "\033[35mF\033[0m ", "\033[36mG\033[0m ", "\033[92mV\033[0m ", "\033[91mQ\033[0m ", "\033[93mL\033[0m ", "\033[94mP\033[0m ", "\033[95mZ\033[0m ", "\033[96mX\033[0m ", "\033[97mN\033[0m ", "\033[45mM\033[0m " };

    vector<string> colorPool;

    for (int i = 0; i < (line * col) / 2; i++) {
        colorPool.push_back(colors[i]);
        colorPool.push_back(colors[i]);
    }

    srand(time(NULL));
    random_shuffle(colorPool.begin(), colorPool.end());

    int index = 0;
    for (int i = 0; i < line; i++) {
        for (int j = 0; j < col; j++) {
            grid[i][j] = colorPool[index++];
        }
    }
}

// Відображає ігрове поле з відкритими та прихованими картами.
void printGrid(const vector<vector<string>>& grid, const vector<vector<bool>>& revealed)
{
    for (int i = 0; i < grid.size(); i++)
    {
        for (int j = 0; j < grid[i].size(); j++)
        {
            cout << "+----";
        }
        cout << "+" << endl;

        for (int j = 0; j < grid[i].size(); j++)
        {
            if (revealed[i][j]) {
                cout << "| " << setw(2) << left << grid[i][j] << " ";
            }
            else {
                cout << "| ?? ";
            }
        }
        cout << "|" << endl;
    }

    for (int j = 0; j < grid[0].size(); j++)
    {
        cout << "+----";
    }
    cout << "+" << endl;
}

// Перевіряє, чи вибрана карта є допустимим вибором.
bool isValidChoice(int x, int y, int line, int col, const vector<vector<bool>>& revealed)
{
    return x >= 0 && x < line && y >= 0 && y < col && !revealed[x][y];
}

// Запис статистики у файл
void saveStatistics(int moves, double timeInSeconds)
{
    ofstream file("game_statistics.txt", ios::app);
    if (file.is_open())
    {
        file << "Кількість переворотів: " << moves << "\n";
        file << "Час гри: " << fixed << setprecision(2) << timeInSeconds << " секунд\n";
        file << "-----------------------------\n";
        file.close();
    }
    else
    {
        cout << "Не вдалося відкрити файл для запису статистики.\n";
    }
}

// Основна функція гри
void playGame(int line, int col, vector<vector<string>>& grid)
{
    vector<vector<bool>> revealed(line, vector<bool>(col, false));
    int moves = 0;
    int matchedPairs = 0;

    time_t startTime = time(NULL);

    while (matchedPairs < (line * col) / 2)
    {
        printGrid(grid, revealed);

        int x1, y1, x2, y2;
        cout << "Виберіть першу карту (рядок колонка): ";
        cin >> x1 >> y1;
        cout << "Виберіть другу карту (рядок колонка): ";
        cin >> x2 >> y2;

        x1--, y1--, x2--, y2--;

        if (!isValidChoice(x1, y1, line, col, revealed) || !isValidChoice(x2, y2, line, col, revealed) || (x1 == x2 && y1 == y2))
        {
            cout << "Некоректний вибір! Спробуйте знову.\n";
            continue;
        }

        revealed[x1][y1] = true;
        revealed[x2][y2] = true;
        printGrid(grid, revealed);

        if (grid[x1][y1] != grid[x2][y2])
        {
            cout << "Карти не співпали!" << endl;
            revealed[x1][y1] = false;
            revealed[x2][y2] = false;
        }
        else
        {
            cout << "Чудово! Карти співпали!" << endl;
            matchedPairs++;
        }

        moves++;
    }

    time_t endTime = time(NULL);
    double timeInSeconds = difftime(endTime, startTime);

    cout << "Вітаємо! Ви виграли за " << moves << " ходів.\n";
    cout << "Час гри: " << fixed << setprecision(2) << timeInSeconds << " секунд.\n";

    saveStatistics(moves, timeInSeconds);
}

int main()
{
    int line = 0, col = 0;
    bool bigGrid = false;
    int que = 0;

    cout << "Оберіть розмір ігрового поля:\n1) 4x4\n2) 5x6" << endl;
    cin >> que;
    bigGrid = (que == 2);

    setSizeGrid(line, col, bigGrid);
    vector<vector<string>> arr(line, vector<string>(col));
    fillGridWithColors(arr, line, col, bigGrid);
    playGame(line, col, arr);
    return 0;
}
