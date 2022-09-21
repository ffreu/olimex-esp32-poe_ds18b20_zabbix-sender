/**
 * Software for an Olimex Esp32 PoE board that reads data from a connected ds18b20 
 * and sends it to a zabbix server 
 *
 * @author Fabian Freudensprung
 * @version 1.0
 * @date 2022-09-21
 *
 * Credit goes to following sources:
 *    Olimex for the Ethernet connection example (https://github.com/OLIMEX/ESP32-POE)
 *    leruetkins and zaphodus for the Esp32ZabbixSender library (https://github.com/leruetkins/ESP32ZabbixSender)
 *    
 */

#include <OneWire.h>
#include <DallasTemperature.h>
#include <ETH.h>
#include <ESP32ZabbixSender.h>

#define SERVERADDR 192, 168, 1, 1   // Zabbix server Address
#define ZABBIXPORT 10051			       // Zabbix server Port
#define ZABBIXAGHOST "tempSensor01"  // Zabbix item's host name
#define ZABBIXITEM "tempC"

#define ONE_WIRE_BUS 2    // Data wire is plugged into pin 2

// Setup a OneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass OneWire reference to Dallas Temperature
DallasTemperature sensors(&oneWire);

// global variables
float tempC;
static bool eth_connected = false;
ESP32ZabbixSender zSender;

void WiFiEvent(WiFiEvent_t event) 
{
   switch (event) {
    case ARDUINO_EVENT_ETH_START:
     Serial.println("ETH Started");
     //set eth hostname here
     ETH.setHostname("esp32-ethernet");
     break;
    case ARDUINO_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;
    case ARDUINO_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;
    case ARDUINO_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case ARDUINO_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void setup(void)
{
  Serial.begin(115200);
  sensors.begin(); // Start up the library
  WiFi.onEvent(WiFiEvent);
  ETH.begin();
  zSender.Init(IPAddress(SERVERADDR), ZABBIXPORT, ZABBIXAGHOST); // Init zabbix server information
}

void loop(void)
{
    // call sensors.requestTemperatures() to issue a global temperature request to all devices on the bus
    // Send the command to get temperature readings
    sensors.requestTemperatures();

    tempC = sensors.getTempCByIndex(0);
    // You can have more than one DS18B20 on the same bus.
    // 0 refers to the first IC on the wire

    Serial.println("Temperature is: " + String(tempC) + "°C");

    if (eth_connected) {
      zSender.ClearItem();
      zSender.AddItem(ZABBIXITEM, (float)tempC);
      if (zSender.Send() == EXIT_SUCCESS) {		  // Send zabbix items
		    Serial.println("ZABBIX SEND: OK");
	    } else {
		    Serial.println("ZABBIX SEND: NG");
	    }
    }
    delay(5000);
}