/*******************************************************************************#
#	FusionTwoCamerasFrames							#
#	Istvan Tiszai <tiszaii@hotmail.com>                                 	#
#	https://github.com/ptiszai/FusionTwoCamerasFrames			#
#                                                                               #
# This program is free software; you can redistribute it and/or modify          #
# it under the terms of the GNU General Public License as published by          #
# the Free Software Foundation; either version 2 of the License, or             #
# (at your option) any later version.                                           #
#                                                                               #
# This program is distributed in the hope that it will be useful,               #
# but WITHOUT ANY WARRANTY; without even the implied warranty of                #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 #
# GNU General Public License for more details.                                  #
#                                                                               #
# You should have received a copy of the GNU General Public License             #
# along with this program; if not, write to the Free Software                   #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA     #
#                                                                               #
********************************************************************************/

#pragma once
#include "stdafx.h"

//---------------------------------------------
inline std::string intToString(int number)
{
	std::stringstream ss;
	ss << number;
	return ss.str();
}

//---------------------------------------------
inline std::string doubleToString(double number)
{
	char stemp[256];
	sprintf_s(stemp, "%.2f", number);
	return std::string(stemp);
}

//---------------------------------------------
inline void fill_and_print_array(cv::Mat& mat, std::string title, uchar data, bool full, bool print)
{
	int w, h;
	std::cout << "\n" << title << ":" << std::endl;

	for (h = 0; h < mat.rows; h++)
	{
		std::cout << h + 1 << ":";
		for (w = 0; w < mat.cols; w++)
		{
			if (full)
				mat.at<uchar>(w, h) = data;
			if (print)
				std::cout << (int)mat.at<uchar>(w, h) << ",";
		}
		std::cout << std::endl;
	}
}

//---------------------------------------------
inline double sqr(double X)
{ 
	return X *X ; 
}

//---------------------------------------------
inline void equalizeIntensity(const cv::Mat& inputImage, cv::Mat& output)
{
	if (inputImage.channels() >= 3)
	{
		cv::Mat ycrcb;

		cv::cvtColor(inputImage, ycrcb, CV_BGR2HSV);

		std::vector<cv::Mat> channels;
		cv::split(ycrcb, channels);

		cv::equalizeHist(channels[2], channels[2]);

		cv::Mat result;
		cv::merge(channels, ycrcb);

		cv::cvtColor(ycrcb, result, CV_HSV2BGR);
		output = result;
	}
	else
	{
		output = inputImage;
	}
}

//---------------------------------------------
inline std::vector<std::list<std::string>> ls_folders(const std::vector<std::string> &folders)
{
	std::vector<std::list<std::string>> imgnames;
	for (const auto &folder : folders)
	{
		std::list<std::string> new_list;
		for (size_t i = 0; i <= 200; ++i)
		{
			std::string name = std::to_string(i) + std::string(".png");
			if (!cv::imread(folder + name).empty())
				new_list.push_back(name);
		}
		imgnames.push_back(new_list);
	}
	return imgnames;
}

//---------------------------------------------
inline std::tuple<std::vector<std::string>, std::vector<std::string>>
get_inout_names(const std::vector<std::string> &infolders, const std::vector<std::string> &outfolders)
{
	assert(infolders.size() == outfolders.size());
	std::vector<std::list<std::string>> imgnames = ls_folders(infolders);
	std::vector<std::string> inputnames, outputnames;
	for (size_t i = 0; i < infolders.size(); ++i)
		for (const auto &str : imgnames[i])
		{
			inputnames.push_back(infolders[i] + str);
			outputnames.push_back(outfolders[i] + str);
		}
	return std::make_tuple(inputnames, outputnames);
}

#ifdef HAVE_CUDA
//---------------------------------------------
inline cv::cuda::GpuMat toNorm32f(const cv::cuda::GpuMat &img_cuda)
{
	cv::cuda::GpuMat imgf_cuda;
	img_cuda.convertTo(imgf_cuda, CV_32FC3, 1.0 / 255);
//	cv::cuda::multiply(imgf_cuda, cv::Scalar(1.0/255, 1.0/255, 1.0/255), imgf_cuda); // normalize bgr
/*	if (imgf_cuda.size() == imgf_cuda1.size())
	{
		printf("egyenlo size\n");
		cv::cuda::GpuMat dst;
		cv::cuda::bitwise_and(imgf_cuda, imgf_cuda1, dst);
		if (cv::cuda::countNonZero(dst) > 0) //check non-0 pixels
			printf("nem egyenlo\n");
		else
		printf("egyenlo\n");
		//do stuff in case they are equal
	}*/
	return imgf_cuda;
}

