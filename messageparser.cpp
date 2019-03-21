#include <stdint.h>
#include <stddef.h>
#include "messageparser.h"
#define stx 0x01
#define str 0x02
#define mir 0x03
int16_t messageOut[0xFF], messageIn[0xff];
uint8_t devAdres[] = {0, 0, 0, 0};

int16_t comandIn = 0;
//Входящие сообщения
int16_t dataIn[0xFF];

//Очистить изходящее сообщение
void cleanMessageOut();
//Сгенерировать CRC
int16_t getCrc(int16_t *data);
//Очистить изходящее сообщение
void cleanDataIn();
//Проверить срц
uint8_t checkCrc(uint8_t *data);
uint8_t getLenth(uint8_t *data);


//_____________Входящие_______________//

//Проверить входящее сообщение.
uint8_t checkMessageIn(uint8_t *data)
{

    //проверяем адрес
    if(data[1]!=devAdres[0] || data[2]!=devAdres[1] ||data[3]!=devAdres[2] ||data[4]!=devAdres[3])
    {
        return 0;
    }
    //проверяем стартовый и стоповые байты
    else if(data[0] != stx || data[data[5]+10-2] != str)
    {
        return 0;
    }
    //проверяем crc
    else if(!checkCrc(data))
    {
        return 0;
    }

    return 1;
}




//Распарсить входящее сообщение, возвращяет код команды.
uint16_t setMessageIn(uint8_t *data)
{
    cleanDataIn();
    comandIn =  0 |data[6];
    comandIn = comandIn<<8;
    comandIn = data[7] | comandIn;
    int j=0;
    for(int i =8; i<data[5]+10-2; i++ )
    {
        if(data[i] != mir)
        {
            dataIn[j] = data[i];
            j++;
        }
        else
        {

        }

    }
    return comandIn;
}

//Получить ссылку на данные входящего сообщения.
int16_t* getDataIn()
{
    return dataIn;
}

//_____________Исходящие_______________//

//Сгенерировать изходящее сообщение.
int16_t* generateMessageOut(uint16_t command ,int16_t *data)
{
    cleanMessageOut();
    messageOut[0] = stx;
    messageOut[1] = devAdres[0];
    messageOut[2] = devAdres[1];
    messageOut[3] = devAdres[2];
    messageOut[4] = devAdres[3];
    //Lennth
    messageOut[6] = (command >> 8) & 0xff;
    messageOut[7] = command & 0xff;
    uint16_t i =0;
    int16_t lenth = 0;
    while(data[i]>=0)
    {
        if(data[i] == stx || data[i] == str || data[i] == mir)
        {
            messageOut[8+lenth] = mir;
            lenth++;
        }
        messageOut[8+lenth] = data[i];
        i++;
        lenth++;
    }
    messageOut[5] = lenth;
    messageOut[8+lenth] = str;
    messageOut[9+lenth] =getCrc(messageOut);
    return messageOut;
}

//_____________Приваты_______________//

//Очистить изходящее сообщение
void cleanMessageOut()
{
    for(uint8_t i=0; i<0xFF; i++)
    {
        messageOut[i] = -1;
    }

}

//Очистить изходящее сообщение
void cleanDataIn()
{
    for(uint8_t i=0; i<0xFF; i++)
    {
        dataIn[i] = -1;
    }

}
//Сгенерировать CRC
int16_t getCrc(int16_t *data)
{
    uint16_t i=0;
    uint8_t crc = 0;

    while(data[i]>=0)
    {
        crc = crc ^ (data[i]);
        i++;
    }
    return crc  & 0xff;
}

//Проверить CRC
uint8_t checkCrc(uint8_t *data)
{
    uint16_t i=0;
    uint8_t crc = 0;

    for(i =0; i<data[5]+10-1; i++ )
    {
        crc = crc ^ (data[i]);
    }
    return crc == data[i];
}

uint8_t getLenth(uint8_t *data)
{
    int i=0;
    while(data[i]>=0)
    {i++;}
    return i;
}
