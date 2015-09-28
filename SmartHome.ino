    // подключаем библиотеки
    #include <dht.h>
    #include <Wire.h>
    #include <BH1750.h>

   // #define DEBUG 0
    //     const int BUFFER_SIZE = 128;
   //String HTMLGoodResponse = "HTTP/1.1 200 OK\r\nConnection: close\r\n";
   //String TableStartHTML = "<html><body><table cellpadding=\"5\" style=\"background-color:#440044;border-style:hidden;color:#ffaaff\" align=center><tbody><tr>";
   //String TableHeaderHTML = "<td>Time</td>\n<td>Termperature</td>\n<td>Humidity</td>\n<td>LIght</td>\n<td>Floodance</td>\n<td>Inhabitance</td>\n</tr><tr>";
   //String TableParameterStartHTML = "<td>";
   //String TableParameterFinishHTML = "</td>";
   //String FinishHTML = "</tr>\n</tbody>\n</table>\n</body></html>";
 
    DHT dht11 = DHT(); // Объект-сенсор
    BH1750 lightMeter;
    int WaterSensor = A3;
    int motionPin = 12;
  
    int led = 13;
    int temperature = 23;
    int humidity = 37;
    int light = 10;
    bool flooded = false;
    int ws = 10;
    bool motion;
    
    void setup()
    {        Serial.begin(115200);
//      return;

        //Serial.begin(9600);
        dht11.attach(A0); delay(1000);
        lightMeter.begin();

        pinMode(led,OUTPUT);
        

        Serial.println("AT+RST\r\n");   delay(800);
        Serial.println("AT+CWMODE=1\r\n"); delay(800);
        Serial.println("AT+CIPSTA=\"192.168.43.77\"\r\n"); delay(800);
        Serial.println("AT+CIPSTAMAC=\"66:66:66:66:66:88\"\r\n"); delay(800);
       //Serial.println("AT+CWJAP=\"bias\",\"yamat0nade$ik0Figslomaesh!\"");
        //Serial.println("AT+CWJAP=\"Frosters\",\"!#DidYouTryHard?!A\"\r\n");
        Serial.println("AT+CWJAP=\"utelGavno\",\"6058372d5618\"\r\n");
        //Serial.println("AT+CWJAP=\"RTK-110407\",\"ELTX1A019338\"");

        
          delay(6000);
            blinkcode ();
        Serial.println("AT+CIPMUX=1\r\n");
        delay(1000);
        Serial.println("AT+CIPSERVER=1,80\r\n");
		delay(1000);
    }
     
     
             //char buf[BUFFER_SIZE];
    void loop()
    {
//        memset(buf, 0, BUFFER_SIZE);
//        Serial.readBytesUntil('\n', buf, BUFFER_SIZE);
//        String req(buf);
//        if(req.indexOf("G", 0)>-1) {
//        PrintToTCP("HTTP/1.1 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\nTransfer-Encoding: chunked\r\n\r\n<html><body><h1>HELLO BITCH!</h1></body></html>\r\n\r\n");
//        delay(1000);        
//        SayToESP("AT+CIPCLOSE=0,80\r\n", 300, DEBUG);
//        SayToESP("AT+CIPSERVER=1,80\r\n", 300, DEBUG);   
        //}
      
        //while(Serial.available())
        //  Serial.read();
       
       
        //SendHTMLPageToTCP();
        UpdateSensors();
        WriteSensorsDataToTCP();
		delay(2000);
    }
 
 //String page;
 //void SendHTMLPageToTCP(){
   //page =  " ";
//  PrintToTCP(HTMLGoodResponse);
//  PrintToTCP(TableStartHTML);
//  PrintToTCP(TableHeaderHTML);
   
//  PrintToTCP(TableParameterStartHTML);
//  PrintToTCP("time");
//  PrintToTCP(TableParameterFinishHTML);
   
//  PrintToTCP(TableParameterStartHTML);
//  PrintToTCP(String(temperature));
// PrintToTCP(TableParameterFinishHTML);
   
//  PrintToTCP(TableParameterStartHTML);
//  PrintToTCP(String(humidity));
//  PrintToTCP(TableParameterFinishHTML);
   
//  PrintToTCP(TableParameterStartHTML);
//  PrintToTCP(String(light));
// PrintToTCP(TableParameterFinishHTML);
   
//  PrintToTCP(TableParameterStartHTML);
//  PrintToTCP(String(flooded));
//  PrintToTCP(TableParameterFinishHTML);
   
//  PrintToTCP(TableParameterStartHTML);
//  PrintToTCP(String(motion));
//  PrintToTCP(TableParameterFinishHTML);  
// }
 
  
void  WriteSensorsDataToTCP(){
	String msg = "Актуальные данные мониторинга: \r\n";
	PrintToTCP(msg);
	
	msg = "Температура: ";
    msg+=temperature; msg+="C\r\n";
    PrintToTCP(msg);
    
	msg="Влажность: ";
    msg+=humidity; msg+=" %\r\n";
    PrintToTCP(msg);
    
	msg="Освещённость: ";
    msg+=light; msg+=" lx\r\n";
    PrintToTCP(msg);
    
    msg="Затопленность: ";
    if(!flooded){msg+= "НЕТ (";}
    else {msg+"ДА (";}
    msg+=ws; msg+=")\r\n";
    PrintToTCP(msg);
    
    msg="Наличие движения: ";
    if(!motion){msg+= "НЕТ\r\n";}
    else{msg+="ДА\r\n";}
    PrintToTCP(msg);
    
    msg="\r\n";
    PrintToTCP(msg);
}
 
void PrintToTCP(String str){
  String cmdmsg="AT+CIPSEND=0,";
  cmdmsg+=str.length();
  cmdmsg+="\r\n";
  SayToESP(cmdmsg, 300);
  SayToESP(str, 300);
}
 
 
    
void UpdateSensors(){
  // метод update заставляет сенсор выдать текущие измерения
      dht11.update();
      if(dht11.getLastError() == DHT_ERROR_OK){
          // данные последнего измерения можно считать соответствующими
          // методами
          //sprintf(msg, "Temperature = %dC, Humidity = %d%%", 
           temperature = dht11.getTemperatureInt();
           humidity = dht11.getHumidityInt();
       }
       
      light = lightMeter.readLightLevel();
      
      ws = analogRead(WaterSensor);
      flooded = (ws > 100);
      
      motion = (digitalRead(motionPin) == HIGH);
}


  void blinkcode ()
  {
    //Simply loop a few times flashing the status light (this is used during boot up)
    int i;
    for (i = 1; i <= 10; i++){
      delay (50);
      digitalWrite(led,HIGH);
      delay (50);
      digitalWrite(led,LOW);
    }
  }
  
  
  String SayToESP(String command, const int timeout/*, boolean debug*/)
{
    String response = "";
    
    Serial.print(command);
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(Serial.available())
      {
        // The esp has data so display its output to the serial window 
        char c = Serial.read(); // read the next character.
        response+=c;
      }  
    }
    
    return response;
}
