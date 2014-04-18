#include "masterwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MasterWindow w;
	w.show();
	
	return a.exec();
}
