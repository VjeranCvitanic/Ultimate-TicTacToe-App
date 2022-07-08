#include "Functions.h"
#include <QDebug>
using namespace std;


//----------------------------------------------------------------------------------
//CTRL + ALT GR + U za collapsat sve funkcije
//----------------------------------------------------------------------------------



//GLOBAL VARIABLES

int draw = 0;                      //broj neodlucenih ishoda
int br = 1;                        // ima vrijednost 1 ili 2, ovisno o tome igra li sljedeci potez igrac 1 ili igrac 2
int AgainstBot = 0;                //ako je ukljucen vsBot mode -> AgainstBot = 1
int rb = 0;                        //pamti korisnikov izbor kod biranja boja buttona
int numberOfMoves = 0;              //broj poteza kod 5x5, koristi se kod bota

int bbn = 0;                     //big board number
int numofmoves = 0;              //Za ultimate TTT

bigBoard matrix;                   //matrica za ultimate
board b;                           // matrica za 3x3
board5x5 b5;                       // matrica za 5x5
bot AI('x', 0);                    // bot
player p1('x', "Player one", 0), p2('o', "Player two", 0);      //dva igraca

int flag[9] = { 0 };                //oznacuje je li neki button pritisnut ili ne
int flag5x5[25] = { 0 };            //isto, za 5x5

int bestMove[2] = { 0 };            // pamti najbolji moguci potez, kod botovog igranja
int num = 1;                        // prilikom biranja boja buttona, pamti mijenja li se boja za 'X' igraca ili za 'O' igraca

QString text1 = "background-color: rgb(255, 155, 0); color: rgb(255, 255, 255)";      //narancasta boja za 'X' igraca
QString text2 = "background-color: rgb(0, 140, 0); color: rgb(255, 255, 255)";        //zelena boja za 'O' igraca
QString text3 = "background-color: rgb(255, 255, 255); color: rgb(255, 255, 255)";        //tie boja za ultimate TTT

//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

//CLASS FUNCTIONS

//board class functions
board::board()
{
    flag = 0;
    int i = 0, j = 0;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            m[i][j] = ' ';
}
char board::GameEnd()
{
    int a = 0, b = 0;

    while (a < 3)
    {
        if (m[a][b] == m[a][b + 1] and m[a][b] == m[a][b + 2] && m[a][b + 2] != ' ')
        {
            return m[a][b];
        }

        a = a + 1;
    }

    a = 0;

    while (b < 3)
    {
        if (m[a][b] == m[a + 1][b] and m[a][b] == m[a + 2][b] && m[a][b] != ' ')
        {
            return m[a][b];
        }

        b = b + 1;
    }

    if (m[0][0] == m[1][1] and m[0][0] == m[2][2] and m[1][1] != ' ')
    {
        return m[1][1];
    }

    else if (m[0][2] == m[1][1] and m[0][2] == m[2][0] and m[1][1] != ' ')
    {
        return m[1][1];
    }

    else if (m[0][0] != ' ' and m[0][1] != ' ' and m[0][2] != ' ' and m[1][0] != ' ' and m[1][1] != ' ' and m[1][2] != ' ' and m[2][0] != ' ' and m[2][1] != ' ' and m[2][2] != ' ')
    {
        return 't';
    }

    else return '0';
}
board board::operator=(board temp)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            m[i][j] = temp.m[i][j];

    return temp;
}
bool board::operator==(board temp)
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (m[i][j] != temp.m[i][j])
                return false;

    return true;
}


//player class functions
player::player()
{
    sign = ' ';
    name = " ";
    win1v1 = 0;
    winBot = 0;
}
player::player(char s, QString n, int w)
{
    sign = s;
    name = n;
    win1v1 = w;
}
void board::MakeAMove(int i, int j, char sign)
{
    m[i][j] = sign;
}
void board5x5::MakeAMove(int i, int j, char sign)
{
    m[i][j] = sign;
}

//bot class functions
bot::bot()
{
    name = "Bot";
    sign = ' ';
    win = 0;
    tie = 0;
}
bot::bot(char s, int w)
{
    name = "Bot";
    sign = s;
    win = w;
    tie = 0;
}
void bot::Move(int* i, int* j)
{

        int bestMove[2] = { 0 };
        board btemp = b;
        int score = 0;
        int bestScore = -5;

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (btemp.m[i][j] == ' ')
                {
                    btemp.m[i][j] = 'o';
                    score = minimax(btemp, false, 'o');
                    btemp.m[i][j] = ' ';
                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestMove[0] = i;
                        bestMove[1] = j;
                    }
                }

        *i = bestMove[0];
        *j = bestMove[1];
}

//board5x5 class functions
board5x5::board5x5()
{
    for(int i = 0;i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            m[i][j] = ' ';
        }
}
char board5x5::GameEnd()
{
    int a = 0, b = 0;
    int flag = 0;

    //checks rows
    for(a = 0; a < 5; a++)
    {
        for (b = 0; b < 2; b++)
        {
            if (m[a][b] == m[a][b + 1] && m[a][b] == m[a][b + 2] && m[a][b] == m[a][b + 3] && m[a][b] != ' ')
            {
                return m[a][b];
            }
        }
    }

    //checks collumns
    for (b = 0; b < 5; b++)
    {
        for (a = 0; a < 2; a++)
        {
            if (m[a][b] == m[a + 1][b] && m[a][b] == m[a + 2][b] && m[a][b] == m[a + 3][b] && m[a][b] != ' ')
            {
                return m[a][b];
            }
        }
    }


    //checks the main diagonal
    for (a = 0; a < 2; a++)
    {
        if (m[a][a] == m[a + 1][a + 1] && m[a][a] == m[a + 2][a + 2] && m[a][a] == m[a + 3][a + 3] && m[a][a] != ' ')
        {
            return m[a][a];
        }

    }

    //checks the other diagonal
    for (a = 0; a < 2; a++)
    {
        if (m[a][4 - a] == m[a + 1][3 - a] && m[a][4 - a] == m[a + 2][2 - a] && m[a][4 - a] == m[a + 3][1 - a] && m[a][4 - a] != ' ')
        {
            return m[a][4 - a];
        }

    }


    if (m[1][0] == m[2][1] && m[1][0] == m[3][2] && m[1][0] == m[4][3] && m[1][0] != ' ')
        return m[1][0];

    if (m[0][1] == m[1][2] && m[0][1] == m[2][3] && m[0][1] == m[3][4] && m[0][1] != ' ')
        return m[0][1];

    if (m[0][3] == m[1][2] && m[0][3] == m[2][1] && m[0][3] == m[3][0] && m[0][3] != ' ')
        return m[0][3];

    if (m[1][4] == m[2][3] && m[1][4] == m[3][2] && m[1][4] == m[4][1] && m[1][4] != ' ')
        return m[1][4];


    //checks if the matrix is full -> (tie game)
    for(a = 0; a < 5; a++)
        for (b = 0; b < 5; b++)
        {
            if (m[a][b] == ' ')flag = 1;
        }

    if (flag == 0)return 't';

    else return '0';
}
board5x5 board5x5:: operator=(board5x5 temp)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            m[i][j] = temp.m[i][j];

    return temp;
}
bool board5x5::operator==(board5x5 temp)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            if (m[i][j] != temp.m[i][j])
                return false;

    return true;
}


//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

//Ultimate TTT

//small board class functions
smallBoard::smallBoard()
{
    int i = 0, j = 0;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            m[i][j] = ' ';

    flag = '0';
}
char smallBoard::GameEnd()
{
    int a = 0, b = 0;

    while (a < 3)
    {
        if (m[a][b] == m[a][b + 1] and m[a][b] == m[a][b + 2] && m[a][b + 2] != ' ')
        {
            flag = m[a][b];
            //Fill();
            return m[a][b];
        }

        a = a + 1;
    }

    a = 0;

    while (b < 3)
    {
        if (m[a][b] == m[a + 1][b] and m[a][b] == m[a + 2][b] && m[a][b] != ' ')
        {
            flag = m[a][b];
            //Fill();
            return m[a][b];
        }

        b = b + 1;
    }

    if (m[0][0] == m[1][1] and m[0][0] == m[2][2] and m[1][1] != ' ')
    {
        flag = m[1][1];
        //Fill();
        return m[1][1];
    }

    else if (m[0][2] == m[1][1] and m[0][2] == m[2][0] and m[1][1] != ' ')
    {
        flag = m[1][1];
        //Fill();
        return m[1][1];
    }

    else if (m[0][0] != ' ' and m[0][1] != ' ' and m[0][2] != ' ' and m[1][0] != ' ' and m[1][1] != ' ' and m[1][2] != ' ' and m[2][0] != ' ' and m[2][1] != ' ' and m[2][2] != ' ')
    {
        flag = 't';
        //Fill();
        return 't';
    }

    else
    {
        flag = '0';
        return '0';
    }
}

