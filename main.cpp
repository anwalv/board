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
    virtual bool fill(Board& board, int x, int y, int height,int width, char color) = 0;

};

class Triangle: Shape{
public:
    bool add(Board& board, int x, int y, int height, int width) override {
        if (height <= 0 || width<0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + height >= BOARD_HEIGHT - 1) {
            cout<<"Invalid input!!!"<<endl;
            return 0;
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
        return 1;
    }
    bool fill(Board& board, int x, int y, int height, int width, char color) override {
        for (int i = 0; i < height; i++) {
            for (int j = -i; j <= i; j++) {
                board.setCell(x + j, y + i, color);
            }
        }
        return 1;
    }
};

class Square: Shape{
public:
    bool add(Board& board, int x, int y, int height, int width) override {
        if (height <= 0 || width<0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + height >= BOARD_HEIGHT - 1) {
            cout << "Invalid input!!!" << endl;
            return 0;
        }
        for (int i = 0; i < height; ++i) {
            board.setCell(x, y + i, '*');
            board.setCell(x + height - 1, y + i, '*');
        }
        for (int j = 0; j < height; ++j) {
            board.setCell(x + j, y, '*' );
            board.setCell(x + j, y + height - 1, '*');
        }
        return 1;
    }
    bool fill(Board& board, int x, int y, int height, int width, char color) override {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < height; ++j) {
                board.setCell(x + j, y + i, color);
            }
        }
        return 1;
    }
};

class Rectangle: Shape{
public:
    bool add(Board& board, int x, int y, int height, int width) override{
        if (width <= 0 || height <= 0 || x < 1 || x + width >= BOARD_WIDTH - 1 ||
            y < 1 || y + height >= BOARD_HEIGHT - 1) {
            cout << "Invalid input!!!" << endl;
            return 0;
        }
        for (int i =0; i<width; i++){
            board.setCell(x+i, y, '*');
            board.setCell(x+i, y + height-1, '*');
        }
        for(int j = 0; j<height; j++){
            board.setCell(x, y+j,'*');
            board.setCell(x+width-1, y+j, '*');
        }
        return 1;
    }
    bool fill(Board& board, int x, int y, int height, int width, char color) override {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                board.setCell(x + j, y + i, color);
            }
        }
        return 1;
    }
};

class Line: Shape{
public:
    bool add(Board& board, int x, int y, int height, int width) override{
        if (height==0 &&width==0||height < 0|| width<0 || x < 1 || x >= BOARD_WIDTH - 1 || y < 1 || y + height >= BOARD_HEIGHT - 1){
            cout<<"Invalid input!!!"<<endl;
            return 0;
        }
        if (width == 0){
            for (int i = 0; i < height; i++) {
                board.setCell(x + i, y, '*');
            }
            return  1;
        }
        else if(height==0){
            for(int i = 0; i<width; i++){
                board.setCell(x,y+i, '*');
            }
            return 1;
        }
    }
    bool fill(Board& board, int x, int y, int height, int width, char color) override {
        if (height==0) {
            for (int i = 0; i < width; ++i) {
                board.setCell(x, y + i, color);
            }
        } else if(width == 0) {
            for (int j = 0; j < height; ++j) {
                board.setCell(x + j, y, color);
            }
        }return 1;
    }
};

class Circle: Shape{
public:

    bool add(Board& board, int x, int y, int radius, int width) override {
        if (radius <= 0 || width<0 || x - radius < 1 || x + radius >= BOARD_WIDTH - 1 ||
            y - radius < 1 || y + radius >= BOARD_HEIGHT - 1) {
            cout << "Invalid input!!!" << endl;
            return 0;
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
        return 1;
    }
    bool fill(Board& board, int x, int y, int radius,  int width, char color) override {
        for (int i = 0; i <= radius; i++) {
            for (int j = 0; j <= radius; j++) {
                if (round(sqrt(i * i + j * j)) == radius) {
                    board.setCell(x + j, y + i, color);
                    board.setCell(x - j, y + i, color);
                    board.setCell(x + j, y - i, color);
                    board.setCell(x - j, y - i, color);
                }
            }
        }
        for (int i = -radius + 1; i < radius; ++i) {
            for (int j = -radius + 1; j < radius; ++j) {
                if (i * i + j * j < radius * radius) {
                    board.setCell(x + i, y + j, color);
                }
            }
        }return 1;
    }

};

class AddShapes{
public:
    void addLine(Board& board, string par, string shape, string fillMode, string color){
        stringstream ss(par);
        int x,y,len;
        string direction;
        string item;
        char symbol = color[0];
        vector<string> parts;
        while (getline(ss, item, ',')) {
            parts.push_back(item);
        }
        if (parts.size() != 4) {
            cout << "Invalid input format." << endl;
            return;
        }
        x = stoi(parts[0]);
        y = stoi((parts[1]));
        len = stoi(parts[2]);
        direction = parts[3];
        Line line;
        if (direction == "vertical"|| direction=="Vertical") {
            if (fillMode == "empty"||fillMode == "Empty"){
                bool value = line.add(board, x, y, 0, len);
                if(value == 0){
                    board.addShape(shape,par);
                    cout<<"Line was successfully added!"<<endl;
                }
            } else{
                bool value = line.fill(board, x, y, 0, len, symbol);
                if(value == 1){
                    board.addShape(shape,par);
                    cout<<"Line was successfully added!"<<endl;
                }
            }
        }
        else if (direction =="horizontal"|| direction=="Horizontal"){
            if (fillMode == "empty"||fillMode == "Empty"){
                bool value = line.add(board, x, y, len, 0);
                if(value == 1){
                    board.addShape(shape,par);
                    cout<<"Line was successfully added!"<<endl;
                }
            } else{
                bool value = line.fill(board, x, y,  len,0, symbol);
                if(value == 1){
                    board.addShape(shape,par);
                    cout<<"Line was successfully added!"<<endl;
                }
            }
        }
    }

