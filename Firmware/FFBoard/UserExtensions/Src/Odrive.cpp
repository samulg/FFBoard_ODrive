/*
 * Odrive.cpp
 *
 *  Created on: Mar 17, 2021
 *      Author: Samullg
 */

#include <Odrive.h>
#ifdef ODRIVE

/*
 * Contains motor drivers based on Odrive
 */
ClassIdentifier Odrive::info = {
		 .name = "Odrive" ,
		 .id=5
 };
const ClassIdentifier Odrive::getInfo(){
	return info;
}

void Odrive::Odrive(){
	char out[]= "w axis0.requested_state  AXIS_STATE_FULL_CALIBRATION_SEQUENCE \n";
	HAL_UART_Transmit(huart, (uint8_t *) out, strlen(out), 20);
}
void Odrive::turn(int16_t power)
{

}
void Odrive::stop()
{
	this.setAxisState(AXIS_STATE_IDLE);

}
void Odrive::start()
{
	this.setParam("axis0.motor.config.current_lim", this.current_lim);
	this.setParam("axis0.controller.config.vel_limit", this.vel_limit);
	this.setParam("axis0.motor.config.calibration_current", this.calibration_current);
	this.setParam("config.brake_resistance", this.brake_resistance);
	this.setParam("axis0.motor.config.pole_pairs", this.pole_pairs);
	this.setParam("axis0.motor.config.torque_constant", this.torque_constant);
	this.setParam("axis0.motor.config.motor_type", this.motor_type);
	this.setParam("axis0.encoder.config.cpr", this.cpr);

	this.setAxisState(AXIS_STATE_FULL_CALIBRATION_SEQUENCE);

	while(this.getAxisState()!= AXIS_STATE_IDLE)
	{
		HAL_Delay(50);
	}

	this.setAxisState(AXIS_STATE_CLOSED_LOOP_CONTROL);

}

void  Odrive::setAxisState(AxisState state)
{
	this.setParam("axis0.requested_state", state);
}
AxisState Odrive::getAxisState()
{
	return (AxisState) this.getParam("axis0.current_state");
}

void  Odrive::setParam (string param, int value)
{
	string out = "w ";
	out.append(param);
	out.append(" ");
	out.append(value);
	out.append('\n');
	HAL_UART_Transmit(huart, (uint8_t *) out, strlen(out), 20);
}
void  Odrive::setParam (string param, float value)
{
	string out = "w ";
	out.append(param);
	out.append(" ");
	out.append(value);
	out.append('\n');
	HAL_UART_Transmit(huart, (uint8_t *) out, strlen(out), 20);
}
float  Odrive::getParam (string param)
{
	string out = "r ";
	out.append(param);
	out.append("\n");
	HAL_UART_Transmit(huart, (uint8_t *) out, strlen(out), 20);
	string in;
	HAL_UART_Receive(huart, (uint8_t *)in, 128, 500);
	return (float)in;

}

void Odrive::getFeedback (float* pos, float* vel)
{
	out="f 0";
	HAL_UART_Transmit(huart, (uint8_t *) out, strlen(out), 20);
	string in;
	HAL_UART_Receive(huart, (uint8_t *)in, 128, 500);
	sscanf(in,"%f %f", pos, vel);
}

void Odrive::setTorque (float torque)
{
	string out = "c 0 ";
	out.append(torque);
	out.append ("\n");
	HAL_UART_Transmit(huart, (uint8_t *) out, strlen(out), 20);
}

void Odrive::setControlMode (ControlMode mode)
{
	this.setParam ("axis0.controller.config.control_mode", mode);
}

ControlMode  Odrive::getControlMode ()
{
	return (ControlMode) this.getParam("axis0.controller.config.control_mode");
}

#endif