inline cv::cuda::GpuMat toNorm32f_cuda(const cv::Mat_<cv::Vec3b> &img)
{
	cv::cuda::GpuMat img_cuda;
	img_cuda.upload(img);
	return toNorm32f(img_cuda);
}
#endif

//---------------------------------------------
inline int log2(int arg)
{
	assert(arg > 0);
	int l;
	for (l = 0; l < 32; ++l)
	{
		if (arg == 1)
			break;
		arg >>= 1;
	}
	return l;
}

//---------------------------------------------
inline cv::Mat_<cv::Vec3f> toNorm32f(const cv::Mat_<cv::Vec3b> &img)
{
	cv::Mat_<cv::Vec3f> imgf;
	img.convertTo(imgf, CV_32FC3, 1.0 / 255);
//	img.convertTo(imgf0, CV_32FC3);
	//imgf *= 1.0/255; // normalize bgr
/*	imgf0 *= 1.0 / 255; // normalize bgr
	if (imgf.size() == imgf0.size())
	{
	printf("egyenlo size\n");	
	//cv::Mat dst(imgf.size(), imgf.type());
	//cv::bitwise_and(imgf, imgf0, dst);
	cv::Mat dst, dst0, dst1;
	cv::compare(imgf, imgf0, dst, cv::CMP_EQ);
	dst.convertTo(dst0, CV_8UC3);
	cv::cvtColor(dst0, dst1, CV_RGB2GRAY);
	if (cv::countNonZero(dst1) > 0) //check non-0 pixels
	printf("nem egyenlo\n");
	else
	printf("egyenlo\n");
	//do stuff in case they are equal
	}*/
	return imgf;
}

//---------------------------------------------
inline cv::Mat_<cv::Vec3b> toColor(const cv::Mat_<uint8_t> &src)
{
	cv::Mat_<cv::Vec3b> dst;
	cv::cvtColor(src, dst, cv::COLOR_GRAY2BGR);
	return dst;
}

#ifdef HAVE_CUDA
//---------------------------------------------
inline cv::cuda::GpuMat/*<uint8_t>*/ to8b(const cv::cuda::GpuMat/*<float>*/ &src)
{
	cv::cuda::GpuMat dst;
	src.convertTo(dst, CV_8U);
	return dst;
}
inline cv::cuda::GpuMat/*<cv::Vec3b>*/ to24b(const cv::cuda::GpuMat/*<cv::Vec3f>*/ &src)
{
	cv::cuda::GpuMat dst;
	src.convertTo(dst, CV_8UC3);
	return dst;
}
#endif

//---------------------------------------------
inline void to8b(const cv::Mat_<float> &src, cv::Mat_<uint8_t> &dst)
{
	src.convertTo(dst, CV_8U);
}

//---------------------------------------------
inline void to24b(const cv::Mat_<cv::Vec3f> &src, cv::Mat_<cv::Vec3b> &dst)
{
	src.convertTo(dst, CV_8UC3);
}

//---------------------------------------------
inline cv::Mat_<uint8_t> to8b(const cv::Mat_<float> &src)
{
	cv::Mat_<uint8_t> dst;
	to8b(src, dst);
	return dst;
}

//---------------------------------------------
inline cv::Mat_<cv::Vec3b> to24b(const cv::Mat_<cv::Vec3f> &src)
{
	cv::Mat_<cv::Vec3b> dst;
	to24b(src, dst);
	return dst;
}

#ifdef HAVE_CUDA
//---------------------------------------------
inline void toLab(const cv::cuda::GpuMat/*<cv::Vec3f>*/ &bgr_cuda, cv::cuda::GpuMat/*<cv::Vec3f>*/ &lab_cuda)
{
	cv::cuda::cvtColor(bgr_cuda, lab_cuda, cv::COLOR_BGR2Lab);
}

//---------------------------------------------
inline cv::cuda::GpuMat/*<cv::Vec3f>*/ toLab(const cv::cuda::GpuMat/*<cv::Vec3f>*/ &bgr_cuda)
{
	cv::cuda::GpuMat lab_cuda;
	toLab(bgr_cuda, lab_cuda);
	return lab_cuda;
}

//---------------------------------------------
inline cv::cuda::GpuMat/*<cv::Vec3f>*/ toLab_cuda(const cv::Mat_<cv::Vec3f> &bgr)
{
	cv::cuda::GpuMat lab_cuda;
	cv::cuda::GpuMat bgr_cuda;
	bgr_cuda.upload(bgr);
	return toLab(bgr_cuda);
}
#endif

