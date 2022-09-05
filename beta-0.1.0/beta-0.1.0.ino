// IRRIGATION SETTINGS
const unsigned long wateringPeriod = 45000L; // 45 seconds
const unsigned long waterEveryMilliseconds = 777600000L; // 9 days
const int motorIntensity = 128; // (min = 0, max = 255)

// PINS 
const int motorPin = 11;
const int overridePin = 7;

// OVERRIDE
boolean overrideWatering = false;
// HOW TO OVERRIDE WATERING TO START PROCEDURE INSTANTLY?
//
// Just connect overridePin to the ground 

// Safety variables
boolean enoughTimePassed = true, wateringCurrently = false;
unsigned long lastWateredBefore = 4294967295L;
unsigned long startedWateringAt = 0;
boolean wateredAtLeastOnce = false;

// Time tracking
unsigned long lastMeasured = 432000000L;

// Function prototypes
void initiateWateringProcedure();
void stopWatering();

// Debug?
const boolean debugMode = true;

void setup() {
  if(debugMode) {
      Serial.begin(9600);
      Serial.println("Microcontroller started.\n\n"); 
  }

  // Motor init
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);

  // Override init
  pinMode(overridePin, INPUT_PULLUP);
}


void loop() {
  
  // Will motor do the motoring?
  enoughTimePassed = lastWateredBefore >= waterEveryMilliseconds;

  // Did we override watering procedure
  checkOverride();

  // Start watering procedure
  if(enoughTimePassed || overrideWatering) {
    initiateWateringProcedure();
  }

  // Keeping track since we last watered 
  if(!wateringCurrently && wateredAtLeastOnce) {
    lastWateredBefore = (unsigned long)(millis() - startedWateringAt);
  }
}

void initiateWateringProcedure() {
    if(!wateringCurrently) {
      wateringCurrently = wateredAtLeastOnce = true;
      startedWateringAt = millis();
      
      analogWrite(motorPin, motorIntensity);
  
      if(debugMode) {
        Serial.print("\n\nStarted watering @ ");
        Serial.print(startedWateringAt);
        Serial.print("ms\n\n\n");
      }
    }

    boolean finishedWatering = (unsigned long)(millis() - startedWateringAt) >= wateringPeriod;
  
    // Motor demotoring
    if(finishedWatering) {
      stopWatering();
    }
}

void stopWatering() {
  wateringCurrently = overrideWatering = false;
  lastWateredBefore = startedWateringAt + wateringPeriod;
  analogWrite(motorPin, 0);

  if(debugMode) {
    Serial.print("\n\nStopped watering @ ");
    Serial.print(millis());
    Serial.print("ms\n\n\n");
  }
}

void checkOverride() {  
  if(digitalRead(overridePin) == LOW) {
    overrideWatering = true;
  }
}
