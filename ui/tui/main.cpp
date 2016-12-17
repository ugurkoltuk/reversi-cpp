#include <iostream>
#include <cctype>
#include <termcap.h>
#include <unistd.h>
#include <ctime>
#include <cstring>

#include "game/gameplay.h"
#include "ai/leyla.h"

using namespace std;

void clear_screen()
{
    char buf[1024];
    char *str;

    tgetent(buf, getenv("TERM"));
    str = tgetstr("cl", NULL);
    fputs(str, stdout);
}

enum Mode
{
    Mode_HumanVsComputer,
    Mode_ComputerVsComputer
};

void printUsage(void)
{
    cerr << "Usage: reversi-cpp <hc|cc> <search depth>" << endl;
    cerr << "hc for human vs computer, cc for computer vs computer." << endl;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        printUsage();
        exit(EXIT_FAILURE);
    }

    Mode gameMode;

    if (!strcmp(argv[1], "hc"))
    {
        gameMode = Mode_HumanVsComputer;
    }
    else if (!strcmp(argv[1], "cc"))
    {
        gameMode = Mode_ComputerVsComputer;
    }
    else
    {
        printUsage();
        exit(EXIT_FAILURE);
    }

    int p, m, c, s;
    int p2, m2, c2, s2;
    p = atoi(argv[3]);
    m = atoi(argv[4]);
    c = atoi(argv[5]);
    s = atoi(argv[6]);

    p2 = atoi(argv[7]);
    m2 = atoi(argv[8]);
    c2 = atoi(argv[9]);
    s2 = atoi(argv[10]);

    Gameplay game;

    Leyla leyla (atoi(argv[2]), Gameplay::Player_White, p, m, c, s);
    Leyla leyla2(atoi(argv[2]), Gameplay::Player_Black, p2, m2, c2, s2);

    bool humanPlayed = true;
    bool leylaPlayed = false;

    clear_screen();
    Board::Coordinates leylaCoordinates;
    cout << game;

    struct timespec tm, tm2;
    clock_gettime(CLOCK_MONOTONIC, &tm);
    while (!game.hasEnded())
    {
        if (gameMode == Mode_HumanVsComputer) {
            size_t row, col;
            char col_char;

            do {
                clear_screen();
                cout << game;

                if (!game.hasValidMoves())
                {
                    cout <<"Human has no valid moves!" << endl;
                    cout << game;
                    cin.ignore();
                    game.skip();
                    break;
                }

                if (!humanPlayed)
                {
                    cout << "Invalid move (" << row << ", " << col_char << ")! Try again." << endl;
                }

                if (leylaPlayed)
                {
                    cout << "Leyla played: (" << leylaCoordinates.first + 1 << ", " << char(leylaCoordinates.second + 'A') << ")." << endl;
                }
                cout << "Enter your move:" << endl;
                cin >> row >> col_char;
                col = toupper(col_char) - 'A';

                humanPlayed = game.play(Board::Coordinates(row - 1, col));
            }
            while (!humanPlayed);
            clear_screen();
            cout << game;
        }
        else if (gameMode == Mode_ComputerVsComputer)
        {
            if (!game.hasValidMoves())
            {
                cout << "Leyla2 has no valid moves." << endl;
                game.skip();
                continue;
            }
            cout << "Leyla2 is thinking ... " << endl;
            game.play(leylaCoordinates = leyla2.play(game));
            clear_screen();
            cout << game;
        }

        if (!game.hasValidMoves())
        {
            cout << "Leyla has no valid moves." << endl;
            cout << game;
            if (gameMode == Mode_HumanVsComputer)
            {
                cin.ignore();
            }
            game.skip();
            continue;
        }
        cout << "Leyla is thinking ... " << endl;
        game.play(leylaCoordinates = leyla.play(game));
        clear_screen();
        cout << game;

        leylaPlayed = true;

    }
    clock_gettime(CLOCK_MONOTONIC, &tm2);


    clear_screen();
    cerr << game;

    if (game.winner() == Gameplay::Player_White)
    {
        cerr << "White player wins!" << endl;
        return 1;
    }
    else if (game.winner() == Gameplay::Player_Black)
    {
        cerr << "Black wins!" << endl;
        return 2;
    }
    else
    {
        cerr << "DRAW!!!" << endl;
        return 0;
    }
    cerr << "elapsed: " << (tm2.tv_nsec - tm.tv_nsec) / 1000000 + ((tm2.tv_sec - tm.tv_sec) * 1000) << " milliseconds." << endl;

    return 0;
}
