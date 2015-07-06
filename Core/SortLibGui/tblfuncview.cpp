// this file is distributed under
// GPL v 3.0 license
#include "tblfuncview.h"
#include "ui_tblfuncview.h"

TblFuncView::TblFuncView(SoTblFunc *father, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::TblFuncView)
{
	ui->setupUi(this);
	master=father;
	refr();
	connect(master,SIGNAL(changed(SObject*)),this, SLOT(refr()));
}

TblFuncView::~TblFuncView()
{
	delete ui;
}

void TblFuncView::on_lineEdit_textChanged(const QString &arg1)
{
	master->Set_Name(arg1);
}

void TblFuncView::refr(){
	ui->lineEdit->setText(master->Name());
}

void TblFuncView::on_pushButton_clicked()
{
	master->Export();
}

void TblFuncView::AddTab(QString caption, QWidget *widg){
	ui->tabWidget->addTab(widg,caption);
}