    void addSquare(Board& board, string par, string shape, string fillMode, string color){
        int x,y,len;
        stringstream ss(par);
        string item;
        char symbol = color[0];
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
        if (fillMode == "empty"||fillMode == "Empty") {
            bool value = square.add(board, x, y, len, 0);
            if (value == 1) {
                board.addShape(shape, par);
                cout << "Square was successfully added!" << endl;
            }
        } else{
            bool value = square.fill(board, x, y,  len,0, symbol);
            if (value == 1) {
                board.addShape(shape, par);
                cout << "Square was successfully added!" << endl;
            }
        }
    }

    void addTriangle(Board& board, string par, string shape,  string fillMode, string color){
        stringstream ss(par);
        int x,y,len;
        string item;
        vector<string> parts;
        char symbol = color[0];
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
        Triangle triangle;
        if (fillMode == "empty"||fillMode == "Empty") {
            bool value = triangle.add(board, x, y, len, 0);
            if (value == 1) {
                board.addShape(shape, par);
                cout << "Triangle was successfully added!" << endl;
            }
        }else{
            bool value = triangle.fill(board, x, y,  len,0, symbol);
            if (value == 1) {
                board.addShape(shape, par);
                cout << "Triangle was successfully added!" << endl;
            }
        }
    }

    void addRectangle(Board& board, string par, string shape, string fillMode, string color){
        stringstream ss(par);
        int x,y,len, width;
        string item;
        vector<string> parts;
        char symbol = color[0];
        while (getline(ss, item, ',')) {
            parts.push_back(item);
        }
        if (parts.size() != 4) {
            cout << "Invalid input format." << endl;
            return;
        }
        x = stoi(parts[0]);
        y = stoi((parts[1]));
        len = stoi(parts[2]);
        width = stoi(parts[3]);

        Rectangle rectangle;
        if (fillMode == "empty"||fillMode == "Empty") {
            bool value = rectangle.add(board, x, y, len, width);
            if (value == 1) {
                board.addShape(shape, par);
                cout << "Rectangle was successfully added!" << endl;

            }
        }else{
            bool value = rectangle.fill(board, x, y,  len,width, symbol);
            if (value == 1) {
                board.addShape(shape, par);
                cout << "Rectangle was successfully added!" << endl;
            }
        }
    }

