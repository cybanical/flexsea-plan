/****************************************************************************
	[Project] FlexSEA: Flexible & Scalable Electronics Architecture
	[Sub-project] 'plan-gui' Graphical User Interface
	Copyright (C) 2016 Dephy, Inc. <http://dephy.com/>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************
	[Lead developper] Jean-Francois (JF) Duval, jfduval at dephy dot com.
	[Origin] Based on Jean-Francois Duval's work at the MIT Media Lab
	Biomechatronics research group <http://biomech.media.mit.edu/>
	[Contributors]
*****************************************************************************
	[This file] batteryDevice: Battery Device Data Class
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-12-08 | sbelanger | Initial GPL-3.0 release
	*
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************

#include "batteryDevice.h"
#include <QDebug>
#include <QTextStream>

//****************************************************************************
// Constructor & Destructor:
//****************************************************************************

BatteryDevice::BatteryDevice(void): FlexseaDevice()
{
	this->dataSource = LogDataFile;
	serializedLength = header.length();
	slaveType = "battery";
}

BatteryDevice::BatteryDevice(battery_s *devicePtr): FlexseaDevice()
{
	this->dataSource = LiveDataFile;
	timeStamp.append(TimeStamp());
	baList.append(devicePtr);
	serializedLength = header.length();
	slaveType = "battery";
}

//****************************************************************************
// Public function(s):
//****************************************************************************

QString BatteryDevice::getHeaderStr(void)
{
		return header.join(',');
}

QStringList BatteryDevice::header =	QStringList()
									<< "Timestamp"
									<< "Timestamp (ms)"
									<< "voltage"
									<< "current"
									<< "temp"
									<< "pushbutton"
									<< "status";

QString BatteryDevice::getLastSerializedStr(void)
{
	QString str;
	QTextStream(&str) <<	timeStamp.last().date		<< ',' << \
							timeStamp.last().ms			<< ',' << \
							baList.last()->voltage		<< ',' << \
							baList.last()->current		<< ',' << \
							baList.last()->temp			<< ',' << \
							baList.last()->pushbutton	<< ',' << \
							baList.last()->status;
	return str;
}

void BatteryDevice::appendSerializedStr(QStringList *splitLine)
{
	//Check if data line contain the number of data expected
	if(splitLine->length() >= serializedLength)
	{
		appendEmptyLine();
		timeStamp.last().date		= (*splitLine)[0];
		timeStamp.last().ms			= (*splitLine)[1].toInt();
		baList.last()->voltage		= (*splitLine)[2].toInt();
		baList.last()->current		= (*splitLine)[3].toInt();
		baList.last()->temp			= (*splitLine)[4].toInt();
		baList.last()->pushbutton	= (*splitLine)[5].toInt();
		baList.last()->status		= (*splitLine)[6].toInt();
	}
}

void BatteryDevice::clear(void)
{
	FlexseaDevice::clear();
	baList.clear();
	timeStamp.clear();
}

void BatteryDevice::appendEmptyLine(void)
{
	timeStamp.append(TimeStamp());
	baList.append(new battery_s());
}

void BatteryDevice::decodeLastLine(void)
{
	if(dataSource == LiveDataFile){decompressRawBytes(baList.last());}
	decode(baList.last());
}

void BatteryDevice::decodeAllLine(void)
{
	for(int i = 0; i < baList.size(); ++i)
	{
		decode(baList[i]);
	}
}

QString BatteryDevice::getStatusStr(int index)
{
	return QString("No decoding available for this board");
}

void BatteryDevice::decode(struct battery_s *baPtr)
{
	baPtr->decoded.voltage = baPtr->voltage;    //TODO mV
	baPtr->decoded.current = baPtr->current;    //TODO mA
	baPtr->decoded.power = baPtr->voltage * baPtr->current; // mW
	baPtr->decoded.temp = baPtr->temp;          //TODO C*10
}

void BatteryDevice::decompressRawBytes(struct battery_s *baPtr)
{
	//Raw bytes to raw values:
	//========================

	baPtr->status = baPtr->rawBytes[0];
	baPtr->voltage = (baPtr->rawBytes[2] << 8) + baPtr->rawBytes[3];
	baPtr->current = (baPtr->rawBytes[4] << 8) + baPtr->rawBytes[5];
	baPtr->temp = baPtr->rawBytes[6];
}

//****************************************************************************
// Public slot(s):
//****************************************************************************


//****************************************************************************
// Private function(s):
//****************************************************************************


//****************************************************************************
// Private slot(s):
//****************************************************************************

