#include <iostream> //20231225 Snake2
#include <random>
#include <cstring>
#include <conio.h> //isn't stl
#include <chrono>  //sleep
#include <thread>
using namespace std;
struct link_snk
{
    int x, y;
    link_snk *next = NULL;
};
class snk
{
public:
    int speed, len;
    char dir, snk_head_image, snk_body_image;
    link_snk *head;
    snk()
    {
        speed = 150;
        len = 2;
        snk_head_image = '@';
        snk_body_image = '*';
    }
    void premove(int &, int &);
    void move(int, int);
    void eat(int, int);
};
int maxscore = 2;
int score = 2;
int rows, cols;
int fruit_x, fruit_y;
int money_x, money_y;
string die_type;
bool fruit_is_exist = true;
bool money_is_exist = false;
bool money_first_genetate = true;
snk snake;
snk *const snake_p = &snake;
void game_init();
bool crash(int, int, bool);
;
bool replay();
void print_x_y(int y, int x, char c)
{
    cout << "\033[" << y << ";" << x << "H" << c;
}
void print_x_y(int y, int x, string c)
{
    cout << "\033[" << y << ";" << x << "H" << c;
}
void Snap(int t)
{
    chrono::milliseconds pauseTime(t);
    this_thread::sleep_for(pauseTime);
}
void fruit_generate()
{
    do
    {
        fruit_x = rand() % (rows - 2) + 2;
        fruit_y = rand() % (cols - 2) + 2;
    } while (crash(fruit_x, fruit_y, false) and (money_is_exist ? (fruit_x != money_x and fruit_y != money_y) : true));
    print_x_y(fruit_x, fruit_y, '.');
    fruit_is_exist = true;
    money_first_genetate = true;
}
void money_genetate()
{
    do
    {
        money_x = rand() % (rows - 2) + 2;
        money_y = rand() % (cols - 2) + 2;
    } while (crash(money_x, money_y, false) and money_x != fruit_x and money_y != fruit_y);
    print_x_y(money_x, money_y, '$');
    money_is_exist = true;
}
int main()
{
    do
    {
        game_init();
        while (true)
        {
            if (!fruit_is_exist)
                fruit_generate();
            int h_x, h_y; // future coordinates of new head
            snake.premove(h_x, h_y);
            if (crash(h_x, h_y, true))
                break;
            else if (fruit_x == h_x and fruit_y == h_y)
                snake.eat(h_x, h_y);
            else if (money_is_exist and money_x == h_x and money_y == h_y)
            {
                score += 5;
                cout << "\033[" << 2 << ";" << cols + 15 << "H" << score;
                snake.move(h_x, h_y);
            }
            else
                snake.move(h_x, h_y);
            if (money_is_exist)
            {
                if (rand() % (snake.speed) == 1)
                {
                    money_is_exist = false;
                    print_x_y(money_x, money_y, ' ');
                }
            }
            else
            {
                if (snake.len % (rand() % 3 + 1) == 0 and rand() % ((rows + cols) / 3) == 1 and money_first_genetate)
                {
                    money_genetate();
                    money_first_genetate = false;
                }
            }
            Snap(snake.speed);
        }
    } while (replay());
    return 0;
}
void snk::premove(int &newhead_x, int &newhead_y)
{
    if (kbhit())
    {
        char dir = getch(); // avoid direction collision
        if (dir == 'a' || dir == 's' || dir == 'd' || dir == 'w')
        {
            if (dir == 'w' and snake_p->dir == 's')
                dir = 's';
            else if (dir == 's' and snake_p->dir == 'w')
                dir = 'w';
            else if (dir == 'a' and snake_p->dir == 'd')
                dir = 'd';
            else if (dir == 'd' and snake_p->dir == 'a')
                dir = 'a';
            snake_p->dir = dir;
        }
    }
    switch (snake_p->dir)
    {
    case 'w':
        newhead_x = (snake_p->head)->x - 1;
        newhead_y = (snake_p->head)->y;
        break;
    case 's':
        newhead_x = (snake_p->head)->x + 1;
        newhead_y = (snake_p->head)->y;
        break;
    case 'a':
        newhead_x = (snake_p->head)->x;
        newhead_y = (snake_p->head)->y - 1;
        break;
    case 'd':
        newhead_x = (snake_p->head)->x;
        newhead_y = (snake_p->head)->y + 1;
    }
}
void snk::move(int new_x, int new_y)
{
    link_snk *p = snake.head;
    while ((p->next)->next != NULL)
        p = p->next;
    print_x_y((p->next)->x, (p->next)->y, ' ');
    delete p->next;
    p->next = NULL;   // delete tail
    p = new link_snk; // change head
    p->next = snake_p->head;
    p->x = new_x;
    p->y = new_y;
    print_x_y(new_x, new_y, snake.snk_head_image);
    print_x_y(snake_p->head->x, snake_p->head->y, snake.snk_body_image); // head image -> body image
    snake_p->head = p;
}
void snk::eat(int new_x, int new_y)
{
    print_x_y(snake_p->head->x, snake_p->head->y, snake.snk_body_image);
    link_snk *p = new link_snk;
    p->x = new_x;
    p->y = new_y;
    p->next = snake_p->head;
    snake.head = p;
    print_x_y(new_x, new_y, snake.snk_head_image);
    snake.len++;
    score++;
    fruit_is_exist = false;
    cout << "\033[" << 2 << ";" << cols + 15 << "H" << score;
}
void game_init()
{
    cout << "\x1b[2J"; // clear screen
    cout << "\e[?25l"; // hide cursor
    srand(static_cast<unsigned int>(time(nullptr)));
    snake.len = 2;
    score = 2;
    rows = 20; //>=7
    cols = 20; //>=7
    int randxy[4][2] = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
    char matchdir[4] = {'d', 's', 'a', 'w'};
    print_x_y(1, 1, string(cols, '#'));
    for (int i = 2; i <= rows - 1; i++)
    {
        print_x_y(i, 1, '#');
        print_x_y(i, cols, '#');
    }
    print_x_y(rows, 1, string(cols, '#'));
    snake.head = new link_snk; // initailize snake
    snake.head->x = rand() % (rows - 4) + 3;
    snake.head->y = rand() % (cols - 4) + 3;
    print_x_y(snake.head->x, snake.head->y, snake.snk_head_image);
    link_snk *tail = new link_snk;
    int i = rand() % 4;
    tail->x = snake.head->x + randxy[i][0];
    tail->y = snake.head->y + randxy[i][1];
    snake.dir = matchdir[i];
    snake.head->next = tail;
    print_x_y(tail->x, tail->y, snake.snk_body_image);
    fruit_generate(); // initailize fruit
    print_x_y(1, cols + 1, "Tap 'a' or 'w' or 'd' or 's' to start");
    bool is_start = false;
    while (!is_start)
    {
        for (int i = cols + 1; i <= cols + 5; i++)
        {
            if (kbhit())
            {
                char dir = getch(); // avoid direction collision
                if (dir == 'a' || dir == 's' || dir == 'd' || dir == 'w')
                {
                    if (dir == 'w' and snake_p->dir == 's')
                        continue;
                    else if (dir == 's' and snake_p->dir == 'w')
                        continue;
                    else if (dir == 'a' and snake_p->dir == 'd')
                        continue;
                    else if (dir == 'd' and snake_p->dir == 'a')
                        continue;
                    snake_p->dir = dir;
                    is_start = true;
                    break;
                }
            }
            print_x_y(2, i, '.');
            Snap(200);
        }
        print_x_y(2, cols + 1, "     ");
    }
    print_x_y(1, cols + 1, "                                     ");
    print_x_y(1, cols + 1, "History record:");
    cout << maxscore << endl;
    print_x_y(2, cols + 1, "Current score:2");
}
bool crash(int x, int y, bool snk_crash)
{
    link_snk *p = snake.head;
    if ((x == 1) || (x == rows) || (y == 1) || (y == cols))
    {
        if (snk_crash)
            die_type = "You crash the wall!";
        return true;
    }
    while (p->next != NULL)
    {
        if ((x == p->x) & (y == p->y))
        {
            if (snk_crash)
                die_type = "You crash yourself!";
            return true;
        }
        p = p->next;
    }
    if ((x == p->x) & (y == p->y))
    {
        if (snk_crash)
            die_type = "You crash yourself!";
        return true;
    }
    return false;
}
bool replay()
{
    cout << "\x1b[2J"; // clear screen
    print_x_y(1, 1, "Game over! " + die_type);
    cout << "\nLen:" << snake.len << " Score:" << score;
    if (score > maxscore)
    {
        maxscore = score;
        cout << " Congratulations! You break the record." << endl;
    }
    else
        cout << endl;
    cout << "Tap ant key to replay or 'q' for quitting";
    while (true)
    {
        for (int i = 1; i <= 5; i++)
        {
            if (kbhit())
            {
                char re = getch();
                if (re == 'q')
                    return false;
                return true;
            }
            print_x_y(4, i, '.');
            Snap(200);
        }
        print_x_y(4, 1, "     ");
    }
}