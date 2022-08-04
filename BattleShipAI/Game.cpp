#include "Game.h"
#include "Board.h"
#include "Player.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cctype>

#include <vector>
using namespace std;

class GameImpl
{
public:
    GameImpl(int nRows, int nCols);
    int rows() const;
    int cols() const;
    bool isValid(Point p) const;
    Point randomPoint() const;
    bool addShip(int length, char symbol, string name);
    int nShips() const;
    int shipLength(int shipId) const;
    char shipSymbol(int shipId) const;
    string shipName(int shipId) const;
    Player* play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause);

private:
    int my_row, my_col;

    struct ship_type
    {
        string ship_name;
        char ship_symbol;
        int ship_length;
    };

    vector<ship_type> ship_arr;
};

void waitForEnter()
{
    cout << "Press enter to continue: ";
    cin.ignore(10000, '\n');
}

GameImpl::GameImpl(int nRows, int nCols)
{
    if (nRows <= MAXROWS && nCols <= MAXCOLS)
    {
        my_row = nRows;
        my_col = nCols;
    }
}

int GameImpl::rows() const
{
    return my_row;
}

int GameImpl::cols() const
{
    return my_col;
}

bool GameImpl::isValid(Point p) const
{
    return p.r >= 0 && p.r < rows() && p.c >= 0 && p.c < cols();
}

Point GameImpl::randomPoint() const
{
    return Point(randInt(rows()), randInt(cols()));
}

bool GameImpl::addShip(int length, char symbol, string name)
{
    if (length < 0 || length > rows() || length > cols())
        return false;

    ship_type new_ship;
    new_ship.ship_name = name;
    new_ship.ship_symbol = symbol;
    new_ship.ship_length = length;

    ship_arr.push_back(new_ship);
    return true;

}

int GameImpl::nShips() const
{

    return ship_arr.size();
}

int GameImpl::shipLength(int shipId) const
{
    return ship_arr[shipId].ship_length;
}

char GameImpl::shipSymbol(int shipId) const
{
    return ship_arr[shipId].ship_symbol;
}

string GameImpl::shipName(int shipId) const
{
    return ship_arr[shipId].ship_name;
}

Player* GameImpl::play(Player* p1, Player* p2, Board& b1, Board& b2, bool shouldPause)
{


    if (!p1->placeShips(b1))
        return nullptr;
    if (!p2->placeShips(b2))
        return nullptr;

    Point p;

    while (!b1.allShipsDestroyed() && !b2.allShipsDestroyed())
    {
        int shipId = 0;
        bool isHit = false;
        bool isDestroyed = false;
        bool isValidShot = false;



        cout << p1->name() << "'s turn.  Board for " << p2->name() << ":" << endl;
        b2.display(p1->isHuman()); //a.)

        p = p1->recommendAttack();

        isValidShot = b2.attack(p, isHit, isDestroyed, shipId);

        p1->recordAttackResult(p, isValidShot, isHit, isDestroyed, shipId);


        if (p1->isHuman() && !isValid(p))
            cout << p1->name() << " wasted a shot at (" << p.r << "," << p.c << ")." << endl;
        else
        {
            cout << p1->name() << " attacked (" << p.r << "," << p.c << ") and ";

            if (isHit && isDestroyed)
                cout << "destroyed the " << shipName(shipId);
            else if (isHit)
                cout << "hit something";
            else
                cout << "missed";
            cout << ", resulting in:" << endl;

            b2.display(p1->isHuman());
        }


        if (b1.allShipsDestroyed() || b2.allShipsDestroyed())
            break;


        if (shouldPause)
            waitForEnter();

        shipId = 0;
        isHit = false;
        isDestroyed = false;
        isValidShot = false;


        // Display second player's board
        cout << p2->name() << "'s turn.  Board for " << p1->name() << ":" << endl;
        b1.display(p2->isHuman()); //a.)

        p = p2->recommendAttack();

        isValidShot = b1.attack(p, isHit, isDestroyed, shipId);

        p2->recordAttackResult(p, isValidShot, isHit, isDestroyed, shipId);

        // display the result of attack
        if (p2->isHuman() && !isValid(p))
            cout << p2->name() << " wasted a shot at (" << p.r << "," << p.c << ")." << endl;
        else
        {
            cout << p2->name() << " attacked (" << p.r << "," << p.c << ") and ";

            if (isHit && isDestroyed)
                cout << "destroyed the " << shipName(shipId);
            else if (isHit)
                cout << "hit something";
            else
                cout << "missed";
            cout << ", resulting in:" << endl;

            b1.display(p2->isHuman());
        }

        if (b1.allShipsDestroyed() || b2.allShipsDestroyed())
            break;

        if (shouldPause)
            waitForEnter();
    }

    if (b1.allShipsDestroyed() && !p1->isHuman()) //if loser isnt human
    {
        cout << p2->name() << " wins!" << endl;
        return p2;
    }

    else if (b1.allShipsDestroyed() && p1->isHuman()) //if  loser is human
    {
        cout << p2->name() << " wins!" << endl;
        b2.display(false);
        return p2;
    }

    else if (b2.allShipsDestroyed() && !p2->isHuman()) //if loser isnt human
    {
        cout << p1->name() << " wins!" << endl;
        return p1;
    }
    else if (b2.allShipsDestroyed() && p2->isHuman()) //if  loser is human
    {
        cout << p1->name() << " wins!" << endl;
        b1.display(false);

        return p1;
    }


}

