#include "Player.h"
#include "Board.h"
#include "Game.h"
#include "globals.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;
class Timer
{
public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
            std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
private:
    std::chrono::high_resolution_clock::time_point m_time;
};
//*********************************************************************
//  AwfulPlayer
//*********************************************************************

class AwfulPlayer : public Player
{
public:
    AwfulPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit,
        bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
private:
    Point m_lastCellAttacked;
};

AwfulPlayer::AwfulPlayer(string nm, const Game& g)
    : Player(nm, g), m_lastCellAttacked(0, 0)
{}

bool AwfulPlayer::placeShips(Board& b)
{
    // Clustering ships is bad strategy
    for (int k = 0; k < game().nShips(); k++)
        if (!b.placeShip(Point(k, 0), k, HORIZONTAL))
            return false;
    return true;
}

Point AwfulPlayer::recommendAttack()
{
    if (m_lastCellAttacked.c > 0)
        m_lastCellAttacked.c--;
    else
    {
        m_lastCellAttacked.c = game().cols() - 1;
        if (m_lastCellAttacked.r > 0)
            m_lastCellAttacked.r--;
        else
            m_lastCellAttacked.r = game().rows() - 1;
    }
    return m_lastCellAttacked;
}

void AwfulPlayer::recordAttackResult(Point /* p */, bool /* validShot */, bool /* shotHit */, bool /* shipDestroyed */, int /* shipId */)
{
    // AwfulPlayer completely ignores the result of any attack
}

void AwfulPlayer::recordAttackByOpponent(Point /* p */)
{
    // AwfulPlayer completely ignores what the opponent does
}

//*********************************************************************
//  HumanPlayer
//*********************************************************************
bool getLineWithTwoIntegers(int& r, int& c)
{
    bool result(cin >> r >> c);
    if (!result)
        cin.clear();  // clear error state so can do more input operations
    cin.ignore(10000, '\n');
    return result;
}

class HumanPlayer : public Player
{
public:
    HumanPlayer(string nm, const Game& g) : Player(nm, g) { }
    virtual bool isHuman() const { return true; }
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);

private:

};

//The placeShips and recommendAttack functions must prompt the user for
//where to place the ships and where to attack
bool HumanPlayer::placeShips(Board& b)
{
    cout << name() << " the Human must place " << game().nShips() << " ships." << endl;
    b.display(false);

    int count = 0;
    string hor_ver;
    int row, col;
    Point topOrleft;

    for (int i = 0; i < game().nShips(); i++)
    {
        cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
        cin >> hor_ver;

        while (hor_ver != "h" && hor_ver != "v") //hor_ver is string to account when inputting letters that arent h or v and have 1 char long string
        {
            cout << "Direction must be h or v." << endl;
            cout << "Enter h or v for direction of " << game().shipName(i) << " (length " << game().shipLength(i) << "): ";
            cin.ignore(10000, '\n') >> hor_ver; //removes error where you input more than 1 word 

        }


        if (hor_ver == "v")
        {
            cout << "Enter row and column of topmost cell (e.g., 3 5): ";
            if (getLineWithTwoIntegers(row, col))
            {
                topOrleft.r = row;
                topOrleft.c = col;
            }
            else {
                cout << "You must enter two integers." << endl;
                cout << "Enter row and column of topmost cell (e.g., 3 5): ";

                while (!getLineWithTwoIntegers(row, col))
                {
                    cout << "You must enter two integers." << endl;
                    cout << "Enter row and column of topmost cell (e.g., 3 5): ";

                }
                topOrleft.r = row;
                topOrleft.c = col;

            }

        }

        else if (hor_ver == "h")
        {
            cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
            if (getLineWithTwoIntegers(row, col))
            {
                topOrleft.r = row;
                topOrleft.c = col;
            }
            else {
                cout << "You must enter two integers." << endl;
                cout << "Enter row and column of leftmost cell (e.g., 3 5): ";

                while (!getLineWithTwoIntegers(row, col))
                {
                    cout << "You must enter two integers." << endl;
                    cout << "Enter row and column of leftmost cell (e.g., 3 5): ";

                }
                topOrleft.r = row;
                topOrleft.c = col;

            }
        }

        if (hor_ver == "h")
        {
            if (!b.placeShip(topOrleft, i, HORIZONTAL))
            {
                while (!b.placeShip(topOrleft, i, HORIZONTAL))
                {

                    cout << "The ship can not be place there." << endl;
                    cout << "Enter row and column of leftmost cell (e.g., 3 5): ";
                    if (getLineWithTwoIntegers(row, col))
                    {
                        topOrleft.r = row;
                        topOrleft.c = col;
                    }

                }

            }


        }
        else if (hor_ver == "v")
        {
            if (!b.placeShip(topOrleft, i, VERTICAL))
            {
                while (!b.placeShip(topOrleft, i, VERTICAL))
                {
                    cout << "The ship can not be place there." << endl;
                    cout << "Enter row and column of topmost cell (e.g., 3 5): ";
                    if (getLineWithTwoIntegers(row, col))
                    {
                        topOrleft.r = row;
                        topOrleft.c = col;
                    }


                }
            }

        }
        if (i == game().nShips() - 1)
        {
            //do nothing
        }
        else
            b.display(false);
    }
    return true;
}

