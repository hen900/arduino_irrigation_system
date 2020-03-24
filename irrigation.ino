


// A tray is defined as a growing container with soil, a moisture sensor and relays to activate water pumps
class tray {

private:
  int sensorNum;
  int relayNum;
  int moistValue1;
  int moistValue2;
  int avgMoistValue;
  int airValue;
  int waterValue;
  int percentMoist;
  int threshold;

  
public:

  tray(int relayInpt, int sensorInpt,double threshInpt, int airConstantInpt, int waterConstantInpt){
    sensorNum = sensorInpt; // Pin number where the moisture sensor is attached
    threshold = threshInpt;  // Lowest acceptable moisture percentage for soil given as an integer. ex 20% = 20
    
// Air and water values vary from sensor from sensorto sensor  and must be individually calibrated
    airValue = airConstantInpt; // Reading sensor gives while soley exposed to air 
    waterValue = waterConstantInpt; // Reading sensor gives while submereged in water
   
    relayNum = relayInpt; // Pin number of the relay for the partcular tray
    int moistValueSum = 0;
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

  void test(){
    moistValue1 = analogRead(sensorNum);
    delay(1500);
    moistValue2=  analogRead(sensorNum);
    avgMoistValue=(moistValue1+moistValue2)/2; // two readings are taken 1.5 seconds apart and averaged
    percentMoist = map(avgMoistValue,airValue,waterValue,0,100); // map is utilized to define moisture as a percentage
    Serial.print(sensorNum);
    Serial.print(":");
    Serial.println(percentMoist);
    //digitalWrite(relayNum, LOW);
    //delay(1000*3);
    //digitalWrite(relayNum, HIGH);
  }
  void activatePump(){// while the soil is below the threshold, water for 15 seconds, turn off the relay, wait 1 minute and take another reading 
    while(isLow()){ 
      digitalWrite(relayNum, LOW);
      delay(1000*20);
      digitalWrite(relayNum, HIGH);
      delay(1000*60*3);
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
    //allTrays[i].test();
}
} 
delay(1000*60*2);


}
