#include "Adafruit_TinyUSB.h"

uint8_t const desc_hid_report[] = {
  // Gamepad 1
  0x05, 0x01, 0x09, 0x05, 0xA1, 0x01,
    0x85, 0x01,
    0x05, 0x09, 0x19, 0x01, 0x29, 0x04,
    0x15, 0x00, 0x25, 0x01,
    0x95, 0x04, 0x75, 0x01, 0x81, 0x02,
    0x95, 0x04, 0x75, 0x01, 0x81, 0x03,
    0x05, 0x01, 0x09, 0x39,
    0x15, 0x00, 0x25, 0x07,
    0x35, 0x00, 0x46, 0x3B, 0x01, 0x65, 0x14,
    0x75, 0x04, 0x95, 0x01, 0x81, 0x02,
    0x75, 0x04, 0x95, 0x01, 0x81, 0x03,
  0xC0,

  // Gamepad 2
  0x05, 0x01, 0x09, 0x05, 0xA1, 0x01,
    0x85, 0x02,
    0x05, 0x09, 0x19, 0x01, 0x29, 0x04,
    0x15, 0x00, 0x25, 0x01,
    0x95, 0x04, 0x75, 0x01, 0x81, 0x02,
    0x95, 0x04, 0x75, 0x01, 0x81, 0x03,
    0x05, 0x01, 0x09, 0x39,
    0x15, 0x00, 0x25, 0x07,
    0x35, 0x00, 0x46, 0x3B, 0x01, 0x65, 0x14,
    0x75, 0x04, 0x95, 0x01, 0x81, 0x02,
    0x75, 0x04, 0x95, 0x01, 0x81, 0x03,
  0xC0,

  // Teclado HID con Report ID 3
  0x05, 0x01, 0x09, 0x06, 0xA1, 0x01,
    0x85, 0x03,
    0x05, 0x07, 0x19, 0xE0, 0x29, 0xE7,
    0x15, 0x00, 0x25, 0x01,
    0x75, 0x01, 0x95, 0x08, 0x81, 0x02,
    0x95, 0x01, 0x75, 0x08, 0x81, 0x01,
    0x95, 0x06, 0x75, 0x08,
    0x15, 0x00, 0x25, 0xFF,
    0x05, 0x07, 0x19, 0x00, 0x29, 0xFF,
    0x81, 0x00,
  0xC0
};

typedef struct __attribute__((packed)) {
  uint8_t buttons : 4;
  uint8_t padding : 4;
  uint8_t hat : 4;
  uint8_t hat_padding : 4;
} gamepad_report_t;

gamepad_report_t gp1 = {0}, gp2 = {0};
gamepad_report_t prev_gp1 = {0}, prev_gp2 = {0};

Adafruit_USBD_HID usb_hid;

// Pines mandos Mega Drive
#define P1_UP     2
#define P1_DOWN   3
#define P1_LEFT   4
#define P1_RIGHT  5
#define P1_BTN6   6
#define P1_SEL    7
#define P1_BTN9   8

#define P2_UP     9
#define P2_DOWN   10
#define P2_LEFT   11
#define P2_RIGHT  12
#define P2_BTN6   13
#define P2_SEL    14
#define P2_BTN9   15

#define ADC_TECLAS A0

// Pines encoder rotativo
#define ENCODER_DT  27
#define ENCODER_CLK 20
#define ENCODER_SW  1

void setup() {
  Serial.begin(115200);

  pinMode(P1_UP, INPUT_PULLUP); pinMode(P1_DOWN, INPUT_PULLUP);
  pinMode(P1_LEFT, INPUT_PULLUP); pinMode(P1_RIGHT, INPUT_PULLUP);
  pinMode(P1_BTN6, INPUT_PULLUP); pinMode(P1_BTN9, INPUT_PULLUP);
  pinMode(P1_SEL, OUTPUT); digitalWrite(P1_SEL, HIGH);

  pinMode(P2_UP, INPUT_PULLUP); pinMode(P2_DOWN, INPUT_PULLUP);
  pinMode(P2_LEFT, INPUT_PULLUP); pinMode(P2_RIGHT, INPUT_PULLUP);
  pinMode(P2_BTN6, INPUT_PULLUP); pinMode(P2_BTN9, INPUT_PULLUP);
  pinMode(P2_SEL, OUTPUT); digitalWrite(P2_SEL, HIGH);

  pinMode(ENCODER_DT, INPUT);
  pinMode(ENCODER_CLK, INPUT);
  pinMode(ENCODER_SW, INPUT);

  TinyUSBDevice.setManufacturerDescriptor("Gamer Garage");
  TinyUSBDevice.setProductDescriptor("MegaDrive USB Adapter Dual");

  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.begin();

  if (TinyUSBDevice.mounted()) {
    TinyUSBDevice.detach(); delay(10); TinyUSBDevice.attach();
  }

  Serial.println("Adaptador Dual Mega Drive + Teclas ADC + Encoder listo.");
}

