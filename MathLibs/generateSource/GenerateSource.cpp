#include "GenerateSource.h"
#include <QFile>
#include <QTextStream>
class genSourceException:public std::exception{
public:
	genSourceException(QString errormsg);
	virtual ~genSourceException() throw(){}
	virtual const char* what() const throw()override;
private:
	QString m_msg;
};
genSourceException::genSourceException(QString errormsg){m_msg=errormsg;}
const char* genSourceException::what() const throw(){
	QString msg="GenerateSource library: ";
	msg+=m_msg;
	return msg.toStdString().c_str();
}
namespace GenerateSource{
IInstruction::~IInstruction(){}
Instruction::Instruction(QString txt){m_text=txt;}
Instruction::~Instruction(){}
QString Instruction::GetOutput(){return m_text;}
AbstractNested::AbstractNested(){}
AbstractNested::~AbstractNested(){
	for (int i=0; i<container.count();i++){
		delete container.at(i);
	}
}
QString AbstractNested::GetOutput(){
	QString beg=__begin();
	QString end=__end();
	QString res= "";
	if(beg!="")res+=beg+"\n";
	for (int i=0; i<container.count();i++){
		IInstruction* instr=container.at(i);
		if(beg!=""){
			res+=QString("\t")+instr->GetOutput().replace("\n","\n\t")+"\n";
		}else{
			res+=instr->GetOutput()+"\n";
		}
	}
	if(end!="")res+=__end()+"\n";
	return res;
}
AbstractNested* AbstractNested::Add(IInstruction *instr){
	add(instr);
	return this;
}
AbstractNested* AbstractNested::AddLine(QString instr){
	add(new Instruction(instr));
	return this;
}
QString AbstractNested::__begin(){return "";}
QString AbstractNested::__end(){return "";}
void AbstractNested::add(IInstruction *instr){
	container.append(instr);
}
void AbstractNested::add(IInstruction *instr, int index){
	container.insert(index,instr);
}
CppNested::CppNested(QString header, bool clr){
	m_header=header;
	m_clr=clr;
}
CppNested::~CppNested(){}
QString CppNested::__begin(){
	return m_header+"{";
}
QString CppNested::__end(){
	if(m_clr) return "}";
	return "};";
}
GeneratedFile::GeneratedFile(QString name){
	m_name=name;
	m_dir=QDir("/");
}
GeneratedFile::~GeneratedFile(){}
GeneratedFile* GeneratedFile::SetDir(QDir dir){
	m_dir=dir;
	return this;
}
QString GeneratedFile::__begin(){return "";}
QString GeneratedFile::__end(){return "";}
GeneratedFile* GeneratedFile::AddInclude(QString incl){
	add(new Instruction(QString("#include "+incl)),0);
	return this;
}
void GeneratedFile::Save(){
	QFile file(m_dir.absoluteFilePath(m_name));
	file.open(QFile::WriteOnly);
	if(file.isOpen()){
		QTextStream str(&file);
		str<<GetOutput();
		file.close();
	}
}
GenerateHeaderFile::GenerateHeaderFile(QString name):GeneratedFile(name){}
GenerateHeaderFile::~GenerateHeaderFile(){}
QString GenerateHeaderFile::__begin(){
	QString defname=QString("___")+
			m_name.toUpper().
			replace(' ','_').replace('.','_').replace('-','_')+
			"___";
	return QString("#ifndef ")+defname+"\n"+
			"#	define "+defname;
}
QString GenerateHeaderFile::__end(){return "#endif";}

IUnit::~IUnit(){
	for(int i=0; i<m_units.count();i++)
		delete m_units.at(i);
}
IUnit* IUnit::AddLine(QString line){
	return AddInstruction(new Instruction(line));
}
FunctionUnit::~FunctionUnit(){}
FunctionUnit::FunctionUnit(QString name, QString type, QString parameters){
	m_name=name; 	m_type=type;
	m_parameters=parameters; funcbody=NULL;
}
IUnit* FunctionUnit::SetOutput(AbstractNested *decl, AbstractNested *impl){
	QString head=m_type+" "+m_name+"("+m_parameters+")";
	decl->AddLine(head+";");
	funcbody=new CppNested(head,true);
	impl->Add(funcbody);
	return this;
}
IUnit* FunctionUnit::AddInstruction(IInstruction *instr){
	if(funcbody!=NULL){
		funcbody->Add(instr);
	}else throw new genSourceException("unknown error");
	return this;
}
MethodUnit::MethodUnit(QString name, QString type, QString parameters, QString modifier){
	m_name=name; 	m_type=type; m_class=NULL;
	m_parameters=parameters; funcbody=NULL;m_modifier=modifier;
}
MethodUnit::~MethodUnit(){}
IUnit* MethodUnit::SetOutput(AbstractNested *decl, AbstractNested *impl){
	QString head=m_name+"("+m_parameters+")";
	decl->AddLine(m_modifier+" "+m_type+" "+head+";");
	if(!m_class)throw new genSourceException("unknown error");
	funcbody=new CppNested(m_type+" "+m_class->m_name+"::"+head,true);
	impl->Add(funcbody);
	return this;
}
IUnit* MethodUnit::AddInstruction(IInstruction *instr){
	if(funcbody!=NULL){
		funcbody->Add(instr);
	}else throw new genSourceException("unknown error");
	return this;
}
ClassUnit::ClassUnit(QString name, QString father, inheritmode mode){
	m_name=name;m_father=father;m_mode=mode;
	classbody=NULL;
}
ClassUnit::~ClassUnit(){}
IUnit* ClassUnit::SetOutput(AbstractNested *decl, AbstractNested *impl){
	QString head="class "+m_name+" : ";
	switch(m_mode){
	case pubmode:head+="public ";break;
	case privmode:head+="private ";break;
	case protmode:head+="protected ";break;
	case none:break;
	}
	head+=m_father;
	classbody=new CppNested(head,false);
	decl->Add(classbody);
	implementation=impl;
	return this;
}
IUnit* ClassUnit::AddInstruction(IInstruction *instr){
	if(classbody!=NULL){
		classbody->Add(instr);
	}else throw new genSourceException("unknown error");
	return this;
}
ClassUnit* ClassUnit::AddMethod(MethodUnit *method){
	method->m_class=this;
	method->SetOutput(classbody,implementation);
	m_units.append(method);
	return this;
}
ClassUnit* ClassUnit::Private(){AddLine("private:");return this;}
ClassUnit* ClassUnit::Protected(){AddLine("protected:");return this;}
ClassUnit* ClassUnit::Public(){AddLine("public:");return this;}
ClassUnit* ClassUnit::Constructor(QString param, QString fatherparam, IInstruction *body){
	AddLine(m_name+"("+param+");");
	QString head=m_name+"::"+m_name+"("+param+")";
	if((m_father!="")&&(fatherparam!=""))
		head+=QString(": ")+m_father+"("+fatherparam+")";
	CppNested* meth=new CppNested(head,true);
	implementation->Add(meth);
	if(body)
		meth->Add(body);
	return this;
}
ClassUnit* ClassUnit::Destructor(bool isvirtual, IInstruction *body){
	QString mod="";
	if(isvirtual)mod="virtual ";
	AddLine(mod+"~"+m_name+"( );");
	QString head=m_name+"::~"+m_name+"( )";
	CppNested* meth=new CppNested(head,true);
	implementation->Add(meth);
	if(body)
		meth->Add(body);
	return this;
}
ClassUnit* ClassUnit::Method(QString modifier, QString type, QString name, QString parameters, IInstruction* body){
	MethodUnit* meth=new MethodUnit(name,type,parameters,modifier);
	AddMethod(meth);
	if(body)
		meth->AddInstruction(body);
	return this;
}
TranslationUnit::TranslationUnit(QString name){
	m_name=name;
	header=new GenerateHeaderFile(name+".h");
	source=new GeneratedFile(name+".cpp");
	source->AddInclude(QString("\"")+name+".h\"");
}
TranslationUnit::~TranslationUnit(){
	header->Save();
	source->Save();
	delete header;
	delete source;
	for(int i=0; i<m_units.count();i++)
		delete m_units.at(i);
}
TranslationUnit* TranslationUnit::SetDir(QDir dir){
	header->SetDir(dir);
	source->SetDir(dir);
	return this;
}
TranslationUnit* TranslationUnit::AddInclude(QString incl, bool head){
	if(head){
		header->AddInclude(incl);
	}else{
		source->AddInclude(incl);
	}
	return this;
}
TranslationUnit* TranslationUnit::AddDefine(QString incl, bool head){
	if(head){
		header->AddLine(QString("#define ")+incl);
	}else{
		source->AddLine(QString("#define ")+incl);
	}
	return this;
}
TranslationUnit* TranslationUnit::AddInstruction(IInstruction *instr, bool hdr){
	if(hdr){
		header->Add(instr);
	}else{
		source->Add(instr);
	}
	return this;
}
TranslationUnit* TranslationUnit::AddUnit(IUnit *unit){
	m_units.append(unit);
	unit->SetOutput(header,source);
	return this;
}

QString GeneratedFile::Name(){return m_name;}
QString TranslationUnit::HeaderName(){return header->Name();}
QString TranslationUnit::SourceName(){return source->Name();}
QString ClassUnit::Name(){return m_name;}
QString ClassUnit::ParentName(){return m_father;}
QString TranslationUnit::Name(){return m_name;}
QString MethodUnit::Name(){return m_name;}
QString FunctionUnit::Name(){return m_name;}

XmlNested::XmlNested(QString header, QString attributes){
	m_header=header;m_attributes=attributes;
}
XmlNested::~XmlNested(){}
QString XmlNested::Name(){return m_header;}
QString XmlNested::__begin(){
	QString res=QString("<")+m_header;
	if(m_attributes!="")res+=QString(" ")+m_attributes;
	res+=">";
	return res;
}
QString XmlNested::__end(){
	return QString("</")+m_header+">";
}

QDir GeneratedFile::Dir(){return m_dir;}
QDir TranslationUnit::Dir(){return source->Dir();}
}
