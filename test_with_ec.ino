#define SensorPin A2 //pH meter Analog output to Arduino Analog Input 0

#define Offset 0.0 //deviation compensate

#define samplingInterval 20

#define printInterval 800

#define ArrayLenth 40 //times of collection
int pHArray[ArrayLenth]; //Store the average value of the sensor feedback
int pHArrayIndex=0;
static float pHValue = 0;
#include <SoftwareSerial.h>
#include "DHT.h"

#define DHTPIN 2   // DEFINE DHT Sensor pin
#define DHTTYPE DHT11   // DHT 
//lcd
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define RX 10
#define TX 11
String AP = "Icamp";       // CHANGE ME
String PASS = "letshack"; // CHANGE ME
String API = "GXF73G8MFJLK3836";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field1 = "field1";
String field2 = "field2";
String field3 = "field3";
String field4 = "field4";
String field5 = "field5";
String field6 = "field6";
String field7 = "field7";
String field8 = "field8";
int countTrueCommand;
int countTimeCommand; 
boolean found = false; 
int valSensor = 1;
SoftwareSerial esp8266(RX,TX);
DHT dht(DHTPIN, DHTTYPE); 
 
  
void setup() {
  Serial.begin(9600);

  //lcd
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Soil Health");
  lcd.setCursor(0,1);
  lcd.print("Monitoring Kit");
  

  
  
  //Moisture sensor Configuration
  pinMode(A0, INPUT);

  //Start DHT process
  dht.begin();

  //Wifi module config.
  esp8266.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}
void loop(void) {
  
  //pHValue = phfunction();
  
  pHValue = 6.56;
 


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  
  // Check if any reads failed and exit early (to try again).?
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  
  
 //moisture sensor read data
 int moisture = analogRead(A0);
 String EC,N,P,K;
 
 if(moisture >= 1024){
  Serial.println("Sensor is in air");
  EC = "Sensor is in air";
  
 }
 else if(moisture >= 800 && moisture <= 1024){
  Serial.println("Very Low Electrical Conductivity");
  EC = "Very Low EC";
   
 }
 else if(moisture >= 300 && moisture <= 800){
  Serial.println("Low Electrical Conductivity");
  EC = "Low EC";
   
 }
 else if(moisture <= 300){
  Serial.println("High Electrical Conductivity");
  EC = "High EC";
   
 }
 
 if(pHValue>=4 && pHValue<=4.5){
  Serial.println("Ithu work aaguthu");   
  N= "30%";
  P= "23%";
  K= "33%";
  }
 else if(pHValue>=4.6 && pHValue<=5.0){
           
  
  N= "53%";
  P= "34%";
  K= "52%";
  }
 else if(pHValue>=5.1 && pHValue<=5.5){
      
  N= "77%";
  P= "48%";
  K= "77%";
  }
 else if(pHValue>=5.6 && pHValue<=6.99){
      
  Serial.println("Ithu work aaguthu");
  N= "89%";
  P= "52%";
  K= "100%";
  }
  else if(pHValue==7 ){
 
  Serial.println("\nNitrogen = 100% \n Phosphorus = 100% \n Potassium = 100% \n ");
  N= "100%";
  P= "100%";
  K= "100%";
  }
 
 
//lcd
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Soil Moisture");
lcd.setCursor(0,1);
lcd.print(moisture);
lcd.print("  m^3");
delay(2000); 
lcd.clear(); 
lcd.setCursor(0,0);
lcd.print("Temperature");
lcd.setCursor(0,1);
lcd.print(t);
lcd.print("Deg Celsius");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Heat Index");
lcd.setCursor(0,1);
lcd.print(hif);
lcd.print("Deg Celsius");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("pH Value");
lcd.setCursor(0,1);
lcd.print(pHValue);
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Electrical conductivity");
lcd.setCursor(0,1);
lcd.print(EC);
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("For Paddy");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Nitrogen");
lcd.setCursor(0,1);
lcd.print(N);
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Phosphorus");
lcd.setCursor(0,1);
lcd.print(P);
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Potassium");
lcd.setCursor(0,1);
lcd.print(K);
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Required Nitrogen");
lcd.setCursor(0,1);
lcd.print("217 kg/ha");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Required Phosporous");
lcd.setCursor(0,1);
lcd.print("68 kg/ha");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Required Potassium");
lcd.setCursor(0,1);
lcd.print("258 kg/ha");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Add Urea");
lcd.setCursor(0,1);
lcd.print("30.43 Kg");
delay(2000);
lcd.clear();
lcd.print("Ammonium polyPhosphate");
lcd.setCursor(0,1);
lcd.print("94.117 Kg");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Latitude");
lcd.setCursor(0,1);
lcd.print("11.08");
delay(2000);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Longitude");
lcd.setCursor(0,1);
lcd.print("76.98");
delay(2000);
lcd.clear();
lcd.print("Gathering Info");
lcd.setCursor(0,1);
lcd.print("Please Wait :-)");




 

 //To push data to thingspeak
 String getData1 = "GET /update?api_key="+ API +"&"+ field1 +"="+String(moisture)+"&"+ field2 +"="+String(t)+"&"+ field3 +"="+String(hif)+"&"+ field4 +"="+String(pHValue,2)+"&"+ field6 +"="+String(EC);
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData1.length()+4),4,">");
 esp8266.println(getData1);delay(1000);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");


 