//Big board class functions
bigBoard::bigBoard()
{
    for (int i = 0; i < 9; i++)
    {
        boards[i].GameEnd();
    }
}
char bigBoard::GameEnd()
{
    for(int i = 0; i < 9; i++)
    {
        if(matrix.boards[i].GameEnd() != '0')
            matrix.boards[i].flag = matrix.boards[i].GameEnd();
    }

    int signal = 0;


    if (boards[0].flag == boards[1].flag && boards[0].flag == boards[2].flag && boards[0].flag != '0')
        return boards[0].flag;
    if (boards[3].flag == boards[4].flag && boards[3].flag == boards[5].flag && boards[3].flag != '0')
        return boards[3].flag;
    if (boards[6].flag == boards[7].flag && boards[6].flag == boards[8].flag && boards[6].flag != '0')
        return boards[6].flag;


    if (boards[0].flag == boards[3].flag && boards[0].flag == boards[6].flag && boards[0].flag != '0')
        return boards[0].flag;
    if (boards[1].flag == boards[4].flag && boards[1].flag == boards[7].flag && boards[1].flag != '0')
        return boards[1].flag;
    if (boards[2].flag == boards[5].flag && boards[2].flag == boards[8].flag && boards[2].flag != '0')
        return boards[2].flag;

    if (boards[0].flag == boards[4].flag and boards[0].flag == boards[8].flag and boards[0].flag != '0')
    {
        return boards[0].flag;
    }

    if (boards[2].flag == boards[4].flag and boards[2].flag == boards[6].flag and boards[2].flag != '0')
    {
        return boards[2].flag;
    }

    for(int i = 0; i < 9; i++)
    {
        if (boards[i].flag == '0')signal = 1;
    }
    if (signal == 0)return 't';

    return '0';
}

//UltimateTTT class functions
void UltimateTTT::re()
{
    ResetGameU();
    reject();
}
//Prozor za igru Ultimate TTT
UltimateTTT::UltimateTTT()
{
    setWindowTitle("Ultimate Tic Tac Toe");
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    QLabel* NextMove = new QLabel(this);
    NextMove->setText("Next move(index of small matrix in big matrix): ");
    NextMove->move(90, 55);

    next = new QLabel(this);
    next->setText("any");
    next->move(420, 55);

    MyLabel = new QLabel(this);
    MyLabel->setText("ULTIMATE TIC TAC TOE:\nTo read info click 'info' button");
    MyLabel->move(90,15);
    setStyleSheet("background-color: rgb(255, 255, 204)");

    QPushButton* oo = new QPushButton(this);

    oo->setText("Info");
    oo->setGeometry(600, 510, 100, 40);
    oo->setStyleSheet("background-color: light gray");
    connect(oo, &QPushButton::clicked, this, &mfun);

    QPushButton* exit = new QPushButton(this);

    exit->setText("Exit");
    exit->setGeometry(600, 550, 100, 40);
    exit->setStyleSheet("background-color: light gray");
    connect(exit, &QPushButton::clicked, this, &UltimateTTT::re);

    int x = 50, y = 50;
    int incrementx = 0;
    int incrementy = 30;

    for(int a = 0; a < 3; a++)
    {

        for(int b = 0; b < 3; b++)
        {

            for(int i = 0; i < 3; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    x = 50*(j + 1) + incrementx;
                    y = 50*(i + 1) + incrementy;
                    MyButton[a*27 + b*9 + i*3 + j] = new QPushButton(this);
                    MyButton[a*27 + b*9 + i*3 + j]->setText(QString::number(3*i + j + 1));
                    MyButton[a*27 + b*9 + i*3 + j]->setStyleSheet("background-color: light gray");

                    MyButton[a*27 + b*9 + i*3 + j]->setGeometry(x, y, 50, 50);
                }
            }

            incrementx += 170;
        }

        incrementy += 170;
        incrementx = 0;
    }


        connect(MyButton[0], &QPushButton::clicked, this, [this]{ handleButton(0); });
        connect(MyButton[1], &QPushButton::clicked, this, [this]{ handleButton(1); });
        connect(MyButton[2], &QPushButton::clicked, this, [this]{ handleButton(2); });
        connect(MyButton[3], &QPushButton::clicked, this, [this]{ handleButton(3); });
        connect(MyButton[4], &QPushButton::clicked, this, [this]{ handleButton(4); });

        connect(MyButton[5], &QPushButton::clicked, this, [this]{ handleButton(5); });
        connect(MyButton[6], &QPushButton::clicked, this, [this]{ handleButton(6); });
        connect(MyButton[7], &QPushButton::clicked, this, [this]{ handleButton(7); });
        connect(MyButton[8], &QPushButton::clicked, this, [this]{ handleButton(8); });
        connect(MyButton[9], &QPushButton::clicked, this, [this]{ handleButton(9); });

        connect(MyButton[10], &QPushButton::clicked, this, [this]{ handleButton(10); });
        connect(MyButton[11], &QPushButton::clicked, this, [this]{ handleButton(11); });
        connect(MyButton[12], &QPushButton::clicked, this, [this]{ handleButton(12); });
        connect(MyButton[13], &QPushButton::clicked, this, [this]{ handleButton(13); });
        connect(MyButton[14], &QPushButton::clicked, this, [this]{ handleButton(14); });

        connect(MyButton[15], &QPushButton::clicked, this, [this]{ handleButton(15); });
        connect(MyButton[16], &QPushButton::clicked, this, [this]{ handleButton(16); });
        connect(MyButton[17], &QPushButton::clicked, this, [this]{ handleButton(17); });
        connect(MyButton[18], &QPushButton::clicked, this, [this]{ handleButton(18); });
        connect(MyButton[19], &QPushButton::clicked, this, [this]{ handleButton(19); });

        connect(MyButton[20], &QPushButton::clicked, this, [this]{ handleButton(20); });
        connect(MyButton[21], &QPushButton::clicked, this, [this]{ handleButton(21); });
        connect(MyButton[22], &QPushButton::clicked, this, [this]{ handleButton(22); });
        connect(MyButton[23], &QPushButton::clicked, this, [this]{ handleButton(23); });
        connect(MyButton[24], &QPushButton::clicked, this, [this]{ handleButton(24); });

        connect(MyButton[25], &QPushButton::clicked, this, [this]{ handleButton(25); });
        connect(MyButton[26], &QPushButton::clicked, this, [this]{ handleButton(26); });
        connect(MyButton[27], &QPushButton::clicked, this, [this]{ handleButton(27); });
        connect(MyButton[28], &QPushButton::clicked, this, [this]{ handleButton(28); });
        connect(MyButton[29], &QPushButton::clicked, this, [this]{ handleButton(29); });

        connect(MyButton[30], &QPushButton::clicked, this, [this]{ handleButton(30); });
        connect(MyButton[31], &QPushButton::clicked, this, [this]{ handleButton(31); });
        connect(MyButton[32], &QPushButton::clicked, this, [this]{ handleButton(32); });
        connect(MyButton[33], &QPushButton::clicked, this, [this]{ handleButton(33); });
        connect(MyButton[34], &QPushButton::clicked, this, [this]{ handleButton(34); });

        connect(MyButton[35], &QPushButton::clicked, this, [this]{ handleButton(35); });
        connect(MyButton[36], &QPushButton::clicked, this, [this]{ handleButton(36); });
        connect(MyButton[37], &QPushButton::clicked, this, [this]{ handleButton(37); });
        connect(MyButton[38], &QPushButton::clicked, this, [this]{ handleButton(38); });
        connect(MyButton[39], &QPushButton::clicked, this, [this]{ handleButton(39); });

        connect(MyButton[40], &QPushButton::clicked, this, [this]{ handleButton(40); });
        connect(MyButton[41], &QPushButton::clicked, this, [this]{ handleButton(41); });
        connect(MyButton[42], &QPushButton::clicked, this, [this]{ handleButton(42); });
        connect(MyButton[43], &QPushButton::clicked, this, [this]{ handleButton(43); });
        connect(MyButton[44], &QPushButton::clicked, this, [this]{ handleButton(44); });

        connect(MyButton[45], &QPushButton::clicked, this, [this]{ handleButton(45); });
        connect(MyButton[46], &QPushButton::clicked, this, [this]{ handleButton(46); });
        connect(MyButton[47], &QPushButton::clicked, this, [this]{ handleButton(47); });
        connect(MyButton[48], &QPushButton::clicked, this, [this]{ handleButton(48); });
        connect(MyButton[49], &QPushButton::clicked, this, [this]{ handleButton(49); });


        connect(MyButton[50], &QPushButton::clicked, this, [this]{ handleButton(50); });
        connect(MyButton[51], &QPushButton::clicked, this, [this]{ handleButton(51); });
        connect(MyButton[52], &QPushButton::clicked, this, [this]{ handleButton(52); });
        connect(MyButton[53], &QPushButton::clicked, this, [this]{ handleButton(53); });
        connect(MyButton[54], &QPushButton::clicked, this, [this]{ handleButton(54); });

        connect(MyButton[55], &QPushButton::clicked, this, [this]{ handleButton(55); });
        connect(MyButton[56], &QPushButton::clicked, this, [this]{ handleButton(56); });
        connect(MyButton[57], &QPushButton::clicked, this, [this]{ handleButton(57); });
        connect(MyButton[58], &QPushButton::clicked, this, [this]{ handleButton(58); });
        connect(MyButton[59], &QPushButton::clicked, this, [this]{ handleButton(59); });


        connect(MyButton[60], &QPushButton::clicked, this, [this]{ handleButton(60); });
        connect(MyButton[61], &QPushButton::clicked, this, [this]{ handleButton(61); });
        connect(MyButton[62], &QPushButton::clicked, this, [this]{ handleButton(62); });
        connect(MyButton[63], &QPushButton::clicked, this, [this]{ handleButton(63); });
        connect(MyButton[64], &QPushButton::clicked, this, [this]{ handleButton(64); });

        connect(MyButton[65], &QPushButton::clicked, this, [this]{ handleButton(65); });
        connect(MyButton[66], &QPushButton::clicked, this, [this]{ handleButton(66); });
        connect(MyButton[67], &QPushButton::clicked, this, [this]{ handleButton(67); });
        connect(MyButton[68], &QPushButton::clicked, this, [this]{ handleButton(68); });
        connect(MyButton[69], &QPushButton::clicked, this, [this]{ handleButton(69); });

        connect(MyButton[70], &QPushButton::clicked, this, [this]{ handleButton(70); });
        connect(MyButton[71], &QPushButton::clicked, this, [this]{ handleButton(71); });
        connect(MyButton[72], &QPushButton::clicked, this, [this]{ handleButton(72); });
        connect(MyButton[73], &QPushButton::clicked, this, [this]{ handleButton(73); });
        connect(MyButton[74], &QPushButton::clicked, this, [this]{ handleButton(74); });

        connect(MyButton[75], &QPushButton::clicked, this, [this]{ handleButton(75); });
        connect(MyButton[76], &QPushButton::clicked, this, [this]{ handleButton(76); });
        connect(MyButton[77], &QPushButton::clicked, this, [this]{ handleButton(77); });
        connect(MyButton[78], &QPushButton::clicked, this, [this]{ handleButton(78); });
        connect(MyButton[79], &QPushButton::clicked, this, [this]{ handleButton(79); });

        connect(MyButton[80], &QPushButton::clicked, this, [this]{ handleButton(80); });
}
void UltimateTTT::handleButton(int i)
{
    if(numofmoves == 0)
    {
        numofmoves = 1;
        bbn = i%9;
        if(matrix.boards[i/9].m[i%9/3][i%9%3] == ' ')
        {
            if(br == 1)
            {
                 MyButton[i]->setText("X");
                 MyButton[i]->setStyleSheet(text1);
                 matrix.boards[i/9].m[i%9/3][i%9%3] = 'x';
                 br = 2;
            }

            else
            {
                MyButton[i]->setText("O");
                MyButton[i]->setStyleSheet(text2);
                matrix.boards[i/9].m[i%9/3][i%9%3] = 'o';
                br = 1;
            }

        }


    }

    else if(bbn == i/9%9)
    {
        if(matrix.boards[i/9].flag == '0' && matrix.boards[i/9].m[i%9/3][i%9%3] == ' ')
        {
            bbn = i%9;
            if(br == 1)
            {
                 MyButton[i]->setText("X");
                 MyButton[i]->setStyleSheet(text1);
                 matrix.boards[i/9].m[i%9/3][i%9%3] = 'x';
                 br = 2;
            }

            else
            {
                MyButton[i]->setText("O");
                MyButton[i]->setStyleSheet(text2);
                matrix.boards[i/9].m[i%9/3][i%9%3] = 'o';
                br = 1;
            }

        }


       }
     CheckIfEndUltimate();

    while(matrix.boards[bbn].flag != '0')
    {
        if(bbn != 8)
        bbn = bbn + 1;
        else bbn = 0;

    }


    next->setText(QString::number(bbn + 1));

    //nacrtaj okvir oko bbn matrice


}
int UltimateTTT::CheckIfEndUltimate()
{
    QString p;

    for(int i = 0; i < 9; i++)
    {
        p = matrix.boards[i].GameEnd();
        if(p != '0')
            for(int j = 0; j < 9; j++)
            {
                if(p == 'x')
                {
                    MyButton[i * 9 + j]->setText(p);
                    MyButton[i * 9 + j]->setStyleSheet(text1);
                }

                else if(p == 'o')
                {
                    MyButton[i * 9 + j]->setText(p);
                    MyButton[i * 9 + j]->setStyleSheet(text2);
                }

                else
                {
                    MyButton[i * 9 + j]->setText(p);
                    MyButton[i * 9 + j]->setStyleSheet(text3);
                }
            }
    }

   char temp = matrix.GameEnd();

    if(temp != '0')
    {
        if(temp == 'x')
        {
            p1.win1v1 = p1.win1v1 + 1;
            QMessageBox msgBox;
            msgBox.setWindowTitle("Winner is " + p1.name + '(' + p1.sign + ')');
            //QString text = "Winner is " + p1.name + '(' + p1.sign + ')';
            msgBox.setIconPixmap(QPixmap("../TTT_QT_NEW/TTT_QT/X.jpg"));
            //msgBox.setText(text);
            //msgBox.setStyleSheet("QLabel{min-width: 700px;}");
            //msgBox.resize(100,100);
            msgBox.exec();
            for(int i = 0; i < 81; i++)
            {
                MyButton[i]->setText("X");
            }
        }

        else if(temp == 't')
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Tie game!");
            //msgBox.setText("Tie game!");
            draw = draw + 1;
            msgBox.setIconPixmap(QPixmap("../TTT_QT_NEW/TTT_QT/tie.jpg"));
            //msgBox.setStyleSheet("QLabel{min-width: 700px;}");
            msgBox.exec();
            for(int i = 0; i < 81; i++)
            {
                MyButton[i]->setText("t");
            }
        }

        else
        {
            p2.win1v1 = p2.win1v1 + 1;
            QMessageBox msgBox;
            msgBox.setWindowTitle("Winner is " + p2.name + '(' + p2.sign + ')');
            //QString text = "Winner is " + p2.name + '(' + p2.sign + ')';
            msgBox.setIconPixmap(QPixmap("../TTT_QT_NEW/TTT_QT/O.png"));
            //msgBox.setText(text);
            //msgBox.setStyleSheet("QLabel{max-width: 500px;}");
            msgBox.exec();
            for(int i = 0; i < 81; i++)
            {
                MyButton[i]->setText("O");
            }
        }
        ResetGameU();

    }
}
void UltimateTTT::ResetGameU()
{

    bigBoard m;
    matrix = m;

    bbn = 0;
    numofmoves = 0;

    for(int i = 0; i < 81; i++)
    {
        MyButton[i]->setText(QString::number(i % 9 + 1));
        MyButton[i]->setStyleSheet("background-color: light gray");
    }

        br = 1;
}

