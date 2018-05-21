#pragma once
#include <string>
#include <fstream>  // NOLINT(readability/streams)
#include <QThread>
/*
ʵ������ת����ʵ��ҵ���߼�,��������Qmake��protoc��ת������
*/
class DataConverter:public QThread
{
public:
	DataConverter();
	~DataConverter();
	enum mode {MNIST,CIFAR,IMAGE,DEFAULT};//����ģʽ,DEFAULT�²������ں�
public:
	///�ⲿ����
	bool MnistToDb(std::string image_filename, std::string label_filename, std::string db_path, std::string db_backend);
	bool CifarToDb(std::string& input_folder,std::string& output_folder,std::string& db_type);
	bool ImagesToDb(std::string&ImgFileList, std::string&RootDir, std::string&DbName, bool gray, bool shuffle, std::string & backend, int resize_width, int resize_height, bool check_size, bool encoded, std::string & encode_type);
	void run();
private :
	///�ڲ�����
	mode CurrentMode;//��ǰ����ģʽ
	//MNISTģʽ����
	std::string _image_filename;
	std::string _label_filename;
	std::string _db_path;
	std::string _db_backend;
	//CIFARģʽ����
	std::string _input_folder;
	std::string _output_folder;
	std::string _db_type;
	//IMAGEģʽ����
	std::string _ImgFileList;
	std::string _RootDir;
	std::string _DbName;
	bool _gray;
	bool _shuffle;
	std::string  _backend;
	int _resize_width;
	int _resize_height;
	bool _check_size;
	bool _encoded;
	std::string  _encode_type;
private:
	///�ڲ�����11
	uint32_t swap_endian(uint32_t val);
	void convert_mnist_dataset(std::string& image_filename,
		std::string& label_filename,
		std::string& db_path,
		std::string& db_backend);

	void read_image(std::ifstream* file, int* label, char* buffer);
	void convert_cifar_dataset(std::string& input_folder,
							   std::string& output_folder,
		                       std::string& db_type);

	void convert_images(std::string&ImgFileList,//ͼƬ�ļ��б�
						std::string&RootDir,//ͼƬ����Ŀ¼
		                std::string&DbName,//���ɵ����ݿ�ľ����ļ���
						bool gray,//�Ƿ�ͼƬ��Ϊ�Ҷ�ͼ
					    bool shuffle,//�Ƿ��ͼƬ����ϴ��
						std::string &backend,//ʹ�ú������ݿ�
						int resize_width,//��ͼƬ�Ŀ�ȵ���������
						int resize_height,//��ͼƬ�ĸ߶ȵ���������
						bool check_size,//�Ƿ������е�datum�ߴ��һ����
						bool encoded,//�Ƿ��ѱ����ͼƬ������Datum��
						std::string &encode_type//ͼƬ�ı�����ʽ(png,jpg��)
						);
};

