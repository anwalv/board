#include <iostream>
#include <vector>
using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;
class Board{
    std::vector<std::vector<char>> grid;
    int width;
    int height;
public:
    Board(int width, int height) : width(width), height(height) {
        grid.resize(height, std::vector<char>(width, ' '));
        drawBorder();
    }
    void drawBorder() {
        for (int x = 0; x < width; ++x) {
            grid[0][x] = '-';
            grid[height - 1][x] = '_';
        }
        for (int y = 0; y < height; ++y) {
            grid[y][0] = '|';
            grid[y][width - 1] = '|';
        }
    }
    void draw() {
        for (const auto& row : grid) {
            for (char cell : row) {
                std::cout << cell;
            }
            std::cout << std::endl;
        }
    }
    void setCell(int x, int y, char value) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x] = value;
        }
    }
};

class Figure{
public:
    virtual void draw(Board& board, int x, int y, int size) = 0;
};

class Triangle:Figure{
public:
    void draw(Board& board, int x, int y, int height) override {
        if (height <= 0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + height >= BOARD_HEIGHT - 1) {
            cout<<"Invalid input!!!"<<endl;
            return;
        }
        for(int i=0;i<height; i++ ){
            int leftMost = x - i;
            int rightMost = x + i;
            int posY = y + i;
            board.setCell(leftMost, posY, '*');
            board.setCell(rightMost, posY, '*');
        }
        for (int j = 0; j < 2 * height - 1; ++j) {
            int baseX = x - height + 1 + j;
            int baseY = y + height - 1;
            if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT) {
                board.setCell(baseX, baseY, '*');
            }
        }
    }
};

class Square:Figure{
public:
    void draw(Board& board, int x, int y, int size) override {
        if (size <= 0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + size >= BOARD_HEIGHT - 1) {
            cout << "Invalid input!!!" << endl;
            return;
        }
        for (int i = 0; i < size; ++i) {
            board.setCell(x, y + i, '*');
            board.setCell(x + size - 1, y + i, '*');
        }
        for (int j = 0; j < size; ++j) {
            board.setCell(x + j, y, '*' );
            board.setCell(x + j, y + size - 1, '*');
        }
    }
};

class Rectangle:Figure{};

class Line:Figure{};

class Manager{};

class Writer{};
int main() {
    Board board(BOARD_WIDTH, BOARD_HEIGHT);
    Triangle triangle;
    triangle.draw(board, 40, 5, 5);
    Square square;
    square.draw(board, 40, 8, 5);

    board.draw();
    return 0;
}
