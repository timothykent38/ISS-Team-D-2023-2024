/**
   @file flying.ino
   @author Your-Name-Here (you@domain.com)
   @brief Logic for experiment My-Cool-Experiment
   @version 0.1
   @date 2023-07-15

*/
#include "DHT.h"
#include "Quest_Flight.h"
#include "Quest_CLI.h"

#include <Wire.h> // this and the next few lines are for the current sensor setup
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;
if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  // To use a slightly lower 32V, 1A range (higher precision on amps):
  //ina219.setCalibration_32V_1A();
  // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
ina219.setCalibration_16V_400mA();

int hydrogenPin = A7; // for reading hydrogen . Temporary pin valye
float R0_CLEAN_AIR_FACTOR = 9.21;
float H2Curve[3] = {2.3, 0.93, -1.44};

#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
//////////////////////////////////////////////////////////////////////////
//    this defines the timers used to control flight operations
//////////////////////////////////////////////////////////////////////////
#define one_sec   1000                         //one second = 1000 millis
#define one_min   60*one_sec                   // one minute of time
#define one_hour  60*one_min                   // one hour of time
#define two_day   48*one_hour  // 2 days of time
#define one_day   24*one_hour // 1 day of time
#define ten_sec   10*one_sec // ten seconds of time
#define thirty_min   30*one_min // thirty minutes of time
//
#define eventTime0  one_sec*30                  //this event every 3 min
#define eventTime1  one_min*60                  //this event every 60 min
#define eventTime5  one_sec*10
#define eventTime6  one_day

