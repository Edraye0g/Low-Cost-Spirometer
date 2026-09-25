# Low-Cost Spirometer Design and Validation Document
(Tushil|Rayeed|Borshon|Nabil|Fuad)
## 1. Objective

The primary objectives of this experiment are given below:

* To design and simulate a Venturi-based transducer capable of measuring human respiratory flow rates (approx. $0\text{--}300\text{ L/min}$).

* To analyze the differential pressure generated across a $30\text{ mm}$ to $10\text{ mm}$ constriction using Computational Fluid Dynamics (Solidworks and SimScale).

* To implement a microcontroller-based data acquisition system using the MPX5010DP sensor to calculate real-time Flow Rate and Tidal Volume.

---

## 2. Introduction

### 2.1 Clinical Significance

Respiratory diseases, particularly Chronic Obstructive Pulmonary Disease (COPD) and Asthma, represent a major global health burden. According to the World Health Organization (WHO), COPD is the third leading cause of death worldwide, causing 3.23 million deaths in 2019. The primary diagnostic tool for these conditions is Spirometry, a pulmonary function test that measures the volume and speed of air inhaled and exhaled.

Key parameters measured include **Tidal Volume ($V_T$)**, the volume of air moved during normal breathing, and **Forced Vital Capacity (FVC)**, the total amount of air exhaled during a forced breath. Accurate monitoring of these parameters is critical for diagnosing airway obstruction and tracking disease progression.

### 2.2 Problem Statement

Standard clinical spirometers, often based on Fleisch pneumotachographs or ultrasonic sensors, provide high accuracy but come with significant barriers for low-resource settings. Commercial units typically cost between $1,70,000\text{ Tk}$ and $3,400\text{ Tk}$ and require frequent calibration and specialized maintenance. In rural healthcare facilities or developing regions, the lack of affordable diagnostic instrumentation leads to underdiagnosis and delayed treatment. Consequently, there is a pressing engineering need to develop low-cost, portable digital spirometers that maintain acceptable accuracy using off-the-shelf components.

### 2.3 Engineering Principle: The Venturi Effect

To address the cost barrier, this project utilizes a Venturi-based differential pressure transducer. Unlike turbine flow meters (which suffer from mechanical inertia) or hot-wire anemometers (which are fragile and computationally complex), a Venturi meter has no moving parts, making it robust and easy to sterilize.

