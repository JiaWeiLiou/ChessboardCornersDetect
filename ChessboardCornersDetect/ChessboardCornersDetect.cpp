// ChessboardCornersDetect.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <io.h>
#include <direct.h>
#include <string>  
#include <vector>

using namespace cv;
using namespace std;

void getFiles(string path, string format, vector<string>& filenames, vector<string>& files);

int main()
{
	/*Ū���C�@�T�Ϲ��A�q�������X���I�A�M��﨤�I�i��ȹϤ���T��*/
	string filePath;							// ��J��󧨥ؿ�
	string format = "JPG";						// ���w�榡
	vector<string> files;						// ����m�ΦW��
	vector<string> filenames;					// ���W��
	int szr;									// �Щw���C���I��
	int szc;									// �Щw���樤�I��
	cout << "Enter the folder directory : ";	// ��J�v����󧨦�m
	cin >> filePath;
	//cout << "Enter the image format (ex:.jpg) : ";	// ��J�v���榡
	//cin >> format;
	cout << "Please enter the size of board : " << endl;
	cout << "row : ";
	cin >> szr;
	cout << "column : ";
	cin >> szc;

	int image_count = 0;						// �Ϲ��ƶq
	Size image_size;							// �Ϲ����ؤo
	Size board_size = Size(szr, szc);			// �Щw�O�W�C��B�C�����I��
	vector<Point2f> image_points_buf;			// �w�s�C�T�Ϲ��W�˴��쪺���I

	getFiles(filePath, format, filenames, files);
	cout << endl << "There are " << files.size() << " images." << endl << endl;

	string subfilePath = filePath + "\\chessboardcorners";			//�Ыؤl��Ƨ�
	_mkdir(subfilePath.c_str());

	filePath += "\\";
	ofstream fout(subfilePath + "\\chessboardcorners.txt");			// �ЫثO�s�Щw���G����r��

	for (int i = 0; i < files.size(); i++)
	{
		Mat imageInput = imread(files[i]);
		image_count++;

		if (image_count == 1)								//Ū�J�Ĥ@�i�Ϥ�������Ϲ��e����T  
		{
			cout << "Start to get image corners..." << endl << endl;
		}
		cout << "No." << image_count << "\t" << filenames[i] << " start..." << endl;		// �Ω��[�������X  

																							/* �������I */
		bool getcorners;
		getcorners = findChessboardCorners(imageInput, board_size, image_points_buf, CALIB_CB_FAST_CHECK);		//����Ϲ��i��ֳt�M��
		//if (getcorners == 0)			//�Y�䤣��Ϲ��h�i��վ��A�M��@��
		//{
		//	getcorners = findChessboardCorners(imageInput, board_size, image_points_buf, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);
		//}

		if (getcorners == 0)
		{
			cout << "corner detect failed!" << endl;		//�䤣�쨤�I
			fout << filenames[i] << " :\t" << endl;
		}
		else
		{
			Mat view_gray;
			cvtColor(imageInput, view_gray, CV_RGB2GRAY);
			TermCriteria param(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1);
			cornerSubPix(view_gray, image_points_buf, Size(5, 5), Size(-1, -1), param);
			cout << "corner detect succeed!" << endl;
			/* �b�Ϲ��W��ܨ��I��m */
			drawChessboardCorners(imageInput, board_size, image_points_buf, true);	//�Ω�b�Ϥ����аO���I
			string findpoint = subfilePath + "\\";
			findpoint.append(filenames[i]);
			imwrite(findpoint, imageInput);											//�s���Хܫ᪺�Ϥ�
			fout << filenames[i] << " :\t";
			for (int i = 0; i < image_points_buf.size(); i++)
			{
				fout << image_points_buf[i].x << "\t";
				fout << image_points_buf[i].y << "\t";
			}
			fout << endl;
		}
	}
	std::cout << endl << "Results are saved!" << endl;
    return 0;
}

/*����S�w�榡���ɮצW*/
// �Ĥ@�Ӭ����|�r��(string����) 
// �ĤG�ӰѼƬ����w�榡(string����) 
// �ĤT�ӰѼƬ��ɮצW�٦s�x�ܼ�(vector����,�ޥζǻ�)
// �ĥ|�ӰѼƬ��ɮצ�m�s�x�ܼ�(vector����,�ޥζǻ�)

void getFiles(string path, string format, vector<string>& filenames, vector<string>& files)
{
	//�ɮױ���N�X    
	intptr_t   hFile = 0;
	//�ɸ�T    
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getFiles(p.assign(path).append("\\").append(fileinfo.name), format, filenames, files);
				}
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
				filenames.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}
}


