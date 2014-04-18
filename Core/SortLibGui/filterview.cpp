#include "filterview.h"
#include "ui_filterview.h"
#include <locview.h>

FilterView::FilterView(SoEFilter *filter, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::FilterView)
{
	m_filter=filter;
	ui->setupUi(this);
	ui->spinBox->setValue(m_filter->ND());
	ui->nameedit->setText(m_filter->Name());
	ui->NDbox->setVisible(m_filter->IsJust(SOT_EFilter));
	if(m_filter->Is(SOT_EFLoc)){
		QWidget *locview=new LocView((SoEFLoc*)m_filter,this);
		locview->move(0,ui->NDbox->y());
		locview->setVisible(true);
	}
}

FilterView::~FilterView()
{
	delete ui;
}

void FilterView::on_spinBox_valueChanged(int arg1)
{
	m_filter->SetND(arg1);
}

void FilterView::on_nameedit_textChanged(const QString &arg1)
{
	m_filter->Set_Name(arg1);
}