Point HumanPlayer::recommendAttack()
{
    Point pos;

    int r = -3;
    int c = -3;
    cout << "Enter the row and column to attack (e.g, 3 5):";

    while (!getLineWithTwoIntegers(r, c))
    {
        cout << "You must enter two integers." << endl;
        cout << "Enter the row and column to attack (e.g, 3 5):";

    }

    pos.r = r;
    pos.c = c;

    return pos;
}

void HumanPlayer::recordAttackResult(Point /* p */, bool /* validShot */, bool /* shotHit */, bool /* shipDestroyed */, int /* shipId */)
{
    //do nothing
}

void HumanPlayer::recordAttackByOpponent(Point /* p */)
{
    //do nothing
}








//*********************************************************************
//  MediocrePlayer
//*********************************************************************
class MediocrePlayer : public Player
{
public:
    MediocrePlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    bool checker(Board& b, int shipId, Point p);
    Point state2_attack();
    Point state1_attack();

    bool checkDir(int id, int pos_of_back);
private:
    int state;
    Point position_of_transition;
    char grid_his[MAXROWS][MAXCOLS];
    vector<Point> points; //to backtrack 

};

MediocrePlayer::MediocrePlayer(string nm, const Game& g) : Player(nm, g), state(1), position_of_transition(0, 0) {
    for (int row = 0; row < game().rows(); row++)
    {
        for (int col = 0; col < game().cols(); col++)
        {
            grid_his[row][col] = '.';
        }
    }
}
bool MediocrePlayer::checker(Board& b, int shipId, Point p) //checks every collumn of row whether ship can be place vertically or horizontally
{
    if (randInt(2) == 1)  //50% chance of first checking horizontal direction vs vertical direction
    {
        if (b.placeShip(p, shipId, HORIZONTAL))
        {
            for (int i = 0; i < points.size(); i++)  //checks if that point has already been visited when backtracking
            {
                if (p.r == points[i].r && p.c == points[i].c)
                {
                    b.unplaceShip(p, shipId, HORIZONTAL);
                    p.c += 1;
                    return checker(b, shipId, p);
                }
            }
            return true;
        }


        if (b.placeShip(p, shipId, VERTICAL))
        {
            for (int i = 0; i < points.size(); i++)
            {
                if (p.r == points[i].r && p.c == points[i].c)
                {
                    b.unplaceShip(p, shipId, VERTICAL);
                    p.c += 1;
                    return checker(b, shipId, p);
                }
            }
            return true;
        }
    }
    else
    {
        if (b.placeShip(p, shipId, VERTICAL))
        {
            for (int i = 0; i < points.size(); i++)
            {
                if (p.r == points[i].r && p.c == points[i].c)
                {
                    b.unplaceShip(p, shipId, VERTICAL);
                    p.c += 1;
                    return checker(b, shipId, p);
                }
            }
            return true;
        }
        else if (b.placeShip(p, shipId, HORIZONTAL))
        {
            for (int i = 0; i < points.size(); i++)  //checks if that point has already been visited when backtracking
            {
                if (p.r == points[i].r && p.c == points[i].c)
                {
                    b.unplaceShip(p, shipId, HORIZONTAL);
                    p.c += 1;
                    return checker(b, shipId, p);
                }
            }
            return true;
        }
    }


    if (p.c == game().cols() - 1)       //if we never return true, and reached the last col, then not possible to fit in that row.
        return false;

    p.c += 1;
    return checker(b, shipId, p);
}

