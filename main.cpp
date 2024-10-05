#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <sstream>
#include <fstream>



using namespace std;
const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;
class Manager;

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
        currentId=0;
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
    virtual bool add(Board& board, int x, int y, int height, int width) = 0;
};

class Triangle: Shape{
public:
    bool add(Board& board, int x, int y, int height, int width) override {
        if (height <= 0 || width<0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + height >= BOARD_HEIGHT - 1) {
            cout<<"Invalid input!!!"<<endl;
            return 1;
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
        return 0;
    }
};

class Square: Shape{
public:
    bool add(Board& board, int x, int y, int height, int width) override {
        if (height <= 0 || width<0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + height >= BOARD_HEIGHT - 1) {
            cout << "Invalid input!!!" << endl;
            return 1;
        }
        for (int i = 0; i < height; ++i) {
            board.setCell(x, y + i, '*');
            board.setCell(x + height - 1, y + i, '*');
        }
        for (int j = 0; j < height; ++j) {
            board.setCell(x + j, y, '*' );
            board.setCell(x + j, y + height - 1, '*');
        }
        return 0;
    }
};

class Rectangle: Shape{
public:
    bool add(Board& board, int x, int y, int height, int width) override{
        if (width <= 0 || height <= 0 || x < 1 || x + width >= BOARD_WIDTH - 1 ||
            y < 1 || y + height >= BOARD_HEIGHT - 1) {
            cout << "Invalid input!!!" << endl;
            return 1;
        }
        for (int i =0; i<width; i++){
            board.setCell(x+i, y, '*');
            board.setCell(x+i, y + height-1, '*');
        }
        for(int j = 0; j<height; j++){
            board.setCell(x, y+j,'*');
            board.setCell(x+width-1, y+j, '*');
        }
        return 0;
    }
};

class Line: Shape{
public:
    bool add(Board& board, int x, int y, int height, int width) override{
        if (height==0&width==0||height < 0|| width<0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + height >= BOARD_HEIGHT - 1){
            cout<<"Invalid input!!!"<<endl;
            return 1;
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
        return 0;
    }
};

class Circle: Shape{
public:

    bool add(Board& board, int x, int y, int radius, int width) override {
        if (radius <= 0 || width<0 || x - radius < 1 || x + radius >= BOARD_WIDTH - 1 ||
            y - radius < 1 || y + radius >= BOARD_HEIGHT - 1) {
            cout << "Invalid input!!!" << endl;
            return 1;
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
        return 0;

    }

};

class AddShapes{
public:
    void addLine(Board& board, string par, string shape){
        stringstream ss(par);
        int x,y,len;
        string direction;
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
            bool value =line.add(board, x, y, 0, len);
            if(value == 0){
                board.addShape(shape,par);
                cout<<"Line was successfully added!"<<endl;
            }
        }
        else if (direction =="horizontal"|| direction=="Horizontal"){
            bool value =line.add(board, x, y, len, 0);
            if(value == 0){
                board.addShape(shape,par);
                cout<<"Line was successfully added!"<<endl;
            }
        }
    }

    void addSquare(Board& board, string par, string shape){
        int x,y,len;
        stringstream ss(par);
        string item;
        vector<string> parts;
        while (getline(ss, item, ',')) {
            parts.push_back(item);
        }
        if (parts.size() != 3) {
            cout << "Invalid input format." << endl;
            return;
        }
        x = stoi(parts[0]);
        y = stoi((parts[1]));
        len = stoi(parts[2]);
        Square square;
        bool value = square.add(board, x, y, len, 0);
        if(value == 0){
            board.addShape(shape,par);
            cout<<"Square was successfully added!"<<endl;
        }
    }

    void addTriangle(Board& board, string par, string shape){
        stringstream ss(par);
        int x,y,len;
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
        bool value = triangle.add(board, x, y, len, 0);
        if(value == 0){
            board.addShape(shape,par);
            cout<<"Triangle was successfully added!"<<endl;
        }
    }

