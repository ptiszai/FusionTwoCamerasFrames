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


#include "stdafx.h"

#pragma region global variables
bool RepeatedMode = false;
bool SaveMode = false;
#pragma endregion

#pragma region variables
TickMeter tm;
cv::Scalar color_red = cv::Scalar(0, 0, 255);
cv::Scalar color_green = cv::Scalar(0, 255, 128);
cv::Scalar color_yellow = cv::Scalar(0, 255, 255);
cv::Scalar color_purple = cv::Scalar(2, 96, 255);
cv::Scalar color_blue = cv::Scalar(255, 32, 32);
int fontface = cv::FONT_HERSHEY_SIMPLEX;
double scale = 0.4;
int thickness = 1;
int baseline = 0;
double alpha = 0.7;
double beta = 0.3;
int    lefttoright_pix = 0;  // this is used only IR image
int    toptobottom_pix = 0;  // this is used only IR image
const std::string trackbarWindowName = "setting properties";
#pragma endregion

#pragma region helper functions

//-----------------------------------
static void createTrackbars()
{
	//create window for trackbars
	cv::namedWindow("setting properties", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
	cv::resizeWindow("setting properties", 600, 250);

	// Trackbars are for the the parameters of cv::addWeighted(...) function.
	// alpha – weight of the first array elements.
	cv::TrackbarCallback on_callbackAlpha = [](int state, void* userdata)
	{
		alpha = (double)state / 100;		
	};
	int iTemp = (int)(alpha * 100);
	cv::createTrackbar("alpha ", "setting properties", &iTemp, 100, on_callbackAlpha, &iTemp);

	// beta – weight of the second array elements.
	cv::TrackbarCallback on_callbackBeta = [](int state, void* userdata)
	{
		beta = (double)state / 100;		
	};
	iTemp = (int)(beta * 100);
	cv::createTrackbar("beta ", "setting properties", &iTemp, 100, on_callbackBeta, &iTemp);

	// Trackbars are for the shifting mode.
	// lefttoright_pix – left to right shifted.
	cv::TrackbarCallback on_callbackLefttoright = [](int state, void* userdata)
	{
		lefttoright_pix = state;		
	};	
	cv::createTrackbar("ltor ", "setting properties", &lefttoright_pix, 300, on_callbackLefttoright, &lefttoright_pix);

	// toptobottom_pix – top to bottom shifted.
	cv::TrackbarCallback on_callbackToptobottom = [](int state, void* userdata)
	{
		toptobottom_pix = state;		
	};
	cv::createTrackbar("ttob ", "setting properties", &toptobottom_pix, 300, on_callbackToptobottom, &toptobottom_pix);
}

#pragma endregion

#pragma region main
//------------------------------------------------------
// MAIN
//------------------------------------------------------
int main(int argc, char* argv[])
{
	char ch;
	const std::string _fname_color("proba1Color.mp4");
	const std::string _fname_ir("proba1ir.mp4");			
	cv::VideoCapture _captureColor;
	cv::VideoCapture _captureIr;

	if (!_captureColor.open(_fname_color))
	{
		std::cout << "Cannot open the video/image file: " << _fname_color << std::endl;
		return -1;
	}		
	if (!_captureIr.open(_fname_ir))
	{
		std::cout << "Cannot open the video/image file: " << _fname_ir << std::endl;
		return -1;
	}

	cv::Mat _cameraColorFrame, _cameraIrFrame, _fusionImage;	
	std::vector<double> cpu_times;
	double cpu_avg = 0, cpu_fps = 0;
	int frameCount;
	
	cv::namedWindow("color image", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
	cv::namedWindow("ir image", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);
	cv::namedWindow("fusion result", CV_WINDOW_AUTOSIZE | CV_WINDOW_KEEPRATIO | CV_GUI_EXPANDED);

	createTrackbars();
	
	frameCount = 0;

	std::vector<cv::Mat> _color_bgr(3);	   
	std::vector<cv::Mat> _ir_bgr(3);
	std::vector<cv::Mat> _fusion_bgr(3);
	std::vector<cv::Mat> _channels;

	try
	{	
		int ii = 0;
		while (1)
		{
			if (!_captureColor.isOpened())
			{
				std::cout << "Broken video open: " << _fname_color << std::endl;
				return -1;
			}
			if (!_captureIr.isOpened())
			{
				std::cout << "Broken video open: " << _fname_ir << std::endl;
				return -1;
			}
			if (_captureColor.read(_cameraColorFrame) && _captureIr.read(_cameraIrFrame))
			{				
				frameCount++;
				if (SaveMode)
				{
					// save output color images 
					std::string filenameColor = "c:/Users/Public/Pictures/img_co_" + intToString(frameCount) + ".jpg";
					cv::imwrite(filenameColor, _cameraColorFrame);
					std::cout << "Saved: " << filenameColor << std::endl;
					// save output ir images 
					std::string filenameIr = "c:/Users/Public/Pictures/img_ir_" + intToString(frameCount) + ".jpg";
					cv::imwrite(filenameIr, _cameraIrFrame);
					std::cout << "Saved: " << filenameIr << std::endl;

				}
				tm.reset();
				tm.start();
				if ((lefttoright_pix != 0) || (toptobottom_pix != 0))
				{ //Frame of Ir content shifted is left to right or top to bottom with x pixels
					cv::Mat out = cv::Mat::zeros(_cameraIrFrame.size(), _cameraIrFrame.type());
					_cameraIrFrame(cv::Rect(lefttoright_pix, toptobottom_pix, _cameraIrFrame.cols - lefttoright_pix, _cameraIrFrame.rows - toptobottom_pix)).copyTo(out(cv::Rect(0, 0, _cameraIrFrame.cols - lefttoright_pix, _cameraIrFrame.rows - toptobottom_pix)));
					_cameraIrFrame = out;
				}
#ifdef BITWISE
				_channels.clear();			
				cv::split(_cameraColorFrame, _color_bgr);//split color frame  source 
				cv::split(_cameraIrFrame, _ir_bgr);//split ir frame  source 
				// OR
			/*	cv::bitwise_or(_color_bgr[0], _ir_bgr[0], _fusion_bgr[0], cv::noArray());
				cv::bitwise_or(_color_bgr[1], _ir_bgr[1], _fusion_bgr[1], cv::noArray());
				cv::bitwise_or(_color_bgr[2], _ir_bgr[2], _fusion_bgr[2], cv::noArray());*/
				// XOR
			/*	cv::bitwise_xor(_color_bgr[0], _ir_bgr[0], _fusion_bgr[0], cv::noArray());
				cv::bitwise_xor(_color_bgr[1], _ir_bgr[1], _fusion_bgr[1], cv::noArray());
				cv::bitwise_xor(_color_bgr[2], _ir_bgr[2], _fusion_bgr[2], cv::noArray());*/
				// AND
				cv::bitwise_and(_color_bgr[0], _ir_bgr[0], _fusion_bgr[0], cv::noArray());
				cv::bitwise_and(_color_bgr[1], _ir_bgr[1], _fusion_bgr[1], cv::noArray());
				cv::bitwise_and(_color_bgr[2], _ir_bgr[2], _fusion_bgr[2], cv::noArray());
				_channels.push_back(_fusion_bgr[0]);
				_channels.push_back(_fusion_bgr[1]);
				_channels.push_back(_fusion_bgr[2]);
				cv::merge(_channels, _fusionImage);	
#else				
				//	addWeighted begin
				cv::addWeighted(_cameraColorFrame, alpha, _cameraIrFrame, beta, 0, _fusionImage);
				//	addWeighted end
#endif
			//	std::this_thread::sleep_for(std::chrono::milliseconds(40));			
				
				tm.stop();
				cpu_times.push_back(tm.getTimeMilliseconds());
				if (!cpu_times.empty() && (cpu_times.size() >= 10))
				{
					cpu_avg = std::accumulate(cpu_times.begin(), cpu_times.end(), 0.0) / cpu_times.size();
					cpu_fps = 1000.0 / cpu_avg;
					cpu_times.clear();
				}
				cv::putText(_cameraColorFrame, "CPU : Avg : " + doubleToString(cpu_avg) + " ms FPS : " + doubleToString(cpu_fps), cv::Point(10, 40), fontface, scale, color_yellow, thickness, 8);
				cv::putText(_cameraColorFrame, "Keys : ' ' - stop or start (repeated) video capture read ; 'q' - quit program", cv::Point(10, 70), fontface, scale, color_yellow, thickness, 8);
				cv::imshow("color image", _cameraColorFrame);
				cv::imshow("ir image", _cameraIrFrame);
				cv::imshow("fusion result", _fusionImage);

				// DELAY BEGIN 
				// The interval of time is between frames
				std::this_thread::sleep_for(std::chrono::milliseconds(90)); // 90 ms
				// DELAY END
			}
			else
			{
				std::cout << "Bad capture frame reading! " << frameCount << std::endl;
				_captureColor.release();
				_captureIr.release();
				frameCount = 0;
				if (!_captureColor.open(_fname_color))
				{
					std::cout << "Cannot open the video/image file: " << _fname_color << std::endl;
					return -1;
				}
				if (!_captureIr.open(_fname_ir))
				{
					std::cout << "Cannot open the video/image file: " << _fname_ir << std::endl;
					return -1;
				}
			}

			if (RepeatedMode)
				ch = (char)cv::waitKey(0);
			else
				ch = (char)cv::waitKey(5);							
			switch (ch)
			{
				case 'q':
					return 0;
				case ' ':
					RepeatedMode = (RepeatedMode) ?  false :  true;
					break;		
			}
		} // while (1)		
	}
	catch (const cv::Exception& ex)
	{
		std::cout << "Error main : " << ex.what() << std::endl;
	}
	cv::destroyWindow("setting properties");
	cv::destroyWindow("color image");
	cv::destroyWindow("ir image");
	cv::destroyWindow("fusion result");
}
#pragma endregion
