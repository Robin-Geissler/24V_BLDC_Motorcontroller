# 24V BLDC Motorcontroller

This is a motorcontroller to control a 24V BLDC motor. In this project the controller was used to control a Faulhaber 4221 BXT R to power a 2DOF robot. The 6 Step control algorithm is used to control the motor. The motorcontroller implements the following control modes:
* Current Control
* Speed Control
* Position Control
* Torque Control

## Control Structure
A current controller and a speed controller are used as low level controllers for the position and torque controller. Each controlmode can be accessed by the user.

**Current Control** 
Uses currentsensors on the PCB to control the motor current. 

**Speed Control** 
Calculates the speed by analysing the HAL Sector change rate. 

**Position Control**
The position of the geaboxshaft is measured with an AksIM-2 absolute encoder from RLS. The control structure of the position controller looks like this:
![grafik](https://github.com/Robin-Geissler/24V_BLDC_Motorcontroller/assets/38495785/23616d61-726f-4471-8209-4c4ee24acf8d)



**Torque Control**
The torque applied to the gearbox shaft is measured with a TS70a torque sensor from ME-Meßsysteme. The control structure of the torque controller looks like this:
![grafik](https://github.com/Robin-Geissler/24V_BLDC_Motorcontroller/assets/38495785/867ea04d-c174-4072-b0a7-ee8d4df544fd)


## Comunication Interface
The controller provides an EtherCAT communication interface. Over the interface the controlmode can be set, and PID controlparameters for each controller can be manipulated.

## Hardware
A Microchip ATSAMD51G19A is used as an MCU. It controls a MP6540HA motordriver IC.
![grafik](https://github.com/Robin-Geissler/24V_BLDC_Motorcontroller/assets/38495785/42268a68-4c14-4efa-a114-c72518d6bee1)