/* String getData7 = "GET /update?api_key="+ API +"&"+ field7 +"="+String("11.08");
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData7.length()+4),4,">");
 esp8266.println(getData7);delay(1000);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");

 String getData8 = "GET /update?api_key="+ API +"&"+ field8 +"="+String("76.98");
 sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData8.length()+4),4,">");
 esp8266.println(getData8);delay(1000);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");*/








}
double avergearray(int* arr, int number){

int i;

int max,min;

double avg;

long amount=0;

if(number<=0){

Serial.println("Error number for the array to avraging!/n");

return 0;

}

if(number<5){ //less than 5, calculated directly statistics

for(i=0;i<number;i++){

amount+=arr[i];

}

avg = amount/number;

return avg;

}

else{

if(arr[0]<arr[1]){

min = arr[0];max=arr[1];

}

else{

min=arr[1];max=arr[0];

}

for(i=2;i<number;i++){

if(arr[i]<min){

amount+=min; //arr<min

min=arr[i];

}

else {

if(arr[i]>max){

amount+=max; //arr>max

max=arr[i];

}

else{

amount+=arr[i]; //min<=arr<=max

}

}

}

avg = (double)amount/(number-2);

}

return avg;


}

//Send to cloud funnction definition
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    esp8266.println(command);//at+cipsend
    if(esp8266.find(readReplay))//ok
    {
      found = true;
      break;
    }
  
    countTimeCommand++;
  }
  
  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  
  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  
  found = false;
 }
 static float phfunction(void)
 {
  static float value;
  while(pHArrayIndex <= ArrayLenth)
  {
  static unsigned long samplingTime = millis();

static unsigned long printTime = millis();

static float pHValue,voltage;

if(millis()-samplingTime > samplingInterval){

pHArray[pHArrayIndex++]=analogRead(SensorPin);

if(pHArrayIndex==ArrayLenth)pHArrayIndex=0;

voltage = avergearray(pHArray, ArrayLenth)*5.0/1024;

pHValue = 3.5*voltage+Offset;

samplingTime=millis();

}

if(millis() - printTime > printInterval){ //Every 800 milliseconds, print a numerical, convert the state of the LED indicator

Serial.print("Voltage:");

Serial.print(voltage,2);

Serial.print(" pH value: ");

Serial.println(pHValue,2);

printTime=millis();
value = pHValue;
break;

}
  }
  return value;
 }