///////////////////////////////////////////////////////////////////////////
/**
   @brief Flying function is used to capture all logic of an experiment during flight.
*/
//************************************************************************
//   Beginning of the flight program setup
//
void Flying() {
  Serial.println("Here to Run flight program, not done yet 20230718");
  Serial.println(" 20230718 working on it");

  uint32_t event0timer = millis();              //set event zero timer
  uint32_t event1timer = millis();              //set event one timer
  uint32_t event2timer = millis();              //set event two timer
  //
  uint32_t one_secTimer = millis();             //set happens every second
  uint32_t sec60Timer = millis();               //set minute timer
   uint32_t event5timer = millis();  // electro read temp
   uint32_t event6timer = millis();  // electro read hydrogen

   

   //event management
   int day = 0; // the way days are cycled needs to be updated and thought out better

  //add timer for collectdata event
 // uint32_t eventDataTimer = millis();    // set data collection timer (doesnt work)
 // dht.begin(); // maybe get rid of this
  //*****************************************************************
  //   Here to set up flight conditions i/o pins, atod, and other special condition
  //   of your program
  //
  // test mode change to carriage return and 115200 baud for ? for commands
  //
  // set up for reading voltage
 
int voltageAnalogInput = A1; // these 4 lines are for reading voltage
float voltage = 0.0;
int rawVoltage = 0;
pinMode(voltageAnalogInput, INPUT);

int cellVoltage = A4; // temporary place holder pin for sending voltage to the cell
pinMode(cellVoltage, OUTPUT);

int pumpWater = A5; // temporary place holder pin for the water pump
pinMode(pumpWater, OUTPUT);

   
  float vout = 0.0; // these 2 values seem to be for logit file??????????
  float vin = 0.0;

   //******************************************************************

  //------------ flying -----------------------

  Serial.println("Flying NOW  -  x=abort");                 //terminal output for abort back to test
  Serial.println("Terminal must be reset after abort");     //terminal reset requirement upon soft reset

  missionMillis = millis();     //Set mission clock millis, you just entered flight conditions
  // ***************** HERE IS WHERE THE EXPERIMENT CODE STARTS *************************************************
      
   // create a day event timer that we can use to run the experiemnt
   // turn power on to the cell over the odd numbered days
   // turn power off to the cell over even numbered daysd
      //
      /////////////////////////////////////////////////////
      //----- Here to start a flight from a reset ---------
      /////////////////////////////////////////////////////
      //
      DateTime now = rtc.now();                   //get time now
      currentunix = (now.unixtime());             //get current unix time, don't count time not flying
      writelongfram(currentunix, PreviousUnix);   //set fram Mission time start now counting seconds unix time
      //
      //***********************************************************************
      //***********************************************************************
      //  All Flight conditions are now set up,  NOW to enter flight operations
      //
      //
      while (1) {
        //
        //----------- Test for terminal abort command from flying ----------------------
        //
        while (Serial.available()) {      //Abort flight program progress
          byte x = Serial.read();         //get the input byte
          if (x == 'x') {                 //check the byte for an abort x
            return  ;                     //return back to poeration sellection
          }                               //end check
        }                                 //end abort check
        //-------------------------------------------------------------------
       //********* event for printing the data from voltage and current 
         /* 
         power cell off every 10 seconds read current delay half second read voltage delay half second read hydrogen delay read humidity.
         if electrolitic phase day and humidity is less than x < then pump the water delay 1 second
         rest of event power cell on or power fell off
         

         */
      if(millis() - event6Timer > eventTime6){
            event6Timer = millis();
         day++;
      }
         
      if(millis() - event5Timer > eventTime5){
         event5Timer = millis();
         // turn on the current sensor DO pins
           float shuntvoltage = 0; // for reading the current data
           float busvoltage = 0;
           float current_mA = 0;
           float loadvoltage = 0;
           float power_mW = 0;
         
           shuntvoltage = ina219.getShuntVoltage_mV();
           busvoltage = ina219.getBusVoltage_V();
           current_mA = ina219.getCurrent_mA();
           power_mW = ina219.getPower_mW();
           loadvoltage = busvoltage + (shuntvoltage / 1000);
         delay(500); // delay half a second
         // turn off the current sensor DO pins
            rawVoltage = analogRead(voltageAnalogInput); // for reading the voltage
            voltage =(value * 5.0) / 1550.0; // see text
            Serial.print("INPUT V= ");
            Serial.println(voltage);
          delay(500);
         // turn on the hydrogen DO pins
             int val; // this should read the value
              val=analogRead(hydrogenPin);//Read Gas value from analog 0
              Serial.println(val,DEC);//Print the value to serial port
                 // for calibrating the senor (do we still need this?)
              float Rs = R0_CLEAN_AIR_FACTOR * val;
              float H2OConcentration = pow(10, (log(Rs / H2Curve[0]) - H2Curve[1]) / H2Curve[2]);
              Serial.print(H2OConcentration);
              Serial.println(" ppm");
         delay(500);
         // turn on humindity DO pins
         
               float t = dht.readTemperature(); // for reading the temperature data
               float f = dht.readTemperature(true);
               // Check if any reads failed and exit early (to try again).
               if (isnan(h) || isnan(t) || isnan(f)) {
                 Serial.println(F("Failed to read from DHT sensor!"));
                 return;
               }
               // Compute heat index in Fahrenheit (the default)
               float hif = dht.computeHeatIndex(f, h);
               // Compute heat index in Celsius (isFahreheit = false)
               float hic = dht.computeHeatIndex(t, h, false);
               Serial.print(F("Humidity: "));
               Serial.print(h);
               Serial.print(F("%  Temperature: "));
               Serial.print(t);
               Serial.print(F("°C "));
               Serial.print(f);
               Serial.print(F("°F  Heat index: "));
               Serial.print(hic);
               Serial.print(F("°C "));
               Serial.print(hif);
               Serial.println(F("°F"));
               if(h < 100 && day%2 == 0){
                  // turn on the pump
                  delay(500); // maybe should try and delay longer for this
               }
               if(day%2 == 0){
                  // turn on DO pins for cell)
               } else {
                  // turn off DO pins for cell
               }
         
      }
     
      // void textFile_string(String data) {                           //store string
      //   Logfile = SD.open("data.txt", FILE_WRITE);  //open syslog file
      //   if (Logfile) {                                //can open the file
      //     Logfile.println();                          //add a carrage return/line feed
      //     delayMicroseconds(100);                     //wait 100 microsec
      //     for (uint8_t x = 0x20; x < 128; x++) {      //print a string to log file
      //       Logfile.write(x);                         //write one charator at a time
      //     }                                           //close string
      //     Logfile.write(data);
      //   }                                             //close the open log file
      //   else {                                        //or else
      //     Serial.println("\r\nlogit error");          //error can not open log file
      //   }                                            //close error else
      //   Logfile.close();                             //close the log file
      // }

      textFile_string("hello");
      // Call the freeMemory function and print the result
      // cmd_stackandheap();                          //enable to know stack and heap after photo time
      //
      // logit_string();                            //log the event
    }                                               //end of eventTime0
    //----------------------------------------------------------------------
    //  This test if eventTime1 time has come
    //  See above for eventTime1 settings between this event
    //
    if ((millis() - event1timer) > eventTime1) {
      event1timer = millis();                    //reset event0timer
      Serial.println();
      Serial.println(millis());
      //
      //  Build the User Text buffer here
      //
      //         strcat(user_text_buf0,("User Buffer space here, millis = "));
      //         itoa(millis(),text_buf,10);
      //         strcat(user_text_buf0,(text_buf));
      //
      //  Take a photo using the SPI c329 camera
      //
      cmd_takeSpiphoto();                              //Take photo and send it
      //          delay(500);
      //         cmd_takeSpiphoto();
    }
    //------------------------------------------------------------------
    //------------  Here one sec timer  ------ -every second ------
    //
    if ((millis() - one_secTimer) > one_sec) {
      one_secTimer = millis();
      DotStarYellow();                              //Blink Yellow when flying
      Serial.write('+');                            //Send to terminal
      //
      //-------------------------------
      // DO NOT TOUCH THIS CODE IT IS NECESARY FOR PROPER MISSION CLOCK OPERATIONS
      //    Mission clock timer
      //    FRAM keep track of cunlitive power on time
      //    and RTC with unix seconds
      //-------------------------------
      //
      DateTime now = rtc.now();                           //get the time time,don't know how long away
      currentunix = (now.unixtime());                     //get current unix time
      Serial.print(currentunix); Serial.print("  ");      //testing print unix clock
      uint32_t framdeltaunix = (currentunix - readlongFromfram(PreviousUnix)); //get delta sec of unix time
      uint32_t cumunix = readlongFromfram(CumUnix);       //Get cumulative unix mission clock
      writelongfram((cumunix + framdeltaunix), CumUnix);  //add and Save cumulative unix time Mission
      writelongfram(currentunix, PreviousUnix);           //reset PreviousUnix to current for next time
      //
      Serial.print("--- Fram Mission Clock = ");      //testing print mission clock
      Serial.print(readlongFromfram(CumUnix));        //mission clock
      Serial.print("  =>   ");                        //spacer
      //
      //------Output the terminal  days hours min sec
      //
      getmissionclk();
      Serial.print(xd); Serial.print(" Days   ");
      Serial.print(xh); Serial.print(" Hours  ");
      Serial.print(xm); Serial.print(" Min  ");
      Serial.print(xs); Serial.println(" Sec");
      //
      //
      //
      //------------ print time once a min -------------------
      //
      if ((millis() - sec60Timer) > (60 * one_sec)) {
        sec60Timer = millis();
        Serial.println();
        cmd_time();
      }
      DotStarOff();
    }
  }
}
