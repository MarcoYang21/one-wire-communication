#include <Arduino.h>
#define dataPin 2 // data pin is used the interrupt
#define ledPin 13 // led pin

extern volatile unsigned long timer0_millis;
unsigned long new_value = 0;

volatile unsigned long timerCount= 0;
volatile unsigned long currentTimer = 0;
int dataIndex =0; //specify the bit array index
byte dataArray[14]; //one wire data buffer
bool dataUpdate;  // info loop to output the one wire data
unsigned int arrayIndex = 0; //specify the data array index
unsigned int bitIndex = 0;   //指定第幾個bit需要更新
int tempData = 0;            //合併資料的暫存區
byte printData = 0;          //輸出資料的暫存區

void setMillis(unsigned long new_millis){
  uint8_t oldSREG = SREG;
  cli();
  timer0_millis = new_millis;
  SREG = oldSREG;
  timerCount = micros();//時間更新
}

void dataCollect() { //ISR的事件
  currentTimer = micros()-timerCount; //計算時間區
  if(currentTimer>50000){             //大於50mS為資料觸發bit
    digitalWrite(ledPin,HIGH);        //點亮LED
    dataIndex = 0;                    //設定資料初值
    bitIndex = 0;                     //設定資料初值
    timerCount = micros();            //時間更新
  }
  else if(currentTimer > 1500){       //與前一上沿區間超過1.5mS，代表此bit為0
    arrayIndex = dataIndex/8;         //計算第幾個byte
    bitIndex = dataIndex % 8 ;        //計算第幾個bit 
    tempData = dataArray[arrayIndex]; //取出該byte資料
    tempData = tempData | (0<<bitIndex); //更新bit資料
    dataArray[arrayIndex] = tempData;   //資料寫回data array
    dataIndex++;                           //bit data index+1
  }
  else{                                //不在上述區間，代表此bit為1
    arrayIndex = dataIndex/8;         //計算第幾個byte
    bitIndex = dataIndex % 8 ;        //計算第幾個bit 
    tempData = dataArray[arrayIndex]; //取出該byte資料
    tempData = tempData | (1<<bitIndex); //更新bit資料
    dataArray[arrayIndex] = tempData;   //資料寫回data array
    dataIndex++;                           //bit data index+1
  }
  if(dataIndex >= 112){                    //收集到14byte(112bit)則更新資料
    dataUpdate = true;                     //輸出資料Flag
    digitalWrite(ledPin,LOW);              //關閉LED
    }
  setMillis(new_value);
  }


void setup() {
  timerCount = micros();                    //設定計時器
  Serial.begin(115200);                     //設定輸出port.
  pinMode(ledPin,OUTPUT);                   //設定LED腳位
  pinMode(dataPin,INPUT_PULLUP);            //設定on wire data pin
  attachInterrupt(digitalPinToInterrupt(dataPin),dataCollect,RISING); //設定中斷腳位
  currentTimer = 0;                         //初始化時間暫存器
}

void loop() {
  if(dataUpdate==true){  //更新資料flag
    for(int i = 0; i < sizeof(dataArray) ; i++ ){
      printData = dataArray[i];
      Serial.write(printData);                //輸出資料陣列
      }
    dataUpdate = false;                       //關閉資料更新flag.
  } 
}
