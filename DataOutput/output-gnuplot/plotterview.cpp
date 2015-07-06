// this file is distributed under
// GPL v 3.0 license
#include "plotterview.h"
#include "gnuplotter.h"
#include "ui_plotterview.h"

PlotterView::PlotterView(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PlotterView)
{
	ui->setupUi(this);acc=false;
	params=new PlotParameters();
	ui->xlabel->setText(params->xlabel);
	ui->ylabel->setText(params->ylabel);
	ui->log_x->setChecked(params->logx);
	ui->log_y->setChecked(params->logy);
	ui->xrange->setText(params->xrange);
	ui->yrange->setText(params->yrange);
	ui->xtics->setText(params->xtics);
	ui->ytics->setText(params->ytics);
	ui->setkey->setText(params->setkey);
}

PlotterView::~PlotterView()
{
	delete ui;
	delete params;
}

void PlotterView::TakeData(QStringList &data){
	QString msg="";
	for(int i=1; i<data.count();i+=2){
		msg+=" - "+data.at(i)+"\n";
	}
	ui->plainTextEdit->setPlainText(msg);
}

void PlotterView::on_buttonBox_accepted()
{
	params->logx=ui->log_x->isChecked();
	params->logy=ui->log_y->isChecked();
	params->xlabel=ui->xlabel->text();
	params->ylabel=ui->ylabel->text();
	params->xrange=ui->xrange->text();
	params->yrange=ui->yrange->text();
	params->xtics=ui->xtics->text();
	params->ytics=ui->ytics->text();
	params->setkey=ui->setkey->text();
	info(true, params);
}

void PlotterView::on_buttonBox_rejected()
{
	info(false,params);
}

void PlotterView::closeEvent(QCloseEvent *){
	info(false,params);
}

void PlotterView::on_pushButton_clicked()
{
	settings();
}
