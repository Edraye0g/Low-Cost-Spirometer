# Low-Cost Spirometer
Tushil|Rayeed|Borshon|Nabil|Fuad


## 1. Objective
 
The primary objectives of this experiment are:
 
- Design and simulate a Venturi-based transducer capable of measuring human respiratory flow rates (approximately 0–300 L/min).
- Analyze the differential pressure generated across a 30 mm to 10 mm constriction using Computational Fluid Dynamics (SolidWorks and SimScale).
- Implement a microcontroller-based data acquisition system using the MPX5010DP sensor to calculate real-time Flow Rate and Tidal Volume.
 
---
 
# 2. Introduction
 
## 2.1 Clinical Significance
 
Respiratory diseases, particularly Chronic Obstructive Pulmonary Disease (COPD) and Asthma, represent a major global health burden.
 
According to the World Health Organization (WHO), COPD is the third leading cause of death worldwide, causing 3.23 million deaths in 2019.
 
The primary diagnostic tool is **Spirometry**, a pulmonary function test that measures:
 
- Volume of air inhaled and exhaled
- Speed of airflow
 
Key parameters include:
 
- **Tidal Volume (TV):** Volume of air moved during normal breathing.
- **Forced Vital Capacity (FVC):** Total amount of air exhaled during a forced breath.
 
Accurate monitoring is essential for diagnosing airway obstruction and tracking disease progression.
 
## 2.2 Problem Statement
 
Standard clinical spirometers:
 
- Often use Fleisch pneumotachographs or ultrasonic sensors.
- Provide high accuracy.
- Cost approximately **BDT 170,000 and above**.
- Require frequent calibration and specialized maintenance.
 
These constraints limit accessibility in rural and resource-limited regions.
 
Therefore, there is a need for:
 
- Low-cost
- Portable
- Digitally enabled spirometers
 
built from readily available components.
 
## 2.3 Engineering Principle: The Venturi Effect
 
The project uses a **Venturi-based differential pressure transducer**.
 
Advantages:
 
- No moving parts
- Robust structure
- Easier sterilization
- Lower cost
 
The principle relies on:
 
1. Conservation of Mass (Continuity Equation)
2. Bernoulli's Principle
 
As air flows through a narrower throat:
 
- Velocity increases.
- Static pressure decreases.
 
Pressure taps located at:
 
- Point 1 (inlet)
- Point 2 (throat)
 
allow differential pressure measurement:
 
\[
\Delta P = P_1 - P_2
\]
 
### Figure 1
 
SOLIDWORKS-designed Venturi meter.
 
## 2.4 Mathematical Derivation
 
Volumetric flow rate:
 
\[
Q = C_d A_2 \sqrt{\frac{2\Delta P}{\rho(1-\beta^4)}}
\]
 
Where:
 
- \(Q\) = Volumetric flow rate (m³/s)
- \(C_d\) = Discharge coefficient (0.96–0.98)
- \(A_2\) = Throat cross-sectional area (m²)
- \(\rho\) = Air density (~1.225 kg/m³)
- \(\beta\) = Diameter ratio
 
Relationship:
 
\[
Q \propto \sqrt{\Delta P}
\]
 
The MPX5010DP sensor converts differential pressure to voltage.
 
The microcontroller then:
 
1. Linearizes the signal.
2. Calculates flow.
3. Integrates flow over time:
 
\[
V = \int Q\,dt
\]
 
to estimate exhaled volume.
 
---
 
# 3. Literature Review
 
A 2025 study by Wallace et al. developed a low-cost Venturi spirometer using:
 
- 3D printing
- MPX5010DP sensor
- Arduino microcontroller
 
Performance:
 
- Measured volume: 1.983 L
- Error: 1.53%
 
The study identified a throat diameter of about **10 mm** as optimal for balancing:
 
- Sensor resolution
- Peak Expiratory Flow (PEF)
 
This supports the design adopted in the present work.
 
---
 
# 4. Apparatus Required
 
## A. Mechanical and Simulation Tools
 
- SolidWorks
- SimScale (OpenFOAM based)
- 3D Printer (PLA)
- Scalp Vein Set
 
## B. Electronics Hardware
 
- Arduino Uno (ATmega328P)
- NXP MPX5010DP sensor
- 0.96" OLED display (128×64, I2C)
- Resistors
- Male-to-male jumper wires
 
---
 
# 5. Design and Simulation
 
## Step 1: Mechanical Design
 
Venturi tube dimensions:
 
- Inlet diameter: 30 mm
- Throat diameter: 10 mm
 
## Step 2: CFD Simulation
 
### Physics
 
- Incompressible fluid flow
 
### Boundary Conditions
 
**Inlet**
 
- 3 m/s
- 6 m/s
 
**Outlet**
 
- 0 Pa gauge pressure
 
**Turbulence Model**
 
- k-ω SST
 
### Results
 
Predicted pressure drops:
 
- 0.497 kPa
- 2.2 kPa
 
### Figure 2
 
- Velocity contour map
- Pressure contour map
- Velocity vectors showing airflow
 
## Step 3: Simulation Validation
 
### Table 1: CFD Results
 
| Parameter | 3 m/s | 6 m/s |
|------------|--------|--------|
| Density (kg/m³) | 1.20508 | 1.20508 |
| Inlet Pressure (kPa) | 101.441 | 101.795 |
| Throat Pressure (kPa) | 100.944 | 99.774 |
| ΔP (kPa) | 0.497 | 2.021 |
| Average Pressure (kPa) | 101.383 | 101.561 |
| Velocity (m/s) | 3.0039 | 6.023 |
| Flow Rate (m³/s) | 0.00207598 | 0.0042 |
| K Value | 201.2 | 176.6 |
 
