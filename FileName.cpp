/*Вариант 8.
Определить структурный тип, описывающий компьютерные игры(название игры,
    студия разработчик, стоимость подписки, объем аудитории игроков,
    поддерживаемые платформы : PC, mobile, gamepad; тип: RPG, стратегия, гонки,
    авиасимулятор и др.).Заполнить структурный массив 20 - ю записями.Переписать из
    исходного массива в другой массив, информацию только о шутерах от id Software.
    Затем новый массив отсортировать по объему аудитории.Вывести все данные по
    конкретной игре.Вывести 4 самых недорогих игр.Реализовать функцию изменения
    игры.В отдельный массив поместить все мобильные игры, в которых можно
    погонять.Реализовать вывод отфильтрованных данных в виде оберточной функции.*/

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

enum Genre { RPG, STRATEGY, RACING, FLIGHT_SIM, SHOOTER };

struct Platforms {
    int pc;
    int mobile;
    int gamepad;
};

struct Game {
    char name[50];
    char developer[50];
    float subscriptionPrice;
    int audienceSize;
    struct Platforms platforms;
    enum Genre genre;
};

const char* genreToString(enum Genre g) {
    if (g == 0) return "RPG";
    if (g == 1) return "Strategy";
    if (g == 2) return "Racing";
    if (g == 3) return "Flight Simulator";
    return "Shooter";
}

void printGame(struct Game g) {
    cout << "Name: " << g.name << endl;
    cout << "Developer: " << g.developer << endl;
    cout << "Price: " << g.subscriptionPrice << endl;
    cout << "Audience: " << g.audienceSize << " mln" << endl;
    cout << "Platforms: PC:" << (g.platforms.pc ? "Y" : "N")
        << " Mobile:" << (g.platforms.mobile ? "Y" : "N")
        << " Gamepad:" << (g.platforms.gamepad ? "Y" : "N") << endl;
    cout << "Genre: " << genreToString(g.genre) << endl;
    cout << "------------------------" << endl;
}

void displayFiltered(struct Game* arr, int n, const char* title) {
    cout << "\n=== " << title << " ===" << endl;
    for (int i = 0; i < n; i++) {
        printGame(arr[i]);
    }
}

void modifyGame(struct Game* g, const char* name, float price, int audience) {
    strcpy(g->name, name);
    g->subscriptionPrice = price;
    g->audienceSize = audience;
    cout << "Game modified!" << endl;
}

void readAudienceFromTextFile(const char* filename, Game* games, int count) {
    ifstream fin;
    fin.open(filename);

    if (!fin.is_open()) {
        cout << "Error: Cannot open text file " << filename << endl;
        return;
    }

    char gameName[50];
    int audience;

    while (fin >> gameName >> audience) {
        for (int i = 0; i < count; i++) {
            if (strcmp(games[i].name, gameName) == 0) {
                games[i].audienceSize = audience;
                cout << "Updated " << gameName << " audience to " << audience << " mln" << endl;
                break;
            }
        }
    }

    fin.close();
}

void writeGamesToBinaryFile(const char* filename, Game* games, int count) {
    ofstream fout;
    fout.open(filename, ios::binary | ios::out);

    if (!fout.is_open()) {
        cout << "Error: Cannot create binary file " << filename << endl;
        return;
    }

    fout.write((char*)games, count * sizeof(Game));
    fout.close();
    cout << "Successfully wrote " << count << " games to " << filename << endl;
}

int readGamesFromBinaryFile(const char* filename, Game* games, int maxCount) {
    ifstream fin;
    fin.open(filename, ios::binary | ios::in);

    if (!fin.is_open()) {
        cout << "Error: Cannot open binary file " << filename << endl;
        return 0;
    }

    fin.read((char*)games, maxCount * sizeof(Game));
    int count = fin.gcount() / sizeof(Game);
    fin.close();

    cout << "Successfully read " << count << " games from " << filename << endl;
    return count;
}

