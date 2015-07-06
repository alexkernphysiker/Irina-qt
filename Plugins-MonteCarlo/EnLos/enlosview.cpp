// this file is distributed under
// GPL v 3.0 license
#include "enlosview.h"
#include "ui_enlosview.h"

EnLosView::EnLosView(EnLos *los, QWidget *parent):
	QGroupBox(parent),
	ui(new Ui::EnLosView)
{
	ui->setupUi(this);
	m_los=los;
	refr();
	connect(m_los,SIGNAL(changed(SObject*)),this,SLOT(refr()));
}

EnLosView::~EnLosView()
{
	delete ui;
}

void EnLosView::refr(){
	ui->name->setText(m_los->Name());
	ui->energyname->setText(m_los->Energy());
	ui->lengthname->setText(m_los->Length());
	ui->dx->setValue(m_los->dx());
}
void EnLosView::on_name_textChanged(const QString &arg1)
{
	m_los->Set_Name(arg1);
}

void EnLosView::on_energyname_textChanged(const QString &arg1)
{
	m_los->SetEnergy(arg1);
}

void EnLosView::on_lengthname_textChanged(const QString &arg1)
{
	m_los->SetLength(arg1);
}

void EnLosView::on_dx_valueChanged(double arg1)
{
	m_los->set_dx(arg1);
}
