#include "Functions.h"
using namespace std;


int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    MyMainMenu Menu;

    Menu.show();
    Menu.resize(300,300);
    int ret = Menu.exec();
    if(ret == QDialog::Accepted)
    {
        QApplication app2 (argc, argv);
        MyMainWindow mainWindow;
        mainWindow.resize(212,230);
        mainWindow.show();
        app2.exec();
    }

    return 0;
}


//Buttoni mijenjaju boju kad prodes preko njih / dizajn buttona
//Mijenjanje igraca koji ima pocetni potez //najizvedivije je kod bota
//7x7 / ultimate TTT
