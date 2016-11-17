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
	[This file] w_execute.h: Execute View Window
*****************************************************************************
	[Change log] (Convention: YYYY-MM-DD | author | comment)
	* 2016-09-09 | jfduval | Initial GPL-3.0 release
	*
****************************************************************************/

//****************************************************************************
// Include(s)
//****************************************************************************

#include "w_execute.h"
#include "flexsea_generic.h"
#include "ui_w_execute.h"
#include "main.h"
#include <QString>
#include <QTextStream>
#include <QDebug>

//****************************************************************************
// Constructor & Destructor:
//****************************************************************************

W_Execute::W_Execute(QWidget *parent, QList<struct execute_s> *logRef, DisplayMode mode) :
	QWidget(parent),
	ui(new Ui::W_Execute)
{
	ui->setupUi(this);

	myExecute_s = logRef;
	setWindowTitle("Execute - Barebone");
	setWindowIcon(QIcon(":icons/d_logo_small.png"));

	displayMode = mode;
	updateDisplayMode(displayMode);
}

W_Execute::~W_Execute()
{
	emit windowClosed();
	delete ui;
}

//****************************************************************************
// Public function(s):
//****************************************************************************

void W_Execute::trackVarEx(uint8_t var, uint8_t *varToPlotPtr8s)
{
	if(var == 0)
	{
		varToPlotPtr8s = &exec1.volt_batt;
	}
}

//****************************************************************************
// Public slot(s):
//****************************************************************************

//Call this function to refresh the display
void W_Execute::refresh(void)
{
	struct execute_s *exPtr;
	FlexSEA_Generic::assignExecutePtr(&exPtr, SL_BASE_ALL, \
									   ui->comboBox_slave->currentIndex());
	displayExecute(exPtr);
}

void W_Execute::displayLogData(int index)
{
   if(myExecute_s->isEmpty() == false)
   {
		displayExecute(&(*myExecute_s)[index]);
   }
}

void W_Execute::updateDisplayMode(DisplayMode mode)
{
	if(mode == DisplayLogData)
	{
		initLog();
	}
	else
	{
		initLive();
	}
}

//****************************************************************************
// Private function(s):
//****************************************************************************

void W_Execute::initLive(void)
{
	//Populates Slave list:
	ui->comboBox_slave->clear();
	FlexSEA_Generic::populateSlaveComboBox(ui->comboBox_slave, \
											SL_BASE_EX, SL_LEN_EX);
}

void W_Execute::initLog(void)
{
	//Populates Slave list:
	ui->comboBox_slave->clear();
	ui->comboBox_slave->addItem("Log 1");
	displayLogData(0);
}

void W_Execute::displayExecute(struct execute_s *ex)
{
	int combined_status = 0;

	//Raw values:
	//===========

	ui->disp_accx->setText(QString::number(ex->accel.x));
	ui->disp_accy->setText(QString::number(ex->accel.y));
	ui->disp_accz->setText(QString::number(ex->accel.z));
	ui->disp_gyrox->setText(QString::number(ex->gyro.x));
	ui->disp_gyroy->setText(QString::number(ex->gyro.y));
	ui->disp_gyroz->setText(QString::number(ex->gyro.z));

	ui->disp_encDisplay->setText(QString::number(ex->enc_display));

	ui->disp_strain->setText(QString::number(ex->strain));
	ui->disp_ana->setText(QString::number(ex->analog[0]));
	ui->disp_ana1->setText(QString::number(ex->analog[1]));

	ui->disp_current->setText(QString::number(ex->current));

	ui->disp_vb->setText(QString::number(ex->volt_batt));
	ui->disp_vg->setText(QString::number(ex->volt_int));
	ui->disp_temp->setText(QString::number(ex->temp));

	combined_status = (ex->status2 << 8) & ex->status1;
	ui->disp_stat1->setText(QString::number(combined_status));

	//Decoded values:
	//===============

	ui->disp_accx_d->setText(QString::number((float)ex->decoded.accel.x/1000,'f',2));
	ui->disp_accy_d->setText(QString::number((float)ex->decoded.accel.y/1000,'f',2));
	ui->disp_accz_d->setText(QString::number((float)ex->decoded.accel.z/1000,'f',2));

	ui->disp_gyrox_d->setText(QString::number((double)ex->decoded.gyro.x, 'i', 0));
	ui->disp_gyroy_d->setText(QString::number((double)ex->decoded.gyro.y, 'i', 0));
	ui->disp_gyroz_d->setText(QString::number((double)ex->decoded.gyro.z, 'i', 0));

	ui->disp_current_d->setText(QString::number(ex->decoded.current, 'i',0));
	ui->disp_vb_d->setText(QString::number((float)ex->decoded.volt_batt/1000, 'f',2));
	ui->disp_vg_d->setText(QString::number((float)ex->decoded.volt_int/1000, 'f',2));
	ui->disp_temp_d->setText(QString::number((float)ex->decoded.temp/10,'f',1));

	ui->disp_ana_d->setText(QString::number((float)ex->decoded.analog[0]/1000,'f',2));
	ui->disp_ana1_d->setText(QString::number((float)ex->decoded.analog[1]/1000,'f',2));

	ui->disp_strain_d->setText(QString::number(ex->decoded.strain,'i', 0));

	QString myStr;
	FlexSEA_Generic::decodeStatus(SL_BASE_EX, ui->comboBox_slave->currentIndex(), \
									  ex->status1, ex->status2, &myStr);
	ui->label_status1->setText(myStr);

	//==========
}

//****************************************************************************
// Private slot(s):
//****************************************************************************