//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

//Babushka mode
BabushkaTTT::BabushkaTTT()
{
    remove = false;
    character_size = 0;
    setWindowTitle("Babushka mode!");
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    MyLabel = new QLabel(this);
    MyLabel->setText("Babushka Mode:\nTo read info click 'info' button");
    MyLabel->move(90,15);
    setStyleSheet("background-color: rgb(135, 185, 234)");

    QPushButton* oo = new QPushButton(this);

    oo->setText("Info");
    oo->setGeometry(600, 510, 100, 40);
    oo->setStyleSheet("background-color: light gray");
    connect(oo, &QPushButton::clicked, this, &BabushkaTTT::info);

    QPushButton* exit = new QPushButton(this);

    exit->setText("Exit");
    exit->setGeometry(600, 550, 100, 40);
    exit->setStyleSheet("background-color: light gray");
    connect(exit, &QPushButton::clicked, this, &BabushkaTTT::re);

    for(int i = 0; i < 3; i++)
    {
        OrangeRemaining[i] = 2;
        BlueRemaining[i] = 2;
    }


    int x = 200, y = 150;
    for(int i = 0; i < 9; i++)
    {
        MyButton[i] = new QPushButton(this);
        MyButton[i]->setText(QString::number(i + 1));
        MyButton[i]->setStyleSheet("background-color: light gray");

        MyButton[i]->setGeometry(x, y, 100, 100);

        if(x < 400)
        {
            x += 100;
        }

        else
        {
            x = 200;
            y += 100;
        }

        flag_array[i] = 0;
    }


    Big = new QPushButton(this);

    QPixmap pixmap("../TTT_QT_NEW/TTT_QT/BigOrange.png");
    QIcon ButtonIcon(pixmap);
    Big->setIcon(ButtonIcon);
    Big->setIconSize(QSize(905, 905));
    Big->setGeometry(60, 150, 90, 90);

    Medium = new QPushButton(this);
    QPixmap pixmap2("../TTT_QT_NEW/TTT_QT/MediumOrange.png");
    QIcon ButtonIcon2(pixmap2);
    Medium->setIcon(ButtonIcon2);
    Medium->setIconSize(QSize(865, 805));
    Medium->setGeometry(60, 254, 90, 90);

    Little = new QPushButton(this);
    QPixmap pixmap3("../TTT_QT_NEW/TTT_QT/MediumOrange.png");
    QIcon ButtonIcon3(pixmap3);
    Little->setIcon(ButtonIcon3);
    Little->setIconSize(QSize(365, 305));
    Little->setGeometry(60, 359, 90, 90);

    Remove_character = new QPushButton(this);
    Remove_character->setText("Remove character");
    Remove_character->setGeometry(58, 100, 120, 40);
    Remove_character->setStyleSheet("background-color: light gray");
    connect(Remove_character, &QPushButton::clicked, this, &BabushkaTTT::Remove_character_function);


    labelO1 = new QLabel(this);
    labelO1->setText(QString::number(OrangeRemaining[2]));
    labelO1->move(40,180);
    labelO1->setStyleSheet("QLabel { background-color : orange; color : white; }");

    labelO2 = new QLabel(this);
    labelO2->setText(QString::number(OrangeRemaining[1]));
    labelO2->move(40,284);
    labelO2->setStyleSheet("QLabel { background-color : orange; color : white; }");
    labelO3 = new QLabel(this);
    labelO3->setText(QString::number(OrangeRemaining[0]));
    labelO3->move(40,389);
    labelO3->setStyleSheet("QLabel { background-color : orange; color : white; }");

    labelB1 = new QLabel(this);
    labelB1->setText(QString::number(BlueRemaining[2]));
    labelB1->move(170,180);
    labelB1->setStyleSheet("QLabel { background-color : blue; color : white; }");
    labelB2 = new QLabel(this);
    labelB2->setText(QString::number(BlueRemaining[1]));
    labelB2->move(170,284);
    labelB2->setStyleSheet("QLabel { background-color : blue; color : white; }");
    labelB3 = new QLabel(this);
    labelB3->setText(QString::number(BlueRemaining[0]));
    labelB3->move(170,389);
    labelB3->setStyleSheet("QLabel { background-color : blue; color : white; }");

    connect(Big, &QPushButton::clicked, this, [this]{UpdateCharacterSize(3);});
    connect(Medium, &QPushButton::clicked, this, [this]{UpdateCharacterSize(2);});
    connect(Little, &QPushButton::clicked, this, [this]{UpdateCharacterSize(1);});

    connect(MyButton[0], &QPushButton::clicked, this, [this]{ handleButton(0); });
    connect(MyButton[1], &QPushButton::clicked, this, [this]{ handleButton(1); });
    connect(MyButton[2], &QPushButton::clicked, this, [this]{ handleButton(2); });
    connect(MyButton[3], &QPushButton::clicked, this, [this]{ handleButton(3); });
    connect(MyButton[4], &QPushButton::clicked, this, [this]{ handleButton(4); });
    connect(MyButton[5], &QPushButton::clicked, this, [this]{ handleButton(5); });

    connect(MyButton[6], &QPushButton::clicked, this, [this]{ handleButton(6); });
    connect(MyButton[7], &QPushButton::clicked, this, [this]{ handleButton(7); });
    connect(MyButton[8], &QPushButton::clicked, this, [this]{ handleButton(8); });

}

