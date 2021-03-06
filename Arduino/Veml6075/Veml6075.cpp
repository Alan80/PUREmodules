#include <Arduino.h>
#include "Veml6075.h"

Veml6075::Veml6075() {
	//empty constructor
}


/**
 * Writes data over i2c
 */
void Veml6075::write_2bytes(uint8_t addr, uint8_t subAddress, uint8_t data_LSB, uint8_t data_MSB){
    
    Wire.beginTransmission(addr);       //Send Start Signal and Address over i2c
    Wire.write(subAddress);             //Subaddress is the register to write to.
    Wire.write(data_LSB);
    Wire.write(data_MSB);                   //Send Data
    Wire.endTransmission();             //End I2c communication
}


/**
 * Read two byte from I2C. Used for sensor data.
 */
//The read is flipped from the others. LSB first then MSB
 int Veml6075::read_2bytes(uint8_t addr, uint8_t subAddress){
    uint16_t val = -1;
    Wire.beginTransmission(addr);        //Send Start Signal and Address over i2c
    Wire.write(subAddress);              //SubAddress is the register to read from
    Wire.endTransmission(false);

    Wire.requestFrom(addr,2);            //Request two bytes of data from Sensor
    while(Wire.available()==0);
    val = Wire.read();                //First byte is LSB

    while(Wire.available()==0);
    val |= Wire.read()<<8;                  //Second byte is MSB
 
    return val;                          //Return Value
 }

uint8_t Veml6075::begin(){
    uint8_t UV_CONF_WORD = 0x00;

    UV_CONF_WORD  = (settings.UV_IT   <<4)  & 0x70;        
    UV_CONF_WORD |= (settings.HD      <<3)  & 0x08;
    UV_CONF_WORD |= (settings.UV_TRIG <<2)  & 0x04;
    UV_CONF_WORD |= (settings.UV_AF   <<1)  & 0x02;
    UV_CONF_WORD |= (settings.Veml6075_SD)  & 0x01;

    write_2bytes(Veml6075::DEVICE_ADDRESS,Veml6075::UV_CONF,UV_CONF_WORD,0x00);

} 

uint16_t Veml6075::whoami(){
    uint16_t who_am_i = read_2bytes(Veml6075::DEVICE_ADDRESS,Veml6075::ID);
    return who_am_i;
}

uint16_t Veml6075::readUV_CONF(){
    uint16_t UV_CONF = read_2bytes(Veml6075::DEVICE_ADDRESS,Veml6075::UV_CONF);
    return UV_CONF;
}

uint16_t Veml6075::readUVA_DATA(){
    uint16_t UVA_data = read_2bytes(Veml6075::DEVICE_ADDRESS,Veml6075::UVA_DATA);
    return UVA_data;
}

uint16_t Veml6075::readUVB_DATA(){
    uint16_t UVB_data = read_2bytes(Veml6075::DEVICE_ADDRESS,Veml6075::UVB_DATA);
    return UVB_data;
}
