/*This program recreats Conway's game of life https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life
Initial inputs are the board dimensions and the initial alive speciments
A=main; g++ -std=c++17 -o $A $A.cpp -lncurses  && ./$A
*/

#include <iostream>
#include <random>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <algorithm>
#include <unistd.h>
#include <term.h>
//#include </usr/include/eigen3/Eigen/Dense>
using namespace std;

const int board_size = 1000;
int initial_alive = 500;
bool printdebug=false;
int board_dims = int((sqrt(board_size)));


class GameOfLife{
    public: void ClearScreen(){
    if (!cur_term)
        {
        int result;
        setupterm( NULL, STDOUT_FILENO, &result );
        if (result <= 0) return;
        }

    putp( tigetstr( "clear" ) );
    }
    void wait_until_next_second()
    {
        time_t before = time(0);
        while (difftime(time(0), before) < 1)
            ;
    }

    public: void PrintBoard(vector<vector<int>> playing_space,int tim){
    int current_population=0;
        {
            for (int y = 0; y < board_dims; y++)
            {
                for (int x = 0; x < board_dims; x++)
                {
                    if (playing_space[x][y] == 1)
                    {current_population+=1;
                        cout << "*"<< " ";}
                    else
                    {
                        cout << " "<< " ";
                    }
                }
                cout << endl;
            }
        }
        cout<<endl;
        cout << "---------------------" << endl;
        cout<<"Board size: "<<pow(board_dims,2)<<" Current population: "<<current_population<<" Time: "<<tim<<endl;
    }


    auto Tick(vector<vector<int>> playing_space, bool printdebug){
        //Loop through board and find the following conditions, then save to a temporary hash table
        //1. Any live cell with two or three live neighbours survives.
        //2. Any dead cell with three live neighbours becomes a live cell.
        //3. All other live cells die in the next generation. Similarly, all other dead cells stay dead.
        int * living_neighbours = new int;
        vector<vector<int>> births{};
        vector<vector<int>> deaths{};
        for (int x = 0; x < board_dims; x++)
        {
            for (int y = 0; y < board_dims; y++)
            {
                *living_neighbours = 0;
                try
                {
                    *living_neighbours += playing_space.at(x - 1).at(y);
                }
                catch (const std::out_of_range &e)
                {
                }
                try
                {
                    *living_neighbours += playing_space.at(x - 1).at(y - 1);
                }
                catch (const std::out_of_range &e)
                {
                }
                try
                {
                    *living_neighbours += playing_space.at(x - 1).at(y + 1);
                }
                catch (const std::out_of_range &e)
                {
                }
                try
                {
                    *living_neighbours += playing_space.at(x).at(y - 1);
                }
                catch (const std::out_of_range &e)
                {
                }
                try
                {
                    *living_neighbours += playing_space.at(x).at(y + 1);
                }
                catch (const std::out_of_range &e)
                {
                }
                try
                {
                    *living_neighbours += playing_space.at(x + 1).at(y - 1);
                }
                catch (const std::out_of_range &e)
                {
                }
                try
                {
                    *living_neighbours += playing_space.at(x + 1).at(y);
                }
                catch (const std::out_of_range &e)
                {
                }
                try
                {
                    *living_neighbours += playing_space.at(x + 1).at(y + 1);
                }
                catch (const std::out_of_range &e)
                {
                }

                //Debug prints
                if (printdebug == true)
                {

                    cout << x << " " << y << " | " << playing_space[x][y];
                    cout << "  LN " << living_neighbours << endl;
                }

                if (playing_space[x][y] == 1 && (*living_neighbours == 2 || *living_neighbours == 3))
                {

                    if (printdebug == true)
                    {
                        cout << playing_space[x][y] << " Cond 1" << endl;
                    }
                    //cout << "  LN " << living_neighbours << endl;
                }

                else if (playing_space[x][y] == 0 && *living_neighbours == 3)
                {

                    if (printdebug == true)
                    {
                        cout << playing_space[x][y] << " Cond 2" << endl;
                    }
                    births.push_back(vector<int>{x, y});
                    //cout << "  LN " << *living_neighbours << endl;
                }

                else if (playing_space[x][y] == 1)
                {
                    if (printdebug == true)
                    {
                        cout << playing_space[x][y] << " Cond 3" << endl;
                    }
                    deaths.push_back(vector<int>{x, y});
                }
            }
            if (printdebug == true)
            {
                cout << endl;
            }
        }
        delete living_neighbours;

        //Apply our new births and deaths to the playing space
        if (printdebug == true)
        {
            cout << "Births" << endl;
        }
        for (int n = 0; n < births.size(); n++)
        {
            if (printdebug == true)
            {
                cout << births[n][0] << " " << births[n][1] << endl;
            }
            playing_space[births[n][0]][births[n][1]] = 1;
        }

        if (printdebug == true)
        {
            cout << "Deaths" << endl;
        }
        for (int n = 0; n < deaths.size(); n++)
        {
            if (printdebug == true)
            {
                cout << deaths[n][0] << " " << deaths[n][1] << endl;
            }
            playing_space[deaths[n][0]][deaths[n][1]] = 0;
        }

        return playing_space;
    }

};


int main()
{
    GameOfLife GOL;
    vector<vector<int>> live_locs{};

    //Create the playing space
    vector<vector<int>>  playing_space(board_dims) ;

    // vector<vector<int>> *playing_space= new vector<vector<int>>(board_dims,vector<int>(board_dims));
    for (int i = 0; i < board_dims; i++)
    {
        playing_space[i].resize(board_dims);
    }

    //Randomize our initial specimens
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<std::mt19937::result_type> dist(0, board_dims - 1); // distribution in range [1, board_soze-1]
    for (int sp = 0; sp < initial_alive; sp++)
    {
        int xx = int(dist(rng));
        int yy = int(dist(rng));
        vector<int> new_point= {xx,yy};
        //if(find(live_locs.begin(),live_locs.end(),new_point) != live_locs.end()){
        live_locs.push_back(new_point);
       // }
    }


    //Input our live locations
    for (int i = 0; i < initial_alive; i++)
    {
        int x;
        int y;
        for (int j = 0; j < initial_alive; j++)
        {
            //cout << live_locs[i][j] << " ";

            if (j == 0)
            {
                y = live_locs[i][j];
                //cout<<y<<endl;
            }

            if (j == 1)
            {
                x = live_locs[i][j];
                //cout<<x<<endl;
            }
        }
        playing_space[x][y] = 1;
        //cout << endl;
    }
    int tim=0;
    // ClearScreen();
    GOL.PrintBoard(playing_space,tim);
    sleep(5);
    while (true)
    {       
        tim+=1;
        playing_space = GOL.Tick(playing_space, printdebug);
        GOL.ClearScreen();
        
        GOL.PrintBoard(playing_space,tim);
        usleep(1000000*0.5);
        
    }

    return 0;
}