void BabushkaTTT::UpdateCharacterSize(int m)
{
    if(cant_pick_size == false)
        character_size = m;

    remove = false;
}

void BabushkaTTT::handleButton(int i)
{
    int a = 0, z = 0;
    a = i / 3;
    z = i % 3;

    if(remove == false)
    {

        if(flag_array[i] * flag_array[i] < character_size * character_size)
        {
            cant_pick_size = false;
            if(br == 1)
            {
                 //MyButton[i]->setText("X");

                 //MyButton[i]->setStyleSheet(text1);

                if(character_size == 3  and OrangeRemaining[2] > 0)
                {
                    //UpdateRemaining(flag_array[i]);

                    QPixmap pixmap("../TTT_QT_NEW/TTT_QT/BigOrange.png");
                    QIcon ButtonIcon(pixmap);
                    MyButton[i]->setIcon(ButtonIcon);
                    MyButton[i]->setIconSize(QSize(1005, 1005));
                    OrangeRemaining[2]--;
                    labelO1->setText(QString::number(OrangeRemaining[2]));
                    b.MakeAMove(a,z, 'x');
                    br = 2;
                    flag_array[i] = character_size;
                }

                else if(character_size == 2 and OrangeRemaining[1] > 0)
                {
                    //UpdateRemaining(flag_array[i]);

                    QPixmap pixmap("../TTT_QT_NEW/TTT_QT/MediumOrange.png");
                    QIcon ButtonIcon(pixmap);
                    MyButton[i]->setIcon(ButtonIcon);
                    MyButton[i]->setIconSize(QSize(865, 805));
                    OrangeRemaining[1]--;
                    labelO2->setText(QString::number(OrangeRemaining[1]));
                    b.MakeAMove(a,z, 'x');
                    br = 2;
                    flag_array[i] = character_size;
                }

                else if(character_size == 1 and OrangeRemaining[0] > 0)
                {
                    //UpdateRemaining(flag_array[i]);

                    QPixmap pixmap("../TTT_QT_NEW/TTT_QT/MediumOrange.png");
                    QIcon ButtonIcon(pixmap);
                    MyButton[i]->setIcon(ButtonIcon);
                    MyButton[i]->setIconSize(QSize(365, 305));
                    OrangeRemaining[0]--;
                    labelO3->setText(QString::number(OrangeRemaining[0]));
                    b.MakeAMove(a,z, 'x');
                    br = 2;
                    flag_array[i] = character_size;
                }

                stack[i].push(character_size);
                cant_pick_size = false;
            }

            else
            {
                //MyButton[i]->setText("O");
                //MyButton[i]->setStyleSheet(text2);

                if(character_size == 3 and BlueRemaining[2] > 0)
                {
                    //UpdateRemaining(flag_array[i]);

                    QPixmap pixmap("../TTT_QT_NEW/TTT_QT/BigBlue.png");
                    QIcon ButtonIcon(pixmap);
                    MyButton[i]->setIcon(ButtonIcon);
                    MyButton[i]->setIconSize(QSize(1005, 1005));
                    BlueRemaining[2]--;
                    labelB1->setText(QString::number(BlueRemaining[2]));
                    b.MakeAMove(a,z, 'o');
                    br = 1;
                    flag_array[i] = -1 * character_size;
                }

                else if(character_size == 2 and BlueRemaining[1] > 0)
                {
                    //UpdateRemaining(flag_array[i]);

                    QPixmap pixmap("../TTT_QT_NEW/TTT_QT/BigBlue.png");
                    QIcon ButtonIcon(pixmap);
                    MyButton[i]->setIcon(ButtonIcon);
                    MyButton[i]->setIconSize(QSize(565, 505));
                    BlueRemaining[1]--;
                    labelB2->setText(QString::number(BlueRemaining[1]));
                    b.MakeAMove(a,z, 'o');
                    br = 1;
                    flag_array[i] = -1 * character_size;
                }

                else if(character_size == 1 and BlueRemaining[0] > 0)
                {
                    //UpdateRemaining(flag_array[i]);

                    QPixmap pixmap("../TTT_QT_NEW/TTT_QT/BigBlue.png");
                    QIcon ButtonIcon(pixmap);
                    MyButton[i]->setIcon(ButtonIcon);
                    MyButton[i]->setIconSize(QSize(245, 245));
                    BlueRemaining[0]--;
                    labelB3->setText(QString::number(BlueRemaining[0]));
                    b.MakeAMove(a,z, 'o');
                    br = 1;
                    flag_array[i] = -1 * character_size;
                }

                stack[i].push(-1 * character_size);
                cant_pick_size = false;
            }
        }
    }

    else
    {
        if(!stack[i].isEmpty())
        {
            if((br == 1 and stack[i].top() > 0) || (br == 2 and stack[i].top() < 0))
            {
                int popped = stack[i].pop();
                UpdateRemaining(popped);
                if(popped > 0)
                    character_size = popped;
                else
                    character_size = -1 * popped;
                cant_pick_size = true;
                if(!stack[i].isEmpty())
                {
                    int peek = stack[i].top();
                    SetButton(i, peek);
                }

                else
                {
                    MyButton[i]->setIcon(QIcon());
                    MyButton[i]->setText(QString::number(i + 1));
                    MyButton[i]->setStyleSheet("background-color: light gray");
                    b.MakeAMove(a, z, '0');
                    flag_array[i] = 0;
                }


            }


        }
        remove = false;

    }

    if(cant_pick_size == false)
    {
        character_size = 0;
        CheckIfEndBabushka();
    }
}

void BabushkaTTT::UpdateRemaining(int br)
{   
    switch (br)
    {
     case -3:
       BlueRemaining[2]++;
       labelB1->setText(QString::number(BlueRemaining[2]));
       break;
    case -2:
      BlueRemaining[1]++;
      labelB2->setText(QString::number(BlueRemaining[1]));
      break;
    case -1:
      BlueRemaining[0]++;
      labelB3->setText(QString::number(BlueRemaining[0]));
      break;
    case 3:
       OrangeRemaining[2]++;
       labelO1->setText(QString::number(OrangeRemaining[2]));
       break;
    case 2:
       OrangeRemaining[1]++;
       labelO2->setText(QString::number(OrangeRemaining[1]));
       break;
    case 1:
       OrangeRemaining[0]++;
       labelO3->setText(QString::number(OrangeRemaining[0]));
       break;

    default:
        break;
    }
}

int BabushkaTTT::CheckIfEndBabushka()
{
    QMessageBox msg;

    msg.setWindowTitle("Game end");

    if(b.GameEnd() == 'x')
    {
       msg.setText("Winner is X");
       msg.exec();
       ResetGameB();
    }
    else if(b.GameEnd() == 'o')
    {
        msg.setText("Winner is O");
        msg.exec();
        ResetGameB();
    }

    return 0;
}

void BabushkaTTT::info()
{

    QMessageBox msg;

    msg.setWindowTitle("Babushka game mode info");

    msg.setText("To win you have to connect three. Each player has six figures, two of each size, bigger figures can eat smaller enemy or friendly figures. You are allowed to change positions of characters that have already been placed.");
    msg.exec();
}

void BabushkaTTT::re()
{
    ResetGameB();
    reject();
}

void BabushkaTTT::ResetGameB()
{
    board m;
    b = m;

    br = 1;

    for(int i = 0; i < 3; i++)
    {
        BlueRemaining[i] = 2;
        OrangeRemaining[i] = 2;
    }

    for(int i = 0; i < 9; i++)
    {
        flag_array[i] = 0;

        MyButton[i]->setIcon(QIcon());
        MyButton[i]->setText(QString::number(i + 1));
        MyButton[i]->setStyleSheet("background-color: light gray");

        //stack[i].empty();
        while(!stack[i].isEmpty())
            stack[i].pop();
    }

    labelO1->setText(QString::number(OrangeRemaining[2]));

    labelO2->setText(QString::number(OrangeRemaining[1]));

    labelO3->setText(QString::number(OrangeRemaining[0]));


    labelB1->setText(QString::number(BlueRemaining[2]));

    labelB2->setText(QString::number(BlueRemaining[1]));

    labelB3->setText(QString::number(BlueRemaining[0]));
}

void BabushkaTTT::Remove_character_function()
{
    remove = true;
}

