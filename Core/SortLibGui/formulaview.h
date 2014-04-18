#ifndef FORMULAVIEW_H
#define FORMULAVIEW_H
#include <SortLib.h>
#include <QGroupBox>
#include <spaxisview.h>

namespace Ui {
	class FormulaView;
}

class FormulaView : public QGroupBox
{
	Q_OBJECT

public:
	explicit FormulaView(SoFormula *formula, QWidget *parent = 0);
	~FormulaView();
private:
	void applyexpr(Math_::Expr *expr);
private slots:
	void on_glnumber_clicked();

	void on_glvariable_clicked();

	void on_glunary_clicked();

	void on_glbinary_clicked();

	void on_treeWidget_itemSelectionChanged();

	void on_applybutton_clicked();

	void on_lineEdit_textChanged(const QString &arg1);

	void on_additional_clicked();

	void on_pushButton_clicked();

	void on_calbutt_clicked();

	void on_ParseButton_clicked();

	void on_expression_changed(Math_::Expr *expr);

private:
	Math_::Expr *SelectedExpr();
	Ui::FormulaView *ui;
	SoFormula *m_formula;
	QGroupBox *m_daughterview;
	QGroupBox *m_dv2;
};

#endif // FORMULAVIEW_H
