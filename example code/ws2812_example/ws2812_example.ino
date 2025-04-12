#include <FastLED.h>

// WS2812B RGB LEDs configuratie
#define RGB_DATA_PIN    5    // Data pin verbonden met WS2812B DIN
#define NUM_LEDS        6    // Aantal WS2812B LEDs in de keten
#define BRIGHTNESS     50    // Standaard helderheid (0-255)
#define LED_TYPE   WS2812B
#define COLOR_ORDER    GRB

// Definieer de LED array
CRGB leds[NUM_LEDS];

// Variabelen voor animaties
uint8_t gHue = 0;  // roterende "basis kleur" voor regenboog effecten
uint8_t currentPattern = 0;
uint8_t numPatterns = 6;  // Totaal aantal patronen

// Variabelen voor animaties (om scope-problemen te voorkomen)
uint8_t pos = 0;       // Voor bewegende kleur
uint8_t posDot = 0;    // Voor stippel-animatie
uint8_t pos1 = 0;      // Voor dubbele animatie
uint8_t pos2 = 0;      // Voor dubbele animatie
uint8_t brightness = 0;    // Voor pulserende animatie
uint8_t temperature = 0;   // Voor temperatuur animatie

// Timing variabelen
unsigned long previousMillis = 0;
unsigned long patternDuration = 10000;  // Tijd tussen patroonwisselingen (ms)

void setup() {
  // Initialiseer seriële communicatie voor debugging
  Serial.begin(115200);
  Serial.println("WS2812B RGB LED Controller");
  
  // Initialiseer WS2812B LEDs
  FastLED.addLeds<LED_TYPE, RGB_DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);
  FastLED.setBrightness(BRIGHTNESS);
  
  // Alle LEDs uitzetten
  FastLED.clear();
  FastLED.show();
  
  // Start met een testsequentie
  startupSequence();
}

void loop() {
  // Controleer of het tijd is om van patroon te veranderen
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= patternDuration) {
    previousMillis = currentMillis;
    currentPattern = (currentPattern + 1) % numPatterns;
    Serial.print("Verandering naar patroon: ");
    Serial.println(currentPattern);
  }
  
  // Update de RGB LEDs op basis van het huidige patroon
  updateRgbPattern(currentPattern);
  
  // Laat de FastLED show functie draaien
  FastLED.show();
  
  // Voeg een kleine vertraging toe om animaties soepel te houden
  FastLED.delay(15);
  
  // Update de kleur die gebruikt wordt voor sommige animaties
  EVERY_N_MILLISECONDS(20) { gHue++; }
}

// Verschillende RGB LED patronen
void updateRgbPattern(uint8_t patternNumber) {
  switch (patternNumber) {
    case 0:
      // Regenboog
      fill_rainbow(leds, NUM_LEDS, gHue, 7);
      break;
      
    case 1:
      // Kleur verschuiven (blauw beweegt over de strip)
      fadeToBlackBy(leds, NUM_LEDS, 20);
      EVERY_N_MILLISECONDS(50) {
        pos = (pos + 1) % (NUM_LEDS + 5);
      }
      if (pos < NUM_LEDS) {
        leds[pos] = CRGB::Blue;
      }
      break;
      
    case 2:
      // Pulserende rode kleur
      brightness = beatsin8(60, 20, 255);
      fill_solid(leds, NUM_LEDS, CHSV(0, 255, brightness));
      break;
      
    case 3:
      // Bewegende regenboog stippen
      fadeToBlackBy(leds, NUM_LEDS, 20);
      posDot = beatsin16(13, 0, NUM_LEDS - 1);
      leds[posDot] += CHSV(gHue, 255, 192);
      break;
      
    case 4:
      // Twee tegengestelde kleurrotaties
      fadeToBlackBy(leds, NUM_LEDS, 20);
      pos1 = beatsin16(10, 0, NUM_LEDS - 1);
      pos2 = beatsin16(15, 0, NUM_LEDS - 1);
      leds[pos1] += CHSV(gHue, 200, 255);
      leds[pos2] += CHSV(gHue + 128, 200, 255);
      break;
      
    case 5:
      // Kleurtemperatuur veranderingen
      temperature = beatsin8(10, 0, 255);
      fill_solid(leds, NUM_LEDS, HeatColor(temperature));
      break;
  }
}

// Opstartsequentie om te controleren of alle RGB LEDs werken
void startupSequence() {
  // RGB LED opstart test - witte flits
  fill_solid(leds, NUM_LEDS, CRGB::White);
  FastLED.show();
  delay(500);
  FastLED.clear();
  FastLED.show();
  delay(500);
  
  // RGB LED kleurentest
  // Rood
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(500);
  
  // Groen
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(500);
  
  // Blauw
  fill_solid(leds, NUM_LEDS, CRGB::Blue);
  FastLED.show();
  delay(500);
  
  // Alle LEDs uit
  FastLED.clear();
  FastLED.show();
  
  Serial.println("Opstartsequentie voltooid");
}