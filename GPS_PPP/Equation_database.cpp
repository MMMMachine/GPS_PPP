#include "StdAfx.h"
#include "Equation_database.h"
#include "Observation.h"
#include "SP3.h"
#include "CLK.h"
#include "MathBase.h"
#include "Epochs.h"
#include "ErrorCrct.h"
#include "SatPosition.h"
#include <iostream>
#include <iomanip>
using namespace std;

Equation_database::Equation_database(Observation obsev,SP3 sp3,SP3 sp3_1,SP3 sp3_3,CLK clk2,CLK clk1,CLK clk3)
{
	O2=obsev;
	Sp3=sp3;
	Sp3_1=sp3_1;
	Sp3_3=sp3_3;
	clk=clk.CLK_add(clk1,clk2,clk3);
	number_of_equations=0;
	D.num_epoch=0;
	int number_ob=obsev.data.Epoch_Number;
	Epochs_ob_node *current_ob=obsev.data.e_first;
	Equation_epoch *temp_ep;
	Equation_epoch *current_ep;
	D.datas=NULL;
	for (int i=0;i<number_ob;i++)//����o�ļ�����ɸѡ������Ϣ
	{
		int flag=current_ob->flag;
		if (flag>1)
		{
		} 
		else if (flag==1)
		{
		}
		else//flag����0ʱ����
		{
			temp_ep=new Equation_epoch;
			temp_ep->num=0;
			Equation_note *current_nt=temp_ep->data;
			Equation_note *temp_nt;
			for (int j=0;j<current_ob->number;j++)//���ζ�ȡ����Ԫ�е�ÿ������,j��Ӧ���ǵı��
			{
				temp_nt=new Equation_note;//��ʼ������㣡��������������������������������������������������������������
				if (current_ob->data[j]->P1 != 0.0 && current_ob->data[j]->P2 != 0.0 && current_ob->data[j]->L1 != 0.0 && current_ob->data[j]->L2 != 0.0)//���P1�����0��������������
				{
					//temp_ep->data=new Equation_note;
					temp_nt->x_approx=obsev.x_approx;//APPROX POSITION XYZ
					temp_nt->y_approx=obsev.y_approx;
					temp_nt->z_approx=obsev.z_approx;
					
					temp_nt->time_ob=current_ob->time_g;//GPST
					temp_nt->L1=current_ob->data[j]->L1;
					temp_nt->L2=current_ob->data[j]->L2;
					temp_nt->P1=current_ob->data[j]->P1;
					temp_nt->P2=current_ob->data[j]->P2;

					temp_nt->atmos=0;//-------------------------------------------------��Ҫ����ģ��,�������ӳ�
					temp_nt->dt=0;//----------------------------------------------------δ֪��
					temp_nt->dT=Insert_dT(current_ob->time_g,current_ob->Sat_ID[j]);//��Ҫ�ڲ� ����������һ��
					temp_nt->X_r=temp_nt->x_approx;//------------------------------δ֪������Ҫ��������
					temp_nt->Y_r=temp_nt->y_approx;//------------------------------δ֪������Ҫ��������
					temp_nt->Z_r=temp_nt->z_approx;//------------------------------δ֪������Ҫ��������

					SatPosition test1=Insert_Lagrange(current_ob->time_g,current_ob->Sat_ID[j]);//����ڲ�õ���xyz����Ϊ0����ʱ�䲻�������Ϊ1����û�ҵ���Ӧ������
					temp_nt->X_s=test1.X;
					temp_nt->Y_s=test1.Y;
					temp_nt->Z_s=test1.Z;
					temp_nt->X_v=test1.X_Vel/604800.0;
					temp_nt->Y_v=test1.Y_Vel/604800.0;
					temp_nt->Z_v=test1.Z_Vel/604800.0;
					//cout<<setprecision(20)<<test1.X<<"    "<<test1.Y<<"    "<<test1.Z<<"    "<<test1.X_Vel<<"    "<<test1.Y_Vel<<"    "<<test1.Z_Vel<<endl;//������
					
					double Elev,Azi;
					double b_r,l_r,h_r;
					ITRF2GEO(temp_nt->X_r,temp_nt->Y_r,temp_nt->Z_r,l_r,b_r,h_r);
					Get_E_A(temp_nt->X_s,temp_nt->Y_s,temp_nt->Z_s,temp_nt->X_r,temp_nt->Y_r,temp_nt->Z_r,b_r,l_r,h_r,Elev,Azi);
					Elev=Elev*180/3.1415926535897932384626433;//�߶Ƚ�
					time_common startd;//��ʼ���������
					double cur_doy;
					double sta_doy;
					double doy;
					startd.year_cm=current_ob->time_c.year_cm;
					startd.month_cm=1;
					startd.day_cm=1;
					startd.hour_cm=current_ob->time_c.hour_cm;
					startd.minte_cm=current_ob->time_c.minte_cm;
					startd.second_cm=current_ob->time_c.second_cm;
					cur_doy=Convert_Cm2Jd(current_ob->time_c);
					sta_doy=Convert_Cm2Jd(startd);
					doy=cur_doy-sta_doy+1;//����õ������
					
					double trop_M_w,trop_tzd_w,trop_M_d,trop_tzd_d;
					temp_nt->atmos=Trop(1,1,temp_nt->X_s,temp_nt->Y_s,temp_nt->Z_s,temp_nt->X_r,temp_nt->Y_r,temp_nt->Z_r,doy,trop_M_w,trop_tzd_w,trop_M_d,trop_tzd_d);
					temp_nt->Trop_M_d=trop_M_d;
					temp_nt->Trop_M_w=trop_M_w;
					temp_nt->Trop_tzd_d=trop_tzd_d;
					temp_nt->Trop_tzd_w=trop_tzd_w;


					if (test1.X != 0 && test1.X != 1)//���Ϊ0��1��ֱ���޳����ݣ�������¼
					{
						if (Elev > 15)//�������Ǹ߶Ƚ�
						{
							if (temp_ep->num==0)
							{
								temp_ep->data=temp_nt;
								current_nt=temp_ep->data;
							} 
							else
							{
								current_nt->next=temp_nt;
								current_nt=current_nt->next;
							}
							number_of_equations++;
							temp_ep->num++;
						}
					}
				}
			}
		
			//cout<<temp_ep->num<<endl;
			if (temp_ep->num > 5)
			{
				if (D.datas==NULL)
				{
					D.datas=temp_ep;
					current_ep=D.datas;
					D.num_epoch++;
				} 
				else
				{
					current_ep->next=temp_ep;
					current_ep=current_ep->next;
					D.num_epoch++;
				}
			}
		}
	
		current_ob=current_ob->next;
	}
//	cout<<number_of_equations<<endl;//�������ݣ���ʾ�ܹ��ж��ٿźϸ����ǲ��뵽������������С�
}

