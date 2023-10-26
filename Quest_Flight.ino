/**
   @file flying.ino
   @author Your-Name-Here (you@domain.com)
   @brief Logic for experiment My-Cool-Experiment
   @version 0.1
   @date 2023-07-15

*/

#include "Quest_Flight.h"
#include "Quest_CLI.h"

//////////////////////////////////////////////////////////////////////////
//    this defines the timers used to control flight operations
//////////////////////////////////////////////////////////////////////////
#define one_sec   1000                         //one second = 1000 millis
#define one_min   60*one_sec                   // one minute of time
#define one_hour  60*one_min                   // one hour of time
//
#define eventTime0  one_sec*30                  //this event every 3 min
#define eventTime1  one_min*60                  //this event every 60 min
#define eventReadData one_min      // this event reads data (current and voltage) and prints it to the logit file (hopefully)
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

  //add timer for collectdata event
  uint32_t eventDataTimer = millis();    // set data collection timer

  //*****************************************************************
  //   Here to set up flight conditions i/o pins, atod, and other special condition
  //   of your program
  //
  // test mode change to carriage return and 115200 baud for ? for commands
  //
  int analogInput = A1;
  float vout = 0.0;
  float vin = 0.0;
  float R1 = 30000.0; //
  float R2 = 7500.0; //
  int value = 0;
  pinMode(analogInput, INPUT);
  Serial.print("DC VOLTMETER");
  int analogReadVoltage = A2;
  pinMode(analogReadVoltage, INPUT);
  int analogReadCurrent = A3;
  pinMode(analogReadCurrent, INPUT);
  int analogReadHydrogen = A4;
  pinMode(analogReadHydrogen, INPUT);
  int digitalPowerOn = 5;
  pinMode(digitalPowerOn, OUTPUT);
  int digitalPumpOn = 6;
  pinMode(digitalPumpOn, OUTPUT);
  int digitalMotorOn = 7;
  pinMode(digitalMotorOn, OUTPUT);
  bool readHydrogen;  // do we read the hydrogen
  //******************************************************************

  //------------ flying -----------------------

  Serial.println("Flying NOW  -  x=abort");                 //terminal output for abort back to test
  Serial.println("Terminal must be reset after abort");     //terminal reset requirement upon soft reset

  missionMillis = millis();     //Set mission clock millis, you just entered flight conditions
  // ***************** HERE IS WHERE THE EXPERIMENT CODE STARTS *************************************************
      // wait 24 hours (this should be done automatically)
      // turn on voltage 
      digitalWrite(digitalPowerOn, HIGH);
      //***** reading the voltage and current happens each minute throughout the entire experiment
    //  read oxygen(or hydrogen) and print that to the logit file
      readHydrogen = true;  // there is probably a better way to control when we do or do not read data from the hydrogen sensor, the current way probably does not work at all 
    //  turn on air pump
        digitalWrite(digitalPumpOn, HIGH);
    //  read voltage and hydrogen (this happens throughout the experiment every minute)
    //  turn off votage and air pump
        digitalWrite(digitalPowerOn, LOW);
        digitalWrite(digitalPumpOn, LOW);
    //  reverse polaroty (i have no idea how to do this)
    //  turn on votage measure current and voltage (measuring continues to happen throughout experiment
          digitalWrite(digitalPowerOn, HIGH);
    //  turrn off voltage
        digitalWrite(digitalPowerOn, LOW);
    //  turn on motor
    //  turn off motor
    //  if 30 days have passed end else go back to turn on power (we may want to put all this code inside a 1 day event so it runs every day and have a variable that counts the number of days that have passed)
      //
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
    
    if ((millis() - eventDataTimer) > eventReadData) {
              // ******* reading temp info 
              // Example testing sketch for various DHT humidity/temperature sensors
    // Written by ladyada, public domain
    
    // REQUIRES the following Arduino libraries:
    // - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
    // - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
    
    #include "DHT.h"
    
    #define DHTPIN 2     // Digital pin connected to the DHT sensor
    // Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
    // Pin 15 can work but DHT must be disconnected during program upload.
    
    // Uncomment whatever type you're using!
    //#define DHTTYPE DHT11   // DHT 11
    #define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
    //#define DHTTYPE DHT21   // DHT 21 (AM2301)
    
    // Connect pin 1 (on the left) of the sensor to +5V
    // NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
    // to 3.3V instead of 5V!
    // Connect pin 2 of the sensor to whatever your DHTPIN is
    // Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
    // Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
    // Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor
    
    // Initialize DHT sensor.
    // Note that older versions of this library took an optional third parameter to
    // tweak the timings for faster processors.  This parameter is no longer needed
    // as the current DHT reading algorithm adjusts itself to work on faster procs.
    DHT dht(DHTPIN, DHTTYPE);
    
  
      // Wait a few seconds between measurements.
      delay(2000);
    
      // Reading temperature or humidity takes about 250 milliseconds!
      // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
      float h = dht.readHumidity();
      // Read temperature as Celsius (the default)
      float t = dht.readTemperature();
      // Read temperature as Fahrenheit (isFahrenheit = true)
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

          if(readHydrogen){
            int hydrogen = analogRead(analogReadHydrogen);  // reads the hydrogen from the sensor
            // print hydrogen to the logit file
          }
          eventDataTimer = millis();                    //yes is time now reset eventDatatTimer
          Serial.println();                          //
          Serial.println(millis());  
   
          int voltage = analogRead(analogReadVoltage);        // read the voltage sensor value
          // print the voltage to the logit file
          
      
          int current = analogRead(analogReadCurrent);      // read the current sensor value 
          // print the current to the logit file
          

        }
         //  this test if eventTime0 time has come
    //  See above for eventTime0 settings between this event
    //
    if ((millis() - event0timer) > eventTime0) {
      event0timer = millis();                    //yes is time now reset event0timer
      Serial.println();                          //
      Serial.println(millis());                  //
      //
      //read the value at analog input
      value = analogRead(analogInput);
      vout = (value * 5.0) / 1024.0; // see text
      vin = vout / (R2/(R1+R2));
      //***** Build the User Text buffer here (for writing the 
      //***** additions to the user text buffer can be added anytime between photo events
      //
      strcat(user_text_buf0, ("User Buffer space here, millis = "));    //put this in user buffer
      itoa(millis(), text_buf, 10);                                     //put millis time in user buffer (itoa = int to ascii, first parameter is for the number to add, text_buf is the name of the variable it is stored in and the 10 is the base of the number of the first parameter)
      strcat(user_text_buf0, (text_buf));               //append the just created text_buf to user buffer
      text_buf[0] = '\0';
      strcat(user_text_buf0, ("INPUT V+ "));
      itoa(vin, text_buf, 10);
      strcat(user_text_buf0, (text_buf));
      //copy the logit sructure in the cli
      //  Take a photo using the serial c329 camera and place file name in Queue
      //
      cmd_takeSphoto();                          //Take photo and send it
      delay(100);

      void logit(uint8_t  x) {
        Logfile = SD.open("syslog.txt", FILE_WRITE);  //open log file ?
        if (Logfile) {                                //with logfile is open
          Logfile.write(x);                         //write the value to file
        }
        else {                                        //if not open say error
          Serial.println("\r\nlogit error");          //Terminal output here
        }
        Logfile.close();                              //close the log file
      }



      void textFile_string(String data) {                           //store string
        Logfile = SD.open("data.txt", FILE_WRITE);  //open syslog file
        if (Logfile) {                                //can open the file
          Logfile.println();                          //add a carrage return/line feed
          delayMicroseconds(100);                     //wait 100 microsec
          for (uint8_t x = 0x20; x < 128; x++) {      //print a string to log file
            Logfile.write(x);                         //write one charator at a time
          }                                           //close string
          Logfile.write(data);
        }                                             //close the open log file
        else {                                        //or else
          Serial.println("\r\nlogit error");          //error can not open log file
        }                                            //close error else
        Logfile.close();                             //close the log file
      }

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
