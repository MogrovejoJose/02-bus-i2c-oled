// ============================================================================
// UETS SOPORTE TÉCNICO — SEMANA 02 — BLOQUE 2: INICIALIZACIÓN OLED & CABECERA
// 3° Bachillerato Técnico en Informática (2026–2027)
// ============================================================================

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET_PIN -1
#define OLED_I2C_ADDR 0x3C
#define SERIAL_BAUD 115200

// Instanciación del objeto display con sus 4 parámetros:
// (Ancho, Alto, Puntero al Bus Wire, Pin de Reset)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    Serial.println("\n[BLOQUE 2] Inicializando Pantalla OLED SSD1306...");

    // Inicializamos el bus I2C en los pines por defecto del ESP32 (SDA=21, SCL=22)
    Wire.begin(21, 22);

    // TODO 2.1: Arrancar la pantalla activando la bomba de carga interna (charge pump)
    // Respuesta a Pregunta Guía: La bomba de carga eleva los 3.3V entregados por el ESP32 
    // hasta aproximadamente 7.5V-9V, que es el voltaje necesario para iluminar los OLEDs.
    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDR)) {
        Serial.println("[OLED] ❌ ERROR: Pantalla no detectada en 0x3C.");
        while (true); // Detiene la ejecución si hay falla física
    }

    Serial.println("[OLED] Pantalla SSD1306 inicializada [OK]");

    // TODO 2.2: Construir la cabecera visual en el buffer RAM
    display.clearDisplay();                            // 1. Limpia el buffer en memoria RAM
    display.setTextColor(SSD1306_WHITE);              // 2. Establece color blanco activo
    display.setTextSize(1);                           // 3. Tamaño de texto base (6x8 px)
    display.setCursor(8, 0);                          // 4. Cursor en coordenadas X=8, Y=0
    display.println(">> ESP32 SISTEMA <<");            // 5. Escribe el título
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);  // 6. Línea horizontal separadora

    // TODO 2.3: ¡LA ORDEN MÁGICA!
    // Respuesta a Pregunta Clave: La pantalla sigue negra porque todas las funciones previas 
    // solo dibujan en un buffer temporal guardado en la memoria RAM del microcontrolador. 
    // La función `display.display();` transfiere todo ese buffer vía I2C hacia el chip SSD1306 
    // para que se enciendan los píxeles en el panel físico.
    display.display();

    Serial.println("[OLED] Cabecera visual renderizada exitosamente.");
}

void loop() {
    delay(1000);
}