#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <sstream>



using namespace std;
const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;
class Board{
    vector<vector<char>> grid;
    vector<string > shape;
    map<int, string > idParameters;
    int width;
    int height;
    int currentId=0;
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
    void addShape(string& shapes, const string& parameters){
        shape.push_back(shapes);
        currentId++;
        idParameters[currentId]= parameters;
    }
    void clear() {
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), ' ');
        }
        shape.clear();
        idParameters.clear();
        drawBorder();
    }

    map<int, string> getDic(){
        return idParameters;
    }
    vector<string> getShapes(){
        return shape;
    }
};

class Shape{
public:
    virtual void add(Board& board, int x, int y, int height, int width) = 0;
};

class Triangle: Shape{
public:
    void add(Board& board, int x, int y, int height, int width) override {
        if (height <= 0 || width<0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + height >= BOARD_HEIGHT - 1) {
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

class Square: Shape{
public:
    void add(Board& board, int x, int y, int height, int width) override {
        if (height <= 0 || width<0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + height >= BOARD_HEIGHT - 1) {
            cout << "Invalid input!!!" << endl;
            return;
        }
        for (int i = 0; i < height; ++i) {
            board.setCell(x, y + i, '*');
            board.setCell(x + height - 1, y + i, '*');
        }
        for (int j = 0; j < height; ++j) {
            board.setCell(x + j, y, '*' );
            board.setCell(x + j, y + height - 1, '*');
        }
    }
};

class Rectangle: Shape{
public:
    void add(Board& board, int x, int y, int height, int width) override{
        if (width <= 0 || height <= 0 || x < 1 || x + width >= BOARD_WIDTH - 1 ||
            y < 1 || y + height >= BOARD_HEIGHT - 1) {
            cout << "Invalid input!!!" << endl;
            return;
        }
        for (int i =0; i<width; i++){
            board.setCell(x+i, y, '*');
            board.setCell(x+i, y + height-1, '*');
        }
        for(int j = 0; j<height; j++){
            board.setCell(x, y+j,'*');
            board.setCell(x+width-1, y+j, '*');
        }
    }
};

class Line: Shape{
public:
    void add(Board& board, int x, int y, int height, int width) override{
        if (height==0&width==0||height < 0|| width<0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + height >= BOARD_HEIGHT - 1){
            cout<<"Invalid input!!!"<<endl;
        }
        if (width == 0){
            for (int i = 0; i < height; i++) {
                board.setCell(x + i, y, '*');
            }
        }
        else if(height==0){
            for(int i = 0; i<width; i++){
                board.setCell(x,y+i, '*');
            }
        }
    }
};

class Circle: Shape{
public:

    void add(Board& board, int x, int y, int radius, int width) override {
        if (radius <= 0 || width<0 || x - radius < 1 || x + radius >= BOARD_WIDTH - 1 ||
            y - radius < 1 || y + radius >= BOARD_HEIGHT - 1) {
            cout << "Invalid input!!!" << endl;
            return;
        }

        for (int i = 0; i <= radius; i++) {
            for (int j = 0; j <= radius; j++) {
                if (round(sqrt(i * i + j * j)) == radius) {
                    board.setCell(x + j, y + i, '*');
                    board.setCell(x - j, y + i, '*');
                    board.setCell(x + j, y - i, '*');
                    board.setCell(x - j, y - i, '*');
                }
            }
        }
    }

};

class Manager{
    int command =0;
public:
    void availableShapes(){
        cout<<"Shapes:\n"
              "~ Line>> <x coordinate, y coordinate, length, direction>\n"
              "~ Triangle>> <x coordinate, y coordinate, height>\n"
              "~ Square>> <x coordinate, y coordinate, length, direction>\n"
              "~ Rectangle>> <x coordinate, y coordinate, length, width>\n"
              "~ Circle>> <x coordinate , y coordinate, radius>\n";
    }

    void printCommands(){
        cout<<"Hello!The list of command:\n"
              "1. List of all available shapes.\n"//+
              "2. Add shape.\n"//+
              "3. Draw board.\n"//+
              "4. List of all added shapes and their parameters.\n"//+
              "5. Undo: remove the last added shape from the blackboard.\n"
              "6. Remove all shapes from blackboard.\n"
              "7. Save the blackboard to the file.\n"
              "8. Load a blackboard from the file.\n"
              "9. Exit.\n"<<endl;
    }

    void add(Board& board){
        string shape;
        string par;
        int x,y,len, width;
        string direction;
        cout<<"Enter shape name:";
        cin.ignore();
        cin>> shape;
        if (shape =="Line"|| shape=="line"){
            cout << "Enter parameters(format: x coordinate,y coordinate,length,direction): ";
            cin.ignore();
            getline(cin, par);
            string cleaned;
            for (char c : par) {
                if (c != ' ') {
                    cleaned += c;
                }
            }
            stringstream ss(cleaned);
            string item;
            vector<string> parts;
            while (getline(ss, item, ',')) {
                parts.push_back(item);
            }
            if (parts.size() != 4) {
                cout << "Invalid input format." << endl;
            }
            x = stoi(parts[0]);
            y = stoi((parts[1]));
            len = stoi(parts[2]);
            direction = parts[3];

            Line line;
            if (direction == "vertical"|| direction=="Vertical") {
                line.add(board, x, y, 0, len);
                board.addShape(shape,cleaned);
                cout<<"Line was successfully added!"<<endl;
            }
            else if (direction =="horizontal"|| direction=="Horizontal"){
                line.add(board, x, y, len, 0);
                board.addShape(shape,cleaned);
                cout<<"Line was successfully added!"<<endl;
            }

        }
        if (shape =="Square"|| shape=="square"){
            cout << "Enter parameters(format: x coordinate,y coordinate,length):";
            cin.ignore();
            getline(cin, par);
            string cleaned;
            for (char c : par) {
                if (c != ' ') {
                    cleaned += c;
                }
            }
            stringstream ss(cleaned);
            string item;
            vector<string> parts;
            while (getline(ss, item, ',')) {
                parts.push_back(item);
            }
            if (parts.size() != 3) {
                cout << "Invalid input format." << endl;
            }
            x = stoi(parts[0]);
            y = stoi((parts[1]));
            len = stoi(parts[2]);
            Square square;
            square.add(board, x, y, len, 0);
            board.addShape(shape,cleaned);
            cout<<"Square was successfully added!"<<endl;

        }
        if (shape =="Triangle"|| shape=="triangle"){
            cout << "Enter parameters(format: x coordinate,y coordinate,height):";
            cin.ignore();
            getline(cin, par);
            string cleaned;
            for (char c : par) {
                if (c != ' ') {
                    cleaned += c;
                }
            }
            stringstream ss(cleaned);
            string item;
            vector<string> parts;
            while (getline(ss, item, ',')) {
                parts.push_back(item);
            }
            if (parts.size() != 3) {
                cout << "Invalid input format." << endl;
            }
            x = stoi(parts[0]);
            y = stoi((parts[1]));
            len = stoi(parts[2]);
            Triangle triangle;
            triangle.add(board, x, y, len, 0);
            board.addShape(shape,cleaned);
            cout<<"Triangle was successfully added!"<<endl;

        }
        if (shape =="Rectangle"|| shape=="rectangle"){
            cout << "Enter parameters(format: x coordinate,y coordinate,height,width):";
            cin.ignore();
            getline(cin, par);
            string cleaned;
            for (char c : par) {
                if (c != ' ') {
                    cleaned += c;
                }
            }
            stringstream ss(cleaned);
            string item;
            vector<string> parts;
            while (getline(ss, item, ',')) {
                parts.push_back(item);
            }
            if (parts.size() != 4) {
                cout << "Invalid input format." << endl;
            }
            x = stoi(parts[0]);
            y = stoi((parts[1]));
            len = stoi(parts[2]);
            width = stoi(parts[3]);

            Rectangle rectangle;
            rectangle.add(board, x, y, len, width);
            board.addShape(shape,cleaned);
            cout<<"Rectangle was successfully added!"<<endl;

        }
        if (shape =="Circle"|| shape=="circle"){
            cout << "Enter parameters(format: x coordinate,y coordinate,radius): ";
            cin.ignore();
            getline(cin, par);
            string cleaned;
            for (char c : par) {
                if (c != ' ') {
                    cleaned += c;
                }
            }
            stringstream ss(cleaned);
            string item;
            vector<string> parts;
            while (getline(ss, item, ',')) {
                parts.push_back(item);
            }
            if (parts.size() != 3) {
                cout << "Invalid input format." << endl;
            }
            x = stoi(parts[0]);
            y = stoi((parts[1]));
            len = stoi(parts[2]);
            Circle circle;
            circle.add(board, x, y, len, 0);
            board.addShape(shape,cleaned);
            cout<<"Circle was successfully added!"<<endl;

        }

    }

    void addedShapes(Board& board){
        vector<string> shapes = board.getShapes();
        map<int, string> idParam = board.getDic();
        int id;
        string shape;
        string par;
        for (const auto& pair:idParam){
            id = pair.first;
            par = pair.second;
            shape = shapes[id-1];
            cout<<id<<". "<<shape<<"->"<<" "<<par << endl;
        }
    }

    Board undo(Board& board, vector<Board>& boardStates) {
        if (boardStates.size() > 1) {
            board = boardStates[boardStates.size() - 2]; // Повертаємось до передостаннього стану
            boardStates.pop_back(); // Видаляємо останній стан, оскільки він більше не потрібен
            cout << "Undo successful." << endl;
        } else {
            cout << "No actions to undo." << endl;
        }
        return board;
    }

    void manage(){
        Board board(BOARD_WIDTH, BOARD_HEIGHT);
        vector<Board> boardStates;
        boardStates.push_back(board);
        printCommands();
        while(command != 9){
            cout << "\nEnter the command:" << endl;
            cin >> command;
            switch (command) {
                case 1:
                    availableShapes();
                    break;
                case 2:
                    add(board);
                    boardStates.push_back(board);
                    break;

                case 3:
                    board.draw();
                    break;

                case 4:
                    addedShapes(board);
                    break;

                case 5:
                    board = undo(board, boardStates);

                    break;
                case 6:
                    board.clear();
                    cout << "Board has been cleared." << endl;
                    break;
            }
        }
        cout<<"Ciao, bella!!";
    }
};

class Writer{};
int main() {
    Manager manager;
    manager.manage();
    return 0;
}
