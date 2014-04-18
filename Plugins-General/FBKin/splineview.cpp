#include "splineview.h"
#include "ui_splineview.h"

splineview::splineview(SoSP1Line *spline, QWidget *parent):
	QGroupBox(parent),
	ui(new Ui::splineview)
{
	m_spline=spline;
	ui->setupUi(this);
	refr();
	connect(m_spline,SIGNAL(changed(SObject*)),this, SLOT(refr()));
}

splineview::~splineview()
{
	delete ui;
}

void splineview::refr(){
	ui->name->setText(m_spline->Name());
	ui->Mp->setText(m_spline->Mp());
	ui->MT->setText(m_spline->MT());
	ui->Mx->setText(m_spline->Mx());
	ui->Theta->setText(m_spline->ThetaCM());
	ui->Eres->setText(m_spline->Q());
	ui->N->setText(m_spline->N());
	ui->sigma->setText(m_spline->Sigma());
}
void splineview::on_name_textChanged(const QString &arg1)
{
	m_spline->Set_Name(arg1);
}

void splineview::on_Mp_textChanged(const QString &arg1)
{
	m_spline->SetMp(arg1);
}

void splineview::on_MT_textChanged(const QString &arg1)
{
	m_spline->SetMT(arg1);
}

void splineview::on_Mx_textChanged(const QString &arg1)
{
	m_spline->SetMx(arg1);
}

void splineview::on_Theta_textChanged(const QString &arg1)
{
	m_spline->SetThetaCM(arg1);
}

void splineview::on_Eres_textChanged(const QString &arg1)
{
	m_spline->SetQ(arg1);
}

void splineview::on_N_textChanged(const QString &arg1)
{
	m_spline->SetN(arg1);
}

void splineview::on_sigma_textChanged(const QString &arg1)
{
	m_spline->SetSigma(arg1);
}
