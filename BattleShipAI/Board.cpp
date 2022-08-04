#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>

using namespace std;

class BoardImpl
{
public:
    BoardImpl(const Game& g);
    void clear();
    void block();
    void unblock();
    bool placeShip(Point topOrLeft, int shipId, Direction dir);
    bool unplaceShip(Point topOrLeft, int shipId, Direction dir);
    void display(bool shotsOnly) const;
    bool attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId);
    bool allShipsDestroyed() const;

private:
    // TODO:  Decide what private members you need.  Here's one that's likely
    //        to be useful:
    char grid[MAXROWS][MAXCOLS];
    const Game& m_game;
};


BoardImpl::BoardImpl(const Game& g) : m_game(g) //constructor
{
    for (int row = 0; row < m_game.rows(); row++)
    {
        for (int col = 0; col < m_game.cols(); col++)
        {
            grid[row][col] = '.';
        }
    }
}

void BoardImpl::clear()
{
    for (int row = 0; row < m_game.rows(); row++)
    {
        for (int col = 0; col < m_game.cols(); col++)
        {
            grid[row][col] = '.';
        }
    }
}

void BoardImpl::block()
{
    int count = 0;

    for (int row = 0; row < m_game.rows(); row++)
    {
        for (int col = 0; col < m_game.cols(); col++)
        {
            if (randInt(2) == 1)  //50% chance of being 0 or 1
            {
                grid[row][col] == '#';
                count++;
                if (count == ((m_game.rows() * m_game.cols()) / 2))
                {

                    return;
                }

            }
        }
    }

    if (count != ((m_game.rows() * m_game.cols()) / 2)) //if we dont replace half of the points, due to chance, just use recursion until it returns to escape void func
        block();
}

void BoardImpl::unblock()
{
    for (int row = 0; row < m_game.rows(); row++)
    {
        for (int col = 0; col < m_game.cols(); col++)
        {
            if (grid[row][col] == '#') {
                grid[row][col] = '.';
            }
        }
    }
}

bool BoardImpl::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    if (shipId < 0 || shipId > m_game.nShips() - 1) //1.) shipId is invalid
        return false;

    for (int row = 0; row < m_game.rows(); row++)  //checks if the symbol of that ship_type has already been placed
    {
        for (int col = 0; col < m_game.cols(); col++)
        {
            if (grid[row][col] == m_game.shipSymbol(shipId))
                return false;
        }
    }

    if (!m_game.isValid(topOrLeft)) //checks if row or col if position is less than 0 or greater than the row/col of game
        return false;


    if (dir == VERTICAL)
    {
        for (int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++)
        {
            if (i >= m_game.rows())
            {
                return false;
            }
            if (grid[i][topOrLeft.c] != '.')
                return false;

        }
        for (int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++)
        {
            grid[i][topOrLeft.c] = m_game.shipSymbol(shipId);
        }
    }

    else if (dir == HORIZONTAL)
    {
        for (int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++) //checks if its possible
        {

            if (i >= m_game.cols())
            {
                return false;
            }
            if (grid[topOrLeft.r][i] != '.')
                return false;

        }
        for (int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++)
        {
            grid[topOrLeft.r][i] = m_game.shipSymbol(shipId);

        }
    }

    return true;
}

bool BoardImpl::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    int count = 0;

    if (shipId < 0 || shipId > m_game.nShips() - 1) //1.) shipId is invalid
        return false;


    if (dir == VERTICAL)
    {
        for (int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++)
        {
            if (grid[i][topOrLeft.c] == m_game.shipSymbol(shipId))
                count++;

        }

        if (count != m_game.shipLength(shipId)) //2.) Board doesnt contain the entire ship
            return false;
        else
        {
            for (int i = topOrLeft.r; i < topOrLeft.r + m_game.shipLength(shipId); i++)
            {
                grid[i][topOrLeft.c] = '.';

            }
        }
        return true;
    }

    else if (dir == HORIZONTAL)
    {
        for (int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++)
        {
            if (grid[topOrLeft.r][i] == m_game.shipSymbol(shipId))
                count++;

        }

        if (count != m_game.shipLength(shipId))
            return false;
        else
        {
            for (int i = topOrLeft.c; i < topOrLeft.c + m_game.shipLength(shipId); i++)
            {
                grid[topOrLeft.r][i] = '.';

            }
            return true;
        }
    }

}

