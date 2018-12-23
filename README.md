# Plant420

DIY Greenhouse control. (WIP)

A repository containing the code to measure temperature, humidity, light intensity and soil moisture with the ESP32.
Temperature + humidity sensor - Si7021
Light intensity               - TSL2561
Soil mosture sensor           - El Cheapo in Chinese

End product features:
-Water plant when soil mositure drops to low.
-Obtain data and analyse the data to be able to predict when the soil mositure will be at the level when more water will be needed.
-Has app to control the watering of the plant.

The ESP32 should then transfer the data through an ethernet client to a server, where the data will be stored to examine and fine tune the watering process.
