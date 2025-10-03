#include <Arduino.h>
#include "gas_model.h"  // Use your exported model header here
#include "EloquentTinyML.h"

Eloquent::ML::Port::RandomForest gasModel;

const int MUX_OUTPUT_PIN = 34;
const int MUX_SELECT_PINS[] = {2, 3, 4, 5};
const int buzzerPin = 25;
const int ledPin = 26;

const int SENSOR_COUNT = 16;
const int BASELINE_SAMPLES = 50;
const int smoothingWindowSize = 5;
const float thresholdMargin = 200.0;
const int consecutiveRequired = 3;

float features[SENSOR_COUNT];
int baseline[SENSOR_COUNT];
int rawReadings[SENSOR_COUNT];
int predictionCounter = 0;
int lastPrediction = 0;
bool alarmActive = false;
int smoothingHistory[SENSOR_COUNT][smoothingWindowSize] = {0};
int historyIndex = 0;

const char* gasNames[] = {
  "No Gas (Clean)", "Ethanol", "Ammonia", "Acetone", "Ethylene", "Acetaldehyde"
};

void setMuxChannel(int channel) {
  for(int i=0; i<4; i++) {
    digitalWrite(MUX_SELECT_PINS[i], (channel >> i) & 1);
  }
}

int readGasSensor(int sensorIndex) {
  setMuxChannel(sensorIndex);
  delay(5);
  return analogRead(MUX_OUTPUT_PIN);
}

float getSmoothedValue(int sensorIndex, int newValue) {
  smoothingHistory[sensorIndex][historyIndex] = newValue;
  long sum = 0;
  for(int i=0; i<smoothingWindowSize; i++) {
    sum += smoothingHistory[sensorIndex][i];
  }
  return (float)sum / smoothingWindowSize;
}

void setAlarm(bool state) {
  if(state != alarmActive) {
    alarmActive = state;
    digitalWrite(ledPin, state ? HIGH : LOW);
    if(state) {
      tone(buzzerPin, 1000);
      Serial.println("!!! ALARM ACTIVATED !!!");
    } else {
      noTone(buzzerPin);
      Serial.println("--- Alarm Deactivated ---");
    }
  }
}

void calibrateBaseline() {
  Serial.println("\n--- Starting Baseline Calibration (Clean Air) ---");
  for(int i=0; i<SENSOR_COUNT; i++) {
    baseline[i] = 0;
    for(int j=0; j<smoothingWindowSize; j++) {
      smoothingHistory[i][j] = 0;
    }
  }
  for(int sample=0; sample<BASELINE_SAMPLES; sample++) {
    Serial.print(".");
    for(int i=0; i<SENSOR_COUNT; i++) {
      int rawVal = readGasSensor(i);
      baseline[i] += rawVal;
      if(sample < smoothingWindowSize) smoothingHistory[i][sample] = rawVal;
    }
    delay(100);
  }
  Serial.println();
  for(int i=0; i<SENSOR_COUNT; i++) {
    baseline[i] /= BASELINE_SAMPLES;
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" Baseline: ");
    Serial.print(baseline[i]);
    Serial.print(" | Alert Threshold: >");
    Serial.println(baseline[i] + thresholdMargin);
  }
  Serial.println("--- Baseline Established ---");
}

void printGasDetectionReport(int elapsedSeconds) {
  Serial.println("\n------------------ Gas Detection Report ------------------");
  Serial.print("Alert threshold: ");
  Serial.println((int)thresholdMargin);
  Serial.print("Time: ");
  Serial.print(elapsedSeconds);
  Serial.println(" seconds\n");
  Serial.println("Sensors (Raw | Baseline Adjusted):");

  for (int i = 0; i < SENSOR_COUNT; i++) {
    Serial.print("S");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(rawReadings[i]);
    Serial.print(" | ");
    float baselineAdjusted = (float)rawReadings[i] - baseline[i];
    Serial.print(baselineAdjusted, 2);

    if ((i + 1) % 4 == 0) Serial.println();
    else Serial.print("\t");
  }
  Serial.println("\n");
}

void reportGasDetection() {
  Serial.print("Gas Detected: ");
  Serial.println(gasNames[lastPrediction]);
  Serial.print("Alarm Status: ");
  Serial.println(alarmActive ? "ON" : "OFF");
  Serial.println();
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++) pinMode(MUX_SELECT_PINS[i], OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  setAlarm(false);
  Serial.println("ESP32 Gas Array Monitor Ready.");
  calibrateBaseline();
}

void loop() {
  bool sensorActive = false;
  for (int i = 0; i < SENSOR_COUNT; i++) {
    int rawValue = readGasSensor(i);
    rawReadings[i] = rawValue;

    int baselineAdjustedVal = rawValue - baseline[i];
    if (baselineAdjustedVal > thresholdMargin) sensorActive = true;

    int featureInput = max(0, baselineAdjustedVal);
    features[i] = getSmoothedValue(i, featureInput);
  }
  historyIndex = (historyIndex + 1) % smoothingWindowSize;

  if (sensorActive) {
    int prediction = gasModel.predict(features);
    if (prediction > 0) {
      predictionCounter++;
      lastPrediction = prediction;
    } else {
      predictionCounter = 0;
      lastPrediction = 0;
    }
    setAlarm(predictionCounter >= consecutiveRequired);
  } else {
    setAlarm(false);
    predictionCounter = 0;
    lastPrediction = 0;
  }

  printGasDetectionReport(millis() / 1000);
  reportGasDetection();

  delay(500);
}
