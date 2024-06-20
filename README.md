# Internet of THings 

## Block Diagram
<img src="https://github.com/WasiatD/Internet-of-Things/blob/d004f344c30fbfdf679d596c1cf52eed86b421c1/Diagram%20Block.png" width="40%" />  

## Sensors

- **Soil Moisture Sensor**: Measures the moisture content in the soil to ensure optimal watering levels.
  <img src="https://m.media-amazon.com/images/I/61pScDFgHyS.jpg" width="40%" />  
- **LDR (Light Sensor)**: Monitors the light intensity to ensure plants receive adequate light.
    <img src="https://images-eu.ssl-images-amazon.com/images/I/61aojoXP0QL._AC_UL600_SR600,600_.jpg" width="40%" />  
- **LM35 (Temperature Sensor)**: Records the ambient temperature to maintain ideal growing conditions.
      <img src="https://images.tokopedia.net/img/cache/700/product-1/2018/12/31/7267218/7267218_44a908b7-691f-4630-9dd3-14b5db984cac_500_500.jpg" width="40%" />  
## Actuator

- **Water Pump**: Automatically waters plants based on the soil moisture levels detected.

## Microcontroller

- **ESP32**: A powerful microcontroller used to process sensor data and control the actuator, providing connectivity for remote monitoring and control.

## System Specifications

- **Microcontroller**: ESP32
- **Components**:
  - Soil Moisture Sensor
  - LDR (Light Sensor)
  - LM35 (Temperature Sensor)
  - Water Pump
- **Connectivity**: Wi-Fi

## Working Principles

The system operates as follows:

1. **Monitoring**:
   - The soil moisture sensor measures the soil's moisture content.
   - The LDR sensor monitors the light intensity.
   - The LM35 sensor records the ambient temperature.
   
2. **Actuation**:
   - Based on the soil moisture readings, the ESP32 controls the water pump to maintain optimal soil moisture levels.

3. **Data Transmission**:
   - Sensor data is transmitted via Wi-Fi to a cloud platform for real-time monitoring and analysis.

## Installation and Setup

1. **Hardware Setup**:
   - Connect the ESP32 to the soil moisture sensor, LDR, LM35, and water pump as per the circuit diagram.
   
2. **Software Setup**:
   - Install the Arduino IDE.
   - Upload the provided Arduino code to the ESP32.

3. **Configuration**:
   - Configure Wi-Fi settings for the ESP32 to enable data transmission.

## Future Enhancements

- Addition of more sensors for comprehensive environmental monitoring.
- Implementation of advanced analytics for predictive maintenance and optimization.