void BabushkaTTT::SetButton(int i, int peek)
{
    int a = 0, z = 0;
    a = i / 3;
    z = i % 3;

    if(peek > 0)
    {
        b.MakeAMove(a,z, 'x');
        flag_array[i] = character_size;
    }
    else
    {
        b.MakeAMove(a,z, 'o');
        flag_array[i] = -1 * character_size;
    }

    switch(peek)
    {
        case 3:
           { QPixmap pixmap("../TTT_QT_NEW/TTT_QT/BigOrange.png");
            QIcon ButtonIcon(pixmap);
            MyButton[i]->setIcon(ButtonIcon);
            MyButton[i]->setIconSize(QSize(1005, 1005));
            break;
            }

        case 2:
            {
            QPixmap pixmap("../TTT_QT_NEW/TTT_QT/MediumOrange.png");
            QIcon ButtonIcon(pixmap);
            MyButton[i]->setIcon(ButtonIcon);
            MyButton[i]->setIconSize(QSize(865, 805));
            break;
             }
        case 1:
            {
            QPixmap pixmap("../TTT_QT_NEW/TTT_QT/MediumOrange.png");
            QIcon ButtonIcon(pixmap);
            MyButton[i]->setIcon(ButtonIcon);
            MyButton[i]->setIconSize(QSize(365, 305));
            break;
            }
        case -3:
            {
            QPixmap pixmap("../TTT_QT_NEW/TTT_QT/BigBlue.png");
            QIcon ButtonIcon(pixmap);
            MyButton[i]->setIcon(ButtonIcon);
            MyButton[i]->setIconSize(QSize(1005, 1005));
            break;
            }
        case -2:
            {
            QPixmap pixmap("../TTT_QT_NEW/TTT_QT/BigBlue.png");
            QIcon ButtonIcon(pixmap);
            MyButton[i]->setIcon(ButtonIcon);
            MyButton[i]->setIconSize(QSize(565, 505));
            break;
            }
        case -1:
            {
            QPixmap pixmap("../TTT_QT_NEW/TTT_QT/BigBlue.png");
            QIcon ButtonIcon(pixmap);
            MyButton[i]->setIcon(ButtonIcon);
            MyButton[i]->setIconSize(QSize(245, 245));
            break;
            }

        default:
    {
            break;
    }
    }
}

void MyMainWindow::StartBabushkaGame()
{
    //ResetGame2();
    BabushkaTTT WindowU;
    WindowU.resize(750,600);
    WindowU.show();
    WindowU.exec();

    WindowU.ResetGameB();
}
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------

//NON CLASS FUNCTIONS

//za poslani 'x' vraca 'o' i obratno
char otherSign(char sign)
{
    if (sign == 'x')return 'o';
    else return 'x';
}
//minimax algorithm
int minimax(board btemp, bool isMax, char sign)
{
    int score = 0;
    if (btemp.GameEnd() == sign)return 1;
    else if (btemp.GameEnd() == 't')return 0;
    else if (btemp.GameEnd() == otherSign(sign))return -1;

    if (isMax)
    {
        int bestScore = -5;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (btemp.m[i][j] == ' ')
                {
                    btemp.m[i][j] = sign;
                    score = minimax(btemp, false, sign);
                    btemp.m[i][j] = ' ';
                    if (score > bestScore)
                    {
                        bestScore = score;
                    }
                }
        return bestScore;
    }

    else
    {
        int bestScore = 5;
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (btemp.m[i][j] == ' ')
                {
                    btemp.m[i][j] = otherSign(sign);
                    score = minimax(btemp, true, sign);
                    btemp.m[i][j] = ' ';
                    if (score < bestScore)
                    {
                        bestScore = score;
                    }
                }
        return bestScore;
    }
}

int minimax5x5(board5x5 b, bool isMax, char sign, int alpha, int beta, int depth, int numberOfMoves)
{
    depth++;
    if (numberOfMoves < 4 && depth > 8)return 0;
    if (numberOfMoves < 8 && depth > 10)return 0;
    else if (depth > 30)return 0;

    int score = 0;
    if (b.GameEnd() == sign)return 1;
    else if (b.GameEnd() == 't')return 0;
    else if (b.GameEnd() == otherSign(sign))return -1;

    if (isMax)
    {
        int bestScore = -5;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (b.m[i][j] == ' ')
                {
                    b.m[i][j] = sign;
                    score = minimax5x5(b, false, sign, alpha, beta, depth, numberOfMoves);
                    b.m[i][j] = ' ';
                    if (alpha < score)alpha = score;
                    if (beta <= alpha)
                    {
                        break;
                    }
                    if (score > bestScore)
                    {
                        bestScore = score;
                    }
                }

        return bestScore;
    }

    else
    {
        int bestScore = 5;
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (b.m[i][j] == ' ')
                {
                    b.m[i][j] = otherSign(sign);
                    score = minimax5x5(b, true, sign, alpha, beta, depth, numberOfMoves);
                    b.m[i][j] = ' ';
                    if (score < beta)beta = score;
                    if (beta <= alpha)
                    {
                        break;
                    }
                    if (score < bestScore)
                    {
                        bestScore = score;
                    }
                }



        return bestScore;
    }
}

//Provjera na 5x5 gameu za bota, u slucajevima kad je minimax algorithman prespor
int WinInOne(char sign, board5x5 b, int& n, int& m)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            if (b.m[i][j] == ' ')
            {
                b.m[i][j] = sign;
                if (b.GameEnd() == sign)
                {
                    b.m[i][j] = ' ';
                    n = i;
                    m = j;
                    return 1;
                }

                b.m[i][j] = ' ';
            }
        }

    return 0;
}
int DefendInOne(char sign, board5x5 b, int& n, int& m)
{
    char stemp = ' ';
    stemp = otherSign(sign);

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            if (b.m[i][j] == ' ')
            {
                b.m[i][j] = stemp;
                if (b.GameEnd() == stemp)
                {
                    b.m[i][j] = ' ';
                    n = i;
                    m = j;
                    return 1;
                };

                b.m[i][j] = ' ';
            }
        }

    return 0;
}

int WinInTwo(char sign, board5x5 b, int& n, int& m)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            if (b.m[i][j] == ' ')
            {
                b.m[i][j] = sign;

                for (int a = 0; a < 5; a++)
                    for (int z = 0; z < 5; z++)
                    {
                        if (b.m[a][z] == ' ')
                        {
                            b.m[a][z] = sign;
                            if (b.GameEnd() == sign)
                            {
                                b.m[i][j] = ' ';
                                b.m[a][z] = ' ';
                                n = i;
                                m = j;
                                return 1;
                            }

                            b.m[a][z] = ' ';
                        }
                    }



                b.m[i][j] = ' ';
            }
        }

    return 0;
}
int DefendInTwo(char sign, board5x5 b, int& n, int& m)
{
    char stemp = ' ';
    stemp = otherSign(sign);

    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
        {
            if (b.m[i][j] == ' ')
            {
                b.m[i][j] = stemp;
                for (int a = 0; a < 5; a++)
                    for (int z = 0; z < 5; z++)
                    {
                        if (b.m[a][z] == ' ')
                        {
                            b.m[a][z] = stemp;
                            if (b.GameEnd() == stemp)
                            {
                                b.m[i][j] = ' ';
                                b.m[a][z] = ' ';
                                n = i;
                                m = j;
                                return 1;
                            };
                            b.m[a][z] = ' ';
                        }

                     }
                b.m[i][j] = ' ';
            }
        }

    return 0;
}

//info for ultimate TTT
void mfun()
{
    QMessageBox msg;

    msg.setText("To win you have to connect three small matrixes. Position of a move in a small matrix determines in which small matrix the nest move can be played.");
    msg.exec();
}

//zapocinje igru Ultimate TTT
void MyMainWindow::StartUltimateGame()
{
    ResetGame2();
    UltimateTTT WindowU;
    WindowU.resize(750,600);
    WindowU.show();
    WindowU.exec();

    WindowU.ResetGameU();
}


//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------

