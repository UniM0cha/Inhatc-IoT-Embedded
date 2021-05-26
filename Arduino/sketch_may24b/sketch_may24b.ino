#include <ESP8266WiFi.h>

const char *ssid = "와이파이 이름";
const char *password = "와이파이 비번";

int pin2 = D2;
int pin8 = D8
int LED_1;
int LED_2;

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    delay(10);
    pinMode(pin2, OUTPUT);
    pinMode(pin8, OUTPUT);
    digitalWrite(pin2, LOW);
    digitalWrite(pin8, LOW);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.print("Use this URL : ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}

void loop()
{
    // Check if a client has connected
    WiFiClient client = server.available();

    if (!client)
    {
        return;
    }

    // Wait until the client sends some data
    Serial.println("new client");
    while (!client.available())
    {
        delay(1);
    }

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Match the request
    if (request.indexOf("/pin2=ON") != -1)
    {
        digitalWrite(pin2, HIGH);
        LED_1 = HIGH;
    }

    if (request.indexOf("/pin2=OFF") != -1)
    {
        digitalWrite(pin2, LOW);
        LED_1 = LOW;
    }

    if (request.indexOf("/pin8=ON") != -1)
    {
        digitalWrite(pin8, HIGH);
        LED_2 = HIGH;
    }

    if (request.indexOf("/pin8=OFF") != -1)
    {
        digitalWrite(pin8, LOW);
        LED_2 = LOW;
    }

    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one

    //client.println("<!DOCTYPE HTML>");
    //client.println("<html>");
    //client.print("Led pin is now: ");

    if (LED_1 == HIGH && LED_2 == LOW)
    {
        client.println("LED_1 ON / LED_2 OFF");
    }
    else if (LED_1 == LOW && LED_2 == HIGH)
    {
        client.println("LED_1 OFF / LED_2 ON");
    }
    else if (LED_1 == HIGH && LED_2 == HIGH)
    {
        client.println("LED_1 ON / LED_2 ON");
    }
    else if (LED_1 == LOW && LED_2 == LOW)
    {
        client.println("LED_1 OFF / LED_2 OFF");
    }
    else
    {
        client.println("LED_1 OFF / LED_2 OFF");
    }

    //client.println("<br><br>");
    //client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 5 ON<br>");
    //client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 5 OFF<br>");
    //client.println("</html>");

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
}#include <ESP8266WiFi.h>

const char *ssid = "와이파이 이름";
const char *password = "와이파이 비번";

int pin2 = D2;
int pin8 = D8
int LED_1;
int LED_2;

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    delay(10);
    pinMode(pin2, OUTPUT);
    pinMode(pin8, OUTPUT);
    digitalWrite(pin2, LOW);
    digitalWrite(pin8, LOW);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");

    // Start the server
    server.begin();
    Serial.println("Server started");

    // Print the IP address
    Serial.print("Use this URL : ");
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println("/");
}

void loop()
{
    // Check if a client has connected
    WiFiClient client = server.available();

    if (!client)
    {
        return;
    }

    // Wait until the client sends some data
    Serial.println("new client");
    while (!client.available())
    {
        delay(1);
    }

    // Read the first line of the request
    String request = client.readStringUntil('\r');
    Serial.println(request);
    client.flush();

    // Match the request
    if (request.indexOf("/pin2=ON") != -1)
    {
        digitalWrite(pin2, HIGH);
        LED_1 = HIGH;
    }

    if (request.indexOf("/pin2=OFF") != -1)
    {
        digitalWrite(pin2, LOW);
        LED_1 = LOW;
    }

    if (request.indexOf("/pin8=ON") != -1)
    {
        digitalWrite(pin8, HIGH);
        LED_2 = HIGH;
    }

    if (request.indexOf("/pin8=OFF") != -1)
    {
        digitalWrite(pin8, LOW);
        LED_2 = LOW;
    }

    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one

    //client.println("<!DOCTYPE HTML>");
    //client.println("<html>");
    //client.print("Led pin is now: ");

    if (LED_1 == HIGH && LED_2 == LOW)
    {
        client.println("LED_1 ON / LED_2 OFF");
    }
    else if (LED_1 == LOW && LED_2 == HIGH)
    {
        client.println("LED_1 OFF / LED_2 ON");
    }
    else if (LED_1 == HIGH && LED_2 == HIGH)
    {
        client.println("LED_1 ON / LED_2 ON");
    }
    else if (LED_1 == LOW && LED_2 == LOW)
    {
        client.println("LED_1 OFF / LED_2 OFF");
    }
    else
    {
        client.println("LED_1 OFF / LED_2 OFF");
    }

    //client.println("<br><br>");
    //client.println("Click <a href=\"/LED=ON\">here</a> turn the LED on pin 5 ON<br>");
    //client.println("Click <a href=\"/LED=OFF\">here</a> turn the LED on pin 5 OFF<br>");
    //client.println("</html>");

    delay(1);
    Serial.println("Client disconnected");
    Serial.println("");
}
