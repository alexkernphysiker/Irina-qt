// this file is distributed under
// GPL v 3.0 license
#ifndef EXPRESSION_H_EXPR_LIB
#define EXPRESSION_H_EXPR_LIB
#define MATH__EXPR_NOTYPE 0
#define MATH__EXPR_NUMBER '#'
#define MATH__EXPR_VARIABLE '$'
#define MATH__EXPR_UNARY '!'
#define MATH__EXPR_BINARY '@'

#include <QtCore/qglobal.h>

#if defined(EXPRESSION_LIBRARY)
#  define EXPRESSIONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EXPRESSIONSHARED_EXPORT Q_DECL_IMPORT
#endif
#include <QObject>

namespace Math_{

class Expr;

EXPRESSIONSHARED_EXPORT Expr *readexpr(QDataStream &str);//reads Expr instance from binary file stream (discovers type itself)

EXPRESSIONSHARED_EXPORT Expr* ParseExprFromString(QString expr_str);

class EXPRESSIONSHARED_EXPORT Expr: public QObject{
	Q_OBJECT
protected:
	Expr();
	Expr(QDataStream &str);
public:
	virtual void Save(QDataStream &str);
	virtual ~Expr();
	virtual double Value();
	char Type();
	virtual Expr *LookForParent(Expr *expr);
	virtual QString ExprStr()=0;
protected:
	void settype(char type);
private:
	char m_type;
protected slots:
	void PassGettingValue(double &retval, char type, QString name, double x1, double x2);
	void catcherror(QString msg);
	void acceptchanged(Math_::Expr *sender);
signals:
	void GetValue(double &retval, char type, QString name, double x1, double x2);
	void error(QString);
	void changed(Math_::Expr *);
};
//unary operator for expression tree
class EXPRESSIONSHARED_EXPORT Unary:public Expr{
	Q_OBJECT
public:
	Unary();
	Unary(QDataStream &str);
	virtual void Save(QDataStream &str)override;
	virtual ~Unary();
	virtual double Value()override;
	QString Name();
	void SetName(QString name);
	Expr *Operand();
	void SetOperand(Expr *opr);
	virtual Expr *LookForParent(Expr *expr)override;
	virtual QString ExprStr()override;
private:
	QString m_name;
	Expr *m_operand;
};
//binary operator in expression tree
class EXPRESSIONSHARED_EXPORT Binary:public Unary{
	Q_OBJECT
public:
	Binary();
	Binary(QDataStream &str);
	virtual void Save(QDataStream &str)override;
	virtual ~Binary();
	virtual double Value()override;
	Expr *Operand2();
	void SetOperand2(Expr *opr);
	virtual Expr *LookForParent(Expr *expr)override;
	virtual QString ExprStr()override;
private:
	Expr *m_operand2;
};
//constant in expression tree
class EXPRESSIONSHARED_EXPORT Number:public Expr{
	Q_OBJECT
public:
	Number();
	Number(QDataStream &str);
	virtual void Save(QDataStream &str)override;
	virtual ~Number();
	virtual double Value()override;
	void SetValue(double val);
	virtual QString ExprStr()override;
private:
	double m_value;
};
//variable in expression tree
class EXPRESSIONSHARED_EXPORT Variable:public Expr{
	Q_OBJECT
public:
	Variable();
	Variable(QDataStream &str);
	virtual void Save(QDataStream &str)override;
	virtual ~Variable();
	virtual double Value()override;
	QString Name();
	void SetName(QString name);
	virtual QString ExprStr()override;
private:
	QString m_name;
};

}
#endif