//******************** Game functions *******************************

// These functions for the most part simply delegate to GameImpl's functions.
// You probably don't want to change any of the code from this point down.

Game::Game(int nRows, int nCols)
{
    if (nRows < 1 || nRows > MAXROWS)
    {
        cout << "Number of rows must be >= 1 and <= " << MAXROWS << endl;
        exit(1);
    }
    if (nCols < 1 || nCols > MAXCOLS)
    {
        cout << "Number of columns must be >= 1 and <= " << MAXCOLS << endl;
        exit(1);
    }
    m_impl = new GameImpl(nRows, nCols);
}

Game::~Game()
{
    delete m_impl;
}

int Game::rows() const
{
    return m_impl->rows();
}

int Game::cols() const
{
    return m_impl->cols();
}

bool Game::isValid(Point p) const
{
    return m_impl->isValid(p);
}

Point Game::randomPoint() const
{
    return m_impl->randomPoint();
}

bool Game::addShip(int length, char symbol, string name)
{
    if (length < 1)
    {
        cout << "Bad ship length " << length << "; it must be >= 1" << endl;
        return false;
    }
    if (length > rows() && length > cols())
    {
        cout << "Bad ship length " << length << "; it won't fit on the board"
            << endl;
        return false;
    }
    if (!isascii(symbol) || !isprint(symbol))
    {
        cout << "Unprintable character with decimal value " << symbol
            << " must not be used as a ship symbol" << endl;
        return false;
    }
    if (symbol == 'X' || symbol == '.' || symbol == 'o')
    {
        cout << "Character " << symbol << " must not be used as a ship symbol"
            << endl;
        return false;
    }
    int totalOfLengths = 0;
    for (int s = 0; s < nShips(); s++)
    {
        totalOfLengths += shipLength(s);
        if (shipSymbol(s) == symbol)
        {
            cout << "Ship symbol " << symbol
                << " must not be used for more than one ship" << endl;
            return false;
        }
    }
    if (totalOfLengths + length > rows() * cols())
    {
        cout << "Board is too small to fit all ships" << endl;
        return false;
    }
    return m_impl->addShip(length, symbol, name);
}

int Game::nShips() const
{
    return m_impl->nShips();
}

int Game::shipLength(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipLength(shipId);
}

char Game::shipSymbol(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipSymbol(shipId);
}

string Game::shipName(int shipId) const
{
    assert(shipId >= 0 && shipId < nShips());
    return m_impl->shipName(shipId);
}

Player* Game::play(Player* p1, Player* p2, bool shouldPause)
{
    if (p1 == nullptr || p2 == nullptr || nShips() == 0)
        return nullptr;
    Board b1(*this);
    Board b2(*this);
    return m_impl->play(p1, p2, b1, b2, shouldPause);
}