The operation relies on the conservation of mass (Continuity Equation) and energy (Bernoulli's Principle). As air flows through a converging pipe, the fluid velocity must increase to pass through the narrower throat.

According to Bernoulli's Principle, this increase in kinetic energy (velocity) results in a simultaneous decrease in potential energy (static pressure). By placing pressure taps at the inlet (**Point 1**) and the throat (**Point 2**), the differential pressure ($\Delta P = P_1 - P_2$) can be measured.


### 2.4 Mathematical Derivation

The relationship between the measured pressure difference and the volumetric flow rate ($Q$) is derived for an incompressible fluid (approximated for low-speed airflow) as follows:
$$Q = C_d A_2 \sqrt{\frac{2\Delta P}{\rho(1 - \beta^4)}} \quad \text{--- (1)}$$

Where:

* $Q$: Volumetric flow rate ($\text{m}^3/\text{s}$)
* $C_d$: Discharge coefficient (accounts for viscous losses, typically $0.96\text{--}0.98$)
* $A_2$: Cross-sectional area of the throat ($\text{m}^2$)
* $\rho$: Density of air (approx. $1.225\text{ kg/m}^3$ at sea level)
* $\beta$: The Beta ratio ($d_{\text{throat}} / d_{\text{inlet}}$)

This non-linear relationship implies that flow is proportional to the square root of pressure ($Q \propto \sqrt{\Delta P}$). The system uses a differential pressure sensor (MPX5010DP) to convert $\Delta P$ into an analog voltage. The microcontroller then linearizes this signal and performs numerical integration over time ($V = \int Q \, dt$) to derive the total volume of air exhaled.

---

## 3. Literature Review

Recent developments in frugal biomedical engineering have demonstrated the viability of Venturi-based systems for respiratory monitoring. In a 2025 study, Wallace et al. designed and validated a low-cost spirometer utilizing the Venturi principle and 3D-printing technology for low-resource settings. Their device, which integrated a differential pressure sensor (MPX5010DP) with an Arduino microcontroller, was tested against a standard 2-liter calibration pump. The study reported a mean measured volume of $1.983\text{ L}$, resulting in an accuracy error of only $1.53\%$. Furthermore, their research highlighted that a throat diameter of approximately $10\text{ mm}$ is optimal for balancing sensor resolution with the maximum peak expiratory flow (PEF) of human subjects. This validates the design approach adopted in our experiment, confirming that differential pressure transduction is a reliable method for estimating tidal volume without expensive mechanical turbines.

---

## 4. Apparatus Required

### A. Mechanical & Simulation Tools

* **CAD Software:** SolidWorks (for geometry modeling)
* **CFD Solver:** SimScale (OpenFOAM based) and SolidWorks for pressure validation
* **3D Printer:** For fabricating the physical Venturi tube (PLA material)
* Scalp Vein Set

### B. Electronics Hardware

* **Microcontroller:** Arduino Uno (ATmega328P)
* **Pressure Sensor:** NXP MPX5010DP (Range: $0\text{--}10\text{ kPa}$, Sensitivity: $450\text{ mV/kPa}$)
* **Display:** 0.96" OLED Display ($128 \times 64$, I2C)
* Resistors
* Wires (Male to Male)

---

## 5. Design & Simulation

### Step 1: Mechanical Design

A Venturi tube was modeled with an ISO-standard inlet diameter of $30\text{ mm}$ to fit standard mouthpieces. To ensure a measurable pressure drop without exceeding the sensor's limit during peak expiration, the throat diameter was optimized to $10\text{ mm}$.

### Step 2: CFD Simulation (SimScale and SOLIDWORKS)

A steady-state simulation was performed to validate the design:

* **Physics:** Incompressible Fluid Flow.
* **Boundary Conditions:**
* **Inlet:** Velocity Inlet set to $3\text{ m/s}$ and $6\text{ m/s}$
* **Outlet:** Pressure Outlet at $0\text{ Pa}$ (Gauge)
* **Turbulence Model:** k-omega SST
* **Results:** The simulation predicted a pressure drop of $0.497\text{ kPa}$ and $2.021\text{ kPa}$.


### Step 3: Simulation Validation

**Table 1: Venturi Tube CFD Simulation Results for the value of constant $K$**

| Parameter | Inlet Velocity $3\text{ m/s}$ | Inlet Velocity $6\text{ m/s}$ |
| --- | --- | --- |
| **Density ($\text{kg/m}^3$)** | 1.20508| 1.20508|
| **Static Pressure inlet ($\text{kPa}$)** | 101.441| 101.795|
| **Static Pressure throat ($\text{kPa}$)** | 100.944| 99.774|
| **Pressure Differential $\Delta P$ ($\text{kPa}$)** | 0.497| 2.021|
| **Avg Static Pressure ($\text{kPa}$)** | 101.383| 101.561|
| **Avg Velocity ($\text{m/s}$)** | 3.0039| 6.023|
| **Volume Flow Rate ($\text{m}^3/\text{s}$)** | 0.00207598| 0.0042|
| **Value of $K$** | 201.2| 176.6|

$$\text{Average value of } K = \frac{201.2 + 176.6}{2} = 186.4$$

### Step 4: Signal Processing Algorithm & Arduino Simulation

The MPX5010DP outputs an analog voltage proportional to pressure. The Arduino performs the following operations:

1. **Analog-to-Digital Conversion (ADC):** Reads the $0\text{--}5\text{V}$ signal as a $0\text{--}1023$ integer.
2. **Pressure Calculation:** Converts voltage to Pascals.
3. **Flow Calculation:** Applies the simplified transfer function $Q = k\sqrt{\Delta P}$, where $k$ was determined to be $186.4$.
4. **Volume Integration:** Calculates Tidal Volume ($V_{\text{tidal}}$) by integrating flow over time:

$$V_{\text{tidal}} = \sum_{t=0}^{T} (Q_t \times \Delta t)$$

**Sensor Characterization:**
* **Sensitivity:** $0.45\text{ V/kPa}$
* **Zero Offset:** $\approx 0.20\text{ V}$ (Measured experimentally.
* **Algorithm Implementation:** $\text{Flow Rate} = Q_{\text{real}} = 186 \times \sqrt{P_{\text{measured}}}$

> **Fig. 3:** Spirometer operation sequence flowchart:
> 
> 
> `Start (Power on device)` $\rightarrow$ `Initialize System` $\rightarrow$ `Calibration (Calculate zero offset)` $\rightarrow$ `Read Sensor (Pin A0)` $\rightarrow$ `Filter Noise` $\rightarrow$ `Calculate Physics` $\rightarrow$ `Calculate Volume` $\rightarrow$ `Update Display (OLED)` $\rightarrow$ `Repeat`.
> 
> 

---

## 6. Methodology

### 6.1 Prototype Fabrication and Assembly

The Venturi tube was fabricated using Fused Deposition Modeling (FDM) 3D printing with Polylactic Acid (PLA) filament.

* **Print Settings:** To ensure air-tightness and structural integrity under pressure, the model was printed with 100% infill and a layer height of $0.12\text{ mm}$.

* **Sensor Integration:** The MPX5010DP differential pressure sensor was coupled to the Venturi tube using $4\text{ mm}$ silicone tubing. The "Pressure Port" ($P_1$) was connected to the inlet tap, and the "Vacuum Port" ($P_2$) was connected to the throat tap to measure the pressure drop ($\Delta P$).

* **Electronics:** The sensor output was wired to Analog Pin A0 of the microcontroller.

### 6.2 Calibration Protocol

Before human testing, a zero-point calibration was performed to eliminate sensor drift:

1. The device was placed on a stable, vibration-free surface.

2. The airflow path was left open to ambient air (zero flow condition).

3. The microcontroller recorded 50 consecutive readings over 0.5 seconds.

4. The average of these readings was stored as the `sensorOffset` voltage, which was subtracted from all subsequent measurements during the trial.

### 6.3 Human Testing Procedure

To validate the system's ability to measure Tidal Volume ($V_T$), tests were conducted on four healthy subjects (ages 20–24). The following protocol was strictly observed:

* **a) Sterilization:** The mouthpiece was sanitized with 70% isopropyl alcohol between subjects to maintain hygiene.

* **b) Posture:** Subjects were seated in an upright position with their feet flat on the floor to maximize thoracic expansion.

