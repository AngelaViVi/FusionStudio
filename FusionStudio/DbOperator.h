#pragma once
//#include "boost/scoped_ptr.hpp"
//#include "caffe/util/db.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
/*
��    ��:��ʹ΢����
����ʱ��:2017��3��22��10:50:39
������ע:�����������װ�����ݿ��йصĲ���
         �������ݿ��ļ�,��ȡ���ݿ��е�ͼƬ
		 ʹ��ʱ,���ļ�·���Ա�������ʼ��
*/
class DbOperator
{
public:
	DbOperator(std::string DbType,std::string filename);//��ʼ��
	~DbOperator();
public:
	///��������,
	//���ݿ����

	//ͼƬ����
	int numOfImage;
	//ͼƬ��
	int length;
	//ͼƬ��
	int width;
	//ͼƬ�Ƿ�Ϊ��ɫ
	bool isColour;
	//��ǰ�α�
	
private:
	///˽�к���
	//��ȡ���ݿ��״̬����,��һ���ж�����ͼƬ,ÿ��ͼƬ�ж��,�Ƿ�Ϊ��ɫ
public:
	///��������
	//��ȡ��ǰͼƬ,�����α����һλ
	bool getImage(cv::Mat & img,int &label);
	//ʹ����ʱ�α�,��ȡ�ض���ĳ��ͼƬ
	//��ȡ��ǩ?
	void heheh();

};

