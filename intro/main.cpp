#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

enum class State {kEmpty, kObstacle};

std::string CellString(State s){
  switch(s){
    case State::kObstacle: return "X   ";
    default: return "_   "; 
  }
}

std::vector<State> ParseLine (std::string line){
    std::istringstream stream(line);
    int n;
    char c;
    std::vector<State> row;

    while(stream >> n >> c && c == ','){
        if (n == 0){row.push_back(State::kEmpty);} 
        else {row.push_back(State::kObstacle);}
    }
    return row;
}

std::vector<std::vector<State>> ReadBoardFile(std::string board_file){
    std::ifstream input (board_file);
    std::vector<std::vector<State>> board{};
    std::string line;

    if(input){
        while (getline(input, line)){
            auto row  = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}

void PrintBoard(const std::vector<std::vector<State>> board){
    for(auto row : board){
        for(auto cell : row){
            std::cout << CellString(cell);
        }
        std::cout << std::endl;
    }
}

int main()
{
    auto board = ReadBoardFile("1.board");
    PrintBoard(board);
}

