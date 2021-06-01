#include <SoftwareSerial.h>

#define DEBUG true

SoftwareSerial esp8266(11,10); // make RX Arduino line is pin 2, make TX Arduino line is pin 3.
                             // This means that you need to connect the TX line from the esp to the Arduino's pin 2
                             // and the RX line from the esp to the Arduino's pin 3
 
String sendCommand(String command, const int timeout, boolean debug)
{
    String response = "";
           
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}


String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    int dataSize = command.length();
    char data[dataSize];
   command.toCharArray(data,dataSize);
          
   esp8266.write(data,dataSize); // send the read character to the esp8266
   if(debug)
   {
     Serial.println("\r\n====== HTTP Response From Arduino ======");
     Serial.write(data,dataSize);
     Serial.println("\r\n========================================");
   }
   
   long int time = millis();
   
   while( (time+timeout) > millis())
   {
     while(esp8266.available())
     {
       
       // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}


void sendCIPData(int connectionId, String data)
{
   String cipSend = "AT+CIPSEND=";
   cipSend += connectionId;
   cipSend += ",";
   cipSend +=data.length();
   cipSend +="\r\n";
   sendCommand(cipSend,1000,DEBUG);
   sendData(data,1000,DEBUG);
}
 

void sendHTTPResponse(int connectionId, String content)
{
     
     // build HTTP response
     String httpResponse;
     String httpHeader;
     // HTTP Header
     httpHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n"; 
     httpHeader += "Content-Length: ";
     httpHeader += content.length();
     httpHeader += "\r\n";
     httpHeader +="Connection: close\r\n\r\n";
     httpResponse = httpHeader + content + " "; // There is a bug in this code: the last character of "content" is not sent, I cheated by adding this extra space
     sendCIPData(connectionId,httpResponse);
}
              
void setup()
{
 Serial.begin(9600);
 esp8266.begin(9600); // your esp's baud rate might be different
 /*
 pinMode(11,OUTPUT);
 digitalWrite(11,LOW);
 
 pinMode(12,OUTPUT);
 digitalWrite(12,LOW);
 
 pinMode(13,OUTPUT);
 digitalWrite(13,LOW);
 
 pinMode(10,OUTPUT);
 digitalWrite(10,LOW);
  */
 pinMode(13,OUTPUT);
 digitalWrite(13,LOW);

  
 sendCommand("AT+RST\r\n",2000,DEBUG); // reset module
 sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); // configure as access point
 sendCommand("AT+CWJAP=\"iptime\",\"\"\r\n",3000,DEBUG);
 delay(10000);
 sendCommand("AT+CIFSR\r\n",1000,DEBUG); // get ip address
 sendCommand("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
 sendCommand("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
 
 Serial.println("Server Ready");
}

void loop()
{
 if(esp8266.available()) // check if the esp is sending a message 
 {


   if(esp8266.find("+IPD,"))
   {
    delay(1000); // wait for the serial buffer to fill up (read all the serial data)
    // get the connection id so that we can then disconnect
    int connectionId = esp8266.read()-48; // subtract 48 because the read() function returns 
                                          // the ASCII decimal value and 0 (the first decimal number) starts at 48
         
    esp8266.find("pin="); // advance cursor to "pin="
         
    int pinNumber = (esp8266.read()-48); // get first number i.e. if the pin 13 then the 1st number is 1
    int secondNumber = (esp8266.read()-48);
    if(secondNumber>=0 && secondNumber<=9)
    {
     pinNumber*=10;
     pinNumber +=secondNumber; // get second number, i.e. if the pin number is 13 then the 2nd number is 3, then add to the first number
    }
    
    digitalWrite(pinNumber, !digitalRead(pinNumber)); // toggle pin    
    
    // build string that is send back to device that is requesting pin toggle
    String content;
    content = "Pin ";
    content += pinNumber;
    content += " is ";
    
    if(digitalRead(pinNumber))
    {
      content += "ON";
    }
    else
    {
      content += "OFF";
    }
    
     sendHTTPResponse(connectionId,content);
     
     // make close command
     String closeCommand = "AT+CIPCLOSE="; 
     closeCommand+=connectionId; // append connection id
     closeCommand+="\r\n";
     
     sendCommand(closeCommand,1000,DEBUG); // close connection
    }
  }
}
 
/*
String sendData(String command, const int timeout, boolean debug)
{
    String response = "";
    
    int dataSize = command.length();
    char data[dataSize];
   command.toCharArray(data,dataSize);
          
   esp8266.write(data,dataSize); // send the read character to the esp8266
   if(debug)
   {
     Serial.println("\r\n====== HTTP Response From Arduino ======");
     Serial.write(data,dataSize);
     Serial.println("\r\n========================================");
   }
   
   long int time = millis();
   
   while( (time+timeout) > millis())
   {
     while(esp8266.available())
     {
       
       // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}
 

void sendHTTPResponse(int connectionId, String content)
{
     
     // build HTTP response
     String httpResponse;
     String httpHeader;
     // HTTP Header
     httpHeader = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n"; 
     httpHeader += "Content-Length: ";
     httpHeader += content.length();
     httpHeader += "\r\n";
     httpHeader +="Connection: close\r\n\r\n";
     httpResponse = httpHeader + content + " "; // There is a bug in this code: the last character of "content" is not sent, I cheated by adding this extra space
     sendCIPData(connectionId,httpResponse);
}
 

void sendCIPData(int connectionId, String data)
{
   String cipSend = "AT+CIPSEND=";
   cipSend += connectionId;
   cipSend += ",";
   cipSend +=data.length();
   cipSend +="\r\n";
   sendCommand(cipSend,1000,DEBUG);
   sendData(data,1000,DEBUG);
}
*/

 /*
String sendCommand(String command, const int timeout, boolean debug)
{
    String response = "";
           
    esp8266.print(command); // send the read character to the esp8266
    
    long int time = millis();
    
    while( (time+timeout) > millis())
    {
      while(esp8266.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = esp8266.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}
*/
