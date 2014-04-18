#include "kinview.h"
#include "ui_kinview.h"

KinView::KinView(SoFBKin *kin, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::KinView)
{
	ui->setupUi(this);
	m_kin=kin;
	ui->nameedit->setText(m_kin->Name());
	ui->retmlabel->setText(m_kin->ReturnType());
	ui->mtedit->setText(m_kin->MT());
	ui->mpedit->setText(m_kin->Mp());
	ui->mredit->setText(m_kin->Mx());
	ui->epedit->setText(m_kin->Ep());
	ui->thetaedit->setText(m_kin->ThetaCM());
	ui->qedit->setText(m_kin->Q());
}

KinView::~KinView()
{
	delete ui;
}

void KinView::on_nameedit_textChanged(const QString &arg1)
{
	m_kin->Set_Name(arg1);
}

void KinView::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int /*column*/)
{
	switch(ui->treeWidget->indexOfTopLevelItem(item)){
	case 0:
		m_kin->SetAngle();
		break;
	case 1:
		m_kin->SetElab();
		break;
	case 2:
		m_kin->SetEcm();
		break;
	case 3:
		m_kin->SetI();
		break;
	}
	ui->retmlabel->setText(m_kin->ReturnType());
}


void KinView::on_mpedit_textChanged(const QString &arg1)
{
	m_kin->SetMp( arg1);
}

void KinView::on_mtedit_textChanged(const QString &arg1)
{
	m_kin->SetMT(arg1);
}

void KinView::on_mredit_textChanged(const QString &arg1)
{
	m_kin->SetMx(arg1);
}

void KinView::on_epedit_textChanged(const QString &arg1)
{
	m_kin->SetEp(arg1);
}

void KinView::on_qedit_textChanged(const QString &arg1)
{
	m_kin->SetQ(arg1);
}

void KinView::on_thetaedit_textChanged(const QString &arg1)
{
	m_kin->SetThetaCM(arg1);
}
