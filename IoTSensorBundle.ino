///////////////////////////////////////////////////////////
// Miguel Pulido - Systems Architect
// IoT Sensor Bundle
// MCU application to test:
// - LED Toggle
// - Gas Sensor
// - Water Pressure
// - IR Motion Detection
///////////////////////////////////////////////////////////

// Output declarations
int myData = 0; 	                                  // declare variable
int ledpin = 12;                                    // declare an ledpin
int Mlight = 13;                                    // declare motion light

int GasReadSensor = A0;                             // Analog pin 0 will be called 'GasReadSensor'
int GasSensorValue = 0;                             // Set initial value of GasSensorValue to 0


// Input Declarations
volatile int NbTopsFan;                             // measuring the rising edges of the signal

int Calc;
int Wsensor = 2;                                    // Pin2 for water sensor on Arduino
int Msensor = 3;                                    // Pin3 for human IR motion sensor on Arduino
int Gsensor = 8;                                    // Digital pin 8 for gas sensor on Arduino



// Interrupt function setup
void rpm ()                                         // This is the function that the interupt calls
{
  NbTopsFan++;                                      // This function measures the rising and falling edge of the hall effect session signal

}

// The setup () method runs once, when the sketch starts

void setup(){
// Start up our serial port, we configured our XBEE devices for 9600 bps and the pinmodes.

   Serial.begin(9600);                  // set baud rate intialization
   pinMode(ledpin, OUTPUT);             // declare LED Pin 13 to be an output…
   pinMode(Mlight, OUTPUT);             // re-declare pin 13 for motion detector
   pinMode(Gsensor, OUTPUT);            // declare pin 8 an OUTPUT for gas sensor
   pinMode(Wsensor, INPUT);             // declare water sensor pin 2 to be an input
   pinMode(Msensor, INPUT);             // declare human IR sensor pin 3 to be an input
   attachInterrupt(0, rpm, RISING);     // Attach interrupt


}

// the loop() method is a forever loop as long as Arduino is powered


void loop()
{

 //////////////////////////////////// Water Sensor Code portion ///////////////


    //input portion
        //if(myData == '3'){
        NbTopsFan = 0;                            // Set NbTops to 0 ready for calculation
        sei();                                    // Enables interrupts
        delay(1000);                              // wait 1 second
        cli();                                    // Disables interrupts
        Calc = (NbTopsFan * 60 / 7.5);            // (Pulse frequency x 60) / 7.5 Q, = flow rate in L/hour
        Serial.print (Calc, DEC);                 // Prints the number of calculations above
        Serial.print ("L/hour\r\n");              // Prints "L/hour" and returns a new line



   ///////////////////////////////////// IR Human Sensor Code portion ///////////////



      digitalWrite(Mlight,LOW);
      delay(1000);                                // this delay is to let the sensor settle down before taking a reading
      int motionSensor = digitalRead(Msensor);
      if(motionSensor == HIGH){
        digitalWrite(Mlight,HIGH);
        Serial.print("Motion Detected\r\n");      // Should print out Motion detected text in real-time over serial
        delay(1000);
        digitalWrite(Mlight,LOW);
       // Serial.print ("No Motion\r\n");         // Should print "No Motion"
        delay(1000);


      }


  /////////////////////////////////// Gas Sensor Code portion /////////////////

         GasSensorValue = analogRead(GasReadSensor);            // Read the input on analog pin 0 (named 'GasReadSensor'
         Serial.print(GasSensorValue, DEC);                     // Print out the value you read
         Serial.print("PPM\r\n");                               // Prints Parts Per Million of respective gas
         if (GasSensorValue > 500) {
           digitalWrite(Gsensor, HIGH);                         // Activate digital output pin 8 - LED will be on

         }

        else {
         digitalWrite(Gsensor, LOW);                            // Deactivate digital output pin 8 - LED will be off
        }



 ////////////////////// LED Communication between XBee test portion, test that XBee is communicating //////////////


        // output portion
        if(Serial.available() > 0){ 	         // This looks at serial code and compares against 0 - nothing
        myData = Serial.read();	        // If greater than 0, it goes into a serial read of myData
        if(myData == '1'){		           // it will read myData and if it is 1, do the next lines
        digitalWrite(ledpin, HIGH);	  // any time myData is equal to 1, it will light LED HIGH and turn it on
        Serial.write(myData);	         // anytime myData is equal to 1, it will type out my serial data "Serial.print(myData,BYTE)"; no longer supported

     }

         // output portion
        if(myData == '0'){		          // this will check for absolute value of 0
        digitalWrite(ledpin, LOW);	     // sets LED pin to LOW or turns it off
        Serial.write(myData);	        // anytime myData is equal to 1, it will type out my serial data "Serial.print(myData,BYTE)"; no longer supported

      }


    else{
    Serial.write(myData);		// if any other character data comes through, all it will do is type the data coming through.
    }

}
}