//---------------------------------------------
inline void toLab(const cv::Mat_<cv::Vec3f> &bgr, cv::Mat_<cv::Vec3f> &lab)
{
	cv::cvtColor(bgr, lab, cv::COLOR_BGR2Lab);
}

//---------------------------------------------
inline cv::Mat_<cv::Vec3f> toLab(const cv::Mat_<cv::Vec3f> &bgr)
{
	cv::Mat_<cv::Vec3f> lab;
	toLab(bgr, lab);
	return lab;
}

//---------------------------------------------
inline cv::Vec3f toLab(const cv::Vec3f &bgr)
{
	// convert bgr->lab
	cv::Mat_<cv::Vec3f> mat(1,1);
	mat(0,0) = bgr;
	mat = toLab(mat);
	return mat(0,0);
}

//---------------------------------------------
inline cv::Mat_<cv::Vec3f> toBgr(const cv::Mat_<cv::Vec3f> &lab)
{
	cv::Mat_<cv::Vec3f> bgr;
	cv::cvtColor(lab, bgr, cv::COLOR_Lab2BGR);
	return bgr;
}

//---------------------------------------------
inline std::vector<cv::Mat_<cv::Vec3f>> toBgr(const std::vector<cv::Mat_<cv::Vec3f>> &vec_lab)
{
	std::vector<cv::Mat_<cv::Vec3f>> vec_bgr(vec_lab.size());
	for (size_t i = 0; i < vec_lab.size(); ++i)
		vec_bgr[i] = toBgr(vec_lab[i]);
	return vec_bgr;
}

//---------------------------------------------
inline std::tuple<double,double> get_minmax(const cv::Mat &src, const cv::Mat_<uint8_t> &mask = cv::Mat_<uint8_t>())
{
	if (!mask.empty())
		assert(src.size() == mask.size());
	double minVal, maxVal;
	cv::minMaxLoc(src, &minVal, &maxVal, 0, 0, mask);
	return std::make_tuple(minVal, maxVal);
}

//---------------------------------------------
inline cv::Mat_<float> normalize(const cv::Mat_<float> &src, const cv::Mat_<uint8_t> &mask = cv::Mat_<uint8_t>())
{
	if (!mask.empty())
		assert(src.size() == mask.size());
	double min, max;
	std::tie(min, max) = get_minmax(src, mask);
	cv::Mat_<float> dst = src.clone();
	dst -= min;
	return dst * 255.0/(max-min);
}

//---------------------------------------------
inline cv::Mat_<float> normalize2mean(const cv::Mat_<float> &src, const cv::Mat_<uint8_t> &mask = cv::Mat_<uint8_t>())
{
	if (!mask.empty())
		assert(src.size() == mask.size());
	// normalize (min==0 && mean==127)
	cv::Scalar mean_s, stddev_s;
	cv::meanStdDev(src, mean_s, stddev_s, mask);
	double min = std::get<0>(get_minmax(src));
	double mean = mean_s[0] - min;

	cv::Mat_<float> dst = src.clone();
	dst -= min;
	dst *= 127.0 / mean;
	return dst;
}

//---------------------------------------------
inline void printMat(const cv::Mat &src)
{
	for (int i = 0; i < src.rows; ++i)
	{
		for (int j = 0; j < src.cols; ++j)
		{
			printf("%f ", src.at<float>(i,j));
		}
		printf("\n");
	}
}

//---------------------------------------------
class Output
{
public:
	void add_right(const cv::Mat_<cv::Vec3b> &img)
	{
		if (out.empty())
		{
			out = img.clone();
			return;
		}
		assert(out.rows == img.rows);
		cv::Mat_<cv::Vec3b> new_out(out.rows, out.cols + img.cols);
		out.copyTo(new_out.colRange(0, out.cols));
		img.copyTo(new_out.colRange(out.cols, new_out.cols));
		std::swap(out, new_out);
	}
	void add_down(const cv::Mat_<cv::Vec3b> &img)
	{
		if (out.empty())
		{
			out = img.clone();
			return;
		}
		assert(out.cols == img.cols);
		cv::Mat_<cv::Vec3b> new_out(out.rows + img.rows, out.cols);
		out.copyTo(new_out.rowRange(0, out.rows));
		img.copyTo(new_out.rowRange(out.rows, new_out.rows));
		std::swap(out, new_out);
	}
	cv::Mat_<cv::Vec3b> get_out() const {return out;}
private:
	cv::Mat_<cv::Vec3b> out;
};
