/******************************************************
�ļ���   :main.cpp
��  ��   :24 ��������ʾ��ͼƬ��
��  ��   :
��  ��   :����
��  ��   :
��  ��   :2018-05-07
˵  ��   :��Ҫ24�������ֿ��֧��
******************************************************/
// ȡ��vs��freopen�Ⱥ����ľ���
#pragma warning(disable:4786)
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <cxcore.h>
#include <highgui.h>
#include <direct.h>
using namespace cv;

class ShowName {
public:
	ShowName(char* filename);
	ShowName(char* name, char* code);
	~ShowName();
	void openfile();//���ֿ�
	void get_name_code();//��ȡ���ֵ������λ��
	void get_mat(unsigned char qh, unsigned char wh);//��ȡͨ������λ����ȷ��mat
	void draw_name(int num);
	void getasi(char incode[]);//��ȡ���ֵ�ASCII��
	void draw_code(int num);
	void Runtodraw();
	void file_Runtodraw();
protected:
	FILE* CONTERNER;//�����ṩ�߼��ļ�logo.txt
	FILE* HZK24;//24�ֿ�
	FILE* ASI816;//ASCII�ֿ�
	unsigned char mat[72];
	unsigned char num_mat[16];
	char name_code_box[30];
	IplImage* img;//ͼƬ
	const int MAPSIZE = 24;//�ֵ�size
	const int INTERSIZE = 5;//������
	unsigned char name_code[3][2];
private:
	char* Name;
	char* code;
	char* Name_code;
	int sum_word;//������Ŀ
	int current_num;
};
/******************************************************
�������ƣ� ~ShowName
�������ܣ� �ͷſռ�
���������
�� �� ֵ��
����ʱ�䣺 2018-05-07
�޸�ʱ�䣺
�� �� �ˣ� ����
�� �� �ˣ�
����˵����
******************************************************/
ShowName::~ShowName() {
	cvReleaseImage(&img);
	fclose(HZK24);
	fclose(ASI816);
	if (CONTERNER != NULL)
		fclose(CONTERNER);
	img = NULL;
	HZK24 = NULL;
	ASI816 = NULL;
	CONTERNER = NULL;
}
/******************************************************
�������ƣ�  ShowName
�������ܣ�  ��ʼ���ļ�
���������	filename
�� �� ֵ��
����ʱ�䣺  2018-05-08
�޸�ʱ�䣺
�� �� �ˣ�  ����
�� �� �ˣ�
����˵����
******************************************************/
ShowName::ShowName(char* filename) {
	/*this->Name = name;
	this->code = code;
	this->current_num = 0;
	this->sum_word = strlen(Name) / 2 + strlen(this->code);*/
	//if ((CONTERNER= fopen(filename, "r") == NULL)exit(1);
	if ((CONTERNER = fopen(filename, "rb")) == NULL)exit(1);
	fseek(CONTERNER, 0, SEEK_SET);
	fread(name_code_box, 30, 1, CONTERNER);
	char* a = name_code_box;
	Name_code = a;
	sum_word = strlen(Name_code) - 2;
	current_num = 0;
	openfile();
	//cvShowImage("bt", img);
	//cvWaitKey();
}
/******************************************************
�������ƣ�  ShowName
�������ܣ�  ��ʼ��
���������	name,code
�� �� ֵ��
����ʱ�䣺  2018-05-07
�޸�ʱ�䣺
�� �� �ˣ�  ����
�� �� �ˣ�
����˵����
******************************************************/
ShowName::ShowName(char* name, char* code) {
	this->Name = name;
	this->code = code;
	this->current_num = 0;
	this->sum_word = strlen(Name) / 2 + strlen(this->code);
	openfile();
	//cvShowImage("bt", img);
	//cvWaitKey();
}
/******************************************************
�������ƣ�  openfile
�������ܣ�  ���ֿ��ͼƬ
���������
�� �� ֵ��
����ʱ�䣺  2018-05-07
�޸�ʱ�䣺
�� �� �ˣ�  ����
�� �� �ˣ�
����˵����
******************************************************/
void ShowName::openfile() {
	char pbuf[100];
	_getcwd(pbuf, 100);
	strcat(pbuf, "/HZKf2424.hz");
	char pbufASC[100];
	_getcwd(pbufASC, 100);
	strcat(pbufASC, "/Asci0816.zf");
	// ��ȡͼƬ
	if ((img = cvLoadImage("flower.jpg")) == NULL)exit(1);
	// �������ļ�
	if ((HZK24 = fopen(pbuf, "rb")) == NULL)exit(1);
	//��asci8*16�ļ�
	if ((ASI816 = fopen(pbufASC, "rb")) == NULL)exit(1);
}
/******************************************************
�������ƣ�  get_name_code
�������ܣ�  ��ȡ���ֵ������λ��
���������
�� �� ֵ��
����ʱ�䣺  2018-05-07
�޸�ʱ�䣺
�� �� �ˣ�  ����
�� �� �ˣ�
����˵����  �෽����ʧЧ
******************************************************/
void ShowName::get_name_code() {
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 2; ++j)
			name_code[i][j] = Name[i * 2 + j] - 0xa0;
}
/******************************************************
�������ƣ�  get_mat
�������ܣ�  ͨ�����ֵ������λ�����д��mat��
���������	qh, wh
�� �� ֵ��
����ʱ�䣺  2018-05-07
�޸�ʱ�䣺
�� �� �ˣ�  ����
�� �� �ˣ�
����˵����
******************************************************/
void ShowName::get_mat(unsigned char qh, unsigned char wh) {
	long offset;
	offset = (94 * (qh - 1) + (wh - 1)) * 72L;
	// ��ȡ���ݴ�������
	fseek(HZK24, offset, SEEK_SET);
	fread(mat, 72, 1, HZK24);
}
/******************************************************
�������ƣ�  draw_name
�������ܣ�  ͨ�����ֵ������λ�����д��mat��
���������	qh, wh
�� �� ֵ��
����ʱ�䣺  2018-05-07
�޸�ʱ�䣺
�� �� �ˣ�  ����
�� �� �ˣ�
����˵����
******************************************************/
void ShowName::draw_name(int num) {
	// ͼƬ������ֵ
	int width, height;
	width = img->width;
	height = img->height;
	// ��ʼ��x y���ص�
	int start_x, start_y, size, current_start_x, current_start_y;
	size = MAPSIZE;// +INTERSIZE;
	start_x = width - sum_word * size;
	start_y = height - MAPSIZE - INTERSIZE;
	// ��ʼ����

	CvScalar cs;

	for (int i = 0; i < 24; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 8; k++)
				if (((mat[i * 3 + j] >> (7 - k)) & 0x1) != NULL)
				{
					// ���
					current_start_x = start_x + i + size * num;//24*24�����������е�i��Ӧ����x
					current_start_y = start_y + j * 8 + k;
					cs = cvGet2D(img, current_start_y, current_start_x);
					cs.val[0] = 0;
					cs.val[1] = 0xFF;//������Ըĳ���ϲ������ɫ
					cs.val[2] = 0xFF;
					cvSet2D(img, current_start_y, current_start_x, cs);
				}
}
/******************************************************
�������ƣ�  getasi
�������ܣ�  ��ȡasci��
���������
�� �� ֵ��
����ʱ�䣺  2018-05-08
�޸�ʱ�䣺
�� �� �ˣ�  ����
�� �� �ˣ�
����˵����
******************************************************/
void ShowName::getasi(char incode[]) {
	unsigned char qh, wh;
	unsigned long offset;
	offset = incode[0] * 16L;
	fseek(ASI816, offset, SEEK_SET);
	fread(num_mat, 16, 1, ASI816);
}
/******************************************************
�������ƣ�  draw_code
�������ܣ�  ����ѧ��
���������
�� �� ֵ��
����ʱ�䣺  2018-05-08
�޸�ʱ�䣺
�� �� �ˣ�  ����
�� �� �ˣ�
����˵����
******************************************************/
void ShowName::draw_code(int num) {
	int width, height;
	width = img->width;
	height = img->height;
	// ��ʼ��x y���ص�
	int start_x, start_y, size, current_start_x, current_start_y;
	size = MAPSIZE; //+INTERSIZE;
	//int numsize = 8;
	start_x = width - sum_word * size;
	start_y = height - 16 - INTERSIZE;
	// ��ʼ����

	CvScalar cs;
	for (int i = 0; i < 16; ++i)
		for (int k = 0; k < 8; k++)
			if ((num_mat[i] & (0x80 >> k)) != NULL)
			{
				current_start_x = k + start_x + size * num;
				current_start_y = start_y + i;
				cs = cvGet2D(img, current_start_y, current_start_x);//��ȡͼ�����λ�õ�RGB��ֵ
				cs.val[0] = 0;//���
				cs.val[1] = 0xFF;//������Ըĳ���ϲ������ɫ
				cs.val[2] = 0xFF;
				cvSet2D(img, current_start_y, current_start_x+10, cs);//������ֵ
			}

}
/******************************************************
�������ƣ�  Runtodraw
�������ܣ�  ��������
���������
�� �� ֵ��
����ʱ�䣺  2018-05-07
�޸�ʱ�䣺
�� �� �ˣ�  ����
�� �� �ˣ�
����˵����
******************************************************/