//glavni prozor
MyMainWindow::MyMainWindow()
{
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    menuBar()->setStyleSheet("background-color: light gray");

    //Edit menu
    EditMenu = menuBar()->addMenu(tr("&Edit"));

    Reset = new QAction(tr("&NewGame"), this);
    Reset->setShortcut(tr("CTRL+N"));
    connect(Reset, &QAction::triggered, this,&MyMainWindow::ResetGame2);
    EditMenu->addAction(Reset);

    Names = new QAction(tr("&EnterNames"), this);
    Names->setShortcut(tr("CTRL+E"));
    connect(Names, &QAction::triggered, this,&Dialog);
    EditMenu->addAction(Names);

    ChangeButtonColors = new QAction(tr("&ChangeButtonColors"), this);
    ChangeButtonColors->setShortcut(tr("CTRL+C"));
    connect(ChangeButtonColors, &QAction::triggered, this, &MyMainWindow::Change);
    EditMenu->addAction(ChangeButtonColors);


    //Modes menu
    EditMenu = menuBar()->addMenu(tr("&Modes"));
    EditMenu->setStyleSheet("background-color: light gray");

    five = new QAction(tr("&5x5 board"), this);
    five->setShortcut(tr("CTRL+5"));
    connect(five, &QAction::triggered, this, &MyMainWindow::Start5x5Game);
    EditMenu->addAction(five);


    //extra modes
    Ultimate = new QAction(tr("Ultimate"), this);
    Ultimate->setShortcut(tr("CTRL+U"));
    connect(Ultimate, &QAction::triggered, this, &MyMainWindow::StartUltimateGame);
    EditMenu->addAction(Ultimate);

    Babushka = new QAction(tr("Babushka"), this);
    Babushka->setShortcut(tr("CTRL+G"));
    connect(Babushka, &QAction::triggered, this, &MyMainWindow::StartBabushkaGame);
    EditMenu->addAction(Babushka);

    //Bot mode

    EditMenu = menuBar()->addMenu(tr("&vsBot"));

    vsBot = new QAction(tr("&Turn on"), this);
    vsBot->setShortcut(tr("CTRL+B"));
    connect(vsBot, &QAction::triggered, this,&MyMainWindow::VsBot);
    EditMenu->addAction(vsBot);



    three = new QAction(tr("&Turn off"), this);
    three->setShortcut(tr("CTRL+3"));
    connect(three, &QAction::triggered, this, &MyMainWindow::UpdateAgainstBot);
    EditMenu->addAction(three);


    //Options menu
    EditMenu = menuBar()->addMenu(tr("&Options"));

    Result = new QAction(tr("&GameResult"), this);
    Result->setShortcut(tr("CTRL+R"));
    connect(Result, &QAction::triggered, this,&MyMainWindow::PrintScoreBoard);
    EditMenu->addAction(Result);

    ReadFile = new QAction(tr("&ReadPreviousResults"), this);
    ReadFile->setShortcut(tr("CTRL+F"));
    connect(ReadFile, &QAction::triggered, this, &ReadFromFile);
    EditMenu->addAction(ReadFile);


    MyLabel = new QLabel(this);
    MyLabel->setText("Tic-Tac_Toe");
    MyLabel->move(74, 23);

    MyButton1 = new QPushButton(this);
    MyButton1->setStyleSheet("background-color: light gray");
    MyButton1->setText("1");
    MyButton1->setGeometry(30,50,50,50);
    connect(MyButton1, &QPushButton::clicked, this, &MyMainWindow::handleButton1);

    MyButton2 = new QPushButton(this);
    MyButton2->setStyleSheet("background-color: light gray");
    MyButton2->setText("2");
    MyButton2->setGeometry(80,50,50,50);
    connect(MyButton2, &QPushButton::clicked, this, &MyMainWindow::handleButton2);

    MyButton3 = new QPushButton(this);
    MyButton3->setStyleSheet("background-color: light gray");
    MyButton3->setText("3");
    MyButton3->setGeometry(130,50,50,50);
    connect(MyButton3, &QPushButton::clicked, this, &MyMainWindow::handleButton3);


    MyButton4 = new QPushButton(this);
    MyButton4->setStyleSheet("background-color: light gray");
    MyButton4->setText("4");
    MyButton4->setGeometry(30,100,50,50);
    connect(MyButton4, &QPushButton::clicked, this, &MyMainWindow::handleButton4);

    MyButton5 = new QPushButton(this);
    MyButton5->setStyleSheet("background-color: light gray");
    MyButton5->setText("5");
    MyButton5->setGeometry(80,100,50,50);
    connect(MyButton5, &QPushButton::clicked, this, &MyMainWindow::handleButton5);

    MyButton6 = new QPushButton(this);
    MyButton6->setStyleSheet("background-color: light gray");
    MyButton6->setText("6");
    MyButton6->setGeometry(130,100,50,50);
    connect(MyButton6, &QPushButton::clicked, this, &MyMainWindow::handleButton6);


    MyButton7 = new QPushButton(this);
    MyButton7->setStyleSheet("background-color: light gray");
    MyButton7->setText("7");
    MyButton7->setGeometry(30,150,50,50);
    connect(MyButton7, &QPushButton::clicked, this, &MyMainWindow::handleButton7);

    MyButton8 = new QPushButton(this);
    MyButton8->setStyleSheet("background-color: light gray");
    MyButton8->setText("8");
    MyButton8->setGeometry(80,150,50,50);
    connect(MyButton8, &QPushButton::clicked, this, &MyMainWindow::handleButton8);

    MyButton9 = new QPushButton(this);
    MyButton9->setStyleSheet("background-color: light gray");
    MyButton9->setText("9");
    MyButton9->setGeometry(130,150,50,50);
    connect(MyButton9, &QPushButton::clicked, this, &MyMainWindow::handleButton9);

    setStyleSheet("background-color: rgb(255, 223, 128);");
    setWindowTitle("Tic-Tac-Toe");
}


//funkcije za updeatanje buttona kod 3x3
void MyMainWindow::handleButton1()
{
    if(flag[0] == 0)
    {

        if(br == 1)
        {
             MyButton1->setText("X");

             MyButton1->setStyleSheet(text1);
             b.MakeAMove(0,0, 'x');
             br = 2;
        }

        else
        {
            MyButton1->setText("O");
            MyButton1->setStyleSheet(text2);
            b.MakeAMove(0,0, 'o');
            br = 1;
        }

        flag[0] = 1;
    }

    CheckIfEnd();


    if(AgainstBot == 1 && br == 2)
    {
        BotMove();
    }


}
void MyMainWindow::handleButton2()
{
    if(flag[1] == 0)
    {
        if(br == 1)
        {
             MyButton2->setText("X");
             MyButton2->setStyleSheet(text1);
             b.MakeAMove(0,1, 'x');
             br = 2;
        }

        else
        {
            MyButton2->setText("O");
            MyButton2->setStyleSheet(text2);
            b.MakeAMove(0,1, 'o');
            br = 1;
        }

        flag[1] = 1;
    }

    CheckIfEnd();
    if(AgainstBot == 1 && br == 2)
    {
        BotMove();
    }
}
void MyMainWindow::handleButton3()
{
    if(flag[2] == 0)
    {
        if(br == 1)
        {
             MyButton3->setText("X");
             MyButton3->setStyleSheet(text1);
             b.MakeAMove(0,2, 'x');
             br = 2;
        }

        else
        {
            MyButton3->setText("O");
            MyButton3->setStyleSheet(text2);
            b.MakeAMove(0,2, 'o');
            br = 1;
        }

        flag[2] = 1;
    }

    CheckIfEnd();
    if(AgainstBot == 1 && br == 2)
    {
        BotMove();
    }
}
void MyMainWindow::handleButton4()
{
    if(flag[3] == 0)
    {
        if(br == 1)
        {
             MyButton4->setText("X");
             MyButton4->setStyleSheet(text1);
             b.MakeAMove(1,0, 'x');
             br = 2;
        }

        else
        {
            MyButton4->setText("O");
            MyButton4->setStyleSheet(text2);
            b.MakeAMove(1,0, 'o');
            br = 1;
        }

        flag[3] = 1;
    }

    CheckIfEnd();
    if(AgainstBot == 1 && br == 2)
    {
        BotMove();
    }
}
void MyMainWindow::handleButton5()
{
    if(flag[4] == 0)
    {
        if(br == 1)
        {
             MyButton5->setText("X");
             MyButton5->setStyleSheet(text1);
             b.MakeAMove(1,1, 'x');
             br = 2;
        }

        else
        {
            MyButton5->setText("O");
            MyButton5->setStyleSheet(text2);
            b.MakeAMove(1,1, 'o');
            br = 1;
        }

        flag[4] = 1;
    }

    CheckIfEnd();
    if(AgainstBot == 1 && br == 2)
    {
        BotMove();
    }
}
void MyMainWindow::handleButton6()
{
    if(flag[5] == 0)
    {
        if(br == 1)
        {
             MyButton6->setText("X");
             MyButton6->setStyleSheet(text1);
             b.MakeAMove(1,2, 'x');
             br = 2;
        }

        else
        {
            MyButton6->setText("O");
            MyButton6->setStyleSheet(text2);
            b.MakeAMove(1,2, 'o');
            br = 1;
        }

        flag[5] = 1;
    }

    CheckIfEnd();
    if(AgainstBot == 1 && br == 2)
    {
        BotMove();
    }
}
void MyMainWindow::handleButton7()
{
    if(flag[6] == 0)
    {
        if(br == 1)
        {
             MyButton7->setText("X");
             MyButton7->setStyleSheet(text1);
             b.MakeAMove(2,0, 'x');
             br = 2;
        }

        else
        {
            MyButton7->setText("O");
            MyButton7->setStyleSheet(text2);
            b.MakeAMove(2,0, 'o');
            br = 1;
        }

        flag[6] = 1;
    }

    CheckIfEnd();
    if(AgainstBot == 1 && br == 2)
    {
        BotMove();
    }
}
void MyMainWindow::handleButton8()
{
    if(flag[7] == 0)
    {
        if(br == 1)
        {
             MyButton8->setText("X");
             MyButton8->setStyleSheet(text1);
             b.MakeAMove(2,1, 'x');
             br = 2;
        }

        else
        {
            MyButton8->setText("O");
            MyButton8->setStyleSheet(text2);
            b.MakeAMove(2,1, 'o');
            br = 1;
        }

        flag[7] = 1;
    }

    CheckIfEnd();
    if(AgainstBot == 1 && br == 2)
    {
        BotMove();
    }
}
void MyMainWindow::handleButton9()
{
    if(flag[8] == 0)
    {
        if(br == 1)
        {
             MyButton9->setText("X");
             MyButton9->setStyleSheet(text1);
             b.MakeAMove(2,2, 'x');
             br = 2;
        }

        else
        {
            MyButton9->setText("O");
            MyButton9->setStyleSheet(text2);
            b.MakeAMove(2,2, 'o');
            br = 1;
        }

        flag[8] = 1;
    }

    CheckIfEnd();
    if(AgainstBot == 1 && br == 2)
    {
        BotMove();
    }
}

