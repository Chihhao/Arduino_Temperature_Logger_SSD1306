# Arduino_Temperature_Logger_SSD1306 (溫溼度紀錄器)
## 利用 DHT22 溫溼度偵測器  

## 成品如下 
![image](https://github.com/Chihhao/Arduino_Temperature_Logger_SSD1306/blob/main/image/1.jpg)

## 材料
(1) Arduino Nano  
(2) DHT22  
(3) 0.96吋OLED (128x64)  
(4) DS3231時鐘  
(5) SPI Nor Flash (Mxic or Winbond)(1Gb)  

## 3D列印外殼
(1) thingiverse: https://www.thingiverse.com/thing:4861300  
(2) tinkercad: https://www.tinkercad.com/things/eZMaNDogAWy  

## 函式庫
(1) 請複製library目錄中的所有資料到Arduino的library目錄 (ex: C:\Users\USERNAME\Documents\Arduino\libraries)  
(2) 或者可以自行下載，網址如下  
https://github.com/jarzebski/Arduino-DS3231  
https://github.com/Marzogh/SPIMemory  
https://github.com/adafruit/Adafruit_Sensor  
https://github.com/greiman/SSD1306Ascii  
https://github.com/adafruit/DHT-sensor-library  

## 注意
(1) 預設每4秒會記錄一筆資料，可以連續紀錄約180天，超過會自動回頭覆蓋，就像行車紀錄器  
(2) Nor Flash 製作成可拆卸式的，需要讀取紀錄的時候，拆下來去另外一個裝置讀取  

![image](https://github.com/Chihhao/Arduino_Temperature_Logger_SSD1306/blob/main/image/wiring.png)
![image](https://github.com/Chihhao/Arduino_Temperature_Logger_SSD1306/blob/main/image/2.png)