bool MediocrePlayer::placeShips(Board& b) //double check
{

    int num_of_tries = 0;
    int num_of_tries_for_1_block = 0;
    Point p;
    int pos_of_back = 0;
    int i = 0;
    while (num_of_tries < 50)
    {
        p.r = 0;
        p.c = 0;

        b.block(); //1
        while (i < game().nShips() && num_of_tries_for_1_block < 50)
        {
            if (checker(b, i, p))
            {
                points.push_back(p); //keeps track of which position has been already been visited
                pos_of_back++;
                if (i == game().nShips() - 1) //sucessfully added all ships
                    return true;

                p.r = 0;

                i++;
            }
            else
            {
                if (p.r == game().rows() - 1) //if it reaches the end of the row, then its impossible to add
                {
                    num_of_tries_for_1_block++;
                    i--;
                    pos_of_back--;

                    if (checkDir(i, pos_of_back)) //check if its hori,verti
                        b.unplaceShip(points[pos_of_back], i, VERTICAL);
                    else
                        b.unplaceShip(points[pos_of_back], i, HORIZONTAL);

                    //resets row back to 0 so we can check every single position again starting from row 0
                    p.r = 0;

                }
                else {
                    p.r += 1;

                }

            }
        }

        num_of_tries++;
        b.unblock();
    }

    return false;


}


void MediocrePlayer::recordAttackByOpponent(Point p)
{
    //do nothing
}

Point MediocrePlayer::state1_attack()
{
    Point temp;
    temp = game().randomPoint();
    if (grid_his[temp.r][temp.c] == '.')
    {
        return temp;
    }
    else
        return state1_attack();
}


Point MediocrePlayer::state2_attack()
{
    Point temp;
    for (int row = position_of_transition.r - 4; row < position_of_transition.r + 4; row++)
    {
        if (row >= 0 && row < game().rows() && grid_his[row][position_of_transition.c] == '.')
        {
            temp.r = row;
            temp.c = position_of_transition.c;
            return temp;
        }

    }
    for (int col = position_of_transition.c - 4; col < position_of_transition.c + 4; col++)
    {
        if (col >= 0 && col < game().cols() && grid_his[position_of_transition.r][col] == '.')
        {
            temp.c = col;
            temp.r = position_of_transition.r;
            return temp;
        }

    }
    state = 1;
    return recommendAttack(); //if there is no open position 4 pts away from position_of_transition, then state=1 and it will return attack1
}

Point MediocrePlayer::recommendAttack() //return a point that indicates the position on the opp board taht player would like to attack next
{

    if (state == 1)
    {
        return state1_attack();
    }
    else if (state == 2)
    {
        return state2_attack();
    }
}
void MediocrePlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{

    if (state == 1)
    {
        if (!shotHit) //a.) if shot missed
        {
            grid_his[p.r][p.c] = 'o';
        }
        else if (shotHit && shipDestroyed) //b.) if shot hit and destroyed ship
        {
            grid_his[p.r][p.c] = 'X';
        }
        else if (shotHit && !shipDestroyed)//c.) if shot hit and but didnt destroy ship
        {
            grid_his[p.r][p.c] = 'X';
            state = 2;
            position_of_transition.r = p.r;
            position_of_transition.c = p.c;
        }


    }
    else if (state == 2)
    {
        if (!shotHit)
        {
            grid_his[p.r][p.c] = 'o';
        }
        else if (shotHit && !shipDestroyed)
        {
            grid_his[p.r][p.c] = 'X';
        }
        else if (shotHit && shipDestroyed)
        {
            grid_his[p.r][p.c] = 'X';
            state = 1;
        }
    }
}


bool MediocrePlayer::checkDir(int id, int pos_of_back)
{
    int row = points[pos_of_back].r + 1;
    int col = points[pos_of_back].c + 1;
    if (row == game().shipSymbol(id))
    {
        return true; //aka vertical
    }
    else if (col == game().shipSymbol(id))
    {
        return false; //aka horizontal
    }
    else {
        return false;  //doesnt matter if size is 1
    }

}