//provjerava je li igra gotova kod 3x3
int MyMainWindow::CheckIfEnd()
{
    if(b.GameEnd() != '0')
    {
        if(b.GameEnd() == 't')
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Tie game!");
            //msgBox.setText(qApp->applicationDirPath());
            draw = draw + 1;

            msgBox.setIconPixmap(QPixmap("../TTT_QT_NEW/TTT_QT/tie.jpg"));
            msgBox.exec();
        }

        else if(b.GameEnd() == 'x')
        {
            p1.win1v1 = p1.win1v1 + 1;
            QMessageBox msgBox;
            msgBox.setWindowTitle("Winner is " + p1.name + '(' + p1.sign + ')');
            //QString text = "Winner is " + p1.name + '(' + p1.sign + ')';
            msgBox.setIconPixmap(QPixmap("../TTT_QT_NEW/TTT_QT/X.jpg"));
            msgBox.exec();
        }

        else
        {
            p2.win1v1 = p2.win1v1 + 1;
            QMessageBox msgBox;
            msgBox.setWindowTitle("Winner is " + p2.name + '(' + p2.sign + ')');
            //QString text = "Winner is " + p2.name + '(' + p2.sign + ')';
            msgBox.setIconPixmap(QPixmap("../TTT_QT_NEW/TTT_QT/O.png"));
            //msgBox.setText(text);
            //msgBox.setStyleSheet("QLabel{max-width: 500px;}");
            msgBox.exec();
        }

        ResetGame();

        return 0;
    }

    return 1;
}

//resetira matricu i brojace pobjeda/nerijesenih
void MyMainWindow::ResetGame2()
{
    ResetGame();
    SaveInFile();
    draw = 0;
    p1.win1v1 = 0;
    p2.win1v1 = 0;

}

//resetira matricu
void MyMainWindow::ResetGame()
{
    board t;
    b = t;

    MyButton1->setText("1");
    MyButton1->setStyleSheet("background-color: light gray");
    flag[0] = 0;
    MyButton2->setText("2");
    MyButton2->setStyleSheet("background-color: light gray");
    flag[1] = 0;
    MyButton3->setText("3");
    MyButton3->setStyleSheet("background-color: light gray");
    flag[2] = 0;
    MyButton4->setText("4");
    MyButton4->setStyleSheet("background-color: light gray");
    flag[3] = 0;
    MyButton5->setText("5");
    MyButton5->setStyleSheet("background-color: light gray");
    flag[4] = 0;
    MyButton6->setText("6");
    MyButton6->setStyleSheet("background-color: light gray");
    flag[5] = 0;
    MyButton7->setText("7");
    MyButton7->setStyleSheet("background-color: light gray");
    flag[6] = 0;
    MyButton8->setText("8");
    MyButton8->setStyleSheet("background-color: light gray");
    flag[7] = 0;
    MyButton9->setText("9");
    MyButton9->setStyleSheet("background-color: light gray");
    flag[8] = 0;

    br = 1;



}

//Putem QMessageBoxa ispisuje rezultat
void MyMainWindow::PrintScoreBoard()
{
    QMessageBox msgBox;
    QString name1 = p1.name;
    QString name2 = p2.name;
    QString text = "";
    QString str1 = QString::number(p1.win1v1);
    QString str2 = QString::number(draw);
    QString str3 = QString::number(p2.win1v1);
    text = name1 + '(' + p1.sign + ')' + " - tie - " + name2 + '(' + p2.sign + ')' + "\n\t" + str1 + '\t' + str2 + '\t' + str3;
    msgBox.setWindowTitle("Game result");
    msgBox.setText(text);
    msgBox.exec();
}

//Konstruktor za dijalog za namjestanje imena igraca
MyDialog::MyDialog()
{
    lineEdit = new QLineEdit;
    lineEdit2 = new QLineEdit;
    editLayout = new QFormLayout;
    editLayout->addRow(new QLabel(tr("Name of the first player:")), lineEdit);
    editLayout->addRow(new QLabel(tr("Name of the second player:")), lineEdit2);
    editWidget = new QWidget;
    editWidget->setLayout(editLayout);
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
    QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(editWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    setWindowTitle("Inputing names of players");
}

//Prihvaca rezultat iz MyDialoga
void Dialog()
{
    MyDialog m;
    int ret = m.exec();
    if( ret == QDialog::Accepted)
    {
        p1.name = m.lineEdit->text();
        p2.name = m.lineEdit2->text();
    }
}

//Konstruktor za pocetni izbornik
MyMainMenu::MyMainMenu()
{
    editLayout = new QFormLayout;

    Label = new QLabel(this);
    Label->setText("Do you want to play Tic-Tac-Toe?");
    Label->move(205, 20);
    Label->resize(200, 205);

    Label2 = new QLabel("<img src='../TTT_QT_NEW/TTT_QT/TTT2.png' />");
    Label2->move(20,20);

    editLayout->addWidget(Label);
    editLayout->addWidget(Label2);

    editWidget = new QWidget;
    editWidget->setLayout(editLayout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Yes |
    QDialogButtonBox::No);

    buttonBox->setStyleSheet("background-color: light gray");

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(editWidget);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
    setStyleSheet("background-color: rgb(219, 224, 210)");
    setWindowTitle("Entering the game");
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
}

//namjesta igru protiv bota
void MyMainWindow::VsBot()
{
    AgainstBot = 1;
    ResetGame2();
    p2.name = "Bot";
}

//ispisuje u file
void SaveInFile()
{
    QString filename = "C:/Users/vjera/OneDrive/Dokumenti/Results.txt";
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite |  QIODevice::Append)) {
            QTextStream stream(&file);
            stream << p1.win1v1 << " " << draw << " " << p2.win1v1 << "\n";
        }

        file.close();
}

//cita iz file-a
void ReadFromFile()
{
    int a = 0, b = 0, c = 0;
    int suma = 0, sumb = 0, sumc = 0;
    QString filename = "C:/Users/vjera/OneDrive/Dokumenti/Results.txt";
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream stream(&file);
            while(!stream.atEnd())
            {
                QString line = stream.readLine();

                QTextStream myteststream(&line);
                myteststream >> a >> b >> c;
                suma += a;
                sumb += b;
                sumc += c;
            }

       }
    file.close();

    int sum = suma + sumb + sumc;
    QString perc_a = QString::number( ((double)suma/(double)sum) * 100, 'f', 1);
    QString perc_b = QString::number( ((double)sumb/(double)sum) * 100, 'f', 1);
    QString perc_c = QString::number( ((double)sumc/(double)sum) * 100, 'f', 1);


    QMessageBox* msg = new QMessageBox();
    msg->setWindowTitle("ALL TIME STATISTICS");
    msg->setText("Statistics of previous games(" + QString::number(sum) + "):\n'X' player - tie - 'O' player\n   " + perc_a + "%     " + perc_b + "%     " + perc_c + "%");
    msg->exec();


}

//Konstruktor dijaloga za mijenjanje boja buttona
ChangeButtonColor::ChangeButtonColor()
{

    editLayout = new QFormLayout();

    QRadioButton* RadioButton1 = new QRadioButton("Blue", this);
    QRadioButton* RadioButton2 = new QRadioButton("Green", this);
    QRadioButton* RadioButton3 = new QRadioButton("Yellow", this);
    QRadioButton* RadioButton4 = new QRadioButton("Red", this);
    QRadioButton* RadioButton5 = new QRadioButton("Orange", this);

    editLayout->addWidget(RadioButton1);
    editLayout->addWidget(RadioButton2);
    editLayout->addWidget(RadioButton3);
    editLayout->addWidget(RadioButton4);
    editLayout->addWidget(RadioButton5);

    editWidget = new QWidget;
    editWidget->setLayout(editLayout);

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok |
    QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    connect(RadioButton1, &QRadioButton::clicked, this, &Update1);
    connect(RadioButton2, &QRadioButton::clicked, this, &Update2);
    connect(RadioButton3, &QRadioButton::clicked, this, &Update3);
    connect(RadioButton4, &QRadioButton::clicked, this, &Update4);
    connect(RadioButton5, &QRadioButton::clicked, this, &Update5);


    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(editWidget);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    if(num == 1)
    setWindowTitle("Button color for 'X'");
    else setWindowTitle("Button color for 'O'");
}

//mijenja boje buttona
void MyMainWindow::Change()
{
    ChangeButtonColor m;
    num = 2;
    ChangeButtonColor m2;
    num = 1;
    int ret = m.exec();
    if(ret == QDialog::Accepted)
    {
        if(rb == 1)
                text1 = "background-color: rgb(0, 183, 234); color: rgb(255, 255, 255)";
        else if(rb == 2)
                text1 = "background-color: rgb(0, 140, 0); color: rgb(255, 255, 255)";
        else if(rb == 3)
                text1 = "background-color: rgb(250, 230, 0); color: rgb(255, 255, 255)";
        else if(rb == 4)
                text1 = "background-color: rgb(250, 0, 0); color: rgb(255, 255, 255)";
        else if(rb == 5)
                text1 = "background-color: rgb(255, 155, 0); color: rgb(255, 255, 255)";
    }

    ret = m2.exec();
    if(ret == QDialog::Accepted)
    {
        if(rb == 1)
                text2 = "background-color: rgb(0, 183, 234); color: rgb(255, 255, 255)";
        else if(rb == 2)
                text2 = "background-color: rgb(0, 140, 0); color: rgb(255, 255, 255)";
        else if(rb == 3)
                text2 = "background-color: rgb(250, 230, 0); color: rgb(255, 255, 255)";
        else if(rb == 4)
                text2 = "background-color: rgb(250, 0, 0); color: rgb(255, 255, 255)";
        else if(rb == 5)
                text2 = "background-color: rgb(255, 155, 0); color: rgb(255, 255, 255)";
    }

    ResetGame();
}