Equation_database::~Equation_database()
{
}

double Equation_database::Insert_dT(time_gpst tg,char *sat_id)
{
	double re;
	Clk_node *current_nd=clk.data;
	while (tg.week_gps > current_nd->gps_data->time_g.week_gps
		|| (tg.week_gps == current_nd->gps_data->time_g.week_gps && tg.fract > current_nd->gps_data->time_g.fract )
		|| (tg.week_gps == current_nd->gps_data->time_g.week_gps && tg.fract == current_nd->gps_data->time_g.fract ))
	{
		current_nd=current_nd->next;
	}
	Clk_node_nd *current_ndnd=current_nd->gps_data;
	int num_s=1;
	while (1)
	{
		if ((sat_id[0] == current_ndnd->id[0] )&&(sat_id[1] == current_ndnd->id[1] )&&(sat_id[2] == current_ndnd->id[2] ))
			break;
		current_ndnd=current_ndnd->next;
		num_s++;
		if (num_s>current_nd->num_s)
		{
			return 0.0;
		}
	}
	
	if ((tg.week_gps == current_nd->gps_data->time_g.week_gps) && (tg.fract == current_nd->gps_data->time_g.fract))
	{
		re=current_ndnd->dT;
		return re;
	} 
	else//������ʼ��ֵ������������
	{
		double t[17];
		double dT[17];

		t[8]=current_ndnd->time_g.week_gps+current_ndnd->time_g.fract;
		dT[8]=current_ndnd->dT;
		Clk_node *copy_nd=current_nd;
		for (int i=7;i>-1;i--)
		{
			current_nd=current_nd->last;
			current_ndnd=current_nd->gps_data;
			while (1)
			{
				if ((sat_id[0] == current_ndnd->id[0] )&&(sat_id[1] == current_ndnd->id[1] )&&(sat_id[2] == current_ndnd->id[2] ))
					break;
				current_ndnd=current_ndnd->next;
			}
			dT[i]=current_ndnd->dT;
			t[i]=current_ndnd->time_g.week_gps+current_ndnd->time_g.fract;
		}
		current_nd=copy_nd;
		for (int i=9;i<17;i++)
		{
			current_nd=current_nd->next;
			current_ndnd=current_nd->gps_data;
			while (1)
			{
				if ((sat_id[0] == current_ndnd->id[0] )&&(sat_id[1] == current_ndnd->id[1] )&&(sat_id[2] == current_ndnd->id[2] ))
					break;
				current_ndnd=current_ndnd->next;
			}
			dT[i]=current_ndnd->dT;
			t[i]=current_ndnd->time_g.week_gps+current_ndnd->time_g.fract;
		}

		double m_add_t=0;
		double tg_db=tg.week_gps+tg.fract;
		for (int i_add=0;i_add<17;i_add++)
		{
			double m_mul_t=1;
			for (int i_mul=0;i_mul<17;i_mul++)
			{
				if (i_mul!=i_add)
				{
					m_mul_t=m_mul_t*((double)( (tg_db-t[i_mul]) / (t[i_add]-t[i_mul]) ));
				}
			}
			m_add_t=m_add_t+m_mul_t*dT[i_add];
		}
		re=m_add_t;
//		cout<<setprecision(20)<<re<<endl;
		return re;
	}
}

