/*
* Project Title:
* - c++ Game of Life (Version 3)
*
* Description:
* This is a textbased game about trying to get the treasure from the 
* cave, and possibly clearing the cave of monsters.
*
* 
*
* There are 6 different locations to search through. each one holding 
* a secret item that could help you on your adventure.
* It utilizes string comparisons in order to determine
*
* Developers:
* - Peter Kroeger - kroegepg@mail.uc.edu
* - Joseph Gardner - gardnjc@mail.uc.edu
*
* Help Received:
* - Ryan Kunkel and Jacob Remm - Talked through the Game of Life 
*                                documentation to understand basics
* 
*
* Special Instructions:
* - There are a few test documents, you can change the contents of
*   the text documents, or change the name of the file it's passing 
*   to the class in the runGame function
* - The first 2 numbers in the text file are the number of rows, followed by number ot columns
* - In the text file, there should be as many 0's or 1's as there would be spaces on te grid
* - Example:
*   5 5
*   0 0 0 0 0
*   0 0 1 0 0
*   0 0 1 0 0           <---- I format it this way so it's easy to manipulate in the file
*   0 0 1 0 0                 
*   0 0 0 0 0
*
*   or
*
*   5 5 0 0 0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0 0 0    <- Hard to manipulate, but works
*
* Developer comments:
* Peter Kroeger:
* Focused on architecture of the base game
* Joesph Gardner:
* Added the Dynamic State rule and organized functions
*
*/
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>
#include <cctype>
#include <vector>
#include <string.h>

using namespace std;

/* Class Name: LifeGame
*
* Class Description:
* This create the game object, with the grid, numbergrid, number of 
* rows, number of columns, and all of the neighboring nodes for a 
* given point
*
*/
class LifeGame{
  friend class RandSpawn;
  public:
  LifeGame(string fileName){
    infile.open(fileName.c_str());
    infile >> nrow >> ncol;

    Grid.resize(nrow * ncol - 1);
    for(int irow=0; irow<nrow; ++irow) {
	    for(size_t jcol=0; jcol<ncol; ++jcol) {
		
		    int nodep = jcol + irow*ncol;
		    infile >> Grid[nodep]; 
      }
    }
    Nbrnode.resize(nrow*ncol-1); // hold neighbors for each cell in grid
    // loop over interior cells…
    for(size_t irow=1; irow<nrow-1; ++irow) {
      for(size_t jcol=1; jcol<ncol-1; ++jcol) {
        
        nodep = jcol + irow*ncol;
        Nbrnode[nodep].resize(8); // interior cells have 8 nbrs
        Nbrnode[nodep][0] = nodeN = jcol + (irow-1)*ncol;
        Nbrnode[nodep][1] = nodeS = jcol + (irow+1)*ncol;
        Nbrnode[nodep][2] = nodeE = jcol+1 + irow*ncol;
        Nbrnode[nodep][3] = nodeW = jcol-1 + irow*ncol;
        Nbrnode[nodep][4] = nodeNE = jcol+1 + (irow-1)*ncol;
        Nbrnode[nodep][5] = nodeSE = jcol+1 + (irow+1)*ncol;
        Nbrnode[nodep][6] = nodeSW = jcol-1 + (irow+1)*ncol;
        Nbrnode[nodep][7] = nodeNW = jcol-1 + (irow-1)*ncol;
      }
    }
  }
  int getRows(){return nrow;}
  int getCols(){return ncol;}
  void nextGeneration(){
    vector<int> newGrid = Grid;
    int sum = 0;
    for(size_t irow=1; irow<nrow-1; ++irow) {
      for(size_t jcol=1; jcol<ncol-1; ++jcol) {
        nodeN = jcol + (irow-1)*ncol;
        nodeS = jcol + (irow+1)*ncol;
        nodeE = jcol+1 + irow*ncol;
        nodeW = jcol-1 + irow*ncol;
        nodeNE = jcol+1 + (irow-1)*ncol;
        nodeSE = jcol+1 + (irow+1)*ncol;
        nodeSW = jcol-1 + (irow+1)*ncol;
        nodeNW = jcol-1 + (irow-1)*ncol;

  //Logic for Game of Life here: Looks at neighbors, and decides life
        sum = Grid[nodeN] + Grid[nodeS] + Grid[nodeE] + Grid[nodeW] + Grid[nodeNE] + Grid[nodeSE] + Grid[nodeSW] + Grid[nodeNW];

        if(sum < 2) newGrid[jcol + irow*ncol] = 0;
        else if (sum > 3) newGrid[jcol + irow*ncol] = 0;
        else if (sum == 3) newGrid[jcol + irow*ncol] = 1;
        //Adds new states into a new grid, then replaces the old generation with the new
    }
    }
    if(DynamicState) checkGens(newGrid); //Option for Dynamic State gamemode
    Grid = newGrid;
  }
  void checkGens(vector<int> & newGrid){    //Function checks to see if it has reach 0 change
    if (Grid == newGrid)
      for(int i = 0; i < newGrid.size(); ++i){
        if (Grid[i + 1] == 1 && Grid[i] == 0){
          newGrid[i] = 1;
        }
        else if (Grid[i + 1] == 1 && Grid[i] == 0){
          newGrid[i] = 1;
        }
      }
  }
  void setDynamicState(bool n){
    DynamicState = n;
  }
  void print() const{                 //Prints the grid
      for(int i = 0; i < nrow * ncol; ++i){
        if (Grid[i] == 1) cout << "* ";
        else cout << "  ";
        if ((i + 1) % ncol == 0) cout << endl;
      }
      cout << endl;
  }
  
  private:
  int nrow, ncol,nodep,nodeN,nodeS,nodeE,nodeW,nodeNE,nodeSE,nodeSW,nodeNW;
  int randPosX, randPosY, randTime;
  bool DynamicState; //Determines if this gamemode is on or not (user choice)
  ifstream infile;
  ofstream out;
  vector<int> Grid;
  vector<vector<int> > Nbrnode;

};

inline ostream & operator<<(ostream & out, const LifeGame theGame)
{
  theGame.print();
  return out;
}

/* Function Name: DisplayRules
*
* Function Description:
* This function displays the rules for the normal version of Conways Game of Life
*
* Parameters:
* None
*/
void DisplayRules(){
  cout <<"\nThere are 4 Rules to Conway's Game of Life: " << endl;
  cout << "1. If a cell has less than 2 live neighbors, it dies." << endl;
  cout << "2. If a cell has 2 or 3 live neighbors, it lives on." << endl;
  cout << "3. If a cell has more than 3 live neighbors, it dies." << endl;
  cout << "4. If a dead cell has exactly 3 live neighbors, it becomes a live cell.\n\n";

}

/* Function Name: runGame
*
* Function Description:
* This function Runs the game, including setting up inital conditions
* and then generates the next generation
*
* Parameters:
* None
*/
void runGame(){
  ofstream out;
  char c;
  bool state;
  
  DisplayRules();

  LifeGame theGame("GliderGun.txt"); //Change filename here to change initial conditions

  cout << "Would you like the game to be in Dynamic State?\nDynamic state is a gamemode where if the cells reach a point of no change (each generation is the same), then the game will give a neighboring cell life. This will not affect start conditions that never end. " << endl;
  cout << "Enter a 0 or 1: ";
  cin >> state;
  
  theGame.setDynamicState(state);

  theGame.print();

  do{
    cout << "Enter 'x' to exit, or anything to continue: ";
    cin >> c;
    theGame.nextGeneration();
    cout << endl;
    theGame.print();
  }while(c != 'x');
}

int main() {
  runGame();
  cout << "You have successfully exited.";
}