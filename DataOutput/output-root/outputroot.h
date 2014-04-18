#ifndef OUTPUTROOT_H
#define OUTPUTROOT_H
#include <SortLib.h>
#include <QtCore>
#include <QList>

class RootOutput : public IDataOutput{
	Q_OBJECT
public:
	RootOutput();
	~RootOutput();

public:
	virtual QString About();//about message
public slots:
	// these slots are used for output data plotting
	virtual void sl_out_1d(QString);// signals about starting of 1-d distribution plotting
	virtual void sl_out_2d(QString);// signals about starting of 2-d distribution plotting
	virtual void sl_out_dataitem(DataEvent*);// transfering a piece of data for plotting
	virtual void sl_out_end();// data terminating signal
	virtual void sl_out_config();// open data output configuration dialog signal
	virtual void sl_out_gui_close();// close all open data output windows
private:
	char mode;
	QString title;
	QList<DataEvent*> data;
	DataEvent* X;
	DataEvent* Y;
};

#endif // OUTPUTROOT_H
