#include "DbOperator.h"

#include <stdint.h>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <stdexcept>  

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include "boost/scoped_ptr.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/io.hpp"


using namespace caffe;  // NOLINT(build/namespaces)

using std::max;
using std::pair;
using boost::scoped_ptr;

boost::scoped_ptr<db::DB> _db;//db���������ĳ�������ռ��ͻ
boost::scoped_ptr<db::Cursor> cursor;
//��ʼ��
DbOperator::DbOperator(string DbType, string filename)
{
	try
	{
		//Ĭ��ʹ��lmdb
		if (DbType=="leveldb")
		{
			_db.reset(db::GetDB("leveldb"));
			_db->Open(filename, db::READ);//��leveldb���ݿ�
		}
		else {
			_db.reset(db::GetDB("lmdb"));
			_db->Open(filename, db::READ);//��lmdb���ݿ�
		}
		cursor.reset(_db->NewCursor());//���α�
	}
	catch (const std::exception&e)
	{
		std::cout << "������!"<<e.what();
	}
}

DbOperator::~DbOperator()
{

}

//��ȡ��ǰ�α��µ�ͼƬ,�����α����һλ
bool DbOperator::getImage(cv::Mat & img, int &label)
{
	if (cursor->valid())//��ǰ�α���Ч
	{
		Datum datum;
		bool flag = datum.ParseFromString(cursor->value());
		string DarkMagic = cursor->value();//ʹ�ú�ħ��ǿ�ƽ���
		DecodeDatumNative(&datum);
		if (datum.has_data())
		{
			try
			{
				const string& data = datum.data();
				int height = datum.height();
				int width = datum.width();
				label =datum.label();
				cv::Mat cv_img_temp(height, width, CV_8UC1);//����һ���յ�Mat
				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width; j++) {
						char _temp = data.at(i * width + j);
						cv_img_temp.at<char>(i,j)= data.at(i * width + j);
					}
				}
				cv_img_temp.copyTo(img);//���ݽ��
			}
			catch (std::out_of_range &exc)
			{
				std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << "\n";
			}
		}
		else
		{
			//�������뷽���Ѿ��޷�ʹ��,����ʹ�ú�ħ��
			try
			{
				const string& data = datum.data();
				int height = datum.height();
				int width = datum.width();
				label = datum.label();
				cv::Mat cv_img_temp(height, width, CV_8UC1);//����һ���յ�Mat
				for (int i = 0; i < height; i++) {
					for (int j = 0; j < width; j++) {
						cv_img_temp.at<char>(i, j) = DarkMagic.at(i * width + j);
					}
				}
				cv_img_temp.copyTo(img);//���ݽ��
			}
			catch (std::out_of_range &exc)
			{
				std::cerr << exc.what() << " Line:" << __LINE__ << " File:" << __FILE__ << "\n";
			}
			//return false;//��ǰ�α�ָ���˿�λ��
		}
		cursor->Next();
		return true;
	}
	else
	{
		return false;//��ǰ�α���Ч
	}
}


