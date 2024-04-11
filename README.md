Arduino Programm für das Arduino Regelungstechnik Modell

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Installation:
- Arduino IDE installieren
- Librarys herunterladen und in Arduino IDE einbinden (Sketch -> Bibliothek einbinden -> .zip Bibliothek hinzufügen -> Downloads -> Bibliothek auswählen)
- Programm herunterladen
- Richtigen COM Port Auswählen (Werkzeuge -> Port)
- Programm Hochladen

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Benötigte Librarys:
- https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library
- https://github.com/MattFryer/Smoothed
- https://github.com/adafruit/MAX6675-library
- https://github.com/terryjmyers/PWM
- https://github.com/br3ttb/Arduino-PID-Library

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Programmbeschreibung:
Es stehen 8 Modis zur Auswahl. Diese können mit den beiden Knöpfen des Modells angewählt werden:


- Mode 0: deaktiviert (Standartmodus nach Einschalten)
- 
- Mode 1: Offener Regelkreis
- Die Position des Potentiometers bestimmt die abgegebene Leistung
- 
- Mode 2: On/Off Regelkreis
- Die Wunschtemperatur wird über das Potentiometer eingestellt. Ist die gemessene Temperatur unter der gewünschten, ist die Leistung 100%,  ist die gemessene Temperatur über der gewünschten, ist die Leistung 0%
- 
- Mode 3: P Regelkreis
- Die Wunschtemperatur wird über das Potentiometer eingestellt. Nur der P-Teil des PID Reglers ist aktiv und steuert die Leistung entsprechend der gewünschten ung der gemessenen Temperatur.
- 
- Mode 4: I Regelkreis
- Die Wunschtemperatur wird über das Potentiometer eingestellt. Nur der I-Teil des PID Reglers ist aktiv und steuert die Leistung entsprechend der gewünschten ung der gemessenen Temperatur.
- 
- Mode 5: PI Regelkreis
- Die Wunschtemperatur wird über das Potentiometer eingestellt. Der P-Teil und der I-Teil des PID Reglers sind aktiv und steuert die Leistung entsprechend der gewünschten ung der gemessenen Temperatur.
- 
- Mode 6: PD Regelkreis
- Die Wunschtemperatur wird über das Potentiometer eingestellt. Der P-Teil und der D-Teil des PID Reglers sind aktiv und steuert die Leistung entsprechend der gewünschten ung der gemessenen Temperatur.
- 
- Mode 7: PD Regelkreis
- Die Wunschtemperatur wird über das Potentiometer eingestellt. Der PID Reglers ist vollständig aktiv und steuert die Leistung entsprechend der gewünschten ung der gemessenen Temperatur.

------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Visualisierung der Daten mit dem Seiellen Plotter:

- Adrsuino IDE starten
- Arduino per USB verbinden
- Richtigen COM Port Auswählen (Werkzeuge -> Port)
- Seriellen Plotter starten (Werkzeuge -> Serieller Plotter)
- Unten rechts 115200 Baud einstellen

(Es ist normal, dass der Aduino nach dem starten des seriellen Plotters neu startet)
