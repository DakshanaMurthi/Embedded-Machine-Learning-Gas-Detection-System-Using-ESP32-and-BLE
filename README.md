# 🚨 Embedded Machine Learning Gas Detection System  

## 📌 Overview  
This project is a **smart multi-gas detection system** built using an **ESP32 microcontroller** and a **16-channel analog gas sensor array**. It leverages an **embedded Random Forest machine learning model** for real-time gas classification and provides **Bluetooth Low Energy (BLE)** alerts for remote monitoring.  

The system ensures **high accuracy, low false alarms, and quick hazard notifications**, making it suitable for both **industrial safety** and **residential environments**.  

---

## 🔍 Problem Statement  
Traditional gas detection systems face challenges such as:  
- Sensor drift and noise.  
- False positives due to static thresholds.  
- Cross-sensitivity between gases.  
- Limited wireless communication and scalability.  

This project addresses these issues through **dynamic calibration, ML-based classification, and BLE alerts**.  

---

## 🎯 Features  
- ✅ Real-time gas detection using 16 analog sensors.  
- ✅ Embedded **Random Forest ML model** (via EloquentTinyML).  
- ✅ **Baseline calibration** and **data smoothing** to reduce drift and noise.  
- ✅ **LED + Buzzer alerts** for immediate hazard indication.  
- ✅ **BLE communication** for mobile-based monitoring.  
- ✅ Achieves **97%+ accuracy** with response times under 1 second.  

---

## 🏗️ Hardware Requirements  
- ESP32 Development Kit  
- 16 × Analog Gas Sensors  
- CD74HC4067 Multiplexer  
- LED and Buzzer Indicators  
- 5V Power Supply (USB/Battery)  

---

## 💻 Software Workflow  
1. **Baseline Calibration** – Sets clean-air reference values.  
2. **Sensor Data Acquisition** – ESP32 collects data from 16 sensors via multiplexer.  
3. **Data Smoothing** – Moving average filtering to reduce noise.  
4. **ML Inference** – Random Forest classifier identifies gases in real-time.  
5. **Alarm Activation** – LED & buzzer trigger upon hazard detection.  
6. **BLE Notification** – Alerts sent wirelessly to connected mobile devices.  

---

## 📊 Performance  
- ⚡ Classification Accuracy: **>97%**  
- ⏱️ Response Time: **<1 second**  
- 📡 BLE Transmission Reliability: **>99%**  

---

## 🔮 Future Enhancements  
- 🌐 Cloud integration for historical data logging and visualization.  
- 📱 Dedicated mobile/web dashboards.  
- 🧪 Support for additional environmental sensors.  
- 🤖 Advanced ML models for improved gas discrimination.  

---

## 📂 Repository Structure  
```
📦 gas-detection-esp32-ml
 ┣ 📂 firmware/        # ESP32 code
 ┣ 📂 ml_model/        # Trained model & datasets
 ┣ 📂 hardware/        # Circuit diagrams, schematics
 ┣ 📂 docs/            # Reports, workflow diagrams
 ┣ 📜 README.md        # Project documentation
 ┗ 📜 LICENSE          # License file (MIT recommended)
```  

---

## 👤 Author  
**Dakshana Murthi**  
💡 AI & Data Science Enthusiast | Embedded ML Developer  
