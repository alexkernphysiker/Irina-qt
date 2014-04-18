#include "randomview.h"
#include "ui_randomview.h"

RandomView::RandomView(RandomMagnitude *father, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::RandomView)
{
	master=father;
	ui->setupUi(this);
	refr();
	connect(master, SIGNAL(changed(SObject*)),this,SLOT(refr()));
}

RandomView::~RandomView()
{
	delete ui;
}
void RandomView::refr(){
	ui->lineEdit->setText(master->Name());
}
void RandomView::on_lineEdit_textChanged(const QString &arg1)
{
	master->Set_Name(arg1);
}
void RandomView::AddWidget(QWidget *widg, QString name){
	ui->tabWidget->addTab(widg,name);
}
