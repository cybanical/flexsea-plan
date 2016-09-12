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

#ifndef W_EXECUTE_H
#define W_EXECUTE_H

//****************************************************************************
// Include(s)
//****************************************************************************

#include <QWidget>
#include "flexsea_generic.h"

//****************************************************************************
// Namespace & Class Definition:
//****************************************************************************

namespace Ui {
class W_Execute;
}

class W_Execute : public QWidget
{
    Q_OBJECT

public:
	//Constructor & Destructor:
    explicit W_Execute(QWidget *parent = 0);
    ~W_Execute();
	
	//Function(s):
    void refresh(void);

public slots:


private:
	//Variables & Objects:
    Ui::W_Execute *ui;
	FlexSEA_Generic myFlexSEA_Generic;
    int active_slave, active_slave_index;
	
	//Function(s):
	void init(void);
    void displayExecute(struct execute_s *ex);
    void status_byte_disp(uint8_t stat1, uint8_t stat2);
};

//****************************************************************************
// Definition(s)
//****************************************************************************

#define GET_WDCLK_FLAG(status1)     ((status1 >> 7) & 0x01)
#define GET_DISCON_FLAG(status1)    ((status1 >> 6) & 0x01)
#define GET_OVERTEMP_FLAG(status1)  ((status1 >> 4) & 0x03)
#define GET_VB_FLAG(status1)        ((status1 >> 2) & 0x03)
#define GET_VG_FLAG(status1)        ((status1 >> 0) & 0x03)
#define GET_3V3_FLAG(status2)       ((status2 >> 0) & 0x03)
#define GET_FSM_FLAG(status2)       ((status2 >> 7) & 0x01)

//Qualitative:
#define V_LOW                       1
#define V_NORMAL                    0
#define V_HIGH                      2
#define T_NORMAL                    0
#define T_WARNING                   1
#define T_ERROR                     2
#define BATT_CONNECTED              0
#define BATT_DISCONNECTED           1
//If everything is normal STATUS1 == 0

//PSoC 4 ADC conversions:
#define P4_ADC_SUPPLY               5.0
#define P4_ADC_MAX                  2048
#define P4_T0                       0.5
#define P4_TC                       0.01

//PSoC 5 ADC conversions:
#define P5_ADC_SUPPLY               5.0
#define P5_ADC_MAX                  4096

#endif // W_EXECUTE_H
