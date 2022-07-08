#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <QApplication>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QMouseEvent>
#include <QString>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QRadioButton>
#include <QPixmap>
#include <QStack>
#include <QFont>


#include <iostream>
#include <random>
#include <math.h>
#include <string>
#include <time.h>
#include <windows.h>
#include <string>
#include <unistd.h>
#include <random>
#include <tuple>
#include <stdlib.h>
#include <QRect>

#define MAX_NUM 9

using namespace std;

//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------

//CLASSES

//board 3x3 class
class board
{
public:
    char m[3][3];
    int flag;

    board();
    void Fill();
    char GameEnd();
    board operator=(board temp);
    bool operator==(board temp);
    void MakeAMove(int i, int j, char sign);
};

//board 5x5 class
class board5x5
{
public:
    char m[5][5];

    board5x5();
    char GameEnd();
    board5x5 operator=(board5x5 temp);
    bool operator==(board5x5 temp);
    void MakeAMove(int i, int j, char sign);

};

//small matrix for ultimate TTT
class smallBoard
{
public:
    char m[3][3];
    char flag;

    smallBoard();
    void Print();
    void Fill();
    int MakeAMove(int number, char sign);
    char GameEnd();
};

//big matrix for ultimate TTT
class bigBoard
{
public:
    smallBoard boards[9];
    bigBoard();
    void Print();
    char GameEnd();
    int MakeAMove(int number, char sign);
};

//player class
class player
{
public:
    char sign;
    QString name;
    int win1v1 = 0;
    int winBot = 0;

    player();
    player(char s, QString n, int w);

    friend class MyMainWindow;
    board5x5 MakeAMove(board5x5 b);
};

//bot class
class bot
{
public:
    string name;
    char sign;
    int win;
    int tie;

    bot();
    bot(char s, int win);
    void Move(int* i, int* j);
    board5x5 Move(board5x5 b);
    friend class MyMainWindow;
};

//Ultimate TTT
class UltimateTTT : public QDialog
{
public:
    void m();
    void re();
    bigBoard matrix;
    QPushButton* MyButton[81];
    UltimateTTT();
    QLabel* MyLabel;
    QLabel* NextMove;
    QLabel* next;
    void handleButton(int i);
    int CheckIfEndUltimate();
    void ResetGameU();
    QRect* rect;
};

//Babushka mode
class BabushkaTTT : public QDialog
{
    // 1->little orange
    // 2->medium orange
    // 3->big orange

    // -1->little blue
    // -2->medium blue
    // -3->big blue

public:
    int character_size;
    board b;
    QLabel* MyLabel;
    QLabel* labelO1;
    QLabel* labelO2;
    QLabel* labelO3;
    QLabel* labelB1;
    QLabel* labelB2;
    QLabel* labelB3;
    BabushkaTTT();
    int OrangeRemaining[3];
    int BlueRemaining[3];
    QStack<int> stack[9];
    void re();
    void info();
    QPushButton* MyButton[9];
    QPushButton* Big;
    QPushButton* Medium;
    QPushButton* Little;
    QPushButton* Remove_character;
    void handleButton(int i);
    void UpdateCharacterSize(int m);
    int CheckIfEndBabushka();
    int flag_array[9];
    void ResetGameB();
    void UpdateRemaining(int br);
    void Remove_character_function();
    bool remove;
    bool cant_pick_size;
    void SetButton(int i, int peek);
};


//Pocetni izbornik
class MyMainMenu : public QDialog
{
public:
    QLabel* Label;
    QLabel* Label2;

    QPixmap* filename;
    QVBoxLayout* mainLayout;
    QWidget* editWidget;
    QFormLayout* editLayout;
    QFormLayout* editLayout2;
    QLineEdit* lineEdit;
    QLineEdit* lineEdit2;
    QDialogButtonBox* buttonBox;

    MyMainMenu();
};

//dijalog za unos imena igraca
class MyDialog : public QDialog
{
public:
    MyDialog();
    QVBoxLayout* mainLayout;
    QWidget* editWidget;
    QFormLayout* editLayout;
    QFormLayout* editLayout2;
    QLineEdit* lineEdit;
    QLineEdit* lineEdit2;
    QDialogButtonBox* buttonBox;
};

//Glavni prozot
class MyMainWindow : public QMainWindow
{
public:
MyMainWindow();
QMenu* EditMenu;
QAction* three;
QAction* five;
QAction* Result;
QAction* Reset;
QAction* vsBot;
QAction* Names;
QAction* ReadFile;
QAction* ChangeButtonColors;
QAction* Ultimate;
QAction* Babushka;
QLabel* MyLabel;

QPushButton* MyButton1;
QPushButton* MyButton2;
QPushButton* MyButton3;
QPushButton* MyButton4;
QPushButton* MyButton5;
QPushButton* MyButton6;
QPushButton* MyButton7;
QPushButton* MyButton8;
QPushButton* MyButton9;
void handleButton1();
void handleButton2();
void handleButton3();
void handleButton4();
void handleButton5();
void handleButton6();
void handleButton7();
void handleButton8();
void handleButton9();

int CheckIfEnd();
void ResetGame();
void ResetGame2();
void PrintScoreBoard();
friend class player;
void Change();
void VsBot();
void BotMove();
void UpdateAgainstBot();
void Start5x5Game();
void StartUltimateGame();
void StartBabushkaGame();
};

//Moji botuni
//...

//5x5 TTT
class Game5x5 : public QDialog
{
public:
    QPushButton* MyButton[25];
    Game5x5();
    QLabel* MyLabel;
    void handleButton(int i);
    int CheckIfEnd5();
    void ResetGame5();
    QMenu* EditMenu;
    QAction* five;
    void UpdateBr();
    void BotMove5x5(int numberOfMoves);
};

//dijalog za mijenjanje boja botuna
class ChangeButtonColor : public QDialog
{
public:
    QVBoxLayout* mainLayout;
    QWidget* editWidget;
    QWidget* editWidget2;
    QFormLayout* editLayout;
    QFormLayout* editLayout2;
    QRadioButton* RadioButton1;
    QRadioButton* RadioButton2;
    QRadioButton* RadioButton3;
    QRadioButton* RadioButton4;
    QRadioButton* RadioButton5;
    QPushButton* OKButton;

    QDialogButtonBox* buttonBox;
  ChangeButtonColor();
  friend void Change();
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------

//OTHER FUNCTIONS

//upis/ispis iz filea
void SaveInFile();
void ReadFromFile();

//pomocne funkcije za promjenu boje botuna
void Update1();
void Update2();
void Update3();
void Update4();
void Update5();

//prihvaca unos iz dijaloga za unos imena
void Dialog();

//bot turn off
void UpdateAgainstBot();

//resetira ultimate TTT game
void ResetGameU();

//info for ultimate TTT
void mfun();

//The minimax algorithm
int minimax(board btemp, bool isMax, char sign);
int minimax5x5(board5x5 b, bool isMax, char sign, int alpha, int beta, int depth, int numberOfMoves);

//return x if given o and vice versa
char otherSign(char sign);

//Pomocne funkcije za 5x5 bota
int WinInOne(char sign, board5x5 b, int& n, int& m);
int DefendInOne(char sign, board5x5 b, int& n, int& m);

int WinInTwo(char sign, board5x5 b, int& n, int& m);
int DefendInTwo(char sign, board5x5 b, int& n, int& m);

#endif
