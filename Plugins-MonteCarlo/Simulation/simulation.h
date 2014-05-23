#ifndef SIMULATION_H
#define SIMULATION_H
#include <functions/templates.h>
#include <QtCore/qglobal.h>
#include <QObject>
#include <SortLib.h>
#include <SortLibGui.h>
#include <functions/randomfunc.h>
#if defined(SIMULATION_LIBRARY)
#  define SIMULATIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SIMULATIONSHARED_EXPORT Q_DECL_IMPORT
#endif


#define SOT_Simulation 0x0401
#define SOT_RandomMagnitude 0x0810
#define SOT_GaussMagnitude 0x010810
#define SOT_MixedDistribution 0x020810
#define SOT_DistributedByFunction 0x040810
#define SOT_BinarySumm 0x0303

class SIMULATIONSHARED_EXPORT Simulation:public SoDFReader
{
	Q_OBJECT
public:
	Simulation(SortProject *father);
	Simulation(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~Simulation();
	virtual QString DisplayName()override;
	virtual void *GetForm()override;
	// Data Event generating
	virtual DataEvent *NextEvent(QDataStream &)override;
	virtual void ReadHeader(QDataStream &datastr)override;
	virtual void DataEventIsGot(DataEvent *Event)override;
	virtual void FinalDataCheck()override;
	virtual ReaderAsync *_getReaderAsync(QString m_path, SoDFReader *father)override;
	// Custom simulation parameters
	uint N();
	void SetN(uint val);
	void SetNADC(char val);
	QString Func(char ind);
	void SetFuncs(QStringList val);
private:
	QMutex m_mutex;
	void initfuncs();
	void clearfuncs();
	QStringList functions;
	uint n;
};

class SIMULATIONSHARED_EXPORT SimulationAsync: public ReaderAsync{
	Q_OBJECT
public:
	SimulationAsync(SoDFReader *father, uint N);
	virtual void Open()override;
protected slots:
	virtual void process()override;
private:
	QMutex m_mutex;
	uint n;
};


class SIMULATIONSHARED_EXPORT RandomMagnitude: public SoFormula {
	Q_OBJECT
public:
	RandomMagnitude(SortProject *proj);
	RandomMagnitude(QDataStream &str, SortProject *proj);
	virtual void Save(QDataStream &str)override;
	virtual ~RandomMagnitude();
	virtual QString DisplayName()override;
	virtual double Value(SoDFReader *fr, DataEvent *event)override;
	virtual void* GetForm()override;
	double MaxVal();
protected:
	double maxval;
	double lastval;
	DataEvent *last;
	SoDFReader *lastreader;
};

class SIMULATIONSHARED_EXPORT GaussMagnitude:public RandomMagnitude{
	Q_OBJECT
public:
	GaussMagnitude(SortProject *proj);
	GaussMagnitude(QDataStream &str,SortProject *proj);
	virtual void Save(QDataStream &str)override;
	virtual ~GaussMagnitude();
	virtual QString DisplayName()override;
	virtual double Value(SoDFReader *fr, DataEvent *event)override;
	virtual void *GetForm()override;
	double Average();
	double Sigma();
	uint Discr();
	void SetAverage(double val);
	void SetSigma(double val);
	void SetDiscr(uint val);
private:
	double average;
	double sigma;
};

class SIMULATIONSHARED_EXPORT SubDistr;

class SIMULATIONSHARED_EXPORT MixDistributions: public RandomMagnitude{
	Q_OBJECT
public:
	MixDistributions(SortProject *proj);
	MixDistributions(QDataStream &str,SortProject *proj);
	virtual void Save(QDataStream &str)override;
	virtual ~MixDistributions();
	virtual QString DisplayName()override;
	virtual double Value(SoDFReader *fr, DataEvent *event)override;
	virtual void *GetForm()override;
	uint Count();
	double Probability(uint n);
	QString Function(uint n);
	void Remove(uint n);
	void Add(QString name, double probability);
private:
	QList<SubDistr> data;
};

class TblFuncGetter{
private:
	SoTblFunc* m_func;
public:
	TblFuncGetter(){m_func=NULL;}
	TblFuncGetter(TblFuncGetter &f){m_func=f.m_func;}
	TblFuncGetter(SoTblFunc* f){m_func=f;}
	virtual ~TblFuncGetter(){}
	SoTblFunc* Owner(){return m_func;}
	double operator()(double x);
};
class SIMULATIONSHARED_EXPORT DistributedByFunction:public RandomMagnitude{
	Q_OBJECT
public:
	DistributedByFunction(SoTblFunc *func);
	DistributedByFunction(QDataStream &str, SortProject *father);
	virtual void Save(QDataStream &str)override;
	virtual ~DistributedByFunction();
	virtual QString DisplayName()override;
	virtual double Value(SoDFReader *fr, DataEvent *event)override;
	virtual void *GetForm()override;
	virtual SObject *DisplParrent()override;
private slots:
	void getdata();
private:
	SoTblFunc *m_func;
	TblFuncGetter *m_getter;
	Math_::RandomValueGenerator<double,TblFuncGetter>* randomizer;
};

class SIMULATIONSHARED_EXPORT SoBinarySumm:public SoBinaryOperator{
	Q_OBJECT
public:
	SoBinarySumm(SortProject *proj);
	SoBinarySumm(QDataStream &str, SortProject *proj);
	virtual void Save(QDataStream &str)override;
	virtual ~SoBinarySumm();
	virtual double F(double x, double y, SoDFReader *dr, DataEvent *event)override;
	virtual void *GetForm()override;
	int Count();
	QString Summand(int index);
	void Reset();
	void AddSummand(QString name);
private:
	QList<QString> m_oprs;
};

#endif // SIMULATION_H