    void addCircle(Board& board, string par, string shape,string fillMode, string color){
        stringstream ss(par);
        int x,y,len;
        string item;
        vector<string> parts;
        char symbol = color[0];
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
        Circle circle;
        if (fillMode == "empty"||fillMode == "Empty") {
            bool value = circle.add(board, x, y, len, 0);
            if (value == 1) {
                board.addShape(shape, par);
                cout << "Circle was successfully added!" << endl;
            }
        } else{
            bool value = circle.fill(board, x, y,  len,0, symbol);
            if (value == 1) {
                board.addShape(shape, par);
                cout << "Circle was successfully added!" << endl;
            }
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
            while (getline(inFile, line)) {
                if (line=="null"){
                    cout << "Board was empty." << endl;
                    inFile.close();
                    return;
                }else{
                    stringstream ss(line);
                    string id;
                    string shape;
                    string parameters;
                    string fillMode;
                    string symbol;

                    getline(ss, id, '-');
                    getline(ss, shape, '-');
                    getline(ss, parameters);
                    if (shape == "Line" || shape == "line") {
                        addShapes.addLine(board, parameters, shape,fillMode,symbol);
                    } else if (shape == "Triangle" || shape == "triangle") {
                        addShapes.addTriangle(board, parameters, shape,fillMode,symbol);
                    } else if (shape == "Square" || shape == "square") {
                        addShapes.addSquare(board, parameters, shape, fillMode,symbol);
                    } else if (shape == "Rectangle" || shape == "rectangle") {
                        addShapes.addRectangle(board, parameters, shape, fillMode,symbol);
                    } else if (shape == "Circle" || shape == "circle") {
                        addShapes.addCircle(board, parameters, shape, fillMode,symbol);
                    } else {
                        cout << "Error!" << endl;
                    }
                }
            }
            inFile.close();
            cout << "Board was successfully loaded from " << fileName << endl;
        } else {
            cout << "Error!" << endl;
        }
    }
};

class Manager{
    string command ="0";
    Writer writer;
public:
    void availableShapes(){
        cout<<"Shapes:\n"
              "~ Line, fill mode or empty, x coordinate, y coordinate, length, direction~\n"
              "~ Triangle, fill mode or empty,x coordinate, y coordinate, height~\n"
              "~ Square,fill mode or empty,x coordinate, y coordinate, length, direction~\n"
              "~ Rectangle,fill mode or empty,x coordinate, y coordinate, length, width~\n"
              "~ Circle,fill mode or empty,x coordinate , y coordinate, radius~\n";
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

    void add(Board& board, string shape, string par, string fillMode,string symbol){
        AddShapes addShapes;
        if (shape =="Line"|| shape=="line"){
            addShapes.addLine(board, par, shape, fillMode,symbol);
        }
        else if (shape =="Square"|| shape=="square"){
            addShapes.addSquare(board, par, shape, fillMode,symbol);

        }
        else if (shape =="Triangle"|| shape=="triangle"){
            addShapes.addTriangle(board, par, shape, fillMode,symbol);
        }
         else if (shape =="Rectangle"|| shape=="rectangle"){
            addShapes.addRectangle(board, par, shape, fillMode,symbol);

        }
        else if (shape =="Circle"|| shape=="circle"){
            addShapes.addCircle(board, par, shape, fillMode,symbol);
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
            board = boardStates[boardStates.size() - 2];
            boardStates.pop_back();
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
        string shape, userInput, item,value, fillMode, params,symbol, fillColor;
        vector<string> parts;
        vector<string>colors;
        vector<Board> boardStates;
        boardStates.push_back(board);
        printCommands();
        string filePath;
        while(command != "9"){
            cout << "Enter the command and info(in add command you must specify fill mode or empty shape):";
            getline(cin, userInput);
            stringstream ss(userInput);
            parts.clear();
            colors.clear();
            while (getline(ss, item, ',')) {
                parts.push_back(item);
            }
            command = parts[0];
            if (command == "1"){
                availableShapes();
                continue;
            }
            else if (command == "2"){
                if (parts.size() < 6) {
                    cout << "Invalid input for add command. Please try again." << endl;
                    continue;
                }
                fillColor =parts[1];
                stringstream ss(fillColor);
                while (getline(ss, value, ' ')) {
                    colors.push_back(value);
                }
                shape =parts[2];
                params= "";
                int requiredParams = 0;
                if (shape == "Line"||shape == "line") {
                    requiredParams = 4;
                } else if (shape == "Triangle"|| shape == "triangle") {
                    requiredParams = 3;
                } else if (shape == "Square"||shape == "square") {
                    requiredParams = 3;
                } else if (shape == "Rectangle"||shape == "rectangle") {
                    requiredParams = 4;
                } else if (shape == "Circle"||shape == "circle") {
                    requiredParams = 3;
                } else {
                    cout << "Unknown shape. Please try again." << endl;
                    continue;
                }
                if (parts.size() < 3 + requiredParams) {
                    cout << "Invalid number of parameters for the shape. Please try again." << endl;
                    continue;
                }
                for (int i=3; i<3 + requiredParams; ++i) {
                    params += parts[i];
                    if (i != (2 + requiredParams)) params += ",";
                }
                fillMode = colors[0];
                if(colors.size()==2){
                    symbol= colors[1];
                }else if (colors.size()==1){
                    symbol = "*";
                }
                add(board, shape, params,fillMode,symbol);
                boardStates.push_back(board);
                continue;
            }
            else if (command == "3"){
                board.draw();
                continue;
            }
            else if (command == "4") {
                addedShapes(board);
                continue;
            }
            else if (command == "5") {
                board = undo(board, boardStates);
                continue;
            }
            else if (command == "6") {
                board.clear();
                cout << "Board has been cleared." << endl;
                continue;
            }
            else if (command == "7"){
                filePath = parts[1];
                writer.saveToFile(board, filePath);
                continue;
            }
            else if (command == "8") {
                filePath = parts[1];
                writer.loadFromFile(board, filePath);
                continue;
            } else{
                cout<<"gh";
            }
        }
    }
};

int main() {
    Manager manager;
    manager.manage();
    return 0;
};