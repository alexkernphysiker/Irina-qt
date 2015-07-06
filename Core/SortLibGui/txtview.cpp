// this file is distributed under
// GPL v 3.0 license
#include "txtview.h"
#include "ui_txtview.h"
#include <QFileDialog>

txtview::txtview(SoTblFuncTxt *father, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::txtview)
{
	ui->setupUi(this);
	master=father;
	refr();
	connect(master,SIGNAL(changed(SObject*)),this,SLOT(refr()));
}

txtview::~txtview()
{
	delete ui;
}

void txtview::on_toolButton_clicked()
{
	QString str=QFileDialog::getOpenFileName(NULL,"Choose text file",QDir::currentPath(),"*.*",NULL,	QFileDialog::DontResolveSymlinks);
	str=QDir::current().relativeFilePath(str);
	if((str!=NULL)&(str!=""))
		master->SetPath(str);
}
void txtview::refr(){
	ui->lineEdit->setText(master->Path());
	ui->col0->setValue(master->Column(0));
	ui->col1->setValue(master->Column(1));
	ui->col2->setValue(master->Column(2));
}

void txtview::on_col0_valueChanged(int arg1)
{
	master->SetColumn(0,arg1);
}

void txtview::on_col1_valueChanged(int arg1)
{
	master->SetColumn(1,arg1);
}

void txtview::on_col2_valueChanged(int arg1)
{
	master->SetColumn(2,arg1);
}
