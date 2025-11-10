EESchema Schematic File Version 4
LIBS:hello.RGB-cache
EELAYER 26 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MCU_Microchip_ATtiny:ATtiny45V-10SU U1
U 1 1 5BB902A4
P 5150 3950
F 0 "U1" H 4850 4000 50  0000 R CNN
F 1 "ATtiny45V-10SU" H 5350 4150 50  0000 R CNN
F 2 "Package_SO:SOIJ-8_5.3x5.3mm_P1.27mm" H 5150 3950 50  0001 C CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/atmel-2586-avr-8-bit-microcontroller-attiny25-attiny45-attiny85_datasheet.pdf" H 5150 3950 50  0001 C CNN
	1    5150 3950
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R1
U 1 1 5BB903C2
P 7300 3800
F 0 "R1" H 7368 3846 50  0000 L CNN
F 1 "10K" H 7368 3755 50  0000 L CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 7340 3790 50  0001 C CNN
F 3 "~" H 7300 3800 50  0001 C CNN
	1    7300 3800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5BB9049C
P 4250 3950
F 0 "C1" H 4365 3996 50  0000 L CNN
F 1 "1uF" H 4365 3905 50  0000 L CNN
F 2 "Capacitor_SMD:C_1206_3216Metric_Pad1.42x1.75mm_HandSolder" H 4288 3800 50  0001 C CNN
F 3 "~" H 4250 3950 50  0001 C CNN
	1    4250 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5150 3350 7150 3350
Wire Wire Line
	7300 3350 7300 3650
$Comp
L Regulator_Linear:LM3480-5.0 U2
U 1 1 5BB924BC
P 3850 3350
F 0 "U2" H 3850 3592 50  0000 C CNN
F 1 "LM3480-5.0" H 3850 3501 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3850 3575 50  0001 C CIN
F 3 "http://www.ti.com/lit/ds/symlink/lm3480.pdf" H 3850 3350 50  0001 C CNN
	1    3850 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 3350 4250 3350
Connection ~ 5150 3350
Wire Wire Line
	5150 4550 4250 4550
$Comp
L LED:CLV1L-FKB D1
U 1 1 5BB92867
P 6900 3850
F 0 "D1" H 6900 3500 50  0000 C CNN
F 1 "CLV1L-FKB" H 6900 4256 50  0000 C CNN
F 2 "LED_SMD:LED_Cree-PLCC4_3.2x2.8mm_CCW" H 6900 3800 50  0001 C CNN
F 3 "http://www.cree.com/led-components/media/documents/CLV1L-FKB-1238.pdf" H 6900 3800 50  0001 C CNN
	1    6900 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R2
U 1 1 5BB93221
P 6250 3650
F 0 "R2" V 6150 3600 50  0000 C CNN
F 1 "1k" V 6150 3750 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 6290 3640 50  0001 C CNN
F 3 "~" H 6250 3650 50  0001 C CNN
	1    6250 3650
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R3
U 1 1 5BB9329B
P 6250 3750
F 0 "R3" V 6300 3600 50  0000 C CNN
F 1 "499" V 6300 3900 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 6290 3740 50  0001 C CNN
F 3 "~" H 6250 3750 50  0001 C CNN
	1    6250 3750
	0    1    1    0   
$EndComp
$Comp
L Device:R_US R4
U 1 1 5BB932E6
P 6250 3850
F 0 "R4" V 6350 3800 50  0000 C CNN
F 1 "1k" V 6350 3950 50  0000 C CNN
F 2 "Resistor_SMD:R_1206_3216Metric_Pad1.42x1.75mm_HandSolder" V 6290 3840 50  0001 C CNN
F 3 "~" H 6250 3850 50  0001 C CNN
	1    6250 3850
	0    1    1    0   
$EndComp
Wire Wire Line
	5750 3750 5800 3750
Wire Wire Line
	5750 3850 6000 3850
Wire Wire Line
	7100 3850 7150 3850
Wire Wire Line
	7150 3850 7150 3350
Connection ~ 7150 3350
Wire Wire Line
	7150 3350 7300 3350
Wire Wire Line
	5750 4150 6100 4150
Wire Wire Line
	7300 4300 7300 3950
$Comp
L Connector:AVR-ISP-6 J1
U 1 1 5BB954F1
P 6000 4800
F 0 "J1" V 5700 5100 50  0000 C CNN
F 1 "AVR-ISP-6" V 5700 4750 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x03_P2.54mm_Vertical_SMD" V 5750 4850 50  0001 C CNN
F 3 " ~" H 4725 4250 50  0001 C CNN
	1    6000 4800
	0    -1   -1   0   
$EndComp
Connection ~ 5800 3750
Wire Wire Line
	5800 3750 6100 3750
Wire Wire Line
	4250 3800 4250 3350
Connection ~ 4250 3350
Wire Wire Line
	4250 3350 4550 3350
Wire Wire Line
	4250 4100 4250 4550
Connection ~ 4250 4550
Wire Wire Line
	4250 4550 3850 4550
Wire Wire Line
	5500 4900 4550 4900
Wire Wire Line
	4550 4900 4550 3350
Connection ~ 4550 3350
Wire Wire Line
	4550 3350 5150 3350
Connection ~ 6000 3850
Wire Wire Line
	6000 3850 6100 3850
Wire Wire Line
	3850 4550 3850 5200
Wire Wire Line
	3850 5200 6400 5200
Wire Wire Line
	6400 5200 6400 4900
Connection ~ 3850 4550
$Comp
L Connector_Generic:Conn_02x02_Odd_Even J2
U 1 1 5BBAE7A8
P 3150 3450
F 0 "J2" H 3200 3667 50  0000 C CNN
F 1 "Power" H 3200 3576 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_2x02_P2.54mm_Vertical_SMD" H 3150 3450 50  0001 C CNN
F 3 "~" H 3150 3450 50  0001 C CNN
	1    3150 3450
	-1   0    0    1   
$EndComp
Wire Wire Line
	3850 3650 3850 3750
Wire Wire Line
	3350 3350 3550 3350
Wire Wire Line
	3350 3450 3450 3450
Wire Wire Line
	3450 3450 3450 3750
Wire Wire Line
	3450 3750 3850 3750
Connection ~ 3850 3750
Wire Wire Line
	3850 3750 3850 4550
Wire Wire Line
	6100 4150 6100 4300
Wire Wire Line
	6000 3850 6000 4400
Wire Wire Line
	5800 3750 5800 4400
Wire Wire Line
	6100 4300 7300 4300
Connection ~ 6100 4300
Wire Wire Line
	6100 4300 6100 4400
Wire Wire Line
	5900 3650 6100 3650
Wire Wire Line
	5750 3650 5900 3650
Connection ~ 5900 3650
Wire Wire Line
	5900 3650 5900 4400
Wire Wire Line
	6400 3750 6500 3750
Wire Wire Line
	6500 3750 6500 4050
Wire Wire Line
	6500 4050 6700 4050
Wire Wire Line
	6700 3850 6650 3850
Wire Wire Line
	6650 3850 6550 3650
Wire Wire Line
	6550 3650 6400 3650
Wire Wire Line
	6400 3850 6550 3850
Wire Wire Line
	6550 3850 6650 3650
Wire Wire Line
	6650 3650 6700 3650
$EndSCHEMATC
