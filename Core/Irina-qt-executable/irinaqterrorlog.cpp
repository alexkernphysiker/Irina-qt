// this file is distributed under
// GPL v 3.0 license
#include "irinaqterrorlog.h"
#include "ui_irinaqterrorlog.h"

ErrorLog::ErrorLog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ErrorLog)
{
	ui->setupUi(this);
	setWindowTitle("Data sorting error output");
}

ErrorLog::~ErrorLog()
{
	delete ui;
}
ErrorLog *ErrorLog::GetInstance(){
	static ErrorLog *form;
	if(form==NULL)form=new ErrorLog();
	return form;
}
void ErrorLog::on_textEdit_destroyed()
{

}
void ErrorLog::error(QString msg){
	ui->plainTextEdit->setPlainText(ui->plainTextEdit->toPlainText()+"\n"+msg);
	if(!isVisible())
		show();
}
