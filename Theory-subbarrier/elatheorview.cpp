// this file is distributed under
// GPL v 3.0 license
#include "elatheorview.h"
#include "ui_elatheorview.h"

ElaTheorView::ElaTheorView(ElaTheor *ela, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ElaTheorView)
{
	ui->setupUi(this);
	m_ela=ela;
	ui->Z->setValue(m_ela->Zt());
	ui->min->setValue(m_ela->Min());
	ui->max->setValue(m_ela->Max());
	ui->step->setValue(m_ela->Step());
	ui->second->setValue(m_ela->SecondVar());
}

ElaTheorView::~ElaTheorView()
{
	delete ui;
}

void ElaTheorView::on_Z_valueChanged(int arg1)
{
	m_ela->SetZt(arg1);
}

void ElaTheorView::on_min_valueChanged(double arg1)
{
	m_ela->SetMin(arg1);
}

void ElaTheorView::on_max_valueChanged(double arg1)
{
	m_ela->SetMax(arg1);
}

void ElaTheorView::on_step_valueChanged(double arg1)
{
	m_ela->SetStep(arg1);
}

void ElaTheorView::on_second_valueChanged(double arg1)
{
	m_ela->SetSecondVar(arg1);
}

void ElaTheorView::on_nameedit_textChanged(const QString &arg1)
{
	m_ela->Set_Name(arg1);
}
