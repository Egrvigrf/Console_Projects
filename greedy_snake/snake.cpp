#include <iostream> 
#include <cstring>
#include <conio.h> //isn't stl
#include <chrono>  //sleep
#include <thread>
#include<map>
using namespace std;
struct link_snk {
    int x, y;
    link_snk* next = nullptr, * pre = nullptr;
};
const char head_image = '@';
const char body_image = '*';
const int speed = 100;
const int rows = 20, cols = 40;
bool Mp[rows + 1][cols + 1];
int snk_len, maxlen = 2;
char snk_dir;
link_snk* snk_head, * snk_tail;
int fruit_x, fruit_y;
int money_x, money_y;
bool fruit_is_exist = true;
const char matchdir[4] = { 'd', 's', 'a', 'w' };
map<char, char> collide = { {'a','d'},{'d','a'},{'w','s'},{'s','w'} };
void game_init();
bool replay();
void print_x_y(int y, int x, char c) { cout << "\033[" << y << ";" << x << "H" << c; }
void print_x_y(int y, int x, string c) { cout << "\033[" << y << ";" << x << "H" << c; }
void Snap(int t) { chrono::milliseconds pauseTime(t); this_thread::sleep_for(pauseTime); }
void fruit_generate();
bool move();
bool check(char);
int main() {
    do {
        game_init();
        while (true) {
            if (!fruit_is_exist) fruit_generate();
            if (!move()) break;
            Snap(speed);
        }
    } while (replay());
    return 0;
}
bool check(char s) {
    for(const char& t : matchdir) 
        if(s == t) return true;
    return false;
}
bool move() {
    if (kbhit()) {
        char dir = getch(); // avoid direction collision
        if (check(dir) && collide[dir] == snk_dir) dir = collide[dir];
        snk_dir = dir;
    }
    map<char, pair<int, int>> mp = { {'w',{-1,0}},{'s',{1,0}},{'a',{0,-1}},{'d',{0,1}} };
    int newhead_x = snk_head->x + mp[snk_dir].first, newhead_y = snk_head->y + mp[snk_dir].second;
    if (Mp[newhead_x][newhead_y]) {
        return false;
    } else {
        print_x_y(newhead_x, newhead_y, head_image);
        Mp[newhead_x][newhead_y] = 1;
        print_x_y(snk_head->x, snk_head->y, body_image);
        link_snk* p = new link_snk;
        p->x = newhead_x;
        p->y = newhead_y;
        p->next = snk_head;
        p->pre = nullptr;
        snk_head->pre = p;
        snk_head = p;
        if (fruit_x == newhead_x and fruit_y == newhead_y) {
            fruit_is_exist = false;
            snk_len++;
        } else {
            print_x_y(snk_tail->x, snk_tail->y, ' ');
            Mp[snk_tail->x][snk_tail->y] = 0;
            snk_tail = snk_tail->pre;
            delete snk_tail->next;       // delete tail
            snk_tail->next = nullptr;
        }
    }
    return true;
}
void game_init() {
    cout << "\x1b[2J"; // clear screen
    cout << "\e[?25l"; // hide cursor
    memset(Mp, 0, sizeof(Mp));//0 means empty
    srand(static_cast<unsigned int>(time(NULL)));
    snk_len = 2;
    int randxy[4][2] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0} };
    for (int i = 1; i <= cols; i++) print_x_y(1, i, '#'), Mp[1][i] = 1;
    for (int i = 2; i <= rows - 1; i++) {
        for (int j = 1; j <= cols; j++) {
            if (j == 1 || j == cols) {
                print_x_y(i, j, '#');
                Mp[i][j] = 1;
            } else print_x_y(i, j, ' ');
        }
    }
    for (int i = 1; i <= cols; i++) print_x_y(rows, i, '#'), Mp[rows][i] = 1;
    snk_head = new link_snk; // initailize snake
    snk_head->x = rand() % (rows - 4) + 3;
    snk_head->y = rand() % (cols - 4) + 3;
    print_x_y(snk_head->x, snk_head->y, head_image);
    snk_tail = new link_snk;
    int i = rand() % 4;
    snk_tail->x = snk_head->x + randxy[i][0];
    snk_tail->y = snk_head->y + randxy[i][1];
    snk_dir = matchdir[i];
    snk_head->next = snk_tail;
    snk_tail->pre = snk_head;
    print_x_y(snk_tail->x, snk_tail->y, body_image);
    fruit_generate(); // initailize fruit
    print_x_y(rows+2, 1, "Tap 'a' or 'w' or 'd' or 's' to start");
    bool is_start = false;
    while (!is_start) {
        for (int i =  1; i <= 5; i++) {
            if (kbhit()) {
                char dir = getch(); // avoid direction collision
                if (check(dir) && collide[dir] != snk_dir) {
                    snk_dir = dir;
                    is_start = true;
                    break;
                }
            }
            print_x_y(rows + 3, i, '.');
            Snap(200);
        }
        print_x_y(rows + 3, 1, "     ");
    }
    print_x_y(rows + 3, 1, "     ");
    print_x_y(rows + 2, 1, "                                     ");
    print_x_y(rows + 2, 1, "History Record:");
    cout << maxlen << endl;
}
void fruit_generate() {
    do {
        fruit_x = rand() % (rows - 2) + 2;
        fruit_y = rand() % (cols - 2) + 2;
    } while (Mp[fruit_x][fruit_y]);
    print_x_y(fruit_x, fruit_y, '.');
    fruit_is_exist = true;
}
bool replay() {
    cout << "\x1b[2J"; // clear screen
    print_x_y(1, 1, "Game over! \n");
    cout << "length:" << snk_len;
    if (snk_len > maxlen) {
        maxlen = snk_len;
        cout << " \nCongratulations! You break the record.";
    } 
    cout << endl;
    cout << "Tap ant key to replay or 'q' for quitting";
    while (true) {
        for (int i = 1; i <= 5; i++) {
            if (kbhit()) {
                char re = getch();
                if (re == 'q')
                    return false;
                return true;
            }
            print_x_y(5, i, '.');
            Snap(200);
        }
        print_x_y(5, 1, "     ");
    }
}
