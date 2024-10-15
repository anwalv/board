#include <iostream>
#include <vector>
#include <cmath>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <memory>
#include <algorithm>


using namespace std;
const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;
class Board;
int currentId=0;

class Shape{
public:
    virtual bool draw(Board& board, int x, int y, int height, int width) = 0;
    virtual bool fill(Board& board, int x, int y, int height,int width, char color) = 0;
    virtual string toString() const = 0;
    virtual bool contains(int px, int py) const = 0;
    virtual int getId()=0;
    virtual string getFillMode()=0;
    virtual char getColor()=0;
    virtual int getHeight()=0;
    virtual int getWidth()=0;
    virtual int getX() =0;
    virtual int getY()=0;
    virtual void setColor(char newColor)=0;
    virtual ~Shape() = default;

};

class Board{
    vector<vector<char>> grid;
    vector<shared_ptr<Shape>> shapes;
    map<int, string > idParameters;
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
    void addShape(shared_ptr<Shape> shape, const string& parameters){
        shapes.push_back(shape);
        idParameters[currentId]= parameters;
    }
    void clear() {
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), ' ');
        }
        drawBorder();
        currentId=0;
    }
    void clearShapes(){
        shapes.clear();
    }
    void clearIdParametres(){
        idParameters.clear();
    }
    map<int, string>& getDic(){
        return idParameters;
    }
    vector<shared_ptr<Shape>>& getShapes(){
        return shapes;
    }
    shared_ptr<Shape> selectByCoordinates(int x, int y) {
        for (int i = shapes.size() - 1; i >= 0; --i) {
            auto shape = shapes[i];
            if (shape->contains(x, y)) {
                return shapes[i];
            }
        }
        return nullptr;
    }
    shared_ptr<Shape> selectById(int id){
        for(auto& shape:shapes){
            int shapeId= shape->getId();
            if(shapeId ==id){
                return shape;
            }
        }
    }


};