* **c) Sealing:** Subjects were instructed to hold the device horizontally and form a tight seal around the mouthpiece with their lips to prevent air leakage.

* **d) The Maneuver:**
1. *Phase 1 (Stabilization):* The subject breathed normally away from the device for 30 seconds.
2. *Phase 2 (Inhalation):* The subject took a standard resting inhalation.
3. *Phase 3 (Exhalation):* The subject exhaled normally (not forcefully) into the Venturi tube until their lungs reached functional residual capacity.

* **e) Data Logging:** The microcontroller sampled pressure data at $10\text{ Hz}$. The computed Flow Rate ($\text{L/min}$) and Accumulated Volume ($\text{L}$) were visualized in real-time on the OLED display.

### 6.4 Safety Precautions

* The device was operated at low voltages ($5\text{V}$) to ensure electrical safety.
* To prevent hyperventilation or dizziness, subjects were given a 60-second rest period between consecutive trials.

---

## 7. Verifying Low Cost

**Table 2: Components and Price Breakdown**

| No. | Component | Price (Tk.) |
| --- | --- | --- |
| 1 | MPX5010DP (Differential Pressure Sensor) | 2400|
| 2 | Arduino Uno R3 | 400|
| 3 | 0.96" OLED Display ($128 \times 64$, I2C) | 300|
| 4 | Breadboard | 50|
| 5 | 3D Printing | 200|
| 6 | Scalp Vein Set $\times 2$ | 20|
| **Total** |  | **$\approx 3,400\text{ Tk.}$**<br> |

---

## 8. Discussion

A comparative study was conducted on four subjects (**Ananna, Nowroz, Shakil, and Rayeed**). Their Tidal Volume ($V_T$) was measured first using a standard commercial spirometer (Control) and subsequently using the designed low-cost Venturi spirometer (Test).