int main() {
    setlocale(LC_ALL, "");

    struct Game games[20] = {
        {"DOOM_Eternal", "id Software", 9.99f, 12, {1,0,1}, SHOOTER},
        {"Quake_Champions", "id Software", 0.00f, 5, {1,0,1}, SHOOTER},
        {"RAGE_2", "id Software", 19.99f, 4, {1,0,1}, SHOOTER},
        {"Wolfenstein", "MachineGames", 29.99f, 3, {1,0,1}, SHOOTER},
        {"Cyberpunk_2077", "CD Projekt", 59.99f, 25, {1,0,1}, RPG},
        {"The_Witcher_3", "CD Projekt", 39.99f, 30, {1,1,1}, RPG},
        {"StarCraft_II", "Blizzard", 0.00f, 10, {1,0,1}, STRATEGY},
        {"Age_of_Empires_IV", "Relic", 59.99f, 8, {1,0,1}, STRATEGY},
        {"Forza_Horizon_5", "Playground", 69.99f, 15, {1,0,1}, RACING},
        {"Need_for_Speed", "Ghost Games", 39.99f, 7, {1,1,1}, RACING},
        {"MS_Flight_Sim", "Asobo", 79.99f, 5, {1,0,1}, FLIGHT_SIM},
        {"X-Plane_11", "Laminar", 59.99f, 3, {1,0,1}, FLIGHT_SIM},
        {"DOOM_2016", "id Software", 19.99f, 8, {1,0,1}, SHOOTER},
        {"Quake_Live", "id Software", 0.00f, 2, {1,0,1}, SHOOTER},
        {"Half-Life_2", "Valve", 9.99f, 20, {1,0,1}, SHOOTER},
        {"Asphalt_9", "Gameloft", 0.00f, 50, {1,1,1}, RACING},
        {"Real_Racing_3", "Firemonkeys", 0.00f, 40, {0,1,0}, RACING},
        {"CS_GO", "Valve", 0.00f, 35, {1,0,1}, SHOOTER},
        {"Genshin_Impact", "miHoYo", 0.00f, 60, {1,1,1}, RPG},
        {"PUBG_Mobile", "Lightspeed", 0.00f, 100, {0,1,0}, SHOOTER}
    };

    cout << "===== FILE OPERATIONS =====" << endl;

    writeGamesToBinaryFile("games.bin", games, 20);

    struct Game gamesFromFile[20];
    int readCount = readGamesFromBinaryFile("games.bin", gamesFromFile, 20);

    cout << "\n=== GAMES READ FROM BINARY FILE ===" << endl;
    for (int i = 0; i < readCount; i++) {
        printGame(gamesFromFile[i]);
    }

    cout << "\n===== ORIGINAL PROGRAM =====" << endl;

    cout << "=== ALL GAMES ===" << endl;
    for (int i = 0; i < 20; i++) {
        printGame(games[i]);
    }

    struct Game idShooters[20];
    int idCount = 0;
    for (int i = 0; i < 20; i++) {
        if (games[i].genre == SHOOTER && strcmp(games[i].developer, "id Software") == 0) {
            idShooters[idCount] = games[i];
            idCount++;
        }
    }

    for (int i = 0; i < idCount - 1; i++) {
        for (int j = 0; j < idCount - i - 1; j++) {
            if (idShooters[j].audienceSize < idShooters[j + 1].audienceSize) {
                struct Game temp = idShooters[j];
                idShooters[j] = idShooters[j + 1];
                idShooters[j + 1] = temp;
            }
        }
    }

    displayFiltered(idShooters, idCount, "ID SOFTWARE SHOOTERS (sorted by audience)");

    cout << "\n=== DETAILS FOR 'DOOM_Eternal' ===" << endl;
    for (int i = 0; i < 20; i++) {
        if (strcmp(games[i].name, "DOOM_Eternal") == 0) {
            printGame(games[i]);
            break;
        }
    }

    struct Game cheap[20];
    for (int i = 0; i < 20; i++) cheap[i] = games[i];

    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19 - i; j++) {
            if (cheap[j].subscriptionPrice > cheap[j + 1].subscriptionPrice) {
                struct Game temp = cheap[j];
                cheap[j] = cheap[j + 1];
                cheap[j + 1] = temp;
            }
        }
    }

    cout << "\n=== 4 CHEAPEST GAMES ===" << endl;
    for (int i = 0; i < 4; i++) {
        cout << i + 1 << ". " << cheap[i].name << " - " << cheap[i].subscriptionPrice << endl;
    }

    cout << "\n=== MODIFYING GAME ===" << endl;
    for (int i = 0; i < 20; i++) {
        if (strcmp(games[i].name, "DOOM_Eternal") == 0) {
            modifyGame(&games[i], "DOOM_Eternal_Updated", 14.99f, 15);
            printGame(games[i]);
            break;
        }
    }

    struct Game mobileRacing[20];
    int mrCount = 0;
    for (int i = 0; i < 20; i++) {
        if (games[i].platforms.mobile == 1 && games[i].genre == RACING) {
            mobileRacing[mrCount] = games[i];
            mrCount++;
        }
    }

    displayFiltered(mobileRacing, mrCount, "MOBILE RACING GAMES");

    return 0;
}