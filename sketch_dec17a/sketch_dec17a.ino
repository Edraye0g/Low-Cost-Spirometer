#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// --- SCREEN SETTINGS ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- HARDWARE ---
const int sensorPin = A0;   
const int buttonPin = 2;    

// --- CALIBRATION ---
float sensorOffset = 0; 

// 1. STABLE K (As requested)
float k_constant = 46.5; 

// 2. VOLUME MULTIPLIER (The Magic Fix)
// Since k=46.5 gives ~0.3L, we multiply by 1.7 to get ~0.5L
// accurate volume without adding noise.
float VOLUME_SCALER = 1.7; 

// --- SMOOTHING ---
float filteredValue = 0.0;  
float alpha = 0.1;          

// --- VARIABLES ---
unsigned long previousMillis = 0;   
unsigned long lastDisplayTime = 0;  
unsigned long breathStopTimer = 0;  

float currentVol = 0.0;        
float peakFlow = 0.0;
float previousFlow = 0.0; 
bool isBreathing = false;           

// --- THRESHOLDS ---
// Low thresholds to catch the full breath, relying on Deadzone for silence
const float START_THRESHOLD = 2.0;  
const float CUTOFF_THRESHOLD = 1.0; 
const int BREATH_TIMEOUT = 1000;    // Wait 1 sec to confirm stop

void setup() {
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { for(;;); }
  pinMode(buttonPin, INPUT_PULLUP); 
   
  // Calibration
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10, 20);
  display.println("CALIBRATING...");
  display.setCursor(10, 35);
  display.println("Stay Still...");
  display.display();
  delay(1500); 

  long total = 0;
  for(int i=0; i<50; i++){
    total += analogRead(sensorPin);
    delay(5);
  }
  sensorOffset = total / 50.0;
   
  display.clearDisplay();
  previousMillis = millis(); 
}

void loop() {
  // 1. Time
  unsigned long currentMillis = millis();
  float dt = (currentMillis - previousMillis) / 1000.0; 
  previousMillis = currentMillis;

  // 2. Read Sensor
  float rawValue = analogRead(sensorPin);
  float adjustedValue = rawValue - sensorOffset;
  
  // --- 3. ZERO LOCK (DEADZONE) ---
  // If the raw sensor value is tiny (noise 1, 2, or 3), FORCE it to 0.
  // This guarantees the math hits 0.00 and the breath STOPS.
  if (adjustedValue < 5.4) {
    adjustedValue = 0;
  }

  // Smooth the data
  filteredValue = (filteredValue * (1.0 - alpha)) + (adjustedValue * alpha);

  // 4. Physics
  float voltage = filteredValue * (5.0 / 1023.0);
  float pressurePa = (voltage / 0.45) * 1000.0;
   
  float flowRate = 0;
  if (pressurePa > 0) {
    flowRate = k_constant * sqrt(pressurePa / 1000.0); 
  }

  // 5. Volume Integration
  if (flowRate > START_THRESHOLD) {
    isBreathing = true;
    breathStopTimer = millis(); 
    if (flowRate > peakFlow) peakFlow = flowRate;
  }

  if (isBreathing && flowRate > CUTOFF_THRESHOLD) {
     float averageFlow = (flowRate + previousFlow) / 2.0;
     
     // We calculate raw volume here...
     float instantVol = (averageFlow / 60.0) * dt;
     
     // ...and apply the SCALER here to get to 0.5L
     currentVol += (instantVol * VOLUME_SCALER);
  }
  previousFlow = flowRate;

  // 6. Stop Logic
  if (isBreathing) {
     // Because of the Zero Lock, flowRate will definitely hit 0.0 here
     if (flowRate <= CUTOFF_THRESHOLD) {
        if (millis() - breathStopTimer > BREATH_TIMEOUT) {
           showResultsWindow(); 
           
           isBreathing = false;
           currentVol = 0;
           peakFlow = 0;
           filteredValue = 0; 
           previousFlow = 0; 
        }
     }
  }

  // 7. Update Screen
  if (millis() - lastDisplayTime > 150) {
    updateLiveScreen(flowRate);
    lastDisplayTime = millis();
  }
}

void updateLiveScreen(float flow) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("LIVE");
  display.drawLine(0, 10, 128, 10, WHITE);

  display.setTextSize(2);
  display.setCursor(0, 20);
  display.print(flow, 1); 
  display.setTextSize(1);
  display.print(" L/m");

  display.setCursor(0, 45);
  display.print("Vol: ");
  display.print(currentVol, 2); 
  display.print(" L");

  // Bar Graph
  int maxScale = (peakFlow > 100) ? 600 : 100;
  int barFlow = flow; if (barFlow > maxScale) barFlow = maxScale; 
  int barWidth = map(barFlow, 0, maxScale, 0, 128); 
  display.fillRect(0, 56, barWidth, 8, WHITE);
  display.display();
}

void showResultsWindow() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("--- RESULT ---");
  display.drawLine(0, 10, 128, 10, WHITE);

 // 1. Peak Flow Rate
  display.setCursor(0, 20);
  display.setTextSize(1);
  display.println("MAX FLOW RATE:");
  
  display.setTextSize(1.4);
  display.print(peakFlow, 1);
  display.setTextSize(1);
  display.println(" L/min");

  // 2. Tidal Volume
  display.setCursor(0,37);
  display.setTextSize(1);
  display.println("TIDAL VOLUME:");
  
  display.setTextSize(2);
  display.print(currentVol, 3); 
  display.setTextSize(1);
  display.println(" L");

  display.setTextSize(1);
  display.setCursor(80, 55);
  display.print("[RESET]");
  display.display();
  
  while(digitalRead(buttonPin) == HIGH) { }
  
  display.clearDisplay();
  display.setCursor(40, 30);
  display.print("RESET!");
  display.display();
  delay(500); 
  previousMillis = millis(); 
  filteredValue = 0; 
  previousFlow = 0;
}