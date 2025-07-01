# MD2USB HUB — Mega Drive to USB + Macro Hub

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**Autor:** Javier García ([@GamerGarage](https://www.youtube.com/@GamerGarage))  
**Diseñado con:** KiCad  
**Fabricado por:** JLCPCB  
**Microcontrolador:** Raspberry Pi Pico (RP2040)  
**Estado del proyecto:** Funcional — *falta solo implementar el sistema de pantalla*

---

## 🔧 Descripción general

**MD2USB HUB** es un proyecto de hardware libre que integra varias funciones útiles en un solo dispositivo compacto:

- ✅ **Hub USB 2.0 de 4 puertos**  
  - 1 puerto se utiliza internamente por la Raspberry Pi Pico  
  - 3 puertos USB disponibles para el usuario

- 🎮 **2 puertos para mandos originales de SEGA Mega Drive / Genesis**

- 🎛️ **Teclado macro de 3 botones físicos**  
  - Totalmente programables mediante [PowerToys](https://learn.microsoft.com/en-us/windows/powertoys/)

- 🌀 **Encoder rotativo con pulsador central**  
  - También programable por software como una entrada de teclado  
  - Feedback visual mediante animaciones en pantalla

- 🖼️ **Pantalla LCD ST7735 (1.8", 128x160 píxeles)**  
  - Muestra una imagen por defecto almacenada en código  
  - Permite cargar una imagen personalizada desde tarjeta SD  
  - Muestra animaciones de retroalimentación al pulsar botones o girar el encoder

---

## 📦 Tecnologías y librerías utilizadas

- [Earle Philhower's Raspberry Pi Pico Arduino Core](https://github.com/earlephilhower/arduino-pico)
- [TinyUSB](https://github.com/hathach/tinyusb)
- [Adafruit ST7735](https://github.com/adafruit/Adafruit-ST7735-Library) *(próximamente)*

---

## 📁 Estructura del hardware

- Basado en chip **GL850G** como controlador del hub USB 2.0
- Raspberry Pi Pico se conecta a uno de los puertos para controlar:
  - Los botones macro
  - El encoder
  - La pantalla ST7735
  - Los puertos de mando de Mega Drive

---

## ⚡ Instalación y uso

> ⚠️ *Instrucciones más detalladas se añadirán cuando el módulo de pantalla esté finalizado.*

1. Conecta el hub a tu PC mediante cable USB
2. Usa [PowerToys](https://learn.microsoft.com/en-us/windows/powertoys/) para asignar funciones a los botones y encoder
3. Inserta una imagen personalizada (`128x160`, formato `.bmp`) en la tarjeta SD si deseas reemplazar la imagen por defecto

---

## 📜 Licencia

Este proyecto está licenciado bajo la Licencia MIT.  
Consulta el archivo [`LICENSE`](./LICENSE) para más detalles.

```text
MIT License © 2025 Javier García (GamerGarage)

---

## 💬 Créditos

Proyecto desarrollado por **Javier García**
Canal de YouTube: [GamerGarage](https://www.youtube.com/@GamerGarage)

---
