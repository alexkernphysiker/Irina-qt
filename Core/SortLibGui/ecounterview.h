#ifndef ECOUNTERVIEW_H
#define ECOUNTERVIEW_H

#include <QGroupBox>
#include <QTreeWidget>
#include <SortLib.h>

namespace Ui {
	class ECounterView;
}

class ECounterView : public QGroupBox
{
	Q_OBJECT
public:
	explicit ECounterView(SoECounter *cnt, QWidget *parent = 0);
	~ECounterView();
private slots:
	void on_nameedit_textChanged(const QString &arg1);

	void on_AvTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

	void on_UsedTree_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
	void refrAv();
	void refrUsed();
	Ui::ECounterView *ui;
	SoECounter *m_counter;
};

#endif // ECOUNTERVIEW_H
