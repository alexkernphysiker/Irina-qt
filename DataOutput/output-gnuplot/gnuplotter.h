#ifndef GNUGnuplotter_H
#define GNUGnuplotter_H
#include <SortLib.h>
#include <QtCore>
#include <QProcess>

class PlotParameters;
class Gnuplotter : public IDataOutput{
	Q_OBJECT
public:
	Gnuplotter();
	virtual ~Gnuplotter();
public slots:
	virtual void sl_out_1d(QString)override;// signals about starting of 1-d distribution plotting
	virtual void sl_out_2d(QString)override;// signals about starting of 2-d distribution plotting
	virtual void sl_out_dataitem(DataEvent*)override;// transfering a piece of data for plotting
	virtual void sl_out_end()override;// data terminating signal
	virtual void sl_out_config()override;// open data output configuration dialog signal
	virtual void sl_out_gui_close()override;// close all open data output windows
	virtual QString About()override;
private slots:
	void processerror(QProcess::ProcessError error);
	void Closing(bool toplot, PlotParameters* params);
signals:
	void changed(QStringList &);
	void closeallrelated();
private:
	void out_1d();
	void out_2d();
	char mode;
	QString title;
	QFile *txtfile;
	QTextStream *txtfilecreator;
	DataEvent *ax1;
	DataEvent *ax2;
	void check_createfile(QTextStream &str);
	void run(QString script);
	QStringList m_data;
public:
	QString gnuplotpath;
	bool create_ps;
	bool create_png;
	QString font;
	QString pngsize;
};

class  PlotParameters{
public:
	QString xlabel;
	QString ylabel;
	bool logx;
	bool logy;
	QString xrange;
	QString yrange;
	QString xtics;
	QString ytics;
	QString setkey;
	PlotParameters();
};


#endif // GNUGnuplotter_H
