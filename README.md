## Smart India Hackathon 2025
. Problem Statement ID - 25058
. Problem Statement Title - Detection and Prevention of Tampering in Weighing and Measuring Instruments
. Theme - Smart Automation
. Problem Statement Category - Hardware

## Anti-Tampering IoT Kit for Weighing and Measuring Devices
A collaborative ESP32-based project submitted for the **Smart India Hackathon 2025**.  
This system detects tampering using light, vibration, and magnetic field changes, and sends real-time alerts via a Blynk IoT dashboard.

## Hardware Used
. ESP32 Dev Board  
. LDR Sensor  
. MPU6050 Accelerometer  
. Reed Switch (LM393 module)  
. Active Buzzer  
. LEDs  
. WiFi connectivity via ESP32  
. Blynk dashboard for remote monitoring

## Features
. **LDR Sensor**: Detects sudden light changes (e.g. lid opened)  
. **MPU6050**: Detects vibration or impact, even slight movement  
. **Reed Switch**: Detects magnetic tampering (lid displacement)  
. **Buzzer**: Activates on any intrusion event  
. **LEDs**: Visual feedback for LDR and MPU6050 triggers  
. **Blynk Dashboard**: Displays sensor data and intrusion alerts in real time
  
## Blynk Dashboard Mapping
____________________________________
| Sensor / Output     | Virtual Pin |
|---------------------|-------------|
| LDR Value           | V0          |
| LDR LED Status      | V1          |
| MPU6050 Jerk Value  | V2          |
| Motion Alert        | V3          |
| Reed Switch State   | V4          |
| Buzzer Status       | V5          |
| Intrusion Flag      | V6          |
_____________________________________

## System Overview
. Built for Smart India Hackathon 2025 as a group submission.  
. While we weren’t selected, the project reflects strong teamwork, embedded design, and real-world prototyping.

## Team
This was a collaborative effort by our SIH team.  

## License
Open source; feel free to fork, remix, or build on it.
