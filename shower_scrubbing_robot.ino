
#include <avr/sleep.h>
#define PI 3.1415926535897932384626433832795
#define NUM_STEPS 200

#define dirPin1 2
#define stpPin1 3
#define dirPin2 4
#define stpPin2 5

#define delayTime 500

  
void setup() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
 /**
  * Setting the OUTPUT pins for Stepper Motor 1
  */
  pinMode(dirPin1, OUTPUT);
  pinMode(stpPin1, OUTPUT);

  /**
   * Setting the OUTPUT pins for Stepper Motor 2
   */
  pinMode(dirPin2, OUTPUT);
  pinMode(stpPin2, OUTPUT);

  Serial.begin(9600);

}

void loop() {

  /**
   * The didstance between the two nodes on "Scrubber"
   */
  double z = 100;

   /**
   * Max Disstance the Motors Will Travel
   */
  double yMax = 489;
  double xMax = 463;

  

  /**
   * Our Home Position
   */
  double y = 200;
  double x = 0;


  

  /**
   * The diameter of each spool
   */
  double steps1Dia = 50.24;
  double steps2Dia = 49.71;

  /**
   * Circumference of each spool
   */
   double steps1Cum = (steps1Dia) * PI;
   double steps2Cum = (steps2Dia) * PI;

   const double degreeStep = 1.8;
   int microStep = 1;
   double newDegreeStep = 1.8 / microStep;

   double arcLen1 = steps1Cum* (newDegreeStep / 360);
   double arcLen2 = steps2Cum* ( newDegreeStep / 360);

    /**
     * Getting the current length of the lines
     */
    double oldLine1 = calculateLine(x, yMax);
    double oldLine2 = calculateLine(xMax-x, yMax);
    
  while(y < yMax){

      Serial.print(int(y));
      Serial.print(" is less than ");
      Serial.println(int(yMax));
    
    while(x <= xMax && oldLine2 > yMax){

         /**
         * Getting the current length of the lines
         */
        double newLine1 = calculateLine(x, yMax);
        double newLine2 = calculateLine(xMax-x, yMax);

        double stepsReq1 = (newLine1 - oldLine1)/arcLen1;
        double stepsReq2 = (newLine2 - oldLine2)/arcLen2;
        printDouble(stepsReq1, 100);

        oldLine1 = newLine1;
        oldLine2 = newLine2;
        
        /**
         * Trigger that will keep a while loop going
         */
         bool keepGoing = true;

         int currentStep1 = 0;
         int currentStep2 = 0;


          /**
           * If our steps for stepper 1 is negative, then it will need to go counter clockwise
           */
         if(stepsReq1 < 0){
            digitalWrite(dirPin1, LOW);
            stepsReq1 = abs(stepsReq1);
         }else{
            digitalWrite(dirPin1, HIGH);
         }

          /**
           * Setting the direction of the stepper 2
           */
         if(stepsReq2 < 0){
          digitalWrite(dirPin2, HIGH);
            stepsReq2 = abs(stepsReq2);
         }else{
          digitalWrite(dirPin2, LOW);
         }

         bool stpBool1 = true;
         bool stpBool2 = true;

         while(keepGoing){

            delay(100);

            
            if(currentStep1 <= stepsReq1){
              digitalWrite(stpPin1, HIGH);
              delayMicroseconds(delayTime);
              digitalWrite(stpPin1, LOW);

              currentStep1++;
              
            }else{
              stpBool1 = false;
            }

            if(currentStep2 <= stepsReq2){

              digitalWrite(stpPin2, HIGH);
              delayMicroseconds(delayTime);
              digitalWrite(stpPin2, LOW);


              currentStep2++;
              
            }else{
              stpBool2 = false;
            }

            keepGoing = stpBool1 || stpBool2;
         }

         x +=  20;

    }
    x=0;
    Serial.println("Decreasing Y");
    yMax -= 100;
  }

  sleep_mode();
}

double calculateLine(double x, double y){

  return sqrt(pow(x, 2) + pow(y, 2));

}

void printDouble( double val, unsigned int precision){
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)

    Serial.print (int(val));  //prints the int part
    Serial.print("."); // print the decimal point
    unsigned int frac;
    if(val >= 0)
        frac = (val - int(val)) * precision;
    else
        frac = (int(val)- val ) * precision;
    Serial.println(frac,DEC) ;
}
