#pragma once
#include "MathBase.h"
#include <fstream>
using namespace std;

class Clk_node_nd
{
public:
	time_gpst time_g;//ʱ��
	char *id;
	double dT;//��Ӧ���ǵ��Ӳ�
	Clk_node_nd *next;
};
class Clk_node
{
public:

	Clk_node_nd *gps_data;
	int num_s;//�ܹ��ж�������
	Clk_node *next;
	Clk_node *last;
};

class CLK
{
public:
	CLK();
	CLK(ifstream &indata);
	CLK CLK_add(CLK clk1,CLK clk2,CLK clk3);
	~CLK();
public:
	Clk_node *data;
	int num;//�ܹ��ж��ٸ�node
};
