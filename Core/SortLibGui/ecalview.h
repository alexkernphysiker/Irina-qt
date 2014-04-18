#ifndef ECALVIEW_H
#define ECALVIEW_H

#include <QGroupBox>
#include <QTreeWidget>
#include <SortLib.h>


namespace Ui {
	class ECalView;
}

class ECalView : public QGroupBox
{
	Q_OBJECT

public:
	explicit ECalView(SoSPEnergy *spe, QWidget *parent = 0);
	~ECalView();

private slots:
	void on_xbut_clicked();

	void on_ybut_clicked();

	void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
	void refr();
	SoSPAxis *SelectedAxis();
	Ui::ECalView *ui;
	SoSPEnergy *m_spe;
};

#endif // ECALVIEW_H