class Triangle:public Shape{
private:
    int baseX;
    int baseY;
    int height;
    int width =0;
    int id;
    string fillMode;
    char color;
public:
    Triangle(int baseX, int baseY, int height, string fillMode, char color):
    baseX(baseX), baseY(baseY), height(height),id(currentId), fillMode(fillMode), color(color){};
    bool draw(Board& board, int x, int y, int height, int width) override {
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
    bool contains(int px, int py) const override{
        return ( py >= baseY && py <= baseY + height && px >= baseX - (height - (py - baseY)) && px <= baseX + (height - (py - baseY)));
    }
    string toString() const override {
        return "triangle";
    }
    int getId() override{
        return id;
    }
    string getFillMode() override{
        return fillMode;
    };
    char getColor()override{
        return color;
    };
    int getHeight() override{
        return height;
    }
    int getWidth() override{
        return width;
    }
    int getX()override{
        return baseX;
    }
    int getY() override{
        return baseY;
    }
    void setColor(char newColor) override {
        color = newColor;
    }

};

class Square: public Shape{
private:
    int baseX;
    int baseY;
    int height;
    int width = 0;
    int id;
    string fillMode;
    char color;
public:
    Square(int baseX, int baseY, int height, string fillMode, char color):
    baseX(baseX), baseY(baseY), height(height), id(currentId), fillMode(fillMode), color(color){};
    bool draw(Board& board, int x, int y, int height, int width) override {
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
    bool contains(int px, int py) const override{
        return (px >= baseX && px <= baseX + height && py >= baseY && py <= baseY + height);
    }
    string toString() const override {
        return "square";
    }
    int getId() override{
        return id;
    }
    string getFillMode() override{
        return fillMode;
    };
    char getColor()override{
        return color;
    };
    int getHeight() override{
        return height;
    }
    int getWidth() override{
        return width;
    }
    int getX()override{
        return baseX;
    }
    int getY() override{
        return baseY;
    }
    void setColor(char newColor) override {
        color = newColor;
    }
};

class Rectangle: public Shape{
    int baseX;
    int baseY;
    int height;
    int width;
    int id;
    string fillMode;
    char color;
public:
    Rectangle(int baseX, int baseY, int height, int width, string fillMode, char color):
    baseX(baseX), baseY(baseY), height(height), width(width), id(currentId), fillMode(fillMode), color(color){};
    bool draw(Board& board, int x, int y, int height, int width) override{
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
    bool contains(int px, int py) const override{
        return (px >= baseX && px <= baseX + width && py >= baseY && py <= baseY + height);
    }
    string toString() const override {
        return "rectangle";
    }
    int getId() override{
        return id;
    }
    string getFillMode() override{
        return fillMode;
    };
    char getColor()override{
        return color;
    };
    int getHeight() override{
        return height;
    }
    int getWidth() override{
        return width;
    }
    int getX()override{
        return baseX;
    }
    int getY() override{
        return baseY;
    }
    void setColor(char newColor) override{
        color = newColor;
    }
};

class Line: public Shape{
    int baseX;
    int baseY;
    int height;
    int width;
    int id;
    string fillMode;
    char color;
public:
    Line(int baseX, int baseY, int height, int width, string fillMode, char color):
    baseX(baseX), baseY(baseY), height(height), width(width), id(currentId), fillMode(fillMode), color(color){};
    bool draw(Board& board, int x, int y, int height, int width) override{
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
    bool contains(int px, int py) const override{
        if (width == 0) {
            return (px == baseX && py >= baseY && py <= baseY + height);
        } else {
            return (py == baseY && px >= baseX && px <= baseX + width);
        }
    }
    string toString() const override {
        return "line";
    }
    int getId() override{
        return id;
    }
    string getFillMode() override{
        return fillMode;
    };
    char getColor()override{
        return color;
    };
    int getHeight() override{
        return height;
    }
    int getWidth() override{
        return width;
    }
    int getX()override{
        return baseX;
    }
    int getY() override{
        return baseY;
    }
    void setColor(char newColor) override {
        color = newColor;
    }
};

class Circle: public Shape{
private:
    int centerX;
    int centerY;
    int radius;
    int width =0;
    int id;
    string fillMode;
    char color;
public:
    Circle(int centerX, int centerY, int radius, string fillMode, char color):
    centerX(centerX), centerY(centerY),radius(radius), id(currentId), fillMode(fillMode),color(color) {}
    bool draw(Board& board, int x, int y, int radius, int width) override {
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
    bool contains(int px, int py) const override{
        return ((px - centerX) * (px - centerX) + (py - centerY) * (py - centerY) <= radius * radius);
    }
    string toString() const override {
        return "circle";
    }
    int getId() override{
        return id;
    }
    string getFillMode() override{
        return fillMode;
    };

    void setColor(char newColor)override {
        color = newColor;
    }

    char getColor()override{
        return color;
    };
    int getHeight() override{
        return radius;
    }
    int getWidth() override{
        return width;
    }
    int getX()override{
        return centerX;
    }
    int getY() override{
        return centerY;
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

        std::shared_ptr<Line> line;
        if (direction == "vertical"|| direction=="Vertical") {
            line = std::make_shared<Line>(x, y, 0, len, fillMode, symbol);
            if (fillMode == "empty"||fillMode == "Empty"){
                bool value = line->draw(board, x, y, 0, len);
                if(value == 1){
                    currentId++;
                    board.addShape(line,par);
                    cout<<"Line was successfully added!"<<endl;
                }
            } else{
                bool value = line->fill(board, x, y, 0, len, symbol);
                if(value == 1){
                    currentId++;
                    board.addShape(line,par);
                    cout<<"Line was successfully added!"<<endl;
                }
            }
        }
        else if (direction =="horizontal"|| direction=="Horizontal"){
            line = std::make_shared<Line>(x, y,  len, 0, fillMode,symbol);
            if (fillMode == "empty"||fillMode == "Empty"){
                bool value = line->draw(board, x, y, len, 0);
                if(value == 1){
                    currentId++;
                    board.addShape(line,par);
                    cout<<"Line was successfully added!"<<endl;
                }
            } else{
                bool value = line->fill(board, x, y,  len,0, symbol);
                if(value == 1){
                    currentId++;
                    board.addShape(line,par);
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
        auto square = std::make_shared<Square>(x, y, len, fillMode,symbol);
        if (fillMode == "empty"||fillMode == "Empty") {
            bool value = square->draw(board, x, y, len, 0);
            if (value == 1) {
                currentId++;
                board.addShape(square, par);
                cout << "Square was successfully added!" << endl;
            }
        } else{
            bool value = square->fill(board, x, y,  len,0, symbol);
            if (value == 1) {
                currentId++;
                board.addShape(square, par);
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
        currentId++;
        auto triangle = std::make_shared<Triangle>(x, y, len, fillMode,symbol);
        if (fillMode == "empty"||fillMode == "Empty") {
            bool value = triangle->draw(board, x, y, len, 0);
            if (value == 1) {
                board.addShape(triangle, par);
                cout << "Triangle was successfully added!" << endl;
            }
        }else{
            bool value = triangle->fill(board, x, y,  len,0, symbol);
            if (value == 1) {
                board.addShape(triangle, par);
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
        currentId++;
        auto rectangle = std::make_shared<Rectangle>(x, y, len, width, fillMode,symbol);
        if (fillMode == "empty"||fillMode == "Empty") {
            bool value = rectangle->draw(board, x, y, len, width);
            if (value == 1) {
                board.addShape(rectangle, par);
                cout << "Rectangle was successfully added!" << endl;

            }
        }else{
            bool value = rectangle->fill(board, x, y,  len,width, symbol);
            if (value == 1) {
                board.addShape(rectangle, par);
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
        currentId++;
        auto circle = std::make_shared<Circle>(x, y, len, fillMode,symbol);
        if (fillMode == "empty"||fillMode == "Empty") {
            bool value = circle->draw(board, x, y, len, 0);
            if (value == 1) {
                board.addShape(circle, par);
                cout << "Circle was successfully added!" << endl;
            }
        } else{
            bool value = circle->fill(board, x, y,  len,0, symbol);
            if (value == 1) {
                board.addShape(circle, par);
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
                vector<shared_ptr<Shape>> shapes = board.getShapes();
                map<int, string> parameters = board.getDic();
                int id;
                string shape;
                string par;
                for (const auto &pair: parameters) {
                    id = pair.first;
                    par = pair.second;
                    shared_ptr<Shape> shape = shapes[id - 1];
                    outFile << id << "-" << shape->toString() << "-" << par << endl;
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
    shared_ptr<Shape> selectedShape;
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
              "5. Undo: remove the last added shape from the blackboard.\n"//+
              "6. Remove all shapes from blackboard.\n"//+
              "7. Select shape.\n"//+
              "8. Remove the selected shape from the blackboard.\n"//+
              "9. Edit shape.\n"
              "10. Change the colour of the selected figure.\n"
              "11. Move.\n"
              "12. Save the blackboard to the file.\n"
              "13. Load a blackboard from the file.\n"
              "14. Exit.\n"<<endl;
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
        vector<shared_ptr<Shape>> shapes = board.getShapes();
        map<int, string> idParam = board.getDic();
        int id;
        string shape;
        string par;
        for (const auto& pair:idParam){
            id = pair.first;
            par = pair.second;
            shared_ptr<Shape> shape = shapes[id - 1];
            cout<<id<<". "<<shape->toString()<<"->"<<" "<<par << endl;
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
    void select(Board& board, shared_ptr<Shape> shape){
        int id = shape->getId();
        string name =shape->toString();
        map<int, string> parameters = board.getDic();
        auto par = parameters.find(id);
        string shapePar= par->second;
        cout<<"Selected "<<name<<", parameters:"<<shapePar<<endl;
    }

    void removeSelected(Board& board, shared_ptr<Shape> shape) {
        vector<shared_ptr<Shape>> shapes = board.getShapes();
        map<int, string >& idParameters = board.getDic();
        if (shape) {
            auto it = find(shapes.begin(), shapes.end(), shape);
            if (it != shapes.end()) {
                shapes.erase(it);
            }
            int selectedId = shape->getId();
            idParameters.erase(selectedId);
            board.clear();
            for(auto& shape: shapes){
                int id= shape->getId();
                auto par =idParameters.find(id);
                string parameters = par->second;
                vector<string> parts;
                string item, fillMode;
                char color;
                stringstream ss(parameters);
                while (getline(ss, item, ',')) {
                    parts.push_back(item);
                }
                color =shape->getColor();
                fillMode = shape->getFillMode();
                int x = stoi(parts[0]);
                int y = stoi(parts[1]);
                int height = stoi(parts[2]);
                int width =0;
                if(parts.size() ==4){
                    if(parts[3]!="Vertical"||parts[3]!="vertical" || parts[3]!="Horizontal"||parts[3]!="horizontal"){
                        width = stoi(parts[3]);
                        if (fillMode =="empty") { shape->draw(board, x, y, height, width); }
                        else {shape->fill(board,x,y,height,width,color); }
                    }else if(parts[3]=="Vertical"||parts[3]=="vertical" ){
                        if (fillMode =="empty"){ shape->draw(board, x, y, height, width); }
                        else{shape->fill(board, x, y, height, width, color);}
                    }else if(parts[3]=="Horizontal"||parts[3]=="horizontal"){
                        if (fillMode =="empty"){ shape->draw(board, x, y, width, height); }
                        else{shape->fill(board, x, y, width,height, color);}
                    }
                }else{
                    shape->draw(board, x, y, height,width);
                }

            }
        }else{
            cout<<"No selected shape";
        }
    }

    void edit(Board& board, shared_ptr<Shape> shape,string newParam){
        vector<shared_ptr<Shape>>& shapes = board.getShapes();
        map<int, string >& idParameters = board.getDic();
        if (shape) {
            string name = shape->toString();
            string fillMode = shape->getFillMode();
            char color = shape->getColor();
            int id = shape->getId();
            auto it = find(shapes.begin(), shapes.end(), shape);
            if (it != shapes.end()) {
                shapes.erase(it);
            }
            if (name == "Line"||name == "line") {
                stringstream ss(newParam);
                int x,y,len;
                string direction;
                string item;
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
                std::shared_ptr<Line> line;
                if (direction == "vertical"|| direction=="Vertical") {
                    line = std::make_shared<Line>(x, y, 0, len, fillMode, color);
                    shapes.insert(shapes.begin()+id-1, line);
                    idParameters[id] = newParam;
                }
                else if (direction =="horizontal"|| direction=="Horizontal"){
                    line = std::make_shared<Line>(x, y,  len, 0, fillMode,color);
                    shapes.insert(shapes.begin()+id-1, line);
                    idParameters[id] = newParam;
                }
            } else if (name == "Triangle"|| name == "triangle") {
                int x,y,len;
                stringstream ss(newParam);
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
                auto triangle = std::make_shared<Triangle>(x, y, len, fillMode,color);
                shapes.insert(shapes.begin()+id-1, triangle);
                idParameters[id] = newParam;
            } else if (name == "Square"||name == "square") {
                int x,y,len;
                stringstream ss(newParam);
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
                auto square = std::make_shared<Square>(x, y, len, fillMode,color);
                shapes.insert(shapes.begin()+id-1, square);
                idParameters[id] = newParam;
            } else if (name == "Rectangle"||name == "rectangle") {
                stringstream ss(newParam);
                int x,y,len, width;
                string item;
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
                width = stoi(parts[3]);

                auto rectangle = std::make_shared<Rectangle>(x, y, len, width, fillMode,color);
                shapes.insert(shapes.begin()+id-1, rectangle);
                idParameters[id] = newParam;
            } else if (name == "Circle"||name == "circle") {
                stringstream ss(newParam);
                int x,y,len;
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
                auto circle = std::make_shared<Circle>(x, y, len, fillMode,color);
                shapes.insert(shapes.begin()+id-1, circle);
                idParameters[id] = newParam;
            }
            board.clear();
            for(auto& shape: shapes){
                int id= shape->getId();
                auto par =idParameters.find(id);
                string parameters = par->second;
                vector<string> parts;
                string item, fillMode;
                char color;
                stringstream ss(parameters);
                while (getline(ss, item, ',')) {
                    parts.push_back(item);
                }
                color =shape->getColor();
                fillMode = shape->getFillMode();
                int x = stoi(parts[0]);
                int y = stoi(parts[1]);
                int height = stoi(parts[2]);
                int width =0;
                if(parts.size() ==4){
                    if(parts[3]!="Vertical"||parts[3]!="vertical" || parts[3]!="Horizontal"||parts[3]!="horizontal"){
                        width = stoi(parts[3]);
                        if (fillMode =="empty") { shape->draw(board, x, y, height, width); }
                        else {shape->fill(board,x,y,height,width,color); }
                    }else if(parts[3]=="Vertical"||parts[3]=="vertical" ){
                        if (fillMode =="empty"){ shape->draw(board, x, y, height, width); }
                        else{shape->fill(board, x, y, height, width, color);}
                    }else if(parts[3]=="Horizontal"||parts[3]=="horizontal"){
                        if (fillMode =="empty"){ shape->draw(board, x, y, width, height); }
                        else{shape->fill(board, x, y, width,height, color);}
                    }
                }else{
                    shape->draw(board, x, y, height,width);
                }

            }

        }else{
            cout<<"No selected shape";
        }
    }

    void move(Board& board, shared_ptr<Shape> shape,string newParam){
        vector<shared_ptr<Shape>>& shapes = board.getShapes();
        map<int, string >& idParameters = board.getDic();
        if (shape) {
            string name = shape->toString();
            string fillMode = shape->getFillMode();
            char color = shape->getColor();
            int id = shape->getId();
            int width = shape->getWidth();
            int height = shape->getHeight();
            auto it = find(shapes.begin(), shapes.end(), shape);
            if (it != shapes.end()) {
                shapes.erase(it);
            }
            stringstream ss(newParam);
            int x,y;
            string item;
            vector<string> parts;
            while (getline(ss, item, ',')) {
                parts.push_back(item);
            }
            if (parts.size() != 2) {
                cout << "Invalid input format." << endl;
                return;
            }
            x = stoi(parts[0]);
            y = stoi((parts[1]));
            newParam ="";
            newParam+=to_string(x);
            newParam +=",";
            newParam +=to_string(y);

            if (name == "Line"||name == "line") {
                std::shared_ptr<Line> line;
                if (width==0){
                    newParam +=",";
                    newParam +=to_string(height);
                    line = std::make_shared<Line>(x, y,  height, 0, fillMode,color);
                    shapes.insert(shapes.begin()+id-1, line);
                    idParameters[id] = newParam;
                }else if(height==0){
                    newParam +=",";
                    newParam+= to_string(width);
                    line = std::make_shared<Line>(x, y, 0, width, fillMode, color);
                    shapes.insert(shapes.begin()+id-1, line);
                    idParameters[id] = newParam;
                }
            } else if (name == "Triangle"|| name == "triangle") {
                newParam +=",";
                newParam +=to_string(height);
                auto triangle = std::make_shared<Triangle>(x, y, height, fillMode,color);
                shapes.insert(shapes.begin()+id-1, triangle);
                idParameters[id] = newParam;
            } else if (name == "Square"||name == "square") {
                newParam +=",";
                newParam +=to_string(height);
                auto square = std::make_shared<Square>(x, y, height, fillMode,color);
                shapes.insert(shapes.begin()+id-1, square);
                idParameters[id] = newParam;
            } else if (name == "Rectangle"||name == "rectangle") {
                newParam +=",";
                newParam +=to_string(height);
                newParam +=",";
                newParam +=to_string(width);
                auto rectangle = std::make_shared<Rectangle>(x, y, height, width, fillMode,color);
                shapes.insert(shapes.begin()+id-1, rectangle);
                idParameters[id] = newParam;
            } else if (name == "Circle"||name == "circle") {
                newParam +=",";
                newParam +=to_string(height);
                auto circle = std::make_shared<Circle>(x, y, height, fillMode,color);
                shapes.insert(shapes.begin()+id-1, circle);
                idParameters[id] = newParam;
            }
            board.clear();
            for(auto& shape: shapes){
                int id= shape->getId();
                auto par =idParameters.find(id);
                string parameters = par->second;
                vector<string> parts;
                string item, fillMode;
                char color;
                stringstream ss(parameters);
                while (getline(ss, item, ',')) {
                    parts.push_back(item);
                }
                color =shape->getColor();
                fillMode = shape->getFillMode();
                int x = stoi(parts[0]);
                int y = stoi(parts[1]);
                int height = stoi(parts[2]);
                int width =0;
                if(parts.size() ==4){
                    if(parts[3]!="Vertical"||parts[3]!="vertical" || parts[3]!="Horizontal"||parts[3]!="horizontal"){
                        width = stoi(parts[3]);
                        if (fillMode =="empty") { shape->draw(board, x, y, height, width); }
                        else {shape->fill(board,x,y,height,width,color); }
                    }else if(parts[3]=="Vertical"||parts[3]=="vertical" ){
                        if (fillMode =="empty"){ shape->draw(board, x, y, height, width); }
                        else{shape->fill(board, x, y, height, width, color);}
                    }else if(parts[3]=="Horizontal"||parts[3]=="horizontal"){
                        if (fillMode =="empty"){ shape->draw(board, x, y, width, height); }
                        else{shape->fill(board, x, y, width,height, color);}
                    }
                }else{
                    shape->draw(board, x, y, height,width);
                }

            }

        }else{
            cout<<"No selected shape";
        }
    }

    void changeColor(Board& board, shared_ptr<Shape>shape, string newColor)  {
        vector<shared_ptr<Shape>>& shapes = board.getShapes();
        map<int, string >& idParameters = board.getDic();
        char color;
        if (shape) {
            int width = shape->getWidth();
            int height = shape->getHeight();
            int x = shape->getX();
            int y = shape->getY();
            color = newColor[0];
            shape->setColor(color);
            shape->fill(board, x, y, height, width, color);
        }else{
            cout<<"No selected shape"<<endl;
        }
    };

    void manage(){
        Board board(BOARD_WIDTH, BOARD_HEIGHT);
        string shape, userInput, item,value, fillMode, params,symbol, fillColor;
        vector<string> parts;
        vector<string>colors;
        vector<Board> boardStates;
        boardStates.push_back(board);
        printCommands();
        string filePath;
        while(command != "14"){
            cout << "Enter the command and info(in draw command you must specify fill mode or empty shape):";
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
                    cout << "Invalid input for draw command. Please try again." << endl;
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
                board.clearShapes();
                board.clearIdParametres();
                cout << "Board has been cleared." << endl;
                continue;
            }
            else if (command == "7"){
                if(parts.size()==3){
                    int x = stoi(parts[1]);
                    int y = stoi(parts[2]);
                    selectedShape = board.selectByCoordinates(x,y);
                    select(board,selectedShape);
                }else if(parts.size()==2){
                    int id = stoi(parts[1]);
                   selectedShape= board.selectById(id);
                   select(board, selectedShape);
                }else{
                    cout << "Invalid number of parameters. Please try again." << endl;
                    continue;
                }
            }
            else if (command=="8"){

                removeSelected(board, selectedShape);
            }
            else if (command =="9"){
                string params = "";
                int requiredParams = 0;
                shape = selectedShape->toString();
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
                for (int i=1; i<1 + requiredParams; ++i) {
                    params += parts[i];
                    if (i != (1 + requiredParams)) params += ",";
                }
                edit(board,selectedShape,params);
            }else if(command == "10"){
                string color = parts[1];
                changeColor(board,selectedShape, color);

            }
            else if (command =="11"){
                params.clear();
                params+=parts[1];
                params+=",";
                params+=parts[2];
                move(board, selectedShape,params);
            }
            else if (command == "12"){
                filePath = parts[1];
                writer.saveToFile(board, filePath);
                continue;
            }
            else if (command == "13") {
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
};//додати перевірку на додавання такої ж фігури