#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};

const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

bool CheckValidCell(int x, int y, std::vector<std::vector<State>>& grid){
    bool on_grid_x = (x >= 0 && x < grid.size());
    bool on_grid_y = (y >= 0 && y < grid[0].size());
    if (on_grid_x && on_grid_y)
        return grid[x][y] == State::kEmpty;
    return false;
}

bool Compare(const std::vector<int> x, const std::vector<int> y){
    int f1 = x[2] + x[3];
    int f2 = y[2] + y[3];
    return f1 > f2;
}

void CellSort(std::vector<std::vector<int>> *v) {
  std::sort(v->begin(), v->end(), Compare);
}


int Heuristic(int x1, int y1, int x2, int y2){
    return abs(x2 - x1) + abs(y2 - y1);
}

void AddToOpen(int x, int y, int g, int h, std::vector<std::vector<int>>& open_nodes, std::vector<std::vector<State>> grid){
    std::vector<int> node{x, y, g, h};
    open_nodes.push_back(node);
    grid[x][y] = State::kClosed;
}

void ExpandNeighbors(const std::vector<int> &current, std::vector<std::vector<int>> &open,
                     std::vector<std::vector<State>> &grid,  int goal[2]){
    int x = current[0];
    int y = current[1];
    int g = current[2];

    for (int i=0; i<4; i++){
        int x2 = x + delta[i][0];
        int y2 = y + delta[i][1];

        if (CheckValidCell(x2, y2, grid)) {
            int g2 = g + 1;
            int h2 = Heuristic(x2, y2, goal[0], goal[1]);
            AddToOpen(x2, y2, g2, h2, open, grid);
        }
    }
}


std::vector<std::vector<State>> Search(std::vector<std::vector<State>> grid, int start[2], int goal[2]){
    std::vector<std::vector<int>> open {};
    int g{0};
    int x = start[0];
    int y = start[1];
    int h = Heuristic(x, y, goal[0], goal[1]);

    AddToOpen(x, y, g, h, open, grid);
    while(!open.empty()){
       CellSort(&open);
       auto current = open.back();
       open.pop_back();
       x = current[0];
       y = current[1];
       grid[x][y] = State::kPath;

       if (x == goal[0] && y == goal[1]){
           grid[start[0]][start[1]] = State::kStart;
           grid[x][y] = State::kFinish;
           return grid;
       }
       ExpandNeighbors(current, open, grid, goal);

    }

    std::cout << "No path found!" << std::endl;
    return std::vector<std::vector<State>> {};
}


std::string CellString(State s){
  switch(s){
    case State::kObstacle: return "X   ";
    case State::kPath: return "O   ";
    case State::kStart: return "S   ";
    case State::kFinish: return "G   ";
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
    int start[2]{0, 0};
    int goal[2]{4, 5};

    auto board = ReadBoardFile("1.board");
    auto solution = Search(board, start, goal);
    PrintBoard(solution);
}

