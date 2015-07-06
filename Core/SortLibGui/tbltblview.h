// this file is distributed under
// GPL v 3.0 license
#ifndef TBLTBLVIEW_H
#define TBLTBLVIEW_H

#include <QGroupBox>
#include <SortLib.h>

namespace Ui {
class TblTblView;
}

class TblTblView : public QGroupBox
{
	Q_OBJECT

public:
	explicit TblTblView(SoTableOfTables *tbl, QWidget *parent = 0);
	~TblTblView();

private slots:
	void refr();

	void on_name_textChanged(const QString &arg1);

	void on_add_clicked();

	void on_remove_clicked();

private:
	Ui::TblTblView *ui;
	SoTableOfTables *m_tbl;
};

#endif // TBLTBLVIEW_H