### 8.1 Quantitative Performance

* **High Accuracy Cases:** For subjects Nowroz and Shakil, the device performed with high precision.


* **Nowroz:** Prototype $\approx 0.27\text{ L}$ vs. Standard $\approx 0.25\text{ L}$ (Difference: $+0.02\text{ L}$).


* **Shakil:** Prototype $\approx 0.42\text{ L}$ vs. Standard $\approx 0.45\text{ L}$ (Difference: $-0.03\text{ L}$).


* *Indication:* For low-to-medium flow rates, the calibrated transfer function is highly effective.




* **Deviation in Higher Volumes:** For subjects Ananna and Rayeed, a noticeable "underestimation" trend was observed.


* **Ananna:** Standard $\approx 0.53\text{ L}$ vs. Prototype $\approx 0.40\text{ L}$ (Error: $-24\%$).


* **Rayeed:** Standard $\approx 0.55\text{ L}$ vs. Prototype $\approx 0.46\text{ L}$ (Error: $\approx -16\%$).





### 8.2 Error Analysis & Engineering Constraints

1. **Air Leakage:** Minor air leakage at the interface between the rigid 3D-printed PLA mouthpiece and the subject's mouth caused exhaled air to bypass the Venturi throat.


2. **Discharge Coefficient Non-Linearity:** The algorithm assumes a constant $C_d$, but $C_d$ varies with Reynolds number. At higher flow rates, turbulence causes energy losses not fully captured by $Q = k\sqrt{P}$.


3. **Sensor Response Lag & Sampling Rate:** Numerical integration ($\sum \text{Flow} \times \Delta t$) sampled at $10\text{ Hz}$ may miss the rapid peak flow of a strong breath, leading to volume under-calculation.



### 8.3 Economic Feasibility (Cost-Benefit Analysis)

* **Traditional Device Cost:** $\approx 170,000\text{ BDT (Tk)}$

* **Low-Cost Prototype Cost:** $\approx 3,400\text{ BDT (Tk)}$

* **Cost Reduction:** $\approx 97.8\%$ (Roughly 45 times cheaper)



### 8.4 The Cost-Utility Trade-off

While standard spirometers ($170,000\text{ Tk}$) are necessary for diagnostic confirmation requiring $<3\%$ error, the proposed device ($3,400\text{ Tk}$) is ideal for mass screening in rural healthcare centers (Upazila Health Complexes). It effectively distinguishes between normal and abnormal respiratory patterns at a fraction of the cost.

### 8.5 Conclusion on Viability

The prototype successfully categorizes patients within their correct tidal volume ranges. Future iterations can bridge the accuracy gap by implementing a non-linear calibration curve via software and a silicone-sealed mouthpiece via hardware.

---

## 9. References

1. World Health Organization (WHO), "Chronic obstructive pulmonary disease (COPD)," Fact sheet. [Online]. Available: [https://www.who.int/news-room/fact-sheets/detail/chronic-obstructive-pulmonary-disease-(copd)](https://www.who.int/news-room/fact-sheets/detail/chronic-obstructive-pulmonary-disease-(copd)?utm_source=gemini)

2. J. E. Hall, M. E. Hall, and A. C. Guyton, *Guyton and Hall textbook of medical physiology*, 14th edition. Philadelphia, PA: Elsevier, 2021.


3. C. W. Carspecken, C. Arteta, and G. D. Clifford, "TeleSpiro: A low-cost mobile spirometer for resource-limited settings," in *2013 IEEE Point-of-Care Healthcare Technologies (PHT)*, Bangalore, India: IEEE, Jan. 2013, pp. 144-147. doi: `10.1109/PHT.2013.6461305`.


4. B. R. Munson, Ed., *Fundamentals of fluid mechanics*, 7th ed. Hoboken, NJ: Wiley, 2013.


5. J. Wallace, P. C. Rifá, T. Kannathasan, C. F. Hayfron-Benjamin, P. Anyanwu, and D. Piaggio, "Design, Manufacture and Validation of a Spirometry Device aimed for Low-Resource Settings," June 11, 2025, *In Review*. doi: `10.21203/rs.3.rs-6812011/v1`.



---
