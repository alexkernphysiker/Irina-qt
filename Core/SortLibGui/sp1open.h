#ifndef SP1OPEN_H
#define SP1OPEN_H

#include <QDialog>
 #include <QKeyEvent>
#include <SortLib.h>
#include <additional.h>


namespace Ui {
	class SP1Open;
}

class  SP1Open : public QDialog
{
	Q_OBJECT

public:
	explicit SP1Open(SoECSP1 *sp1, QWidget *parent = 0);
	~SP1Open();

private slots:
	void busy(bool val);
	void on_mover_clicked();

	void on_movel_clicked();

	void on_xsinc_clicked();

	void on_xsdec_clicked();

	void on_ysdec_clicked();

	void on_ysinc_clicked();

	void on_wintree_itemSelectionChanged();

	void on_llmb_clicked();

	void on_lmb_clicked();

	void on_rmb_clicked();

	void on_rrmb_clicked();

	void on_toolButton_clicked();

	void on_toolButton_2_clicked();

	void on_peaktree_itemSelectionChanged();

	void close();
	void refr(SObject* so);

	void on_toolButton_4_clicked();

	void on_toolButton_3_clicked();

	void on_pushButton_clicked();

private:
	void paintEvent(QPaintEvent *);
	void keyPressEvent(QKeyEvent *ke);
	Ui::SP1Open *ui;
	SoECSP1 *m_sp1;
	SoEFWin *SelectedWindow();
	Peak *SelectedPeak();
	uint m_offs;
	uint m_xscale;
	int m_yscale_log;
	int x_beg;
	int channelsperscreen();
	int cntperscreen();
	int y2cnt(int y);
	int x2ch(int x);
	void drawpnt(int ch,int cnt, QPainter &painter,QColor &color,QColor &color2);
	int m_markerpos;
	void updmarkermsg();
};

#endif // SP1OPEN_H
