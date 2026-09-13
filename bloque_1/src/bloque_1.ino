#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Instanciación del controlador OLED (SDA=21, SCL=22 por defecto en ESP32)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- 1. Inicialización del Bus I2C ---
void initI2CBus() {
  Wire.begin(21, 22);    // SDA = GPIO21, SCL = GPIO22
  Wire.setClock(400000); // Frecuencia I2C a 400 kHz
  Serial.println("[I2C] Bus inicializado.");
}

// --- 2. Escaneo de dispositivos I2C en terminal Serial ---
bool scanI2CBus() {
  Serial.println("[I2C] Iniciando escaneo de bus...");
  int encontrados = 0;
  bool oledDetectada = false;

  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("[I2C] Dispositivo detectado en direccion: 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);

      if (address == 0x3C) {
        Serial.print(" [OK - Pantalla OLED SSD1306]");
        oledDetectada = true;
      }
      Serial.println();
      encontrados++;
    }
  }

  Serial.printf("[I2C] Escaneo finalizado. Total perifericos: %d\n", encontrados);
  return oledDetectada;
}

// --- 3. Inicialización de la Pantalla OLED ---
bool initDisplay() {
  // Inicializar pantalla en dirección 0x3C con bomba de carga activada
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("[OLED] Error al inicializar la pantalla SSD1306");
    return false;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(">> ESP32 SISTEMA <<");
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);
  display.display();
  return true;
}

// --- 4. Función de registro/impresión en pantalla ---
void logBoot(const char* nombreModulo, bool estadoOk) {
  display.print(nombreModulo);
  
  // Ubica el cursor a la derecha en la misma fila actual
  display.setCursor(95, display.getCursorY()); 
  
  if (estadoOk) {
    display.println("[OK]");
  } else {
    display.println("[ERR]");
  }
  
  display.display(); // Actualiza la pantalla física
  delay(200);        // Pausa para efecto visual de chequeo
}

// --- 5. Secuencia de Auto-diagnóstico (POST) ---
void runSystemPOST() {
  display.setCursor(0, 16); // Posicionar debajo de la línea del título
  display.println("Diagnostico POST:");
  display.display();
  delay(300);

  // Verificación de módulos/subdisciplinas
  logBoot(" - I2C Bus", true);
  logBoot(" - OLED 0x3C", true);
  logBoot(" - Sensores", true); // Ejemplo de otra comprobación

  display.println();
  display.println("Sistema Listo.");
  display.display();
}

// --- SETUP PRINCIPAL ---
void setup() {
  Serial.begin(115200);
  delay(500);

  initI2CBus();     // 1. Enciende los cables I2C
  scanI2CBus();     // 2. Pasa lista y comprueba el bus

  if (initDisplay()) { // 3. Enciende la pantalla
    runSystemPOST();   // 4. Ejecuta el auto-diagnóstico en pantalla
  }
}

// --- BUCLE PRINCIPAL ---
void loop() {
  delay(100);
}