    void addRectangle(Board& board, string par, string shape){
        stringstream ss(par);
        int x,y,len, width;
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
        bool value = rectangle.add(board, x, y, len, width);
        if(value == 0){
            board.addShape(shape,par);
            cout<<"Rectangle was successfully added!"<<endl;
        }
    }

    void addCircle(Board& board, string par, string shape){
        stringstream ss(par);
        int x,y,len;
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
        bool value = circle.add(board, x, y, len, 0);
        if(value == 0){
            board.addShape(shape,par);
            cout<<"Circle was successfully added!"<<endl;
        }
    }
};

class Writer{
public:
    void saveToFile(Board& board, string& fileName){
        ofstream outFile(fileName);
        if (outFile.is_open()) {
            if (board.getShapes().empty()) {
                outFile << "null" << endl;
            }else {
                vector<string> shapes = board.getShapes();
                map<int, string> parameters = board.getDic();
                int id;
                string shape;
                string par;
                for (const auto &pair: parameters) {
                    id = pair.first;
                    par = pair.second;
                    shape = shapes[id - 1];
                    outFile << id << "-" << shape << "-" << par << endl;
                }
                outFile.close();
                cout << "Board was saved in " << fileName << endl;
            }
        } else {
            cout << "Error!" << endl;
        }
    }

    void loadFromFile(Board& board,string& fileName){
        ifstream inFile(fileName);
        AddShapes addShapes;
        if (inFile.is_open()) {
            string line;
            board.clear();
            if (getline(inFile, line) && line == "null") {
                cout << "Board was empty." << endl;
                inFile.close();
                return;
            }
            else {
                while (getline(inFile, line)) {
                    stringstream ss(line);
                    string id;
                    string shape;
                    string parameters;

                    getline(ss, id, '-');
                    getline(ss, shape, '-');
                    getline(ss, parameters);
                    if (shape == "Line" || shape == "line") {
                        addShapes.addLine(board, parameters, shape);
                    } else if (shape == "Triangle" || shape == "triangle") {
                        addShapes.addTriangle(board, parameters, shape);
                    } else if (shape == "Square" || shape == "square") {
                        addShapes.addSquare(board, parameters, shape);
                    } else if (shape == "Rectangle" || shape == "rectangle") {
                        addShapes.addRectangle(board, parameters, shape);
                    } else if (shape == "Circle" || shape == "circle") {
                        addShapes.addCircle(board, parameters, shape);
                    } else {
                        cout << "Error!" << endl;
                    }
                }
                inFile.close();
                cout << "Board was successfully loaded from " << fileName << endl;
            }
        } else {
            cout << "Error!" << endl;
        }
    }
};

class Manager{
    int command =0;
    Writer writer;
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
        AddShapes addShapes;
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
            addShapes.addLine(board, cleaned, shape);

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
            addShapes.addSquare(board, cleaned, shape);

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
            addShapes.addTriangle(board, cleaned, shape);

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
            addShapes.addRectangle(board, cleaned, shape);

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
            addShapes.addCircle(board, cleaned, shape);
        }
        else{
            cout<<"Invalid name:("<<endl;
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

    void fileWriter(Board& board){
        string file;
        cout<< "Write file name: "<<endl;
        cin.ignore();
        cin>>file;
        writer.saveToFile(board, file);
    }

    void fileReader(Board& board){
        string file;
        cout<< "Write file name: "<<endl;
        cin.ignore();
        cin>>file;
        writer.loadFromFile(board, file);
    }

    void manage(){
        Board board(BOARD_WIDTH, BOARD_HEIGHT);
        vector<Board> boardStates;
        Manager manager;
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
                case 7:
                    fileWriter(board);
                    break;
                case 8:
                    fileReader(board);
                    break;

            }
        }
        cout<<"Ciao, bella!!";
    }
};

int main() {
    Manager manager;
    manager.manage();
    return 0;
}