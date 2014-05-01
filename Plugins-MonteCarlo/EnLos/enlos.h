#ifndef ENLOSINTARGET_H
#define ENLOSINTARGET_H
#include <SortLib.h>
#include <SortLibGui.h>
#include <Plugins-MonteCarlo/Simulation/simulation.h>

#define SOT_EnLos 0x011010
#define SOT_EnLosInTarget 0x021010

class EnLos:public SoFormula{
	Q_OBJECT
public:
	EnLos(SoTblFunc *stoppingpower);
	EnLos(QDataStream &str,SortProject *proj);
	virtual void Save(QDataStream &str)override;
	virtual ~EnLos();
	virtual QString DisplayName()override;
	virtual double Value(SoDFReader *fr, DataEvent *event)override;
	virtual void* GetForm()override;
	virtual SObject *DisplParrent()override;
	QString Energy();
	void SetEnergy(QString energy);
	QString Length();
	void SetLength(QString length);
	double dx();
	void set_dx(double val);
private:
	QString m_energyfuncname;
	QString m_lengthfuncname;
	SoUnaryOperator *m_stoppingpower;
	double m_dx;
	QMutex m_mutex;
};
//*/
class EnLossInTarget:public SoFormula{
	Q_OBJECT
public:
	EnLossInTarget(SortProject *proj);
	EnLossInTarget(QDataStream &str,SortProject *proj);
	virtual void Save(QDataStream &str)override;
	virtual ~EnLossInTarget();
	virtual QString DisplayName()override;
	virtual double Value(SoDFReader *fr, DataEvent *event)override;
	virtual void* GetForm()override;
	QString Energy();
	void SetEnergy(QString energy);
	QString ThetaDetector();
	void SetThetaDetector(QString theta);
	QString ThetaTarget();
	void SetThetaTarget(QString theta);
	QString TargetThickness();
	void SetTargetThickness(QString thickness);
	QString ProjectileStoppingPower();
	void SetProjectileStoppingPower(QString unary);
	QString XStoppingPower();
	void SetXStoppingPower(QString unary);
	double dx();
	void set_dx(double val);
	double dE();
	void set_dE(double val);
	double Emax();
	void set_Emax(double val);
	QString CrossSection();
	void SetCrossSection(QString binary);

private:
	void allocate_cstbl();
	void remove_cstbl();
	double *prob_tbl;
	QString m_energyfuncname;
	QString m_thetadetectorfunc;
	QString m_thetatargetfunc;
	QString m_targetthicknessfunc;
	QString m_projectilestp;
	QString m_xstp;
	double m_dx;
	double m_dE;
	double m_Emax;
	QString m_d3cs_domegadE;
	RandomMagnitude *random_pos;
	RandomMagnitude *random_en;
	QMutex m_mutex;
};
//*/
#endif // ENLOSINTARGET_H
