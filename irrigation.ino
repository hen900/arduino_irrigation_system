


// A tray is defined as a growing container with soil, a moisture sensor and relays to activate water pumps
class tray {

private:
  int sensorNum; // pin which moisture sensor is attached to
  int relayNum; // pin which relay is attached to
  int moistValue1;
  int moistValue2;
  int avgMoistValue; 
  int airValue; //moisture reading of air
  int waterValue; //moisture reading of water
  int percentMoist;
  int threshold; // lowest acceptable percent moisture 
  int safetyCatch; // var defined to detect overlflow

  
public:

  tray(int relayInpt, int sensorInpt,double threshInpt, int airConstantInpt, int waterConstantInpt){
    sensorNum = sensorInpt; // Pin number where the moisture sensor is attached
    threshold = threshInpt;  // Lowest acceptable moisture percentage for soil given as an integer. ex 20% = 20
    
// Air and water values vary from sensor from sensorto sensor  and must be individually calibrated
    airValue = airConstantInpt; // Reading sensor gives while soley exposed to air 
    waterValue = waterConstantInpt; // Reading sensor gives while submereged in water
   
    relayNum = relayInpt; // Pin number of the relay for the partcular tray
    int safetyCatch = 0; // var defined to detect overlflow
  };

  void begin() { //Sets up relays and sensors
    pinMode(relayNum, OUTPUT); // Sets relay pin to output
    digitalWrite(relayNum, HIGH); // Turns the relay off by defualt
    Serial.begin(9600); //Begins serial stream for moisture readings
  };
  
  boolean isLow(){ //Determines if the soil moisture in the tray is below the acceptable threshhold
    moistValue1 = analogRead(sensorNum);
    delay(1500);
    moistValue2=  analogRead(sensorNum);
    avgMoistValue=(moistValue1+moistValue2)/2; // two readings are taken 1.5 seconds apart and averaged
    percentMoist = map(avgMoistValue,airValue,waterValue,0,100); // map is utilized to define moisture as a percentage
    if(percentMoist < threshold){
      return true;

    }
    else { 
     return false;

    }
  };
  void emergencyShutoff() { // function to turn off the system to prevent water overflow. activates a buzzer every 3 seconds
    digitalWrite(relayNum, HIGH); // deactivate pump 
    while(true){ // loop forever to prevent more water from pumping!
      Serial.print("Relay Number ");
      Serial.print(relayNum);
      Serial.println(" Emergency Shutoff");
      tone(8,500 ,500); // plays a 500 Hz tone on pin 8 for 500 ms
      delay(3000);
    }
  }
  void test(){ // function just for testing and calibration purposes
    moistValue1 = analogRead(sensorNum);
    delay(1500);
    moistValue2=  analogRead(sensorNum);
    avgMoistValue=(moistValue1+moistValue2)/2; // two readings are taken 1.5 seconds apart and averaged
    percentMoist = map(avgMoistValue,airValue,waterValue,0,100); // map is utilized to define moisture as a percentage
    Serial.println(sensorNum);
    Serial.print("% Moisture: ");
    Serial.print(percentMoist);
    Serial.print("Raw value: ");
    Serial.println(avgMoistValue);
    
    //digitalWrite(relayNum, LOW);
    //delay(1000*3);
    //digitalWrite(relayNum, HIGH);
  }
  void activatePump(){// while the soil is below the threshold, water for 15 seconds, turn off the relay, wait 1 minute and take another reading 
    while(isLow()){ 
      if(safetyCatch > 10) { // if pump is activated more than 10 times in a row, initiate emergency shutoff
        emergencyShutoff();
      }
      digitalWrite(relayNum, LOW);
      delay(1000*20);
      digitalWrite(relayNum, HIGH);
      delay(1000*60*3);
      safetyCatch++;
    }

  };
    
  
};


// tray(pinForRelay,pinForSensor,%moistureAsThreshold,airConstantForSensor,WaterConstantForSensor)

tray allTrays[4] = {
 tray(2,A1,28,585,294),
 tray(3,A2,28,577,292),
 tray(4,A3,28,580,314),
 tray(5,A4 ,28,585,308) };



void setup() { 
  
  for(int i = 0; i < 4;i++){ //initializes all the trays
  allTrays[i].begin();
 }


  
}

void loop() {
  //allTrays[3].test();

for(int i = 0; i< 4;i++) {
  
  if(allTrays[i].isLow()){
    allTrays[i].activatePump();
}
} 
delay(1000*60*2);


}