void loop() {
  #ifdef TINYUSB_NEED_POLLING_TASK
    TinyUSBDevice.task();
  #endif

  if (!TinyUSBDevice.mounted() || !usb_hid.ready()) return;

  leerMando(P1_UP, P1_DOWN, P1_LEFT, P1_RIGHT, P1_BTN6, P1_BTN9, P1_SEL, gp1);
  if (memcmp(&gp1, &prev_gp1, sizeof(gp1)) != 0) {
    usb_hid.sendReport(1, &gp1, sizeof(gp1));
    prev_gp1 = gp1;
  }

  leerMando(P2_UP, P2_DOWN, P2_LEFT, P2_RIGHT, P2_BTN6, P2_BTN9, P2_SEL, gp2);
  if (memcmp(&gp2, &prev_gp2, sizeof(gp2)) != 0) {
    usb_hid.sendReport(2, &gp2, sizeof(gp2));
    prev_gp2 = gp2;
  }

  enviarTeclaADC();
  leerEncoder();

  delay(10);
}

void leerMando(uint8_t upPin, uint8_t downPin, uint8_t leftPin, uint8_t rightPin,
               uint8_t btn6Pin, uint8_t btn9Pin, uint8_t selPin, gamepad_report_t &gp) {
  memset(&gp, 0, sizeof(gp));

  digitalWrite(selPin, HIGH); delayMicroseconds(10);
  bool up = !digitalRead(upPin);
  bool down = !digitalRead(downPin);
  bool left = !digitalRead(leftPin);
  bool right = !digitalRead(rightPin);

  gp.hat = 8;
  if (up && left)       gp.hat = 7;
  else if (up && right) gp.hat = 1;
  else if (down && left)gp.hat = 5;
  else if (down && right)gp.hat = 3;
  else if (up)          gp.hat = 0;
  else if (down)        gp.hat = 4;
  else if (left)        gp.hat = 6;
  else if (right)       gp.hat = 2;

  bool B = !digitalRead(btn6Pin);
  bool C = !digitalRead(btn9Pin);

  digitalWrite(selPin, LOW); delayMicroseconds(10);
  bool A = !digitalRead(btn6Pin);
  bool START = !digitalRead(btn9Pin);

  if (A)     gp.buttons |= (1 << 0);
  if (B)     gp.buttons |= (1 << 1);
  if (C)     gp.buttons |= (1 << 2);
  if (START) gp.buttons |= (1 << 3);
}

void enviarTeclaADC() {
  static uint8_t teclaAnterior = 0;
  uint16_t valor = analogRead(ADC_TECLAS);
  uint8_t teclaActual = 0;

  if (valor < 100)       teclaActual = 1; // F15
  else if (valor < 600) teclaActual = 2; // F14
  else if (valor < 800) teclaActual = 3; // F13

  if (teclaActual != teclaAnterior) {
    if (teclaAnterior != 0) {
      uint8_t vacio[8] = {0};
      usb_hid.sendReport(3, vacio, sizeof(vacio));
    }

    if (teclaActual != 0) {
      uint8_t reporte[8] = {0};
      switch (teclaActual) {
        case 1: reporte[2] = 0x6A; break; // F15
        case 2: reporte[2] = 0x69; break; // F14
        case 3: reporte[2] = 0x68; break; // F13
      }
      Serial.print("ADC tecla enviada: ");
      Serial.println(reporte[2]);
      usb_hid.sendReport(3, reporte, sizeof(reporte));
    }

    teclaAnterior = teclaActual;
  }
}

void enviarTecla(uint8_t keycode) {
  uint8_t reporte[8] = {0};
  reporte[2] = keycode;
  usb_hid.sendReport(3, reporte, sizeof(reporte));
  delay(5);
  uint8_t vacio[8] = {0};
  usb_hid.sendReport(3, vacio, sizeof(vacio));
}

void leerEncoder() {
  static int lastClk = HIGH;
  static uint8_t lastButton = HIGH;

  int clk = digitalRead(ENCODER_CLK);
  int dt  = digitalRead(ENCODER_DT);

  if (clk != lastClk && clk == LOW) {
    if (dt != clk) {
      Serial.println("Encoder → F16 (horario)");
      enviarTecla(0x6B); // F16
    } else {
      Serial.println("Encoder → F17 (antihorario)");
      enviarTecla(0x6C); // F17
    }
  }
  lastClk = clk;

  uint8_t button = digitalRead(ENCODER_SW);
  if (lastButton == HIGH && button == LOW) {
    Serial.println("Encoder → F18 (pulsador)");
    enviarTecla(0x6D); // F18
  }
  lastButton = button;
}





