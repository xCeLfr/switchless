# switchless with ATtiny 45/85

## Master system
INSTALL [INSTALL French](MasterSystem/INSTALL_MS2_fr.md).  
ATTiny 45/85 [Master System sketch](MasterSystem/switchless/switchless.ino)

## Megadrive

1. Check if your console **RESET** is **Active-HIGH** or **Active-LOW**

Turn on your console and measure the voltage between the RESET button and GND:
- If you have 5V ==> Active-HIGH
- If you have 0V ==> Active-LOW

![Reset](Megadrive/Reset.png)

2. Program your ATTiny (Edit the sketch and set RESET_ACTIVE_LOW_OR_HIGH HIGH or LOW)
ATTiny 45/85 [Megadrive sketch](Megadrive/switchless/switchless.ino)

3. Install
PAL :  
INSTALL [INSTALL French](Megadrive/INSTALL_MD1.md).  
- VA4 1600-09, 1601-09  
- VA5-VA6 1601-18  
