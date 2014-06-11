#ifndef GENERATE_CLASSES_H
#define GENERATE_CLASSES_H
#include <QtCore/qglobal.h>
#if defined(GEN_SOURCE_LIBRARY)
#  define GEN_SOURCE_SHARED_EXPORT Q_DECL_EXPORT
#else
#  define GEN_SOURCE_SHARED_EXPORT Q_DECL_IMPORT
#endif
#include <QString>
#include <QList>
#include <QDir>
namespace GenerateSource{
class GEN_SOURCE_SHARED_EXPORT IInstruction{
public:
	virtual ~IInstruction();
	virtual QString GetOutput()=0;
};
class GEN_SOURCE_SHARED_EXPORT Instruction:public IInstruction{
public:
	Instruction(QString txt);
	virtual ~Instruction();
	virtual QString GetOutput()override;
private:
	QString m_text;
};
class GEN_SOURCE_SHARED_EXPORT AbstractNested:public IInstruction{
public:
	AbstractNested();
	virtual ~AbstractNested();
	virtual QString GetOutput()override;
	virtual AbstractNested* Add(IInstruction* instr);
	AbstractNested* AddLine(QString instr);
protected:
	virtual QString __begin();
	virtual QString __end();
	void add(IInstruction* instr);
	void add(IInstruction* instr, int index);
private:
	QList<IInstruction*> container;
};
class GEN_SOURCE_SHARED_EXPORT CppNested:public AbstractNested{
public:
	CppNested(QString header, bool clr);// clr means that no ';' is put after '}'
	virtual ~CppNested();
protected:
	virtual QString __begin();
	virtual QString __end();
private:
	QString m_header;
	bool m_clr;
};
class GEN_SOURCE_SHARED_EXPORT XmlNested:public AbstractNested{
public:
	XmlNested(QString header, QString attributes="");
	virtual ~XmlNested();
	QString Name();
protected:
	virtual QString __begin()override;
	virtual QString __end()override;
private:
	QString m_header;
	QString m_attributes;
};
class GEN_SOURCE_SHARED_EXPORT GeneratedFile:public AbstractNested{
public:
	GeneratedFile(QString name);
	virtual ~GeneratedFile();
	GeneratedFile* SetDir(QDir dir);
	GeneratedFile* AddInclude(QString incl);
	void Save();
	QString Name();
	QDir Dir();
protected:
	virtual QString __begin()override;
	virtual QString __end()override;
	QDir m_dir;
	QString m_name;
};
class GEN_SOURCE_SHARED_EXPORT GenerateHeaderFile:public GeneratedFile{
public:
	GenerateHeaderFile(QString name);
	virtual ~GenerateHeaderFile();
protected:
	virtual QString __begin()override;
	virtual QString __end()override;
};
class GEN_SOURCE_SHARED_EXPORT IUnit{
public:
	virtual ~IUnit();
	virtual IUnit* SetOutput(AbstractNested* decl, AbstractNested* impl)=0;
	virtual IUnit* AddInstruction(IInstruction* instr)=0;
	virtual IUnit* AddLine(QString line);
protected:
	QList<IUnit*> m_units;
};
class GEN_SOURCE_SHARED_EXPORT FunctionUnit:public IUnit{
public:
	FunctionUnit(QString name,QString type, QString parameters);
	virtual ~FunctionUnit();
	virtual IUnit* SetOutput(AbstractNested* decl, AbstractNested* impl)override;
	virtual IUnit* AddInstruction(IInstruction *instr)override;
	QString Name();
protected:
	AbstractNested* funcbody;
	QString m_name;
	QString m_type;
	QString m_parameters;
};
class ClassUnit;
class GEN_SOURCE_SHARED_EXPORT MethodUnit:public IUnit{
	friend class ClassUnit;
public:
	MethodUnit(QString name,QString type, QString parameters, QString modifier="");
	virtual ~MethodUnit();
	virtual IUnit* SetOutput(AbstractNested* decl, AbstractNested* impl)override;
	virtual IUnit* AddInstruction(IInstruction *instr)override;
	QString Name();
protected:
	AbstractNested* funcbody;
	ClassUnit* m_class;
	QString m_name;
	QString m_type;
	QString m_parameters;
	QString m_modifier;

};
enum inheritmode{none, pubmode, privmode, protmode};
class GEN_SOURCE_SHARED_EXPORT ClassUnit:public IUnit{
	friend class MethodUnit;
public:
	ClassUnit(QString name,QString father, inheritmode mode=none);
	virtual ~ClassUnit();
	virtual IUnit* SetOutput(AbstractNested* decl, AbstractNested* impl)override;
	virtual IUnit* AddInstruction(IInstruction *instr)override;
	ClassUnit* Private();
	ClassUnit* Protected();
	ClassUnit* Public();
	ClassUnit* Constructor(QString param, QString fatherparam, IInstruction* body);
	ClassUnit* Destructor(bool isvirtual, IInstruction* body);
	ClassUnit* Method(QString modifier,QString type, QString name, QString parameters, IInstruction* body);
	ClassUnit* AddMethod(MethodUnit* method);
	QString Name();
	QString ParentName();
protected:
	inheritmode m_mode;
	AbstractNested* classbody;
	AbstractNested* implementation;
	QString m_name;
	QString m_father;
};
#define IncludeInHeader true
class GEN_SOURCE_SHARED_EXPORT TranslationUnit{
public:
	TranslationUnit(QString name);
	virtual ~TranslationUnit();
	TranslationUnit* SetDir(QDir dir);
	QDir Dir();
	virtual TranslationUnit* AddInclude(QString incl, bool hdr=false);
	virtual TranslationUnit* AddDefine(QString incl, bool hdr=false);
	virtual TranslationUnit* AddInstruction(IInstruction* instr, bool hdr=false);
	virtual TranslationUnit* AddUnit(IUnit* unit);
	QString HeaderName();
	QString SourceName();
	QString Name();
protected:
	GenerateHeaderFile* header;
	GeneratedFile* source;
	QList<IUnit*> m_units;
	QString m_name;
};
}
#endif // GENERATE_CLASSES_H
