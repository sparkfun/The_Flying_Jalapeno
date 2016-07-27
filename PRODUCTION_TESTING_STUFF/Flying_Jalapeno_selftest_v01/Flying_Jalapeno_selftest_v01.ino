/* Flying Jalapeno Self test
 *  Pete Lewis
 *  SparkFun Electronics
 *  7/26/2016
 * Board HW v02
 * Dummy HW v01 (HW cabinet # 526)
 * 
 * Plug in dummy
 * hit reset
 * Verify Stat LED blinks twice (indicating bootloader is good)
 * Hit Capsense pad 1, 
 *        This engages IO net1/net2 testing and LED lines testing.
 *        Success indicated by turning on V1 (and corresponding V1 LED)
 * Hit Capsense pad 2, 
 *        This engages Analog pin testing
 *        Success indicated by turning on V2 (and correstponding V2 LED)
 * Total success indicated by a solid STAT LED.
 */


#include <FlyingJalapeno.h>
FlyingJalapeno FJ(13);

#include <CapacitiveSensor.h>
CapacitiveSensor   cs_1 = CapacitiveSensor(47,45);
CapacitiveSensor   cs_2 = CapacitiveSensor(31,46);

int failures = 0;

int net1_control_pin = 21; // also known as SCL, and is routed to the PCA level converter on the FJ board.
int net1_pins[] = {3,5,7,9,11,13,15,17,19,23,25,50,52,A0,A6,A8,A10,A12}; // QTY 18
int net2_control_pin = 20; // also known as SDA, and is routed to the PCA level converter on the FJ board.
int net2_pins[] = {2,4,6,8,10,12,14,16,18,22,24,26,51,53,A1,A5,A7,A9,A11}; // QTY 19

boolean test_part1_result = false;

void setup()
{
  Serial.begin(9600);
  Serial.println("Flying Jalapeno selftest v01, for hardware v02");
}

void loop()
{
  long total1 =  cs_1.capacitiveSensor(30);
  long total2 =  cs_2.capacitiveSensor(30);
//  Serial.print(total1);
//  Serial.print("\t");
//  Serial.println(total2);

  if(total1 > 5000)
  {
    FJ.dot();
    failures = 0; // reset
    IO_net_test();
    if(failures == 0) LED_lines_test();
    if(failures == 0) test_part1_result = true;
    else test_part1_result = false;
  }
  else if(total2 > 5000)
  {
    FJ.dot();
    analog_pins_test();
    if((failures == 0) && (test_part1_result == true)) {pinMode(13, OUTPUT); digitalWrite(13,HIGH);}     
  } 
}

void IO_net_test()
{
  Serial.println("\n\r\n\r#### IO net testing");
    
  // set all pins as inputs
  for(int i = 0; i < 18; i++) pinMode(net1_pins[i], INPUT_PULLUP);
  for(int i = 0; i < 19; i++) pinMode(net2_pins[i], INPUT_PULLUP);

  pinMode(net1_control_pin, OUTPUT);
  digitalWrite(net1_control_pin, LOW);
  pinMode(net2_control_pin, INPUT_PULLUP);
  Serial.print("\n\rnet1:LOW, net2:INPUT_PULLUP");
  delay(100);
  verify_nets(0,1);

  pinMode(net2_control_pin, OUTPUT);
  digitalWrite(net2_control_pin, LOW);
  pinMode(net1_control_pin, INPUT_PULLUP);
  Serial.print("\n\rnet1:INPUT_PULLUP, net2:LOW");
  delay(100);
  verify_nets(1,0);

  // back to input pullup when done
  pinMode(net1_control_pin, INPUT); 
  pinMode(net2_control_pin, INPUT);
  for(int i = 0; i < 18; i++) pinMode(net1_pins[i], INPUT);
  for(int i = 0; i < 19; i++) pinMode(net2_pins[i], INPUT);
}

