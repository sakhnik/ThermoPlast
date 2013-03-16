////////////////////////////////////////////////////////////////
// dfmc.cpp
// dfmc <file1.dfm> <file2.dfm> ...
//
// ������ -- ��������� ������� *.dfm ��� �������� guiserv
// �����, 2003
// "������� �������", ���
////////////////////////////////////////////////////////////////


#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <windows.h>

using namespace std;

// �� ���������� � �������� ���� � ��� "��������"
const char strCork[] =
			"\t{IDK_END, ID_NULL,\n"
			"\t0,0,0,0,\n"
			"\t\"\",\"\",\n"
			"\tNULL, NULL,\n"
			"\t&NullEnabled, NULL\n"
			"\t}";

//////////////////////////////////////////////////////////////////
//   ����� �������� �������
// ������� true � ������� �����
// ������� ���� ����������� � ����� str[]
//
//////////////////////////////////////////////////////////////////
bool GetNextToken(istream &in, char str[])
{
	//char str1[256];
	try
	{
		if (!in)
			throw 1;
/*		in >> str;

		/////// Pascal strings //////////////
		if (str[0]=='\'')
		{
			strcpy(str1,str+1); // Cutting out first quotation mark
			strcpy(str,str1);
			while (str[strlen(str)-1] != '\'')
			{
				if (!in)
					throw 1;
				in >> str1;
				strcat(str," ");
				strcat(str,str1);
			}
			str[strlen(str)-1]=0; // Cutting out last quotation mark
		}
		/////////////////////////////////////
*/
		char str1[256];
		int i=0;
		char c;
		while (in && (c=in.peek()) && isspace(c))
			in.get(c);

		while (in.get(str1[i]) && str1[i] && !isspace(str1[i]))
		{
			if (str1[i]=='\'')
			{
				// Pascal string begins here
				while (in.get(str1[i]) && str1[i]!='\'')
					++i;
				--i;
			}
			if (str1[i]=='#')
			{
			// another Pascal joke.
				int n=0;
				for (int j=0;j<4;j++)
				{
					in.get(c);
					n=n*10+c-'0';
				}
				if (n>=1040 && n<=1071)
					c=n-1040+'�';
				else if (n>=1072 && n<=1104)
					c=n-1072+'�';
				else if (n==1105)
					c='�';
				else if (n==1025)
					c='�';
				str1[i]=c;
			}
			++i;
		}
		str1[i]=0;

		::CharToOem(str1,str);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

/////////////////////////////////////////////////////
// ���������� ������������� ����� � ��������� �����
/////////////////////////////////////////////////////
inline int ConvertToDigit(char c)
{
	if (c>='0' && c<='9')
		return c-'0';
	if (c>='A' && c<='F')
		return c-'A'+10;
	if (c>='a' && c<='f')
		return c-'a'+10;
	return -1;
}

//////////////////////////////////////////////////////
// ����� � ����������� ������ ����� ����, ���������
// ����� ���������������� �������
//////////////////////////////////////////////////////
int GetNextChar(istream &in)
{
	char c1,c2;
	in >> c1 >> c2;
	return ConvertToDigit(c1)*16+ConvertToDigit(c2);
}

///////////////////////////////////////////////////////////
// ����� � ����������� ������ ����� ���� �����, ��������
// ������� ���������������� �������. �� ���������������
// ��� ����� ������
//////////////////////////////////////////////////////////
inline int GetNextInt(istream &in)
{
	int i0=GetNextChar(in),
		i1=GetNextChar(in),
		i2=GetNextChar(in),
		i3=GetNextChar(in);
	return (i3<<24)+(i2<<16)+(i1<<8)+i0;
}

//////////////////////////////////////////////////////////////////
// ������ ���� ����������� ��"����, ����� ��� ���, �� � �
// iface.h
//////////////////////////////////////////////////////////////////
enum ObjectTypes
	{
		IDK_DIALOG,
		IDK_LABEL,
		IDK_BUTTON,
		IDK_EDIT,
		IDK_CHECK,
		IDK_TABCONTROL,
		IDK_TAB,
		IDK_BMP,
		IDK_TABLE,

		IDK_EMPTY // ������ �������� ��� ��, �� ��"��� �� ������ �����
	};

//////////////////////////////////////////////////////////////////////
// ����-������, ���� ��������������� ��� ������������� ������,
// ��������� ���������� � ����������� �����
//////////////////////////////////////////////////////////////////////
class CParsedObject
{
private:
	CParsedObject *next,*down,*up; // ��������� �� ��������� ������� ����� ������ ����,
	// �� ������ ��������� �������, �� �������� ��������
	unsigned int ID; // �������������
	unsigned int Left,Top,Width,Height; // ���������� ���������
	unsigned int Rows,Cols; // ������� ����� �� �������� � �������, �� ���� ����
	// �������� �� ���������� �� ��������������
	char *Caption, // or Text for TEdit or Data for BmpItem
		*Name, *Tip; // ��"� �� ��������� ����������
	char *IDstring; // ��������� ����������� ��������������
	enum ObjectTypes Type; // ��� ����� ����������
	bool IsEnabled, IsChecked,IsVisible; // ����������, ���������� (����� ��� ��������),
	// ��������
	char CharactersReserved; // ������� �������������� ������� ��� Text

public:
	CParsedObject() // ����������� �� ���������
	{
		next=down=up = 0;
		Name=Caption=Tip = 0;
		IDstring = 0;
		IsEnabled = true;
		IsChecked = false;
		IsVisible = true;
		Type = IDK_EMPTY;
	};
	~CParsedObject(); // ����������, ���� ������� � ���� ���� ������
	// �����, ���� ������ ����� ������ in �� ������� ����� HeaderLine,
	// ����, �������, ��� ��� ���������� � ������ ����� (����)
	// �������� StreamName -- �� �������, ���� ���� ������ �� ��� ����
	// Name, ��� ����, ��� �������� �������� ����
	bool ParseStream(istream &in, char HeaderLine[], char StreamName[]);
	// ����������������� ����� ��� ������������, �������� ����� �����-
	// ����� ��� �������� ��"���� CParsedObject �� cout
	void Out();
	// ������ �������� �������� �� ������������ ���� ������ ��������.
	// ��� ���� �������� ���������� ������ �������� � ����� �� ������������
	bool Check();
	// ������� ��������������� ��� ������� ���������� ������ -- ������-
	// ��������
	void GetID(int &ID);
	// ϳ�������� ����������� ������� ���������� �� ���.
	// ����� ����� �� ������� ����������� ������� (��� ��������)
	// �� ������� ��� ��������� ������� DRAW_QUEUE_MAX � iface.h
	void CalculateDepth(int &Count);
	// ϳ������� ������� ��"���� �� ������ ���
	int CalculateObjectsOnTheLevel();
	// �������� � �������� ���� ������� ��������������
	void ReportID(ostream &out);
	// �������� � �������� ���� ����� �������, �� ������ ����� ������������ ������
	void ReportTexts(ostream &out);
	// �������� ��������� ��� ��� ������
	void ReportBmps(ostream &out);
	// �������� ��������� ����������
	void ReportAdditional(ostream &out);
	// �������� ���������� ����������
	void ReportSpecific(ostream &out);
	// �������� ����� ������� ���������
	void ReportMainStructure(ostream &out);
	// �������� ����� action.c
	void ReportCases(ostream &out);
	// �������� ���� ����� ��"���� � ����, ��������������� ��� ���������
	// �������� ����� � ���� ��� ����� ������� �����
	friend void Merge(CParsedObject *&First, CParsedObject *&Second);
};

////////////////////////////////////////////////////////////////////////////////////
// ������������ ����� ����� *.dfm
// =======================================
// �� ����� ���� ���� ��������������, �� ��������� ����������� ������� � ���� ������
// ������� ������.
// ������ �����:
//
// object ObjName: ObjType
//   property1 = value1
//   property2 = value2
//   ...................
//   object ObjName1: ObjType
//     property3 = value3
//     ......................
//   end
// end
//
// �����, �� � ������ ��������� ���� -- ��"���, ���� ������ ���� ������������
// ��, �������, ���� ��� �����.
bool CParsedObject::ParseStream(istream &in, char HeaderLine[], char StreamName[])
{
	char Token[256], Value[256], Line[256];

	// ���� ���� �������� ����� � HeaderLine, ������� �������� ����� ��������� ���
	// ������ ������ ���������� ����
	if (HeaderLine[0]==0)
	{
		in.getline(Line,256);
	}
	else
	{
		strcpy(Line,HeaderLine);
	}
	istringstream strin(Line); // ��������� � ����� ����

	// �� ������� ������� ���� ���� "object". �������� �� �����������, �����������
	// �� ��, �� ���������� ����� �������� � ���� ������ �� ���� � ��������� �����
	// ���������� �� �����
	if (!GetNextToken(strin,Value))
		return false;

	// � �� ��"� ��"����
	if (!GetNextToken(strin,Value))
		return false;
	Value[strlen(Value)-1] = 0; // ��������� ��������� ���� ����� �����
	// �������� ������ ���"��� � ���� ��� ��������� ����, ������� ��
	// ���� ��� �� ����� ���� �������� �������-����� �����, � �����
	// �������� �������
	// ��"� -- �� ���� �������� ��������, � ����� ���� ���������� ������
	// ��������������
	Name = new char [strlen(Value)+1+strlen(StreamName)+1];
	strcpy(Name,StreamName);
	Name[strlen(StreamName)]='_';
	strcpy(Name+strlen(StreamName)+1,Value);

	// ����� ����� �� ����
	if (!GetNextToken(strin,Token))
		return false;
	// ����� ����:
	if (strstr(Token,"TForm"))
	{
		Type = IDK_DIALOG;
	}
	else if (strstr(Token,"TLabel"))
	{
		Type = IDK_LABEL;
	}
	else if (strstr(Token,"TButton"))
	{
		Type = IDK_BUTTON;
	}
	else if (strstr(Token,"TEdit"))
	{
		Type = IDK_EDIT;
	}
	else if (strstr(Token,"TCheckBox"))
	{
		Type = IDK_CHECK;
	}
	else if (strstr(Token,"TTabControl"))
	{
		Type = IDK_TABCONTROL;
	}
	else if (strstr(Token,"TPanel"))
	{
		// ������ ���� ����� ��� ��� � ��������� �� �� ��������
		if (up->Type==IDK_TABCONTROL)
			Type = IDK_TAB;
		else
			Type = IDK_TABLE;
	}
	else if (strstr(Token,"TImage"))
	{
		Type = IDK_BMP;
	}
	else
	{
		cerr << "Cannot parse " << Token << endl;
		// Do it more correctly later!
		return false;
	}

	// ��������� ���� ������ � �������� ������ ���� property, value
	// �� �������� ���� ���� �����.
	while (in.getline(Line,256))
	{
		istringstream strin(Line); // ����� ����������� ���� �� �����
		if (!GetNextToken(strin,Token))
			return false;

		// ���� ������� end, �� ���� ��������� ����, ��������, ���� object ���������
		if (strcmp(Token,"end")==0)
			break;

		// �������, ���� ������� object, �� ����������� �� ������ ���� ��"��� --
		// ���������, ���� ����� ���������� ����� ����� �����
		if (strcmp(Token,"object")==0)
		{
			CParsedObject *temp = new CParsedObject;
			down = temp;
			temp->up = this;
			// ����� Line ��������� ����������������, ���� ��� � ����� ����� ���������
			// ������� ParseStream()
			if (!temp->ParseStream(in,Line,StreamName))
				return false;
			break;
		}

		// ��� ��� ����� ����� ������ �� �� �������
		if (!GetNextToken(strin,Value))
			return false; // =
		if (!GetNextToken(strin,Value))
			return false; //

		// ���� ������������ ���������
		if (strcmp(Token,"Left")==0)
		{
			Left = atoi(Value);
			continue;
		}

		if (strcmp(Token,"Top")==0)
		{
			Top = atoi(Value);
			continue;
		}

		if (strcmp(Token,"Width")==0)
		{
			Width = atoi(Value);
			continue;
		}

		if (strcmp(Token,"Height")==0)
		{
			Height = atoi(Value);
			continue;
		}

		// Caption i Text ����������� � ���� � �� ���� �����, ��� ��
		// ���� ������� � ��������� ��� ��� ����� � �������� ����
		if (strcmp(Token,"Caption")==0 || strcmp(Token,"Text")==0)
		{
			Caption = new char[strlen(Value)+1];
			strcpy(Caption,Value);
			continue;
		}

		// Hint -- �� ��������� ����������, ��� ���� ���� ���������� ���
		// ������ �����������. �������: ��� ������� ���� ����������
		// �������� ���������� -- ������� �������� �� ����� ����� �����
		if (strcmp(Token,"Hint")==0)
		{
			Tip = new char[strlen(Value)+1];
			strcpy(Tip,Value);
			continue;
		}

		// ���� ����������� �������
		if (strcmp(Token,"Enabled")==0)
		{
			if (strcmp(Value,"False")==0)
				IsEnabled = false;
			else
				IsEnabled = true;
			continue;
		}

		if (strcmp(Token,"Checked")==0)
		{
			if (strcmp(Value,"False")==0)
				IsChecked = false;
			else
				IsChecked = true;
			continue;
		}

		if (strcmp(Token,"Visible")==0)
		{
			if (strcmp(Value,"False")==0)
				IsVisible = false;
			else
				IsVisible = true;
			continue;
		}

		// ����� �������� ���������� � ���, �� �� ���������������, ��
		// � ��� � ������� ����"���� �� ��������� �������� �����
		if (strcmp(Token,"Tabs.Strings")==0) // ������� �������������� �����
		{
			while (Line[strlen(Line)-1]!=')')
				in.getline(Line,256);
		}

		// ������� ������
		if (strcmp(Token,"Picture.Data")==0)
		{
			char c;
			for (int i=0;i<44;i++)
				in >> c;
			int addr=GetNextInt(in);
			Height=GetNextInt(in); // ����� ���������� ����� ������
			Width=GetNextInt(in);
			Height=GetNextInt(in);

			for (int i=0;i<addr-26;i++)
				in >> c >> c;
// ������� ������
			int w=Width>>3; // ʳ������ ����� �����
			if (Width&7) w++; // ������ �����

			// ��������� ����������� �� 4
			int skip=0;
			if (w&3)
				skip = (4-w&3)&3;

			Caption = new char [Height*w];
			for (int y=0;y<Height;y++)
			{
				for (int x=0;x<w;x++)
				{
					// ������� ����� ����� ���� ��������� ��� �������, ��
					// ��� ����������� ������������ ������ 255 -- �� ����,
					// � ��� LCD -- �� ������ ����
					char c=GetNextChar(in);
					Caption[x+(Height-1-y)*w]=~c;
				}
				for (int i=0;i<skip;i++)
					GetNextChar(in);
			}
			while (Line[strlen(Line)-1]!='}')
				in.getline(Line,256);
		}
	}

	// �������, � ����� ��� � �� ����...
	if (in.getline(Line,256))
	{
		istringstream strin(Line);
		if (!GetNextToken(strin,Token))
			return false;
		// ����� ������
		if (strcmp(Token,"end")==0)
		{
			return true;
		}
		// ������� � ����� �������� �� ����� ���
		if (strcmp(Token,"object")==0)
		{
			CParsedObject *temp = new CParsedObject;
			next = temp;
			temp->up = this->up;
			if (!temp->ParseStream(in, Line,StreamName))
				return false;
		}
	}
	// �������� �����!
	return true;
}

CParsedObject::~CParsedObject()
{
	delete [] Name;
	delete [] Caption;
	delete [] Tip;
	delete [] IDstring;

	delete down;
	delete next;
}

void CParsedObject::Out()
{
	cout << Name << " ";
	if (next)
		next->Out();
	if (down)
	{
		cout << endl;
		down->Out();
	}
}

bool CParsedObject::Check()
{
	// ϳ������� ����� �������� ������
	if (Caption==0)
	{
		Caption = new char [1];
		Caption[0]=0;
	}
	if (Tip==0)
	{
		Tip = new char [1];
		Tip[0]=0;
	}

	// ����������� ������
	if (Type == IDK_DIALOG)
	{
		Left = (320-Width)/2;
		Top = (240-Height)/2;
	}
	else
	{
		// ����������� ��������� � �������� � ��������
		Left += up->Left;
		Top += up->Top;
	}

	// ��������� ����������� ��� ������������ � ���������
	if ((Left % 4) != 0)
		cout << "Warning: quaternion misalignment. "
			<< "(\"" << Name << "\"): Left == " << Left << endl;
	if ((Width & 3) != 0)
		cout << "Warning: quaternion misalignment. "
			<< "(\"" << Name << "\"): Width == " << Width << endl;

	// ������ �� ����������
	if (Type==IDK_BMP)
	{
		IsEnabled=false;
		//IsVisible=true;
	}

	// ������� ������� �������� �������
	// �� ������� ������� �� ����� ������ ���� ���� IDK_LABEL, � �
	// �������� ���� �� ���� IDK_TEXT �� IDK_EDIT
	if (Type==IDK_TABLE)
	{
		istringstream in(Tip);
		in >> Cols >> Rows;
		CParsedObject *p=down; // ����� ������
		for (int i=0;i<Cols;i++)
		{
			p->Type = IDK_LABEL;
			p->IsEnabled=false;
			p=p->next;
		}
		p=down;
		for (int j=0;j<Rows;j++)
		{
			p->Type=IDK_LABEL;
			p->IsEnabled=false;
			p=p->next;
			for (int i=1;i<Cols;i++)
			{
				p->Width-=4;
				p->Height-=2;
				p=p->next;
			}
		}

	}

	if (next)
		next->Check();
	if (down)
		down->Check();
	return true;
}

void CParsedObject::GetID(int &ID)
{
	IDstring = new char[strlen(Name)+4];
	strcpy(IDstring,"ID_");
	this->ID = ID++;
	strcpy(IDstring+3,Name);
	for (unsigned int i=0;i<strlen(IDstring);i++)
		if (isalpha(IDstring[i]))
			IDstring[i]=toupper(IDstring[i]);
	if (next)
		next->GetID(ID);
	else
		++ID;
	if (down)
		down->GetID(ID);
}

void CParsedObject::CalculateDepth(int &Count)
{
	int count=0;
	CParsedObject *p=this;
	while (p)
	{
		if (p->Type==IDK_DIALOG || p->Type==IDK_TABCONTROL
			|| p->Type==IDK_TABLE || p->Type==IDK_TAB)
		{
			count++;
		}
		p=p->next;
	}
	if (Count<count)
		Count=count;
	if (next)
		next->CalculateDepth(Count);
	if (down)
		down->CalculateDepth(Count);
}

int CParsedObject::CalculateObjectsOnTheLevel()
{
	if (next)
		return next->CalculateObjectsOnTheLevel()+1;
	return 1;
}

void CParsedObject::ReportID(ostream &out)
{
	out << "#define " << IDstring << "\t" << ID << "\n";
	if (next)
		next->ReportID(out);
	if (down)
		down->ReportID(out);
}

void CParsedObject::ReportTexts(ostream &out)
{
	if (Type==IDK_EDIT || Type==IDK_LABEL && IsEnabled)
	{
		out << "unsigned char " << "str" << Name << "[] = \"";
		int len=strlen(Caption);
		int len_max = Type==IDK_EDIT ? ((Width - 4) / 8) : Width/8;
		this->CharactersReserved = Type==IDK_EDIT ? len_max-len : len_max-len+1;
		if (len_max>=len)
		{
			out << Caption << "\\0";
			for (int i=0;i<CharactersReserved-1;i++)
				out << ' ';
			out << "\";\n";
		}
		else
		{
			cout << "***Warning: string " << Caption << " is longer than the width of " << Name << ".\n";
			cout << "It will be truncated to normal length\n";
			CharactersReserved = 0;
			Caption[len]=0;
			out << Caption << "\\0";
			out << "\";\n";
		}
	}
	if (next)
		next->ReportTexts(out);
	if (down)
		down->ReportTexts(out);
}

void CParsedObject::ReportBmps(ostream &out)
{
	if (Type==IDK_BMP)
	{
		out << "const unsigned char " << "bmp" << Name << "[] = {\n";
		int w=Width>>3; // ʳ������ ����� �����
		if (w&7) w++; // ������ �����

		out << oct;
		out << "\'\\";
		out << (unsigned int)(Caption[0] & 0xff);
		out << "\'";

		for (int i=1;i<w*Height;i++)
		{
			out << ", ";
			if ((i&15)==15)
				out << "\n";
			out << "\'\\";
			out << (unsigned int)(Caption[i] & 0xff);
			out << "\'";
		}

		out << dec;
		out << "};\n";
	}
	if (next)
		next->ReportBmps(out);
	if (down)
		down->ReportBmps(out);
}

void CParsedObject::ReportAdditional(ostream &out)
{
	out << "struct CAdditionalData " << Name << "Additional\t= {";
	if (this->IsEnabled)
		out << "1";
	else
		out << "0";
	out << ", ";
	if (this->IsVisible)
		out << "1";
	else
		out << "0";
	out << ", ";
	if (this->Type==IDK_LABEL && this->Height>=16)
		out << "1";
	else
		out << "0";

	out << "};\n";

	if (next)
		next->ReportAdditional(out);
	if (down)
		down->ReportAdditional(out);
}

void CParsedObject::ReportSpecific(ostream &out)
{
	switch (Type)
	{
		case IDK_TABCONTROL:
			out << "struct CTabControlItem TabControl" << Name << " = { ConstDeskTop + "
				<< down->IDstring << ", ConstDeskTop + " << down->IDstring << " };\n";
			break;
		case IDK_EDIT:
			out << "struct CEditBoxItem EditControl" << Name << " = { "
				<< "str" << Name << ", " << (int)CharactersReserved << " };\n";
			break;
		case IDK_LABEL:
			if (IsEnabled)
				out << "struct CEditBoxItem Label" << Name << " = { "
					<< "str" << Name << ", " << (int)CharactersReserved << " };\n";
			break;
		case IDK_CHECK:
			out << "struct CCheckBoxItem CheckBox" << Name << " = { "
				<< (IsChecked ? 1 : 0) << " };\n";
			break;
		case IDK_BMP:
			out << "struct CBmpItem BmpItem" << Name << " = { "
				<< Left << ", " << Top << "};\n";
			break;
		case IDK_TABLE:
			out << "struct CTableCtrl TableCtrl" << Name << " = {"
				<< Cols << ", " << Rows << "};\n";
		case IDK_DIALOG:
			out << "struct CDialogCtrl DialogCtrl" << Name << " = { "
				<< "NULL };\n";
			break;
	}

	if (next)
		next->ReportSpecific(out);
	if (down)
		down->ReportSpecific(out);
}

void CParsedObject::ReportMainStructure(ostream &out)
{
	switch (Type)
	{
	case IDK_DIALOG:
                out << ",\n\n"
			<< "\t{IDK_DIALOG, " << IDstring << ",\n"
			<< "\t" << Left << ", " << Top << ", " << Width << ", " << Height << ",\n"
			<< "\t\"" << Caption << "\", \"" << Tip << "\",\n"
			<< "\tConstDeskTop + " << down->IDstring << ", // Contained\n"
			<< "\tNULL, // This is a top-level GUI Item\n"
			<< "\t&" << Name << "Additional, // Additional data\n"
			<< "\t(void *)&DialogCtrl" << Name << "\n"
			<< "\t}";
		break;
	case IDK_TABCONTROL:
		out << ",\n\n"
			<< "\t{IDK_TABCONTROL, " << IDstring << ",\n"
			<< "\t" << Left << ", " << Top << ", " << Width << ", " << Height << ",\n"
			<< "\t\"" << Caption << "\", \"" << Tip << "\",\n"
			<< "\tConstDeskTop + " << down->IDstring << ", // Contained\n"
			<< "\tConstDeskTop + " << up->IDstring << ", // Owner\n"
			<< "\t&" << Name << "Additional, // Additional data\n"
			<< "\t(void *)&" << "TabControl" << Name << " // Specific data\n"
			<< "\t}";
		break;
	case IDK_TAB:
		out << ",\n\n"
			<< "\t{IDK_TAB, " << IDstring << ",\n"
			<< "\t" << Left << ", " << Top << ", " << Width << ", " << Height << ",\n"
			<< "\t\"" << Caption << "\", \"" << Tip << "\",\n"
			<< "\tConstDeskTop + " << down->IDstring << ", // Contained\n"
			<< "\tConstDeskTop + " << up->IDstring << ", // Owner\n"
			<< "\t&" << Name << "Additional, // Additional data\n"
			<< "\tNULL // Specific data\n"
			<< "\t}";
		break;
	case IDK_LABEL:
		if (!IsEnabled)
			out << ",\n\n"
				<< "\t{IDK_LABEL, " << IDstring << ",\n"
				<< "\t" << Left << ", " << Top << ", " << Width << ", " << Height << ",\n"
				<< "\t\"" << Caption << "\", \"" << Tip << "\",\n"
				<< "\tNULL, // Common Items cannot contain\n"
				<< "\tConstDeskTop + " << up->IDstring << ", // Owner\n"
				<< "\t&" << Name << "Additional, // Additional data\n"
				<< "\tNULL // No specific data for a Label\n"
				<< "\t}";
		else
			out << ",\n\n"
				<< "\t{IDK_LABEL, " << IDstring << ",\n"
				<< "\t" << Left << ", " << Top << ", " << Width << ", " << Height << ",\n"
				<< "\t\"\", \"" << Tip << "\", // There's no caption for such a label\n"
				<< "\tNULL, // This is not a container\n"
				<< "\tConstDeskTop + " << up->IDstring << ", // Owner\n"
				<< "\t&" << Name << "Additional, // Additional data\n"
				<< "\t(void *)&" << "Label" << Name << " // Specific data\n"
				<< "\t}";
		break;
	case IDK_EDIT:
		out << ",\n\n"
			<< "\t{IDK_EDIT, " << IDstring << ",\n"
			<< "\t" << Left << ", " << Top << ", " << Width << ", " << Height << ",\n"
			<< "\t\"\", \"" << Tip << "\", // There's no caption for EditControl\n"
			<< "\tNULL, // This is not a container\n"
			<< "\tConstDeskTop + " << up->IDstring << ", // Owner\n"
			<< "\t&" << Name << "Additional, // Additional data\n"
			<< "\t(void *)&" << "EditControl" << Name << " // Specific data\n"
			<< "\t}";
		break;
	case IDK_BUTTON:
		out << ",\n\n"
			<< "\t{IDK_BUTTON, " << IDstring << ",\n"
			<< "\t" << Left << ", " << Top << ", " << Width << ", " << Height << ",\n"
			<< "\t\"" << Caption << "\", \"" << Tip << "\",\n"
			<< "\tNULL, // A Button cannot contain\n"
			<< "\tConstDeskTop + " << up->IDstring << ", // Owner\n"
			<< "\t&" << Name << "Additional, // Additional data\n"
			<< "\tNULL // No specific data for a Button\n"
			<< "\t}";
		break;
	case IDK_CHECK:
		out << ",\n\n"
			<< "\t{IDK_CHECK, " << IDstring << ",\n"
			<< "\t" << Left << ", " << Top << ", " << Width << ", " << Height << ",\n"
			<< "\t\"" << Caption << "\", \"" << Tip << "\",\n"
			<< "\tNULL, // This is not a container\n"
			<< "\tConstDeskTop + " << up->IDstring << ", // Owner\n"
			<< "\t&" << Name << "Additional, // Additional data\n"
			<< "\t(void *)&" << "CheckBox" << Name << " // Specific data for a CheckBox\n"
			<< "\t}";
		break;
	case IDK_BMP:
		out << ",\n\n"
			<< "\t{IDK_BMP, " << IDstring << ",\n"
			<< "\t" << Left << ", " << Top << ", " << Width << ", " << Height << ",\n"
			<< "\tbmp" << Name << ", \"" << Tip << "\",\n"
			<< "\tNULL, // A Bitmap cannot contain\n"
			<< "\tConstDeskTop + " << up->IDstring << ", // Owner\n"
			<< "\t&" << Name << "Additional, // Additional data\n"
			<< "\t(void *)&" << "BmpItem" << Name << " // Specific data for a BmpItem\n"
			<< "\t}";
		break;
	case IDK_TABLE:
		out << ",\n\n"
			<< "\t{IDK_TABLE, " << IDstring << ",\n"
			<< "\t" << Left << ", " << Top << ", " << Width << ", " << Height << ",\n"
			<< "\t\"" << Caption << "\", \"\",\n"
			<< "\tConstDeskTop + " << down->IDstring << ", // Contained\n"
			<< "\tConstDeskTop + " << up->IDstring << ", // Owner\n"
			<< "\t&" << Name << "Additional, // Additional data\n"
			<< "\t(void *)&" << "TableCtrl" << Name << " // Specific data\n"
			<< "\t}";
		break;
	}

	if (next)
		next->ReportMainStructure(out);
	else
	{
		out << ",\n\n" << strCork;
	}
	if (down)
		down->ReportMainStructure(out);
}

void CParsedObject::ReportCases(ostream &out)
{
	out << "\tcase " << IDstring << ":\n"
		<< "\t\tbreak;\n";
	if (next)
		next->ReportCases(out);
	if (down)
		down->ReportCases(out);
}

void Merge(CParsedObject *&First, CParsedObject *&Second)
{
	if (First->Type == IDK_EMPTY)
	{
		CParsedObject *temp=First;
		First = Second;
		temp->next = 0;
		temp->down = 0;
		Second=temp;
		return;
	}
	CParsedObject *Temp = First;

	while (Temp->next)
	{
		Temp=Temp->next;
	}
	Temp->next = Second;
	Second->up=Temp->up;
	Second = 0;
}

int main(int argc, char **argv)
{
	if (argc<2)
	{
		cout << "Too few parameters!\n"
			<< "Usage:\n"
			<< "dfmc <file_1> <file_2> ...\n"
			<< " where <file_i> is Borland Delphi/Builder .dfm file\n\n";
		return -1;
	}

	CParsedObject *A=new CParsedObject,*B;

	for (int i=1;i<argc;i++)
	{
		B=new CParsedObject;
		ifstream in(argv[i]);
		if (in.fail())
		{
			cerr << "Warning! No such file. Skipping... " << endl;
			return -1;
		}

		*strchr(argv[i],'.')=0;
		if (!B->ParseStream(in,"",argv[i]))
		{
			cerr << "Error through parsing. Exiting..." << endl;
			delete B;
			delete A;
			return -1;
		}
		Merge(A,B);
		delete B;
	}

	A->Check(); // Some checking

	// ID's calculation
	int ID_ = 1;
	A->GetID(ID_);

	///////////////////////////////////////////////////////////////////
	// iface.h
	///////////////////////////////////////////////////////////////////
	ofstream out("iface.h");
	out << "// This is program-generated source. Do no changes in it!\n";
	out << "// Sakhnik, 2003\n\n";
	out << "#ifndef _INTERFACE\n"
		<< "#define _INTERFACE\n\n";
	out << "#define TOTAL_GUI_ITEMS_COUNT " << ID_+1 << "\n";
	int count=0;
	A->CalculateDepth(count);
	out << "#define DRAW_QUEUE_MAX " << count+2 << "\n";
	out << "#define DIALOG_STACK_MAX " << A->CalculateObjectsOnTheLevel()+1 << "\n\n";
	out << "// You should pick up this parameter experimentally, so that the repainting\n"
		<< "// was the most efficient\n";
	out << "#define MAX_ITEMS_TO_DRAW 25\n\n";

	out << "enum GUIItemKind\n"
		<< "{\n"
		<< "\tIDK_NOTHING,\n"
		<< "\tIDK_BEGIN,\n"
		<< "\tIDK_END,\n\n"
		<< "\tIDK_DIALOG,\n\n"
		<< "\tIDK_TABCONTROL,\n"
		<< "\tIDK_TAB,\n"
		<< "\tIDK_LABEL,\n"
		<< "\tIDK_EDIT,\n"
		<< "\tIDK_BUTTON,\n"
		<< "\tIDK_CHECK,\n"
		<< "\tIDK_BMP,\n"
		<< "\tIDK_TABLE\n"
		<< "};\n\n";

	out << "// GUI Item's IDs\n";
	out << "#define ID_NULL 0\n\n";

	A->ReportID(out);

	out << "\n\n";
	out << "struct CAdditionalData\n"
		<< "{\n"
		<< "\tchar IsEnabled:1;\n"
		<< "\tchar IsVisible:1;\n"
		<< "\tchar IsExtra:1;\n"
		<< "};\n\n"
		<< "struct CGUIConstItem\n"
		<< "{\n"
		<< "\tenum GUIItemKind Kind; // Type of Item or Container\n"
		<< "\tint ID; //This field is used in user part for identification\n"
		<< "\tconst unsigned int Left, Top, Width, Height; // Geometrical parameters in screen coordinates.\n"
		<< "\t\t\t\t\t// Best works if Left and Width are divisible by 4\n"
		<< "\tconst unsigned char *Caption;\n"
		<< "\tconst unsigned char *Tip;\n"
		<< "\tconst struct CGUIConstItem *Contained; // First Item in this Container\n"
		<< "\tconst struct CGUIConstItem *Owner; // The container of the Item or of this Container\n"
		<< "\tstruct CAdditionalData *Additional; // Other changeable data\n"
		<< "\tvoid *Specific;  // Specific information for every ItemKind, it allocates in the RAM and can be modified during execution\n"
		<< "};\n\n";
	out << "struct CTabControlItem\n"
		<< "{\n"
		<< "\tconst struct CGUIConstItem *First; // First visible Tab\n"
		<< "\tconst struct CGUIConstItem *Active; // Active Tab\n"
		<< "};\n\n";
	out << "struct CEditBoxItem\n"
		<< "{\n"
		<< "\tunsigned char *Text;\n"
		<< "\tchar CharactersReserved;\n"
		<< "};\n\n";
	out << "struct CCheckBoxItem\n"
		<< "{\n"
		<< "\tchar IsChecked;\n"
		<< "};\n\n";
	out << "struct CBmpItem\n"
		<< "{\n"
		<< "\tunsigned int Left,Top;\n"
		<< "};\n\n";
	out << "struct CTableCtrl\n"
		<< "{\n"
		<< "\tunsigned char ColCount,RowCount;\n"
		<< "};\n\n";
	out << "struct CDialogCtrl\n"
		<< "{\n"
		<< "\tconst struct CGUIConstItem *Cursor;\n"
		<< "};\n\n";
	out << "extern const struct CGUIConstItem ConstDeskTop[TOTAL_GUI_ITEMS_COUNT];\n\n";
	out << "#endif\n";
	out << "// End of iface.h\n";

	out.close();
	//////////////////////////////////////////////////////////////
	// Iface.c
	//////////////////////////////////////////////////////////////
	out.open("iface.c");

	out << "// This is program-generated source. Do no changes in it!\n";
	out << "// Sakhnik, 2003\n\n";
	out << "#include <stdlib.h>\n\n"
		<< "#include \"iface.h\"\n\n";
	out << "// Changeable strings for EditControls\n";
	A->ReportTexts(out);
	out << endl;

	out << "// BmpData for BmpItems\n";
	A->ReportBmps(out);
	out << endl;

	out << "// Structures for Additional data of GUI Items\n";
	out << "// Corks must be enabled and visible; this is used by the algorithms\n"
		<< "struct CAdditionalData NullEnabled = {1,1,0};\n";

	A->ReportAdditional(out);

	out << endl;
	out << "// Specific data for some GUI Items\n";

	A->ReportSpecific(out);
	out << endl << endl;

	out << "// General GUI Items data structure\n"
		<< "const struct CGUIConstItem ConstDeskTop[TOTAL_GUI_ITEMS_COUNT] = \n"
		<< "{\n";

	out << strCork;
	A->ReportMainStructure(out);

	out << "\n};\n\n";
	out << "// End of iface.c\n";

	out.close();
	//////////////////////////////////////////////////////////////////
	// action.c
	//////////////////////////////////////////////////////////////////

	out.open("action.c");

	out << "// This is program-generated source file. Add your action-handlers code here\n"
		<< "// Sakhnik, 2003\n\n";
	out << "#include <assert.h>\n\n"
		<< "#include \"iface.h\"\n"
		<< "#include \"action.h\"\n\n";
	out << "void ActionPerformed(const struct CGUIConstItem *Item,\n"
		<< "\t\t\t\tconst struct CEventDescriptor *Event)\n"
		<< "{\n"
		<< "\tunsigned int ItemID = Item -> ID;\n\n"
		<< "\tassert(Item);\n\n"
		<< "\tswitch (ItemID)\n"
		<< "\t{\n";

	A->ReportCases(out);

	out << "\tdefault:; // Probably EVENT_NULL\n";
	out << "\t}\n";
	out << "}\n\n";

	out << "// End of action.c\n";

	cout << "Ok" << endl;
	delete A;
	return 1;
}

