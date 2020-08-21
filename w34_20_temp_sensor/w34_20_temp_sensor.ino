/*
Copyright (C) 2020  Janos Peter Toth

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#define MAXTRTABLE 50  //temperature lookup table elements
int ain[6]= {A0,A1,A2,A3,A4,A5};

int PinLED = 13;      // Usually a built-in LED
int sensorValue;  // variable to store the value coming from the sensor
float R_pullup,R_temp,temp;
int AIMAX=1023; //determined by the resolution of the AD
float R1=10000.0; //Reference resistor value
int PinAbort = A0; //pin names of the GRBL shield
int PinHOLD = A1;
int PinResume = A2;
int PinCoolEn =A3;


struct tr //NTC resistance values at certain temperatures
{
  int dtemp;
  float dresistance;
};
//
struct tr tr_table[MAXTRTABLE]=
{
{-40,332094.0},
{-35,239900.0},
{-30,175200.0},
{-25,129287.0},
{-20,96358.0},
{-15,72500.0},
{-10,55046.0},
{-5,42157.0},
{0,32554.0},
{5,25339.0},
{10,19872.0},
{15,15698.0},
{20,12488.0},
{25,10000.0},
{30,8059.0},
{35,6535.0},
{40,5330.0},
{45,4372.0},
{50,3605.0},
{55,2989.0},
{60,2490.0},
{65,2084.0},
{70,1753.0},
{75,1481.0},
{80,1256.0},
{85,1070.0},
{90,915.4},
{95,786.0},
{100,677.3},
{105,585.7},
{110,508.3},
{115,442.6},
{120,386.6},
{125,338.7},
{130,297.7},
{135,262.4},
{140,231.9},
{145,205.5},
{150,182.6},
{155,162.7},
{160,145.3},
{165,130.1},
{170,116.7},
{175,104.9},
{180,94.6},
{185,85.4},
{190,77.3},
{195,70.1},
{200,63.7}
};


void setup() {
  int i;
  Serial.begin(115200);

  for(i=0;i<6;i++) 
    {
      pinMode(ain[i],INPUT);
      digitalWrite(ain[i],HIGH);  
    }
  pinMode(PinLED, OUTPUT);
}

void loop() {
  int k;
  float l;
  // read the value from the sensor:
 
  for(k=0;k<6;k++)
  {
    sensorValue = analogRead(ain[k]);
    Serial.print("A");
    Serial.print(k);
    Serial.print("=");
    Serial.print(sensorValue);
    Serial.print(" ");
  }

 sensorValue=analogRead(PinCoolEn);
 Serial.print("U2=");
 Serial.print(sensorValue);
 Serial.print(" R_pullup=");
 l=measure_pullup(PinCoolEn);
 Serial.print(l);
 Serial.print(" Sensor resistance=");
 R_temp=measure_temp(PinAbort);
 Serial.print(R_temp);
 Serial.print(" Temperature details=");
 Serial.print(get_temp(PinAbort));
 Serial.println();
 digitalWrite(PinLED,HIGH);
 delay(150);
 digitalWrite(PinLED,LOW);
 delay(150);
 
}

float get_temp(int pin_to_measure)

{
  int tlower,tupper,k;
  float rrr,rupper,rlower,temperature;
  rrr=measure_temp(pin_to_measure);
  for (k=0;k<MAXTRTABLE;k++)
  {
    if(rrr>tr_table[k].dresistance) break;
  }
  Serial.print(k);
  Serial.print(" ");
  tlower=tr_table[k].dtemp;
  tupper=tr_table[k+1].dtemp;
  Serial.print(tlower);
  Serial.print(" ");
  Serial.print(tupper);
  Serial.print(" ");
  rlower=tr_table[k].dresistance;
  rupper=tr_table[k+1].dresistance;
  Serial.print(rlower);
  Serial.print(" ");
  Serial.print(rupper);
  Serial.print(" ");
  temperature=((rrr-rlower)/(rlower-rupper))*(tupper-tlower)+tlower;
  return(temperature);
}

float measure_temp(int pin_to_measure)
{
 sensorValue=analogRead(pin_to_measure);
 R_temp=R_pullup*sensorValue/(AIMAX-sensorValue);
 return(R_temp);
 
}

float measure_pullup(int pin_to_measure)
{
 sensorValue=analogRead(pin_to_measure);
 R_pullup=R1*(AIMAX-sensorValue)/sensorValue;
 return(R_pullup);

}
