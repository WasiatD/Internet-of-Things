# Internet of THings 

## Block Diagram
<img src="https://github.com/WasiatD/Internet-of-Things/blob/d004f344c30fbfdf679d596c1cf52eed86b421c1/Diagram%20Block.png" width="40%" />  

## Sensors

- **Soil Moisture Sensor**: Measures the moisture content in the soil to ensure optimal watering levels.
  <img src="[https://github.com/WasiatD/Internet-of-Things/blob/d004f344c30fbfdf679d596c1cf52eed86b421c1/Diagram%20Block.png](https://www.google.com/url?sa=i&url=https%3A%2F%2Fen.indotrading.com%2Frytechindonesia1%2Fsoil-moisture-sensor-transducer-kelembaban-tanah-hygrometer-humidity-p943308.aspx&psig=AOvVaw3unbmLvl4zDh_xX9GpCR6x&ust=1718964546780000&source=images&cd=vfe&opi=89978449&ved=0CBEQjRxqFwoTCKD5vIr46YYDFQAAAAAdAAAAABAI)" width="40%" />  
- **LDR (Light Sensor)**: Monitors the light intensity to ensure plants receive adequate light.
- **LM35 (Temperature Sensor)**: Records the ambient temperature to maintain ideal growing conditions.

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
