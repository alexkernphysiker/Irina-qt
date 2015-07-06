// this file is distributed under
// GPL v 3.0 license
#include "valuegb.h"
#include "ui_valuegb.h"


valuegb::valuegb(SoValue *value, QWidget *parent) :
	QGroupBox(parent),
	ui(new Ui::valuegb)
{
	m_value=value;
	ui->setupUi(this);
	ui->lineEdit->setText(m_value->Name());
	ui->doubleSpinBox->setValue(m_value->Value());
}

valuegb::~valuegb()
{
	delete ui;
}

void valuegb::on_pushButton_clicked()
{
	m_value->Set_Name(ui->lineEdit->text());
	m_value->SetValue(ui->doubleSpinBox->value());
}
