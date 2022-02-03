# switchless with ATTiny 45/85
Yet another Switchless mod for Master System and Megadrive.
This a simple mod is designed to be easy to install and understand.


## 1. Master system
INSTALL [INSTALL French](MasterSystem/INSTALL_MS2_fr.md).  
ATTiny 45/85 [Master System sketch](MasterSystem/switchless/switchless.ino)


## 2. Megadrive

2.1. Check if your console **RESET** is **Active-HIGH** or **Active-LOW**

Turn on your console and measure the voltage between the RESET button and GND:
- If you have 5V ==> Active-HIGH
- If you have 0V ==> Active-LOW

![Reset](Megadrive/Reset.png)

2.2. Program your ATTiny (Edit the sketch and set RESET_ACTIVE_LOW_OR_HIGH HIGH or LOW)
ATTiny 45/85 [Megadrive sketch](Megadrive/switchless/switchless.ino)

3. Install the Mod

PAL :  
INSTALL [INSTALL French](Megadrive/INSTALL_MD1.md).  
- VA4 1600-09, 1601-09  
- VA5-VA6 1601-18  
