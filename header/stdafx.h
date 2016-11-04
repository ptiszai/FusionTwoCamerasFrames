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

#include "targetver.h"

#include <stdio.h>
#include <cstdio>
#include <cstdint>
#include <tchar.h>
#include <cassert>
#include <vector>
#include <list>
#include <string>
#include <tuple>
#include <numeric>
#include <functional>
#include <algorithm>
#include <array>
#include <thread>
#include <chrono>
#include <iomanip>

#include <windows.h>
#include <opencv2/opencv.hpp>
#include "TickMeter.h"
#include "helper_functions.h"


// TODO: reference additional headers your program requires here