void BoardImpl::display(bool shotsOnly) const
{
    cout << "  ";
    for (int i = 0; i < m_game.cols(); i++)
    {
        cout << i;
    }
    cout << endl;

    if (!shotsOnly)
    {
        for (int i = 0; i < m_game.rows(); i++)
        {
            cout << i << " ";
            for (int j = 0; j < m_game.cols(); j++)
            {
                if (j == m_game.cols() - 1)
                    cout << grid[i][j] << endl;
                else
                    cout << grid[i][j];
            }
        }

    }
    else
    {
        for (int i = 0; i < m_game.rows(); i++)
        {
            cout << i << " ";
            for (int j = 0; j < m_game.cols(); j++)
            {
                if (grid[i][j] != '.' && grid[i][j] != 'X' && grid[i][j] != 'o' && j == m_game.cols() - 1)
                    cout << '.' << endl;
                else if (grid[i][j] != '.' && grid[i][j] != 'X' && grid[i][j] != 'o')
                    cout << '.';
                else if (j == m_game.cols() - 1)
                    cout << grid[i][j] << endl;
                else
                    cout << grid[i][j];
            }
        }
    }
}

bool BoardImpl::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    if (!m_game.isValid(p)) //the attack point is outside of the board area : return false
        return false;

    int ship_there = 0;

    if (grid[p.r][p.c] == 'X' || grid[p.r][p.c] == 'o') //or an attack is made on a previously attacked location : return false
    {
        shotHit = false;
        return false;
    }

    if (grid[p.r][p.c] != '.' && grid[p.r][p.c] != '#' && grid[p.r][p.c] != 'X' && grid[p.r][p.c] != 'o') //checks if there is a ship at that point
    {
        shotHit = true;
        char temp = grid[p.r][p.c];
        grid[p.r][p.c] = 'X';



        for (int row = 0; row < m_game.rows(); row++) //checks if ship is destroyed
        {
            for (int col = 0; col < m_game.cols(); col++)
            {
                if (grid[row][col] == temp)
                {
                    ship_there = 1;

                }
            }
        }

        if (ship_there == 1)
        {
            shipDestroyed = false;

        }
        else
        {
            shipDestroyed = true;
            for (int i = 0; i < m_game.nShips(); i++)  //finds ship id
            {
                if (m_game.shipSymbol(i) == temp)
                {
                    shipId = i;
                }

            }
        }

        return true;
    }
    else
    {
        shotHit = false;
        if (grid[p.r][p.c] == '.')  //if its a sea, then it will become 'o' to indicate that its been attacked
        {
            grid[p.r][p.c] = 'o';
        }

        return true;


    }

}

bool BoardImpl::allShipsDestroyed() const
{
    for (int row = 0; row < m_game.rows(); row++)
    {
        for (int col = 0; col < m_game.cols(); col++)
        {
            if (grid[row][col] != '#' && grid[row][col] != 'o' && grid[row][col] != 'X' && grid[row][col] != '.')
                return false;
        }
    }
    return true;
}

//******************** Board functions ********************************

// These functions simply delegate to BoardImpl's functions.
// You probably don't want to change any of this code.

Board::Board(const Game& g)
{
    m_impl = new BoardImpl(g);
}

Board::~Board()
{
    delete m_impl;
}

void Board::clear()
{
    m_impl->clear();
}

void Board::block()
{
    return m_impl->block();
}

void Board::unblock()
{
    return m_impl->unblock();
}

bool Board::placeShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->placeShip(topOrLeft, shipId, dir);
}

bool Board::unplaceShip(Point topOrLeft, int shipId, Direction dir)
{
    return m_impl->unplaceShip(topOrLeft, shipId, dir);
}

void Board::display(bool shotsOnly) const
{
    m_impl->display(shotsOnly);
}

bool Board::attack(Point p, bool& shotHit, bool& shipDestroyed, int& shipId)
{
    return m_impl->attack(p, shotHit, shipDestroyed, shipId);
}

bool Board::allShipsDestroyed() const
{
    return m_impl->allShipsDestroyed();
}