void ShowName::Runtodraw() {
	unsigned char mask = 0x80;
	char tmpcode[3] = { 0 };
	while (*Name != NULL)
	{
		tmpcode[0] = *Name;
		tmpcode[1] = *(Name + 1);
		if (tmpcode[0] & mask) {
			unsigned char qh, wh;
			qh = tmpcode[0] - 0xaf;    //������   
			wh = tmpcode[1] - 0xa0;//��λ��
			get_mat(qh, wh);
			draw_name(current_num++);
			Name += 2;
		}
	}
	while (*code != NULL)
	{
		tmpcode[0] = *code;
		if (tmpcode[0])
		{
			getasi(code);
			draw_code(current_num++);
			code++;
		}
	}
	cvShowImage("bt", img);
	cvWaitKey();
}
/******************************************************
�������ƣ�  file_Runtodraw
�������ܣ�  ��������
���������
�� �� ֵ��
����ʱ�䣺  2018-05-07
�޸�ʱ�䣺
�� �� �ˣ�  ����
�� �� �ˣ�
����˵����
******************************************************/
void ShowName::file_Runtodraw() {
	unsigned char mask = 0x80;
	char tmpcode[3] = { 0 };
	int x = 0;
	while (*Name_code != NULL)
	{
		tmpcode[0] = *Name_code;
		tmpcode[1] = *(Name_code + 1);
		if (tmpcode[0] & mask) {
			unsigned char qh, wh;
			qh = tmpcode[0] - 0xaf;
			wh = tmpcode[1] - 0xa0;
			get_mat(qh, wh);
			draw_name(current_num++);
			Name_code += 2;
		}
		else if (tmpcode[0]) {
			getasi(Name_code);
			draw_code(current_num++);
			Name_code++;
		}
	}
	cvShowImage("bt", img);
	cvWaitKey();
}
int main() {
	char* filename = (char*)"data.txt";
	ShowName* newname = new ShowName(filename);
	newname->file_Runtodraw();
	delete newname;
	return 0;
}