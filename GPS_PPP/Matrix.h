#pragma once

class Matrix
{
public:
	Matrix();
	Matrix(int n,int m);
	Matrix(int n, int m,double d);
	Matrix(int n, int m,double d[]);
	~Matrix();
	Matrix & operator +(Matrix &a);
	Matrix & operator -(Matrix &a);
	Matrix & operator *(Matrix &a);
	Matrix & operator *(double d);
	Matrix & Transpose();
	Matrix & Inverse();
	Matrix & Inverse_P();//�Խ�������
	Matrix & operator= (const Matrix &a);
	void input();
	void disp();

	int row;//���������
	int col;//���������
	double **data;
};
