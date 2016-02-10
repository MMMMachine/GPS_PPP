#pragma once
#include "Observation.h"
#include "SP3.h"
#include "MathBase.h"
#include "SatPosition.h"
#include "CLK.h"



class Equation_note//һ���۲���Ԫ��ĳ�����ǹ��췽�����õ�����
{
public:
	time_gpst time_ob;//�۲�ʱ��
	double X_r,Y_r,Z_r;//���ջ����꣬����ֵ��δ֪
	double X_s,Y_s,Z_s;//�������꣬�ɾ��������ļ��ڲ�õ�
	double X_v,Y_v,Z_v;//�����ٶȣ��ɾ��������ļ��ڲ�õ�
	double x_approx,y_approx,z_approx;//���ջ��������꣬�������ݣ���o�ļ��õ�
	double L1,L2,P1,P2;//��ʱ�̵Ĺ۲�����
	double dt;//���ջ��Ӳ�
	double dT;//�����Ӳ�
	double atmos;//�����ӳ����
	double Trop_M_w,Trop_M_d;//ӳ�亯��
	double Trop_tzd_w,Trop_tzd_d;//�ӳٷ���

	Equation_note *next;
};

class Equation_epoch//һ����Ԫ��ɷ������õ�����������
{
public:
	Equation_note *data;
	Equation_epoch *next;//�¸���Ԫ������
	int num;//�����Ԫ���ж��ٿ�������
};

class Equation_epochs//һ��o�ļ���Ӧ�����з�����
{
public:
	Equation_epoch *datas;
	int num_epoch;
};

class Equation_database
{
public:
	Equation_database(){};
	Equation_database(Observation obsev,SP3 sp3,SP3 sp3_1,SP3 sp3_3,CLK clk2,CLK clk1,CLK clk3);
	~Equation_database();
	SatPosition Insert_Lagrange(time_gpst t,char *sat_id);
	double Insert_dT(time_gpst t,char *sat_id);
	Equation_epochs D;
	Observation O2;
	SP3 Sp3,Sp3_1,Sp3_3;
	CLK clk;
	int number_of_equations;
};