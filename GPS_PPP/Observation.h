#pragma once
#include <fstream>
#include "MathBase.h"
#include "Epochs.h"
using namespace std;

class Hu_node
{
public:
	double RMS;
	double b_delta_avg;
	char *id;
	time_gpst time_g;
	Hu_node *next;
};

class Hu
{
public:
	int num;//�û����й��ж�����Ԫ
	Hu_node *datas;
	Hu *next;//��һ�λ�
};

class HuZong
{
public:
	int num_hu;//�ܹ��ж��ٻ���
	Hu *datas;
};

class Observation
{
public:
	Observation(ifstream &indata);
	Observation();
	void Zhoutiao();
	~Observation();
public:
	double x_approx,y_approx,z_approx;//14
	double antenna_h;//14
	double interval;//10
	Epochs_ob data;
	HuZong data_n;
};

