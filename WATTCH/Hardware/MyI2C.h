#ifndef __MYI2C_H
#define __MYI2C_H
void MYI2C_Init(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void sendByte(uint8_t Byte);
uint8_t receiveByte(void);
void sendAck(uint8_t ACKbit);
uint8_t receiveAck(void);
#endif