//*********************************************************************
//  GoodPlayer
//*********************************************************************
class GoodPlayer : public Player
{
public:
    GoodPlayer(string nm, const Game& g);
    virtual bool placeShips(Board& b);
    virtual Point recommendAttack();
    virtual void recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId);
    virtual void recordAttackByOpponent(Point p);
    bool checker(Board& b, int shipId, Point p);
    Point state4_attack();
    Point state3_attack();
    Point state2_attack();
    Point state1_attack();
    Point state0_attack();

    bool checkDir(int id, int pos_of_back);
private:
    int count;
    int vert_or_hor;
    int state;
    Point position_of_transition;
    Point og;

    char grid_his[MAXROWS][MAXCOLS];
    vector<Point> points; //to backtrack 

};
bool GoodPlayer::checkDir(int id, int pos_of_back)
{
    int row = points[pos_of_back].r + 1;
    int col = points[pos_of_back].c + 1;
    if (row == game().shipSymbol(id))
    {
        return true; //aka vertical
    }
    else if (col == game().shipSymbol(id))
    {
        return false; //aka horizontal
    }
    else {
        return false;  //doesnt matter if size is 1
    }

}
GoodPlayer::GoodPlayer(string nm, const Game& g) : Player(nm, g), state(1), position_of_transition(0, 0), vert_or_hor(0), count(0)
{
    for (int row = 0; row < game().rows(); row++)
    {
        for (int col = 0; col < game().cols(); col++)
        {
            grid_his[row][col] = '.';
        }
    }
}
bool GoodPlayer::checker(Board& b, int shipId, Point p)
{
    if (randInt(2) == 1)  //50% chance of first checking horizontal direction vs vertical direction
    {
        if (b.placeShip(p, shipId, HORIZONTAL))
        {
            for (int i = 0; i < points.size(); i++)  //checks if that point has already been visited when backtracking
            {
                if (p.r == points[i].r && p.c == points[i].c)
                {
                    b.unplaceShip(p, shipId, HORIZONTAL);
                    p.c += 1;
                    return checker(b, shipId, p);
                }
            }
            return true;
        }


        if (b.placeShip(p, shipId, VERTICAL))
        {
            for (int i = 0; i < points.size(); i++)
            {
                if (p.r == points[i].r && p.c == points[i].c)
                {
                    b.unplaceShip(p, shipId, VERTICAL);
                    p.c += 1;
                    return checker(b, shipId, p);
                }
            }
            return true;
        }
    }
    else
    {
        if (b.placeShip(p, shipId, VERTICAL))
        {
            for (int i = 0; i < points.size(); i++)
            {
                if (p.r == points[i].r && p.c == points[i].c)
                {
                    b.unplaceShip(p, shipId, VERTICAL);
                    p.c += 1;
                    return checker(b, shipId, p);
                }
            }
            return true;
        }
        else if (b.placeShip(p, shipId, HORIZONTAL))
        {
            for (int i = 0; i < points.size(); i++)  //checks if that point has already been visited when backtracking
            {
                if (p.r == points[i].r && p.c == points[i].c)
                {
                    b.unplaceShip(p, shipId, HORIZONTAL);
                    p.c += 1;
                    return checker(b, shipId, p);
                }
            }
            return true;
        }
    }


    if (p.c == game().cols() - 1)       //if we never return true, and reached the last col, then not possible to fit in that row.
        return false;

    p.c += 1;
    return checker(b, shipId, p);
}

bool GoodPlayer::placeShips(Board& b) //double check
{

    int num_of_tries = 0;
    int num_of_tries_for_1_block = 0;
    Point p;
    int pos_of_back = 0;
    int i = 0;
    while (num_of_tries < 50)
    {
        p.r = 0;
        p.c = 0;

        b.block(); //1
        while (i < game().nShips() && num_of_tries_for_1_block < 50)
        {
            if (checker(b, i, p))
            {
                points.push_back(p); //keeps track of which position has been already been visited
                pos_of_back++;
                if (i == game().nShips() - 1) //sucessfully added all ships
                    return true;

                p.r = 0;

                i++;
            }
            else
            {
                if (p.r == game().rows() - 1) //if it reaches the end of the row, then its impossible to add
                {
                    num_of_tries_for_1_block++;
                    i--;
                    pos_of_back--;

                    if (checkDir(i, pos_of_back)) //check if its hori,verti
                        b.unplaceShip(points[pos_of_back], i, VERTICAL);
                    else
                        b.unplaceShip(points[pos_of_back], i, HORIZONTAL);

                    //resets row back to 0 so we can check every single position again starting from row 0
                    p.r = 0;

                }
                else {
                    p.r += 1;

                }

            }
        }

        num_of_tries++;
        b.unblock();
    }

    return false;


}

