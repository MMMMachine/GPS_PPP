#pragma once


void Saastamoinen(double P,double e,double B,double H,double T,double &D,double &W);
void Niell(double x,double y,double z,double doy,double E,double &D,double &W);//doy����գ�xyz�������γ��,EΪ���Ǹ߶Ƚ�
double Trop(int slt1, int slt2,double x_s,double y_s,double z_s,double x_r,double y_r,double z_r,double doy,double &trop_M_w,double &trop_tzd_w,double &trop_M_d,double &trop_tzd_d);//slt1�����ʪ������slt2����ӳ�亯��,xyzΪ��վ�Ľ�������