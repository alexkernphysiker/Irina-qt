#ifndef FBKIN_H
#define FBKIN_H
#include <SortLib.h>
#include <SortLibGui.h>

#define SOT_FBKin 0x010210
#define SOT_Ruth 0x020210
#define SOT_SP1Gauss 0x0203

	//binary kinemathics formula
	class SoFBKin:public SoFormula{
		Q_OBJECT
	public:
		SoFBKin(SortProject *father);
		SoFBKin(QDataStream &str, SortProject *father);
		virtual void Save(QDataStream &str)override;
		virtual ~SoFBKin();
		virtual QString DisplayName()override;
		virtual double Value(SoDFReader *fr, DataEvent *event)override;
		virtual void *GetForm()override;
		QString ReturnType();
		void SetI();
		void SetAngle();
		void SetEcm();
		void SetElab();
		QString MT();
		void SetMT(QString name);
		QString Mp();
		void SetMp(QString name);
		QString Mx();
		void SetMx(QString name);
		QString Ep();
		void SetEp(QString name);
		QString ThetaCM();
		void SetThetaCM(QString name);
		QString Q();
		void SetQ(QString name);
	private:
		QString mt;
		QString mp;
		QString mr;
		QString Eplab;
		QString thetapcm;
		QString Qreact;
		char returntype;
	};

	class SoRuth:public SoFormula{
		Q_OBJECT
	public:
		SoRuth(SortProject *proj);
		SoRuth(QDataStream &str, SortProject *proj);
		virtual void Save(QDataStream &str)override;
		virtual ~SoRuth();
		virtual QString DisplayName()override;
		virtual double Value(SoDFReader *fr, DataEvent *event)override;
		virtual void *GetForm()override;
		QString Zp();
		void SetZp(QString name);
		QString Zt();
		void SetZt(QString name);
		QString Theta();
		void SetTheta(QString name);
		QString E();
		void SetE(QString name);
	private:
		QString m_zp;
		QString m_zt;
		QString m_theta;
		QString m_e;
	};

	class SoSP1Line:public SoBinaryOperator{
		Q_OBJECT
	public:
		SoSP1Line(SortProject *proj);
		SoSP1Line(QDataStream &str,SortProject *proj);
		virtual void Save(QDataStream &str)override;
		virtual ~SoSP1Line();
		virtual double F(double ex, double Ep, SoDFReader *dr, DataEvent *event)override;
		virtual void *GetForm()override;
		QString Sigma();
		void SetSigma(QString name);
		QString N();
		void SetN(QString name);

		QString MT();
		void SetMT(QString name);
		QString Mp();
		void SetMp(QString name);
		QString Mx();
		void SetMx(QString name);
		QString ThetaCM();
		void SetThetaCM(QString name);
		QString Q();
		void SetQ(QString name);
private slots:
		void resetlast();
	private:
		QString m_sigma;
		QString m_N;

		QString mt;
		QString mp;
		QString mr;
		QString thetapcm;
		QString Qreact;

		double lastEp;
		double lastRes;
	};

#endif // FBKIN_H