void verify_nets(int net1, int net2)
{
  boolean reading = false;
  // Read all inputs
  Serial.print("\n\rnet1_pins:");
  for(int i = 0; i < 18; i++)
  {
    reading = digitalRead(net1_pins[i]);
    Serial.print(reading);
    if(reading != net1) failures++;
  }  
  Serial.print("\tfailures:");
  Serial.print(failures);
  
  Serial.print("\n\rnet2_pins:");
  for(int i = 0; i < 19; i++)
  {
    reading = digitalRead(net2_pins[i]);
    Serial.print(reading);
    if(reading != net2) failures++;
  }      
  Serial.print("\tfailures:");
  Serial.println(failures);
}

void LED_lines_test()
{
  Serial.println("\n\r\n\r#### LED pins testing");
  set_led_lines(0,1,0,1);
  verify_led_lines(0,1,0,1);
  set_led_lines(1,0,1,0);
  verify_led_lines(1,0,1,0);
}

void set_led_lines(boolean one, boolean two, boolean three, boolean four)
{
  if(one)pinMode(40, INPUT_PULLUP);
  else{pinMode(40, OUTPUT); digitalWrite(40, LOW);}

  if(two)pinMode(41, INPUT_PULLUP);
  else{pinMode(41, OUTPUT); digitalWrite(41, LOW);}

  if(three)pinMode(42, INPUT_PULLUP);
  else{pinMode(42, OUTPUT); digitalWrite(42, LOW);}

  if(four)pinMode(43, INPUT_PULLUP);
  else{pinMode(43, OUTPUT); digitalWrite(43, LOW);}    
}

void verify_led_lines(boolean one, boolean two, boolean three, boolean four)
{
  int read_pins[] = {30,29,28,27};
  boolean vals[] = {0,0,0,0};
  for(int i = 0; i < 4; i++) pinMode(read_pins[i], INPUT_PULLUP);
  
  for(int i = 0; i < 4; i++) vals[i] = digitalRead(read_pins[i]);

  Serial.print("\n\rLED_pin_readings:");
  for(int i = 0; i < 4; i++) Serial.print(vals[i]);

  if(vals[0] != one) failures++;
  if(vals[1] != two) failures++;
  if(vals[2] != three) failures++;
  if(vals[3] != four) failures++;  

  Serial.print("\t\tfailures:");
  Serial.print(failures);
  
}

void analog_pins_test()
{
  Serial.println("\n\r\n\r#### Analog Pins testing");
  Serial.println("\n\rRAW:");
  if(FJ.verify_voltage(A4, 689, 0.05, true) == false) failures++;

  Serial.print("\t\t\t\tfailures:");
  Serial.println(failures);

  Serial.println("\n\rV1 (3.3V/5V):");
  FJ.setV1(true, 3.3);
  delay(100);
  if(FJ.verify_voltage(A3, 345, 0.05, true) == false) failures++;
  FJ.setV1(true, 5);
  delay(100);
  if(FJ.verify_voltage(A3, 521, 0.05, true) == false) failures++;
  FJ.setV1(false, 3.3);

  Serial.print("\t\t\t\tfailures:");
  Serial.println(failures);

  Serial.println("\n\rV2 (3.3V, 3.7V, 4.2V, 5V):");
  FJ.setV2(true, 3.3);
  delay(100);
  if(FJ.verify_voltage(A2, 345, 0.05, true) == false) failures++;
  FJ.setV2(true, 3.7);
  delay(100);
  if(FJ.verify_voltage(A2, 393, 0.05, true) == false) failures++;
  FJ.setV2(true, 4.2);
  delay(100);
  if(FJ.verify_voltage(A2, 448, 0.05, true) == false) failures++;
  FJ.setV2(true, 5);
  delay(100);
  if(FJ.verify_voltage(A2, 525, 0.05, true) == false) failures++;
  FJ.setV2(false, 5);

  Serial.print("\t\t\t\tfailures:");
  Serial.println(failures);
  
}



