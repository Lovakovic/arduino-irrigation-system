// GENERAL SETTINGS 
const unsigned long idleTime = 900000L; // 15 minutes

// IRRIGATION SETTINGS
const unsigned long wateringPeriod = 45000L; // 45 seconds
const unsigned long minIntervalBetweenWatering = 432000000L; // 5 days
const int motorIntensity = 128; // (min = 0, max = 255)

// MOISTURE SETTINGS
const int wet = 110, dry = 460;
const int dryThreshold = 45; // in percentage (lower is dryer)

// PINS 
const int sensorPin = 0;
const int motorPin = 11;
const int overridePin = 7;

// OVERRIDE
boolean overrideWatering = false;
// HOW TO OVERRIDE WATERING TO START PROCEDURE INSTANTLY?
//
// Just connect overridePin to the ground 

// Safety variables
int readingsBelowThreshold = 0;
boolean enoughTimePassed = true, wateringCurrently = false;
unsigned long lastWateredBefore = 4294967295L;
unsigned long startedWateringAt = 0;
boolean wateredAtLeastOnce = false;

// Time tracking
unsigned long lastMeasured = 432000000L;

// Function prototypes
void measureMoisture();
void initiateWateringProcedure();
void stopWatering();

// Debug?
const boolean debugMode = true;

void setup() {
  if(debugMode) {
      Serial.begin(9600);
      Serial.println("Microcontroller started.\n\n"); 
  }
  
  // Sensor init
  pinMode(sensorPin, INPUT_PULLUP);

  // Motor init
  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);

  // Override init
  pinMode(overridePin, INPUT_PULLUP);
}


void loop() {

  // We measure every once in a while
  measureMoisture();
  
  // Will motor do the motoring?
  enoughTimePassed = lastWateredBefore >= minIntervalBetweenWatering;

  // Did we override watering procedure with serial input
  checkOverride();

  // Start watering procedure
  if((readingsBelowThreshold >= 10 && enoughTimePassed) || overrideWatering) {
    initiateWateringProcedure();
  }

  // Keeping track since we last watered 
  if(!wateringCurrently && wateredAtLeastOnce) {
    lastWateredBefore = (unsigned long)(millis() - startedWateringAt);
  }
}

void measureMoisture() {
  if((unsigned long)(millis() - lastMeasured) >= idleTime && !wateringCurrently) {
    lastMeasured = millis();
    
     // Sensor tingling
    int moistureLevel = analogRead(sensorPin);
    int moisturePercentage = map(moistureLevel, wet, dry, 100, 0);
  
    // Safety safetying
    if(moisturePercentage <= dryThreshold && !wateringCurrently) {
      readingsBelowThreshold += 1;
    } else {
      readingsBelowThreshold = 0;
    }

    if(debugMode) {
      Serial.print("Moisture = ");
      Serial.print(moistureLevel);
      Serial.print(" --> ");
      Serial.print(moisturePercentage);
      Serial.print("% (");
      Serial.print(readingsBelowThreshold);
      Serial.print(")\n");

      Serial.print("Last watered before: ");
      Serial.print(lastWateredBefore);
      Serial.print("ms\n");
    }
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
  readingsBelowThreshold = 0;
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