Average:
 
\[
K = \frac{201.2 + 176.6}{2} = 186.4
\]
 
## Step 3: Signal Processing Algorithm
 
Arduino performs:
 
- ADC conversion
- Pressure calculation
- Flow calculation
 
Flow equation:
 
\[
Q = K\sqrt{\Delta P}
\]
 
where:
 
\[
K = 186.4
\]
 
Volume calculation:
 
\[
V_{tidal} = \sum (Q_t \Delta t)
\]
 
## Step 4: Arduino Validation
 
### Sensor Characterization
 
- Sensitivity: 0.45 V/kPa
- Zero Offset: ~0.20 V
 
Flow computation:
 
\[
Flow = 186\sqrt{Pressure}
\]
 
### Figure 3
 
Spirometer operation sequence.
 
---
 
# 6. Methodology
 
## 6.1 Prototype Fabrication
 
Material:
 
- PLA
 
Printing settings:
 
- 100% infill
- 0.12 mm layer height
 
Sensor integration:
 
- 4 mm silicone tubing
- P1 connected to inlet
- P2 connected to throat
 
Electronics:
 
- Sensor output connected to Arduino A0
 
## 6.2 Calibration Protocol
 
1. Place device on stable surface.
2. Leave airflow path open.
3. Record 50 readings over 0.5 seconds.
4. Compute average offset.
5. Subtract offset during operation.
 
## 6.3 Human Testing Procedure
 
Participants:
 
- Four healthy subjects
- Age: 20–24 years
 
### Procedure
 
#### a. Sterilization
 
70% isopropyl alcohol between tests.
 
#### b. Posture
 
- Seated upright
- Feet flat
 
#### c. Sealing
 
Tight lip seal on mouthpiece.
 
#### d. Test Maneuver
 
1. Normal breathing for 30 s.
2. Standard inhalation.
3. Normal exhalation into device.
 
#### e. Data Logging
 
Sampling frequency:
 
- 10 Hz
 
Outputs displayed:
 
- Flow Rate (L/min)
- Accumulated Volume (L)
 
## 6.4 Safety Precautions
 
- Operated at 5 V
- 60 s rest period between trials
 
---
 
# 7. Verifying Low Cost
 
## Table 2: Cost Breakdown
 
| Component | Cost (Tk.) |
|------------|------------|
| MPX5010DP | 2400 |
| Arduino Uno R3 | 400 |
| OLED Display | 300 |
| Breadboard | 50 |
| 3D Printing | 200 |
| Scalp Vein Set ×2 | 20 |
 
### Total Cost
 
**≈ BDT 3400**
 
### Figure 4
 
Component cost distribution.
 
### Figure 5
 
Medical-grade vs low-cost spirometer comparison.
 
---
 
# 8. Discussion
 
Subjects tested:
 
- Ananna
- Nowroz
- Shakil
- Rayeed
 
Measurements from:
 
1. Commercial spirometer
2. Prototype spirometer
 
### Figure 6
 
Tidal Volume comparison.
 
## 8.1 Quantitative Performance
 
### High Accuracy Cases
 
**Nowroz**
 
- Standard: 0.25 L
- Prototype: 0.27 L
- Difference: +0.02 L
 
**Shakil**
 
- Standard: 0.45 L
- Prototype: 0.42 L
- Difference: -0.03 L
 
### Higher Volume Deviation
 
**Ananna**
 
- Standard: 0.53 L
- Prototype: 0.40 L
- Error: ~24%
 
**Rayeed**
 
- Standard: 0.55 L
- Prototype: 0.46 L
- Error: ~16%
 
## 8.2 Error Analysis
 
### 1. Air Leakage
 
Potential leakage around the rigid PLA mouthpiece.
 
### 2. Discharge Coefficient Variability
 
Coefficient changes with Reynolds number.
 
### 3. Sensor Response and Sampling
 
Limited sampling frequency can underestimate peak flow.
 
## 8.3 Economic Feasibility
 
- Commercial spirometer: ~170,000 BDT
- Prototype: ~3,400 BDT
 
Cost reduction:
 
**≈ 97.8%**
 
Approximately:
 
**45× cheaper**
 
## 8.4 Cost-Utility Trade-off
 
Commercial spirometers remain necessary for diagnostic confirmation.
 
The prototype is suitable for:
 
- Mass screening
- Rural healthcare
- Low-resource environments
 
## 8.5 Conclusion
 
The spirometer:
 
- Correctly categorizes tidal volume ranges.
- Provides acceptable screening performance.
- Can be improved through:
- Better mouthpiece sealing
- Non-linear calibration curves
 
Future improvements are expected to increase accuracy while retaining low manufacturing cost.
 
---
 
# 9. References
 
1. WHO COPD Fact Sheet
2. Guyton and Hall Textbook of Medical Physiology (14th Edition)
3. TeleSpiro: A Low-Cost Mobile Spirometer for Resource-Limited Settings (IEEE, 2013)
4. Fundamentals of Fluid Mechanics (Munson)
5. Wallace et al., *Design, Manufacture and Validation of a Spirometry Device aimed for Low-Resource Settings* (2025)
 
---
 
# 10. Git Repository
 
https://github.com/Edraye0g/Low-Cost-Spirometer.git