//botov potez
void MyMainWindow::BotMove()
{
    int i = 0, j = 0;
    AI.Move(&i, &j);
    if(i == 0)
    {
        if(j == 0)handleButton1();
        else if(j == 1)handleButton2();
        else if(j == 2)handleButton3();
    }

    else if(i == 1)
    {
        if(j == 0)handleButton4();
        else if(j == 1)handleButton5();
        else if(j == 2)handleButton6();
    }

    else if(i == 2)
    {
        if(j == 0)handleButton7();
        else if(j == 1)handleButton8();
        else if(j == 2)handleButton9();
    }

    //br = 1;
}

//funkcije koje poomazu kod mijenjanja boje buttona
void Update1()
{
    rb = 1;
}
void Update2()
{
    rb = 2;
}
void Update3()
{
    rb = 3;
}
void Update4()
{
    rb = 4;
}
void Update5()
{
    rb = 5;
}

//iskljucuje bota
void MyMainWindow::UpdateAgainstBot()
{
    AgainstBot = 0;
    ResetGame2();
}

//Prozor za igru 5x5
Game5x5::Game5x5()
{
    setWindowTitle("Tic-Tac-Toe");
    MyLabel = new QLabel(this);
    MyLabel->setText("Tic Tac Toe on 5x5 board:\nGoal is to connect four!");
    MyLabel->move(90,15);
    setStyleSheet("background-color: rgb(135, 173, 128)");
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    int x = 30, y = 50;
    for(int i = 0; i < 25; i++)
    {
        MyButton[i] = new QPushButton(this);
        MyButton[i]->setText(QString::number(i + 1));
        MyButton[i]->setStyleSheet("background-color: light gray");

        MyButton[i]->setGeometry(x, y, 50, 50);

        if(x < 230)
        {
            x += 50;
        }

        else
        {
            x = 30;
            y += 50;
        }

    }
    connect(MyButton[0], &QPushButton::clicked, this, [this]{ handleButton(0); });
    connect(MyButton[1], &QPushButton::clicked, this, [this]{ handleButton(1); });
    connect(MyButton[2], &QPushButton::clicked, this, [this]{ handleButton(2); });
    connect(MyButton[3], &QPushButton::clicked, this, [this]{ handleButton(3); });
    connect(MyButton[4], &QPushButton::clicked, this, [this]{ handleButton(4); });
    connect(MyButton[5], &QPushButton::clicked, this, [this]{ handleButton(5); });

    connect(MyButton[6], &QPushButton::clicked, this, [this]{ handleButton(6); });
    connect(MyButton[7], &QPushButton::clicked, this, [this]{ handleButton(7); });
    connect(MyButton[8], &QPushButton::clicked, this, [this]{ handleButton(8); });
    connect(MyButton[9], &QPushButton::clicked, this, [this]{ handleButton(9); });
    connect(MyButton[10], &QPushButton::clicked, this, [this]{ handleButton(10); });

    connect(MyButton[11], &QPushButton::clicked, this, [this]{ handleButton(11); });
    connect(MyButton[12], &QPushButton::clicked, this, [this]{ handleButton(12); });
    connect(MyButton[13], &QPushButton::clicked, this, [this]{ handleButton(13); });
    connect(MyButton[14], &QPushButton::clicked, this, [this]{ handleButton(14); });
    connect(MyButton[15], &QPushButton::clicked, this, [this]{ handleButton(15); });

    connect(MyButton[16], &QPushButton::clicked, this, [this]{ handleButton(16); });
    connect(MyButton[17], &QPushButton::clicked, this, [this]{ handleButton(17); });
    connect(MyButton[18], &QPushButton::clicked, this, [this]{ handleButton(18); });
    connect(MyButton[19], &QPushButton::clicked, this, [this]{ handleButton(19); });
    connect(MyButton[20], &QPushButton::clicked, this, [this]{ handleButton(20); });

    connect(MyButton[21], &QPushButton::clicked, this, [this]{ handleButton(21); });
    connect(MyButton[22], &QPushButton::clicked, this, [this]{ handleButton(22); });
    connect(MyButton[23], &QPushButton::clicked, this, [this]{ handleButton(23); });
    connect(MyButton[24], &QPushButton::clicked, this, [this]{ handleButton(24); });


    QPushButton* exit = new QPushButton(this);

    exit->setText("Exit");
    exit->setGeometry(110, 310, 100, 40);
    exit->setStyleSheet("background-color: light gray");
    connect(exit, &QPushButton::clicked, this, &Game5x5::UpdateBr);

}

//Resetira 5x5 game
void Game5x5::UpdateBr()
{
    br = 1;
    numberOfMoves = 0;
    ResetGame5();
    board5x5 temp;
    b5 = temp;
    reject();
}

//updatea buttone za 5x5
void Game5x5::handleButton(int i)
{
    int a = 0, b = 0;
    a = i / 5;
    b = i % 5;
    if(flag5x5[i] == 0)
    {

        if(br == 1)
        {
             MyButton[i]->setText("X");

             MyButton[i]->setStyleSheet(text1);
             b5.MakeAMove(a,b, 'x');
             br = 2;
        }

        else
        {
            MyButton[i]->setText("O");
            MyButton[i]->setStyleSheet(text2);
            b5.MakeAMove(a,b, 'o');
            br = 1;
        }

        flag5x5[i] = 1;
    }

    CheckIfEnd5();


    if(AgainstBot == 1 && br == 2)
    {

        BotMove5x5(numberOfMoves++);
        br = 1;
    }
}


//zapocinje igru na 5x5
void MyMainWindow::Start5x5Game()
{
    br = 1;
    ResetGame2();
    Game5x5 Window5;
    Window5.resize(315,380);
    Window5.show();
    Window5.exec();
}

//provjerava je li kraj igre na 5x5
int Game5x5::CheckIfEnd5()
{
    char check = b5.GameEnd();
    if(check != '0')
    {
        br = 1;
        numberOfMoves = 0;
        if(check == 't')
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("Tie game!");
            //msgBox.setText("Tie game!");
            draw = draw + 1;
            msgBox.setIconPixmap(QPixmap("../TTT_QT_NEW/TTT_QT/tie.jpg"));
            //msgBox.setStyleSheet("QLabel{min-width: 700px;}");
            msgBox.exec();
        }

        else if(check == 'x')
        {
            p1.win1v1 = p1.win1v1 + 1;
            QMessageBox msgBox;
            msgBox.setWindowTitle("Winner is " + p1.name + '(' + p1.sign + ')');
            //QString text = "Winner is " + p1.name + '(' + p1.sign + ')';
            msgBox.setIconPixmap(QPixmap("../TTT_QT_NEW/TTT_QT/X.jpg"));
            //msgBox.setText(text);
            //msgBox.setStyleSheet("QLabel{min-width: 700px;}");
            //msgBox.resize(100,100);
            msgBox.exec();
        }

        else
        {
            p2.win1v1 = p2.win1v1 + 1;
            QMessageBox msgBox;
            msgBox.setWindowTitle("Winner is " + p2.name + '(' + p2.sign + ')');
            //QString text = "Winner is " + p2.name + '(' + p2.sign + ')';
            msgBox.setIconPixmap(QPixmap("../TTT_QT_NEW/TTT_QT/O.png"));
            //msgBox.setText(text);
            //msgBox.setStyleSheet("QLabel{max-width: 500px;}");
            msgBox.exec();
        }

        ResetGame5();

        return 0;
    }

    return 1;
}

//resetira 5x5 matricu
void Game5x5::ResetGame5()
{
    board5x5 temp;
    b5 = temp;


    /*for(int i = 0; i < 5; i++)
        for(int j = 0; j < 5; j++)
            b5.m[i][j] = ' ';
       */

    for(int i = 0; i < 25; i++)
    {
        MyButton[i]->setText(QString::number(i + 1));
        MyButton[i]->setStyleSheet("background-color: light gray");
        flag5x5[i] = 0;
    }

    br = 1;
    numberOfMoves = 0;
}

//zove botov potez na 5x5
void Game5x5::BotMove5x5(int numOfMoves)
{
    int depth = 0;
    int i = 0, j = 0;
    int bestMove[2] = { 0 };
    board5x5 btemp = b5;
    int score = 0;
    int bestScore = -5;
    srand(time(NULL));

    if (numOfMoves <= 2)
    {
        while (true)
        {

            int a = rand() % 2 + 1;
            int r = rand() % 2 + 1;
            if (b.m[a][r] == ' ')
            {
                bestMove[0] = a;
                bestMove[1] = r;
                break;
            }

        }
     }

    else if (WinInOne(AI.sign, b5, i, j)) { bestMove[0] = i; bestMove[1] = j; }
    else if (DefendInOne(AI.sign, b5, i, j)) { bestMove[0] = i; bestMove[1] = j; }
    else if (WinInTwo(AI.sign, b5, i, j)) { bestMove[0] = i; bestMove[1] = j; }
    else if (DefendInTwo(AI.sign, b5, i, j)) { bestMove[0] = i; bestMove[1] = j; }


    else
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                if (btemp.m[i][j] == ' ')
                {
                    int alpha = -5, beta = 5;
                    btemp.m[i][j] = AI.sign;
                    score = minimax5x5(btemp, false, AI.sign, alpha, beta, depth, numOfMoves);
                    btemp.m[i][j] = ' ';
                    if (score > bestScore)
                    {
                        bestScore = score;
                        bestMove[0] = i;
                        bestMove[1] = j;
                    }
                }

    b5.m[bestMove[0]][bestMove[1]] = AI.sign;
    handleButton(bestMove[0] * 5 + bestMove[1]);

}
