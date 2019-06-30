/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//OLED GND – Arduino GND
//OLED VCC – Arduino 5V
//OLED SCL – Arduino Uno A5
//OLED SDA – Arduino Uno A4
void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
//    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(500);
  display.clearDisplay();

  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  // Display static text
  display.print("Init...");
  display.display();
  Serial.println("AT+NAMEJazComBT01");
//	Serial.println("AT+HELP");
}

#define STR_LENGTH 64
char buffer[STR_LENGTH];
char incoming[STR_LENGTH];
int bufferEndPos = 0;
int lastMessagePos = -1;
char data[STR_LENGTH + 1];

int parseSerial() {
  if (Serial.available()) {
		int length = Serial.readBytes(incoming, STR_LENGTH);
  	for (int i = 0; i < length; i++) {
	  	if (incoming[i] == '\r') {
	  		incoming[i] = '\0';
	  	}
  	}
		for (int i = 0; i < length; i++) {
			buffer[(bufferEndPos++) % STR_LENGTH] = incoming[i];
		}
  }
  int dataLength = 0;
	for (int i = lastMessagePos + 1; i < bufferEndPos && i < lastMessagePos + 11; i++) {
		char current = buffer[i % STR_LENGTH];
		lastMessagePos++;
		if (current == '\n' || current == '\0') {
			break;
		}
		data[dataLength++] = current;
		if (dataLength == 10) {
			break;
		}
	}
	data[dataLength] = '\0';
//	if(dataLength > 0) Serial.println(data);
	return dataLength;
}

int processMessage(int length) {
	if (data[0] == 'J' && data[1] == 'A' && data[2] == 'Z' && data[3] == ':') {
		for (int j = 0; j < length; j++) {
			data[j] = data[j + 4];
		}
		return 1;
	}
	return 2;
}

void loop() {
	int length = parseSerial();
	int messageType = processMessage(length);
	if (messageType == 1) {
		length -= 4;
	}
	if (length > 0) {
		if (messageType == 1) {
			display.fillRect(0, 20, display.width(), display.height() - 20, BLACK);
		  if (length > 3) {
		  	display.setTextSize(3);
		  } else {
		  	display.setTextSize(6);
		  }
		  display.setCursor(5, 20);
		  if (length == 2) {
		  	display.print(" ");
		  }
		  display.println(data);
		} else {
			display.fillRect(0, 0, display.width(), 19, BLACK);
	  	display.setTextSize(2);
		  display.setCursor(0, 0);
		  data[10] = '\0';
		  display.print(data);
		}
	  display.display();
	  if (length == 10) {
	  	delay(25);
	  }
	}
}
