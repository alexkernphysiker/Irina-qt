// this file is distributed under
// GPL v 3.0 license
#include "gaussview.h"
#include "ui_gaussview.h"

GaussView::GaussView(GaussMagnitude *father, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::GaussView)
{
	master=father;
	ui->setupUi(this);
	refr();
	connect(master,SIGNAL(changed(SObject*)),this, SLOT(refr()));
}

GaussView::~GaussView()
{
	delete ui;
}

void GaussView::refr(){
	ui->average->setValue(master->Average());
	ui->sigma->setValue(master->Sigma());
	ui->precision->setValue(master->Discr());
}
void GaussView::on_average_valueChanged(double arg1)
{
	master->SetAverage(arg1);
}

void GaussView::on_sigma_valueChanged(double arg1)
{
	master->SetSigma(arg1);
}

void GaussView::on_precision_valueChanged(int arg1)
{
	master->SetDiscr(arg1);
}
