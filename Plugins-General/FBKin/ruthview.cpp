// this file is distributed under
// GPL v 3.0 license
#include "ruthview.h"
#include "ui_ruthview.h"

RuthView::RuthView(SoRuth *r, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::RuthView)
{
	ui->setupUi(this);
	ruth=r;
	refr();
	connect(ruth,SIGNAL(changed(SObject*)),this,SLOT(refr()));
}

RuthView::~RuthView()
{
	delete ui;
}

void RuthView::refr(){
	ui->name->setText(ruth->Name());
	ui->zp->setText(ruth->Zp());
	ui->zt->setText(ruth->Zt());
	ui->e->setText(ruth->E());
	ui->theta->setText(ruth->Theta());
}
void RuthView::on_name_textChanged(const QString &arg1)
{
	ruth->Set_Name(arg1);
}

void RuthView::on_zp_textChanged(const QString &arg1)
{
	ruth->SetZp(arg1);
}

void RuthView::on_zt_textChanged(const QString &arg1)
{
	ruth->SetZt(arg1);
}

void RuthView::on_e_textChanged(const QString &arg1)
{
	ruth->SetE(arg1);
}

void RuthView::on_theta_textChanged(const QString &arg1)
{
	ruth->SetTheta(arg1);
}