void GoodPlayer::recordAttackByOpponent(Point p)
{
    //do nothing
}

Point GoodPlayer::state0_attack() //gets random point
{
    Point temp;
    temp = game().randomPoint();
    if (grid_his[temp.r][temp.c] == '.')
    {
        return temp;
    }
    else
        return state0_attack();
}

Point GoodPlayer::state1_attack() //abuses mediocre player placements
{
    Point temp;
    for (int row = 0; row < game().rows(); row++)
    {
        for (int col = 0; col < game().cols(); col++)
        {

            if (grid_his[row][col] == '.')
            {
                temp.r = row;
                temp.c = col;
                return temp;
            }
        }
    }

}


Point GoodPlayer::state2_attack() //checks surrounding positions
{
    Point temp;


    if (grid_his[og.r + 1][og.c] == '.')
    {
        temp.r = og.r + 1;
        temp.c = og.c;
        vert_or_hor = 11; //11 = vertical down
        return temp;
    }
    else if (grid_his[og.r - 1][og.c] == '.')
    {
        temp.r = og.r - 1;
        temp.c = og.c;
        vert_or_hor = 12; //12 = vertical up
        return temp;
    }

    else if (grid_his[og.r][og.c + 1] == '.')
    {
        temp.r = og.r;
        temp.c = og.c + 1;
        vert_or_hor = 21; //21 = horizontal to the right
        return temp;
    }
    else if (grid_his[og.r][og.c - 1] == '.')
    {
        temp.r = og.r;
        temp.c = og.c - 1;
        vert_or_hor = 22; //22 = horizontal to the left
        return temp;
    }

    // checks the new positions left or right or up or down if state3 seems to fail and there is no legal surrounding positions of og
    if (grid_his[position_of_transition.r + 1][position_of_transition.c] == '.')
    {
        temp.r = position_of_transition.r + 1;
        temp.c = position_of_transition.c;
        vert_or_hor = 11; //11 = vertical down
        return temp;
    }
    else if (grid_his[position_of_transition.r - 1][position_of_transition.c] == '.')
    {
        temp.r = position_of_transition.r - 1;
        temp.c = position_of_transition.c;
        vert_or_hor = 12; //12 = vertical up
        return temp;
    }

    else if (grid_his[position_of_transition.r][position_of_transition.c + 1] == '.')
    {
        temp.r = position_of_transition.r;
        temp.c = position_of_transition.c + 1;
        vert_or_hor = 21; //21 = horizontal to the right
        return temp;
    }
    else if (grid_his[position_of_transition.r][position_of_transition.c - 1] == '.')
    {
        temp.r = position_of_transition.r;
        temp.c = position_of_transition.c - 1;
        vert_or_hor = 22; //22 = horizontal to the left
        return temp;
    }
    //if all else fails then just return a random positions thats legal
    state = 1;
    return state1_attack();
}

Point GoodPlayer::recommendAttack() //return a point that indicates the position on the opp board taht player would like to attack next
{

    if (state == 1)
    {
        return state1_attack();
    }
    else if (state == 2)
    {
        return state2_attack();
    }
    else if (state == 3)
    {
        return state2_attack();
    }
    else if (state == 4)
    {
        return state4_attack();
    }

}

Point GoodPlayer::state3_attack() //vertical
{
    Point temp;

    if (vert_or_hor == 11) //new pos is down
    {
        if (grid_his[position_of_transition.r + 1][position_of_transition.c] == '.')
        {
            temp.r = position_of_transition.r + 1;
            temp.c = position_of_transition.c;
            return temp;
        }
        else if (grid_his[og.r - 1][og.c] == '.')
        {
            temp.r = og.r - 1;
            temp.c = og.c;
            return temp;
        }
    }
    else if (vert_or_hor == 12)
    {
        if (grid_his[position_of_transition.r - 1][position_of_transition.c] == '.')
        {
            temp.r = position_of_transition.r - 1;
            temp.c = position_of_transition.c;
            return temp;
        }
        else if (grid_his[og.r + 1][og.c] == '.')
        {
            temp.r = og.r + 1;
            temp.c = og.c;
            return temp;
        }
    }

    vert_or_hor = 0;
    state = 2;
    return state2_attack();
}