SatPosition Equation_database::Insert_Lagrange(time_gpst tg,char *sat_id)//����O�ļ���Ĺ۲�ʱ�䣬���Ǳ�ţ����ظ����ǵľ�ȷλ��
{
	SatPosition re;
	int mark_i=0;//�ҵ�������������t��ӽ�����Ԫ�ı�ʶ
	while (tg.week_gps > Sp3.epochs[mark_i]->time_gps.week_gps
		|| (tg.week_gps == Sp3.epochs[mark_i]->time_gps.week_gps && tg.fract > Sp3.epochs[mark_i]->time_gps.fract )
		|| (tg.week_gps == Sp3.epochs[mark_i]->time_gps.week_gps && tg.fract == Sp3.epochs[mark_i]->time_gps.fract ))
	{
		mark_i++;
		if ((mark_i+1) > Sp3.Number_of_Epochs)
			break;
	}
	//----------------------------------------------------whileѭ���ҵ�mark_i;
	mark_i--;
	if ((mark_i+1) > Sp3.Number_of_Epochs)//û�ҵ�mark_i����������ֵΪ0�ĵ�
	{
		re.X=0;
		re.Y=0;
		re.Z=0;
		return re;
	} 
	else
	{
		int mark_j=0;
		while ( 1 )
		{
			if ((sat_id[0] == Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->Sat_Id[0] )&&(sat_id[1] == Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->Sat_Id[1] )&&(sat_id[2] == Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->Sat_Id[2] ))
				break;
			mark_j++;
			if ((mark_j+1)>Sp3.Number_of_Sats)
				break;
		}
//		cout<<mark_j<<endl;//---------------------------------------------------------------------������
		if ((mark_j+1)>Sp3.Number_of_Sats)//û�ҵ�mark_j����������ֵΪ1�ĵ�
		{
			re.X=1;
			re.Y=1;
			re.Z=1;
			return re;
		} 
		else//�Ҷ�Ӧ�����Ǳ��
		{
			if ((tg.week_gps == Sp3.epochs[mark_i]->time_gps.week_gps) && (tg.fract == Sp3.epochs[mark_i]->time_gps.fract))
			{
				re.X=Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->X;
				re.Y=Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->Y;
				re.Z=Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->Z;
				re.X_Vel=Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->X_Vel;
				re.Y_Vel=Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->Y_Vel;
				re.Z_Vel=Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->Z_Vel;
				re.clock_each=Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->clock_each;
				re.clock_rate_change=Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->clock_rate_change;
				re.Sat_Accuracy=Sp3.epochs[mark_i]->Sat_XYZ[mark_j]->Sat_Accuracy;
			} 
			else//������ʼ��ֵ������������
			{
				double t[17];
				double x[17];
				double y[17];
				double z[17];
//				double c[17];
				if (mark_i>7 && mark_i< (Sp3.Number_of_Epochs-1-7))//����Ҫsp3_1,sp3_3�ļ�
				{
					for (int i=0;i<17;i++)
					{
						t[i]=Sp3.epochs[mark_i-8+i]->time_gps.fract+Sp3.epochs[mark_i-8+i]->time_gps.week_gps;
						x[i]=Sp3.epochs[mark_i-8+i]->Sat_XYZ[mark_j]->X;
						y[i]=Sp3.epochs[mark_i-8+i]->Sat_XYZ[mark_j]->Y;
						z[i]=Sp3.epochs[mark_i-8+i]->Sat_XYZ[mark_j]->Z;
						//if (i>0)//���еĴ���ֻ�в���weekʱ����
						//{
						//	if (t[i]-t[i-1] < 0 )//�����ǰ��ʱ�����һ��ʱ��С��˵��ʱ�����һ�ܽ�λ�ˣ���С����1��ʹ��ʱ�䱣��������Ҳ������������
						//		t[i]=t[i]+1;
						//}
					}
					//for (int i=0;i<17;i++)//---------------------------------------------------���Ժ���
					//{
					//	cout<<setprecision(20)<<x[i]<<"here"<<endl;
					//}
				}
				if (mark_i<8)//��Ҫsp3_1�ļ�
				{
					mark_i++;
					int sub_i=8-mark_i;
					int suppl_i=17-sub_i;
					
					for (int i=0;i<(suppl_i-1);i++)//�ȶ���Sp3�ļ�
					{
						t[i+sub_i+1]=Sp3.epochs[i]->time_gps.fract+Sp3.epochs[i]->time_gps.week_gps;
						x[i+sub_i+1]=Sp3.epochs[i]->Sat_XYZ[mark_j]->X;
						y[i+sub_i+1]=Sp3.epochs[i]->Sat_XYZ[mark_j]->Y;
						z[i+sub_i+1]=Sp3.epochs[i]->Sat_XYZ[mark_j]->Z;
						//if (i>0)//���еĴ���ֻ�в���weekʱ����
						//{
						//	if (t[i]-t[i-1] < 0 )//�����ǰ��ʱ�����һ��ʱ��С��˵��ʱ�����һ�ܽ�λ�ˣ���С����1��ʹ��ʱ�䱣��������Ҳ������������
						//		t[i]=t[i]+1;
						//}
					}
					for (int i=0;i<(sub_i+1);i++)//�ٶ�ǰһ��sp3�ļ�������
					{
						t[i]=Sp3_1.epochs[i+(Sp3_1.Number_of_Epochs-sub_i)-1]->time_gps.fract+Sp3_1.epochs[i+(Sp3_1.Number_of_Epochs-sub_i)-1]->time_gps.week_gps;
						x[i]=Sp3_1.epochs[i+(Sp3_1.Number_of_Epochs-sub_i)-1]->Sat_XYZ[mark_j]->X;
						y[i]=Sp3_1.epochs[i+(Sp3_1.Number_of_Epochs-sub_i)-1]->Sat_XYZ[mark_j]->Y;
						z[i]=Sp3_1.epochs[i+(Sp3_1.Number_of_Epochs-sub_i)-1]->Sat_XYZ[mark_j]->Z;
						//if (i>0)//���еĴ���ֻ�в���weekʱ����
						//{
						//	if (t[i]-t[i-1] < 0 )//�����ǰ��ʱ�����һ��ʱ��С��˵��ʱ�����һ�ܽ�λ�ˣ���С����1��ʹ��ʱ�䱣��������Ҳ������������
						//		t[i]=t[i]+1;
						//}
					}
					//for (int i=0;i<17;i++)//-----------------------------------------------------------------���Ժ���
					//{
					//	cout<<setprecision(20)<<x[i]<<"here"<<endl;
					//}
					mark_i--;
				}
				if (  mark_i > (Sp3.Number_of_Epochs-1-8) )//��Ҫsp3_3�ļ�
				{
					mark_i++;
					int sub_i=Sp3.Number_of_Epochs-mark_i+9;
					int suppl_i=17-sub_i;

					for (int i=0;i<sub_i;i++)//�ȶ���Sp3�ļ�
					{
						t[i]=Sp3.epochs[i+(mark_i-9)]->time_gps.fract+Sp3.epochs[i+(mark_i-9)]->time_gps.week_gps;
						x[i]=Sp3.epochs[i+(mark_i-9)]->Sat_XYZ[mark_j]->X;
						y[i]=Sp3.epochs[i+(mark_i-9)]->Sat_XYZ[mark_j]->Y;
						z[i]=Sp3.epochs[i+(mark_i-9)]->Sat_XYZ[mark_j]->Z;
						//if (i>0)//���еĴ���ֻ�в���weekʱ����
						//{
						//	if (t[i]-t[i-1] < 0 )//�����ǰ��ʱ�����һ��ʱ��С��˵��ʱ�����һ�ܽ�λ�ˣ���С����1��ʹ��ʱ�䱣��������Ҳ������������
						//		t[i]=t[i]+1;
						//}
					}
					for (int i=0;i<suppl_i;i++)//�ٶ���һ��sp3�ļ�������
					{
						t[i+sub_i]=Sp3_3.epochs[i]->time_gps.fract+Sp3_3.epochs[i]->time_gps.week_gps;
						x[i+sub_i]=Sp3_3.epochs[i]->Sat_XYZ[mark_j]->X;
						y[i+sub_i]=Sp3_3.epochs[i]->Sat_XYZ[mark_j]->Y;
						z[i+sub_i]=Sp3_3.epochs[i]->Sat_XYZ[mark_j]->Z;
						//if (i>0)//���еĴ���ֻ�в���weekʱ����
						//{
						//	if (t[i]-t[i-1] < 0 )//�����ǰ��ʱ�����һ��ʱ��С��˵��ʱ�����һ�ܽ�λ�ˣ���С����1��ʹ��ʱ�䱣��������Ҳ������������
						//		t[i]=t[i]+1;
						//}
					}
					//for (int i=0;i<17;i++)//-----------------------------------------------------------------���Ժ���
					//{
					//	cout<<setprecision(20)<<x[i]<<endl;
					//}
					mark_i--;
				}
				//------------------------------------------------------------����λ��
				double m_add_x=0;
				double m_add_y=0;
				double m_add_z=0;
				double m_add_vx=0;
				double m_add_vy=0;
				double m_add_vz=0;
				double tg_db=tg.week_gps+tg.fract;
				for (int i_add=0;i_add<17;i_add++)
				{
					double m_mul_x=1;
					double m_mul_y=1;
					double m_mul_z=1;
					
					double m_mul_v2x=1;//��ĸ
					double m_mul_v2y=1;//��ĸ
					double m_mul_v2z=1;//��ĸ
					for (int i_mul=0;i_mul<17;i_mul++)
					{
						if (i_mul!=i_add)
						{
							m_mul_x=m_mul_x*((double)( (tg_db-t[i_mul]) / (t[i_add]-t[i_mul]) ));
							m_mul_y=m_mul_y*((double)( (tg_db-t[i_mul]) / (t[i_add]-t[i_mul]) ));
							m_mul_z=m_mul_z*((double)( (tg_db-t[i_mul]) / (t[i_add]-t[i_mul]) ));
							m_mul_v2x=m_mul_v2x*(t[i_add]-t[i_mul]);
							m_mul_v2y=m_mul_v2y*(t[i_add]-t[i_mul]);
							m_mul_v2z=m_mul_v2z*(t[i_add]-t[i_mul]);
						}

					}
					double add_vx=0;
					double add_vy=0;
					double add_vz=0;
					for (int i_add_v=0;i_add_v<17;i_add_v++)
					{
						double m_mul_v1x=1;//����
						double m_mul_v1y=1;//����
						double m_mul_v1z=1;//����
						if (i_add_v!=i_add)
						{
							for (int i_mull=0;i_mull<17;i_mull++)
							{
								if (i_mull!=i_add && i_mull!=i_add_v)
								{
									m_mul_v1x=m_mul_v1x*(tg_db-t[i_mull]);
									m_mul_v1y=m_mul_v1y*(tg_db-t[i_mull]);
									m_mul_v1z=m_mul_v1z*(tg_db-t[i_mull]);
								}
							}
							add_vx=add_vx+m_mul_v1x;
							add_vy=add_vy+m_mul_v1y;
							add_vz=add_vz+m_mul_v1z;
						}
					}
					m_add_x=m_add_x+m_mul_x*x[i_add];
					m_add_y=m_add_y+m_mul_y*y[i_add];
					m_add_z=m_add_z+m_mul_z*z[i_add];
					m_add_vx=m_add_vx+(double)(add_vx/m_mul_v2x)*x[i_add];
					m_add_vy=m_add_vy+(double)(add_vy/m_mul_v2y)*y[i_add];
					m_add_vz=m_add_vz+(double)(add_vz/m_mul_v2z)*z[i_add];
				}
				re.X=m_add_x;
				re.Y=m_add_y;
				re.Z=m_add_z;
				re.X_Vel=m_add_vx;
				re.Y_Vel=m_add_vy;
				re.Z_Vel=m_add_vz;
				//--------------------------------------------------------------�����ٶ�


			}
		}
	}
//	cout<<mark_i<<endl;//---------------------------------------------------------------------------------������

	return re;
}