/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Saifudheen
 *
 * Created on October 11, 2019, 7:38 PM
 */

#include <cstdlib>
#include<iostream>
#include "stdio.h"
#include <ctime>
#include <fstream>
#include <cstdlib>
//FILE *eeprom;
  //("foobar.bin", std::ios::binary);
 
 enum AlarmEvent {
      UNKNOWN=0,
      PUMP1_START=1,
      PUMP1_STOP=2,
      PUMP1_DRY_RUN=3,
      PUMP2_START=4,
      PUMP2_STOP=5,
      PUMP2_DRY_RUN=6,
      TANK1_LOW=7,
      TANK1_HIGH=8,
      TANK1_OK=9,
      TANK1_SENSOR_FAULT=10,
      TANK2_LOW=11,
      TANK2_HIGH=12,
      TANK2_OK=13,
      TANK2_SENSOR_FAULT =14,
      TANKUG_LOW=15,
      TANKUG_HIGH=16,
      TANKUG_OK=17,
      TANKUG_SENSOR_FAULT=18,
      PWR_SUPPLY_LOW=19,
      PWR_SUPPLY_HIGH=20, 
      PWR_LOSS=21,
      SYSTEM_START=22       
    };
#define uint8_t unsigned char   
#define uint16_t unsigned short
#define uint32_t unsigned int
    
#define byte unsigned char

#define eeprom_size 1024

template <typename TR> bool eeprom_read(const unsigned int address, const TR *data, unsigned int n);
template <typename TW> bool eeprom_write(const unsigned int address, const TW data, unsigned int n);

using namespace std;

uint16_t next_serial=1;
struct Alarm{
      uint16_t serial;
      uint32_t timestamp;
      AlarmEvent alarmevent; //Actual data
};

fstream eeprom;//  ("C:\\temp\\Test.bin", ios::in | ios::out | ios::binary);


Alarm makeRandomAlarm(time_t prev_timestamp){
    uint16_t rnd_alarm=rand() % 22;
    time_t  rnd_timestamp=prev_timestamp+rand()%7200;
    Alarm alarm;
    alarm.serial=next_serial;
    alarm.timestamp=rnd_timestamp;
    alarm.alarmevent=(AlarmEvent)rnd_alarm;
    next_serial++;
    return alarm;
}

int main(int argc, char** argv) {
         
    time_t result = std::time(nullptr);
    eeprom=fstream("test.bin",   ios::out | ios::binary);
          
#define arraysize 80

    Alarm alarm[arraysize];
    time_t tm=result;
    for (uint16_t i=0;i<arraysize;i++){
        alarm[i]=makeRandomAlarm(tm);
        tm=alarm[i].timestamp;        
    }
    printf("**********************************************");
    printf("Alarms written to file.");
    printf("**********************************************\n");
    for (uint16_t i=0;i<arraysize;i++){
        printf("Alarm Serial: %d, Timestamp: %u, Event %d: \n",(uint16_t)alarm[i].serial, alarm[i].timestamp,(uint8_t)alarm[i].alarmevent);         
    }
    eeprom_write(0,alarm,arraysize);
    //fill remaining byts wth 0xFF
    uint16_t bdata=arraysize*sizeof(Alarm);
    printf("\nSize of Alarm structure: %u\n",sizeof(Alarm));
    uint16_t rem=eeprom_size-bdata;
    uint8_t remdata[rem];
    for (uint16_t i=0;i<rem;i++){
        remdata[i]=0xFF;              
    }
    eeprom_write(bdata,remdata,rem);
    eeprom.close();
    eeprom=fstream("C:\\temp\\Test.bin",   ios::in | ios::binary);
    printf("----------------------------------------------");
    printf("Alarms read from file.");
    printf("----------------------------------------------\n");
    Alarm alarmR[arraysize];    
    eeprom_read(0,&alarmR[0],arraysize);
    for (uint16_t i=0;i<arraysize;i++){
        printf("Alarm Serial: %d, Timestamp: %u, Event %d: \n",(uint16_t)alarmR[i].serial, alarmR[i].timestamp,(uint8_t)alarmR[i].alarmevent);         
    }    
    printf("----------------------------------------------\n");
    //Alarm alarm3;
    //eeprom_read(15,&alarm3,1);
    //printf("Alarm Serial: %d, Timestamp: %u, Event %d: \n",(uint8_t)alarm3.serial, alarm3.timestamp,(uint8_t)alarm3.alarmevent);
    //printf("Timestamp read: %u\n",alarm3.timestamp);
    eeprom.close();
    
    return 0;
}

template <typename TR> bool eeprom_read(const unsigned int address, const TR *data, unsigned int n) {
     
    eeprom.seekg(address*sizeof(*data));
    eeprom.read((char*)data, sizeof(*data)*n); 
    return true;
}


template <typename TW> bool eeprom_write(const unsigned int address, const TW data, unsigned int n) {
    eeprom.seekg(address*sizeof(*data));    
    eeprom.write((char*) data, sizeof(*data)*n);
    printf("\nSize of Alarm structure: %u\n",sizeof(*data)*n);
    return true;
}

 