Point GoodPlayer::state4_attack() //vertical
{
    Point temp;

    if (vert_or_hor == 21) //hori to the right
    {
        if (grid_his[position_of_transition.r][position_of_transition.c + 1] == '.')
        {
            temp.r = position_of_transition.r;
            temp.c = position_of_transition.c + 1;
            return temp;
        }
        else if (grid_his[og.r][og.c - 1] == '.')
        {
            temp.r = og.r;
            temp.c = og.c - 1;
            return temp;
        }
    }
    else if (vert_or_hor == 22)
    {
        if (grid_his[position_of_transition.r][position_of_transition.c - 1] == '.')
        {
            temp.r = position_of_transition.r;
            temp.c = position_of_transition.c - 1;
            return temp;
        }
        else if (grid_his[og.r][og.c + 1] == '.')
        {
            temp.r = og.r;
            temp.c = og.c + 1;
            return temp;
        }
    }
    vert_or_hor = 0;
    state = 2;
    return state2_attack();

}

void GoodPlayer::recordAttackResult(Point p, bool validShot, bool shotHit, bool shipDestroyed, int shipId)
{

    if (state == 1)
    {
        if (!shotHit) //a.) if shot missed
        {
            grid_his[p.r][p.c] = 'o';
        }
        else if (shotHit && shipDestroyed) //b.) if shot hit and destroyed ship
        {
            grid_his[p.r][p.c] = 'X';
        }
        else if (shotHit && !shipDestroyed)//c.) if shot hit and but didnt destroy ship
        {
            grid_his[p.r][p.c] = 'X';
            state = 2;
            og.r = p.r;
            og.c = p.c;

        }


    }
    else if (state == 2)
    {
        if (!shotHit)
        {
            grid_his[p.r][p.c] = 'o';



        }
        else if (shotHit && !shipDestroyed)
        {
            grid_his[p.r][p.c] = 'X';
            position_of_transition.r = p.r;
            position_of_transition.c = p.c;

            if (vert_or_hor == 12 || vert_or_hor == 11)
            {
                state = 3;
            }
            else if (vert_or_hor == 21 || vert_or_hor == 22)
                state = 4;

        }
        else if (shotHit && shipDestroyed)
        {
            grid_his[p.r][p.c] = 'X';
            state = 1;
        }
    }
    else if (state == 3) //vertical
    {
        if (!shotHit)
        {
            grid_his[p.r][p.c] = 'o';


        }
        else if (shotHit && !shipDestroyed)
        {
            grid_his[p.r][p.c] = 'X';
            position_of_transition.r = p.r;
            position_of_transition.c = p.c;


        }
        else if (shotHit && shipDestroyed)
        {
            grid_his[p.r][p.c] = 'X';
            vert_or_hor = 0;
            state = 1;
        }
    }
    else if (state == 4) //vertical
    {
        if (!shotHit)
        {
            grid_his[p.r][p.c] = 'o';


        }
        else if (shotHit && !shipDestroyed)
        {
            grid_his[p.r][p.c] = 'X';
            position_of_transition.r = p.r;
            position_of_transition.c = p.c;


        }
        else if (shotHit && shipDestroyed)
        {
            grid_his[p.r][p.c] = 'X';
            vert_or_hor = 0;
            state = 1;
        }
    }

}



//*********************************************************************
//  createPlayer
//*********************************************************************


Player* createPlayer(string type, string nm, const Game& g)
{
    static string types[] = {
        "human", "awful", "mediocre", "good"
    };

    int pos;
    for (pos = 0; pos != sizeof(types) / sizeof(types[0]) &&
        type != types[pos]; pos++)
        ;
    switch (pos)
    {
    case 0:  return new HumanPlayer(nm, g);
    case 1:  return new AwfulPlayer(nm, g);
    case 2:  return new MediocrePlayer(nm, g);
    case 3:  return new GoodPlayer(nm, g);
    default: return nullptr;
    }
}