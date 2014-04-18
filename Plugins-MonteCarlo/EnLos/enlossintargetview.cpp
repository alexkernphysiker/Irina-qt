#include "enlossintargetview.h"
#include "ui_enlossintargetview.h"

EnLossInTargetView::EnLossInTargetView(EnLossInTarget *loss, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::EnLossInTargetView)
{
	m_loss=loss;
	ui->setupUi(this);
	refr();
	connect(m_loss,SIGNAL(changed(SObject*)),this,SLOT(refr()));
}

EnLossInTargetView::~EnLossInTargetView()
{
	delete ui;
}
void EnLossInTargetView::refr(){
	ui->name->setText(m_loss->Name());
	ui->energy->setText(m_loss->Energy());
	ui->thetad->setText(m_loss->ThetaDetector());
	ui->thetat->setText(m_loss->ThetaTarget());
	ui->targetthickness->setText(m_loss->TargetThickness());
	ui->pstp->setText(m_loss->ProjectileStoppingPower());
	ui->xstp->setText(m_loss->XStoppingPower());
	ui->dx->setValue(m_loss->dx());
	ui->de->setValue(m_loss->dE());
	ui->emax->setValue(m_loss->Emax());
	ui->crosssection->setText(m_loss->CrossSection());
}

void EnLossInTargetView::on_name_textChanged(const QString &arg1)
{
	m_loss->Set_Name(arg1);
}


void EnLossInTargetView::on_energy_textChanged(const QString &arg1)
{
	m_loss->SetEnergy(arg1);
}

void EnLossInTargetView::on_thetad_textChanged(const QString &arg1)
{
	m_loss->SetThetaDetector(arg1);
}

void EnLossInTargetView::on_thetat_textChanged(const QString &arg1)
{
	m_loss->SetThetaTarget(arg1);
}

void EnLossInTargetView::on_targetthickness_textChanged(const QString &arg1)
{
	m_loss->SetTargetThickness(arg1);
}

void EnLossInTargetView::on_pstp_textChanged(const QString &arg1)
{
	m_loss->SetProjectileStoppingPower(arg1);
}

void EnLossInTargetView::on_xstp_textChanged(const QString &arg1)
{
	m_loss->SetXStoppingPower(arg1);
}

void EnLossInTargetView::on_dx_valueChanged(double arg1)
{
	m_loss->set_dx(arg1);
}

void EnLossInTargetView::on_crosssection_textChanged(const QString &arg1)
{
	m_loss->SetCrossSection(arg1);
}

void EnLossInTargetView::on_de_valueChanged(double arg1)
{
	m_loss->set_dE(arg1);
}

void EnLossInTargetView::on_emax_valueChanged(double arg1)
{
	m_loss->set_Emax(arg1);
}
