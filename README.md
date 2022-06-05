# COMSYS723_Assignment_1_Team12_Cruise_Control

## Portal 

[miro board](https://miro.com/app/board/uXjVO1jlEYU=/?share_link_id=275421005499)

## Time Tracker
Hao:
- (10/05/22) = 2
- (16/05/22) = 2
- (17/05/22) = 4

Chamith Nanayakkara:
- (10/05/22) = 2
- (17/05/22) = 5 (PI controller and c file combine)
- (26/05/22) = 4 (cruise speed updater implementation)
- (02/06/22) = 4 (cruise state updater & final implementations)

1. Must use cruiser regulation
2. `vectors.in` & `vectors.out` verification files
	- Output do not need to match tick, but must settle to correct value over reasonable time.

## Deliverables (3rd June)
- Peer review
- SINGLE `group_X.zip` 
	- joint hardcopy brief report `report_group_X.pdf`
		- 5 A4 pages with 10 point Times New Roman font
		- describes main elements of the solution
		- formal specification diagrams
	- Project files with `README`
	- Project evaluate form.

## Steps
Model-based approach.

- develop the system context diagram
- perform data-flow/control-flow decomposition, 
- describe the cruise control behaviour using a FSM
	- With concurrency
- map model to Esterel V5 using the hierarchy,  concurrency, and communication primitives within the language

# Specifications
- Maintain constant speed without:
	- Acceleration
	- Break

### Cruise Parameters
Speed Parameters:

1) **`SpeedMin`:** 30.0 km/h  
2) **`SpeedMax`:** 150.0 km/h  
3) **`SpeedInc`:** 2.5 km/h

Integral Parameters:

5) **`Kp`:** 8.113  
6) **`Ki`:** 0.5  
8) **`ThrottleSatMax`:** 45.0 percent  

Driver Operation Parameters:
10) **`PedalsMin`:** 3.0 percent

### Interface Requirements
**Inputs:**
- Panel Settings:
	1) **`On` (pure):** Enable the cruise control.  
	2) **`Off` (pure):** Disable the cruise control.  
	3) **`Resume` (pure):** Resume the cruise control after braking.  
	4) **`Set` (pure):** Set the current speed as the new cruise speed.  
	5) **`QuickDecel` (pure):** Decrease the cruise speed.  
	6) **`QuickAccel` (pure):** Increase the cruise speed.  
- Driver Operation:
	1) **`Accel` (float):** Accelerator pedal sensor.  
	2) **`Brake` (float):** Brake pedal sensor.  
	3) **`Speed` (float):** Car speed sensor

**Outputs:**
1) **`CruiseSpeed` (float):** Cruise speed value.  
2) **`ThrottleCmd` (float):** Throttle command.  
3) **`CruiseState` (enumeration):** State of the cruise control. 
	- OFF, ON,  STDBY, or DISABLE

### Behavioural Requirements
- **Intial Conditions:**
- Cruise control shall be off. 
	- The output `CruiseState = OFF`  
		- Disables inputs: 
			- `Off`, `Resume`, `Set`, `QuickAccel` and `QuickDecel`.
- **Input Response:**
	- `On` = cruse control enable.
		- `CruiseState = ON`   
	- `Off` = cruise control disable.
		- `CruiseState = OFF` 
- **State Response:**
	- `CruiseState = ON` 
		- If `Speed` between `SpeedMin, SpeedMax` && `!Accel`:
			- Regulate `Speed`
			- `CruiseState = ON`   
	- `CruiseState = STDBY` (only entered by `Brake`, but resumes by `Resume` input)
		- If `Resume = present`, depending on `Accel` and the `Speed` value:
			- Using last set (stored) `CruiseSpeed`
			- `CruiseState = ON`  
			- `CruiseState = OFF`  
- **Driver Operation Response:**
	- `Accel = present` ||   `Speed` not between `SpeedMin, SpeedMax`
		- `CruiseState = DISABLE`
	- `Accel = absent` ||   `Speed` between `SpeedMin, SpeedMax`
	 	- `CruiseState = ON`, still dependent on `on/off`
	 	- Return to previosly set (stored)  `CruiseSpeed`.
	- `Brake = present`
		- Cruse immediate abort
		 	- `CruiseState = STDBY`
 - **Assumptions:**
	 - `Accel` & `Brake` will never be present together. 

**Notes:**
- Cruse may be enabled by:
	- External pure signal `ON`
	- Internal violation of `SpeedMin, SpeedMax`

### Control Propertities
- `CruiseState = OFF`  
	- `Speed` dependent on `Accel` only.
- `CruiseState = ON` 
	-  `Speed` regulated by cruise control logic

- **Regulation Logic:**
	- Proportional and integral algorithm
		- `Kp` and `Ki` factors
	- Integral part:
		- Must be protected from overshoot.
		- If cruise control is operating `CruiseState = ON` && `Speed` between `SpeedMin, SpeedMax` && `!Accel`:
			- Integral action is reset.
	- `ThrottleCmd` will saturate at `ThrottleCmd = ThrottleSatMax`
		- Integral action is frozen.
		- Limits acceleration.

### Speed management
- `CruiseSpeed` only able to be changed if `CruiseState = ON, STBDY, or DISABLE`
	- `CruiseSpeed = Speed`  when:
		- First entering `CruiseState = ON`
		- `Set = present`
	- `CruiseSpeed` clipped by `SpeedMin, SpeedMax`
	- **Changes:**
		- While obying `SpeedMin, SpeedMax`:
			- `QuickAccel`
				- Increase `CruiseSpeed` by `SpeedInc`
			- `QuickDecel`
				- Decrease `CruiseSpeed` by `SpeedInc`

### Driver Operation Detection
- `Accel > PedalsMin` -> `Brake = present`
- `Brake > PedalsMin` -> `Brake = present`
