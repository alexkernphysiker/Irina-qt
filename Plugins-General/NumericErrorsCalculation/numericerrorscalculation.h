// this file is distributed under
// GPL v 3.0 license
#ifndef NUMERICERRORSCALCULATION_H
#define NUMERICERRORSCALCULATION_H
#include <SortLib.h>
#include <SortLibGui.h>

#define SOT_NumErrCalc 0x0410

class NumericErrorsCalculation : public SoFormula {
	Q_OBJECT
public:
	NumericErrorsCalculation(SortProject *proj);
	NumericErrorsCalculation(QDataStream &str, SortProject *proj);
	virtual void Save(QDataStream &str)override;
	virtual ~NumericErrorsCalculation();
	virtual QString DisplayName()override;
	QString FuncName();
	void SetFuncName(QString nme);
	int Count();
	QString ErrName(int N);
	double ErrValue(int N);
	int ErrNum(QString name);
	double ErrValue(QString name);
	void RemoveErr(int N);
	void SetError(QString name, double val);
	virtual double Value(SoDFReader *fr, DataEvent *event)override;
	virtual void *GetForm()override;
private slots:
	void varvalueintersector(QString name, double *val, SoDFReader *dr,DataEvent*);
private:
	void testdfr(double *res, double zer, double d,QString func,SoDFReader *fr, DataEvent *event);
	double m_delta;
	QString m_curvar;
	SoDFReader *m_reader;
	QString m_funcname;
	QList<void *> m_errors;
};

#endif // NUMERICERRORSCALCULATION_H
