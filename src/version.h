/*�� ����� ������ ���� ����. ������� .pro-���� 
 ������ ���� ������������ version_maker.bat ��� ��������� ������ �������. 
 ��������� ������ ���� 22.06.2017 � 11:03:52,83*/ 
#include <QString> 
class Version 
{ 
protected: 
   static const QString version(){return "1.28";}; 
   QString build_stamp()
   { 
        return QString(__DATE__)+" :: "+QString(__TIME__); 
   }
}; 
