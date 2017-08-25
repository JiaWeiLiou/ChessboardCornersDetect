// ChessboardCornersDetect.cpp : 定義主控台應用程式的進入點。
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
	/*讀取每一幅圖像，從中提取出角點，然後對角點進行亞圖元精確化*/
	string filePath;							// 輸入文件夾目錄
	string format = "JPG";						// 指定格式
	vector<string> files;						// 文件位置及名稱
	vector<string> filenames;					// 文件名稱
	int szr;									// 標定版列角點數
	int szc;									// 標定版行角點數
	cout << "Enter the folder directory : ";	// 輸入影像文件夾位置
	cin >> filePath;
	//cout << "Enter the image format (ex:.jpg) : ";	// 輸入影像格式
	//cin >> format;
	cout << "Please enter the size of board : " << endl;
	cout << "row : ";
	cin >> szr;
	cout << "column : ";
	cin >> szc;

	int image_count = 0;						// 圖像數量
	Size image_size;							// 圖像的尺寸
	Size board_size = Size(szr, szc);			// 標定板上每行、列的角點數
	vector<Point2f> image_points_buf;			// 緩存每幅圖像上檢測到的角點

	getFiles(filePath, format, filenames, files);
	cout << endl << "There are " << files.size() << " images." << endl << endl;

	string subfilePath = filePath + "\\chessboardcorners";			//創建子資料夾
	_mkdir(subfilePath.c_str());

	filePath += "\\";
	ofstream fout(subfilePath + "\\chessboardcorners.txt");			// 創建保存標定結果的文字檔

	for (int i = 0; i < files.size(); i++)
	{
		Mat imageInput = imread(files[i]);
		image_count++;

		if (image_count == 1)								//讀入第一張圖片時獲取圖像寬高資訊  
		{
			cout << "Start to get image corners..." << endl << endl;
		}
		cout << "No." << image_count << "\t" << filenames[i] << " start..." << endl;		// 用於觀察檢驗輸出  

																							/* 提取角點 */
		bool getcorners;
		getcorners = findChessboardCorners(imageInput, board_size, image_points_buf, CALIB_CB_FAST_CHECK);		//先對圖像進行快速尋找
		//if (getcorners == 0)			//若找不到圖像則進行調整後再尋找一次
		//{
		//	getcorners = findChessboardCorners(imageInput, board_size, image_points_buf, CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);
		//}

		if (getcorners == 0)
		{
			cout << "corner detect failed!" << endl;		//找不到角點
			fout << filenames[i] << " :\t" << endl;
		}
		else
		{
			Mat view_gray;
			cvtColor(imageInput, view_gray, CV_RGB2GRAY);
			TermCriteria param(TermCriteria::MAX_ITER + TermCriteria::EPS, 30, 0.1);
			cornerSubPix(view_gray, image_points_buf, Size(5, 5), Size(-1, -1), param);
			cout << "corner detect succeed!" << endl;
			/* 在圖像上顯示角點位置 */
			drawChessboardCorners(imageInput, board_size, image_points_buf, true);	//用於在圖片中標記角點
			string findpoint = subfilePath + "\\";
			findpoint.append(filenames[i]);
			imwrite(findpoint, imageInput);											//存取標示後的圖片
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

/*獲取特定格式的檔案名*/
// 第一個為路徑字串(string類型) 
// 第二個參數為指定格式(string類型) 
// 第三個參數為檔案名稱存儲變數(vector類型,引用傳遞)
// 第四個參數為檔案位置存儲變數(vector類型,引用傳遞)

void getFiles(string path, string format, vector<string>& filenames, vector<string>& files)
{
	//檔案控制代碼    
	intptr_t   hFile = 0;
	//檔資訊    
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


