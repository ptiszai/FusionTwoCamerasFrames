/*******************************************************************************#
#           FusionTwoCamerasFrames												#
#           Istvan Tiszai <tiszaii@hotmail.com>                                 #
#			https://github.com/ptiszai/FusionTwoCamerasFrames					#
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
#include "TickMeter.h"

std::ostream& operator << (std::ostream& out, const TickMeter& tm);
TickMeter::TickMeter() { reset(); }
int64 TickMeter::getTimeTicks() const { return sumTime; }
double TickMeter::getTimeMicro() const { return  getTimeMilliseconds()*1e3; }
double TickMeter::getTimeMilliseconds() const { return getTimeSec()*1e3; }
double TickMeter::getTimeSec() const { return (double)getTimeTicks() / cv::getTickFrequency(); }
int64 TickMeter::getCounter() const { return counter; }
void TickMeter::reset() { startTime = 0; sumTime = 0; counter = 0; }
void TickMeter::start(){ startTime = cv::getTickCount(); }
void TickMeter::stop()
{
	int64 time = cv::getTickCount();
	if (startTime == 0)
		return;
	++counter;
	sumTime += (time - startTime);
	startTime = 0;
}

std::ostream& operator << (std::ostream& out, const TickMeter& tm) { return out << tm.getTimeSec() << "sec"; }