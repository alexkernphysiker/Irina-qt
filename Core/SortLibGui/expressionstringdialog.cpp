// this file is distributed under
// GPL v 3.0 license
#include "expressionstringdialog.h"
#include "ui_expressionstringdialog.h"

ExpressionStringDialog::ExpressionStringDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ExpressionStringDialog)
{
	ui->setupUi(this);
}

ExpressionStringDialog::~ExpressionStringDialog()
{
	delete ui;
}

QString ExpressionStringDialog::ExprStr(){
	return ui->lineEdit->text();
}

void ExpressionStringDialog::SetExprStr(QString str){
	ui->lineEdit->setText(str);
}
