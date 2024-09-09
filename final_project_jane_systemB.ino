/* SYDE 263 Final Project Skeleton
    Copyright (C) 2023  Calvin Young

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3 (GNU-GPLv3)
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>. 
*/

// T1 = 99, 54 --> Intermediate: 50, 43
// T2 = 120, 74--> Intermidiate: 50, 43
// T3 = 157, 108--> Intermidiate: 50, 43


#include <Servo.h>

Servo gripper; 
Servo ax1;
Servo ax2;

//Variables to store the servo angle associated with the gripper open and closed positions
int gr_open = 1;
int gr_close = 90;

//Variables to store the servo angles associated with the home location
float ax1_home = 40;
float ax2_home = 35;

//Variables to store the servo angles associated with the current location
float ax1_current;
float ax2_current;

// Variables to store current and desired location (1 2 or 3)
int current_position;
int target_position;

// Storing all approach vals
float ax1_approach_t1 = 45;
float ax2_approach_t1 = 95;

float ax1_approach_t2 = 30;
float ax2_approach_t2 = 82;

float ax1_approach_t3 = 15;
float ax2_approach_t3 = 62; //couldve been 62...if things so wrong change to 62

// JANE YOU NEED TO ADD UR APPROACH VALUES AND CHANGE THEM IN THE CODE

// Store target vals
float ax1_t1 = 87;
float ax2_t1 = 105;

float ax1_t2 = 72;
float ax2_t2 = 83;

float ax1_t3 = 52;
float ax2_t3 = 67;

void moveTopos(float ax1s, float ax1e, float ax2s, float ax2e) {
  /*
   This function taskes the starting and ending angles for both axes and
   divides them into an even number of steps. This ensures that the movement
   from both motors occurs simultaneously. It also reduces the speed of
   movement to help protect the mechanism. This is a workable but sub-optimal
   way of moving the mechanism.

   ax1s -> Axis 1 Start Position
   ax1e -> Axis 1 End Position
   ax2s -> Axis 2 Start Position
   ax2e -> Axis 2 End Position
   */
  float num_steps_1 = ax1e - ax1s;
  float num_steps_2 = ax2e - ax2s;
  float num_steps = 0;

  if (abs(num_steps_1) >= abs(num_steps_2)){
    num_steps = abs(num_steps_1);
  }
  else {
     num_steps = abs(num_steps_2);
  }
  float inc1 = num_steps_1 / num_steps;
  float inc2 = num_steps_2 / num_steps;
  float stp = 0;
  for(stp = 0; stp<=num_steps; stp += 1){
    ax1_current += inc1;
    ax1.write(ax1_current);
    ax2_current += inc2;
    ax2.write(ax2_current);
    delay(25);
  }
  ax1_current = ax1e;
  ax1.write(ax1_current);
  ax2_current = ax2e;
  ax2.write(ax2_current);
  delay(25);
}


void setup() {
  pinMode(5, INPUT); // get/receive info
  pinMode(6, INPUT);
  pinMode(7, OUTPUT); // give/send info
  pinMode(8, OUTPUT);

  //What pins are the servos attached to?
  gripper.attach(12); 
  ax1.attach(11);
  ax2.attach(10);

  //Setting the axes to the home position
  ax1_current = ax1_home;
  ax2_current = ax2_home;

  ax1.write(ax1_current);
  ax2.write(ax2_current);
  gripper.write(gr_open);
  delay(500);

  // Set t1 to starting point on both systems, so both know to start at t1
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);

  Serial.begin(9600);
}

void loop() {
  Serial.println("Enter your target position (1, 2, or 3):");

  // Wait for user to enter target
  while (Serial.available() == 0){
    // code will wait here until integer entered
  }

  int pin5 = digitalRead(5); //problem! This overrides the current pos = target pos at the end
  int pin6 = digitalRead(6);

  // Payload is at T1
  if (pin5 == HIGH && pin6 == HIGH) {
    current_position = 1;
    Serial.println("Payload is at T1");
  }

  // Payload is at T2
  if (pin5 == HIGH && pin6 == LOW) {
    current_position = 2;
    Serial.println("Payload is at T2");
  }

  // Payload is at T3
  if (pin5 == LOW && pin6 == HIGH) {
    current_position = 3;
    Serial.println("Payload is at T3");
  }

  int target_position = Serial.parseInt(); //storing what the serial monitor enters
  Serial.println("Selected target: ");
  Serial.println(target_position);

  if (target_position == 1 || target_position == 2 || target_position == 3) {
    
    // target position 1
    if(current_position == 1) {
      // move to approach
      moveTopos(ax1_home, ax1_approach_t1, ax2_home, ax2_approach_t1);
      delay(500);
      // move to pickup location and grab payload
      moveTopos(ax1_approach_t1, ax1_t1, ax2_approach_t1, ax2_t1);
      delay(500);
      gripper.write(gr_close);
      delay(500);

      if(target_position ==1) {
        Serial.println("Payload is already at T1");
        gripper.write(gr_open);
        delay(500);
        // move to approach
        moveTopos(ax1_t1, ax1_approach_t1, ax2_t1, ax2_approach_t1);
        // move to home from T1
        moveTopos(ax1_approach_t1, ax1_home, ax2_approach_t1, ax2_home);
      }
      if(target_position ==2) {
        // move to drop off location
        moveTopos(ax1_t1, ax1_t2, ax2_t1, ax2_t2);
        gripper.write(gr_open);
        delay(500);
        // move to approach
        moveTopos(ax1_t2, ax1_approach_t2, ax2_t2, ax2_approach_t2);
        delay(200);
        // move to home
        moveTopos(ax1_approach_t2, ax1_home, ax2_approach_t2, ax2_home);
      }
      if(target_position ==3) {
        // move to drop off location
        moveTopos(ax1_t1, ax1_t3, ax2_t1, ax2_t3);
        gripper.write(gr_open);
        delay(500);
        // move to approach
        moveTopos(ax1_t3, ax1_approach_t3, ax2_t3, ax2_approach_t3);
        delay(200);
        // move to home
        moveTopos(ax1_approach_t3, ax1_home, ax2_approach_t3, ax2_home);
      }
    }

    // target position 2
    if(current_position == 2) {
      // move to approach
      moveTopos(ax1_home, ax1_approach_t2, ax2_home, ax2_approach_t2);
      delay(500);
      // move to pickup location and grab payload
      moveTopos(ax1_approach_t2, ax1_t2, ax2_approach_t2, ax2_t2);
      delay(500);
      gripper.write(gr_close);
      delay(500);

      if(target_position ==2) {
        Serial.println("Payload is already at T2");
        gripper.write(gr_open);
        delay(500);
        // move to approach
        moveTopos(ax1_t2, ax1_approach_t2, ax2_t2, ax2_approach_t2);
        // move to home from T2
        moveTopos(ax1_approach_t2, ax1_home, ax2_approach_t2, ax2_home);
      }
      if(target_position ==1) {
        // move to drop off location
        moveTopos(ax1_t2, ax1_t1, ax2_t2, ax2_t1);
        gripper.write(gr_open);
        delay(500);
        // move to approach
        moveTopos(ax1_t1, ax1_approach_t1, ax2_t1, ax2_approach_t1);
        delay(200);
        // move to home
        moveTopos(ax1_approach_t1, ax1_home, ax2_approach_t1, ax2_home);
      }
      if(target_position ==3) {
        // move to drop off location
        moveTopos(ax1_t2, ax1_t3, ax2_t2, ax2_t3);
        gripper.write(gr_open);
        delay(500);
        // move to approach
        moveTopos(ax1_t3, ax1_approach_t3, ax2_t3, ax2_approach_t3);
        delay(200);
        // move to home
        moveTopos(ax1_approach_t3, ax1_home, ax2_approach_t3, ax2_home);
      }
    }

    // target position 3
    if(current_position == 3) {
      // move to approach
      moveTopos(ax1_home, ax1_approach_t3, ax2_home, ax2_approach_t3);
      delay(500);
      // move to pickup location and grab payload
      moveTopos(ax1_approach_t3, ax1_t3, ax2_approach_t3, ax2_t3);
      delay(500);
      gripper.write(gr_close);
      delay(500);

      if(target_position ==3) {
        Serial.println("Payload is already at T3");
        gripper.write(gr_open);
        delay(500);
        // move to approach
        moveTopos(ax1_t3, ax1_approach_t3, ax2_t3, ax2_approach_t3);
        // move to home from T3
        moveTopos(ax1_approach_t3, ax1_home, ax2_approach_t3, ax2_home);
      }
      if(target_position ==2) {
        // move to drop off location
        moveTopos(ax1_t3, ax1_t2, ax2_t3, ax2_t2);
        gripper.write(gr_open);
        delay(500);
        // move to approach
        moveTopos(ax1_t2, ax1_approach_t2, ax2_t2, ax2_approach_t2);
        delay(200);
        // move to home
        moveTopos(ax1_approach_t2, ax1_home, ax2_approach_t2, ax2_home);
      }
      if(target_position ==1) {
        // move to drop off location
        moveTopos(ax1_t3, ax1_t1, ax2_t3, ax2_t1);
        gripper.write(gr_open);
        delay(500);
        // move to approach
        moveTopos(ax1_t1, ax1_approach_t1, ax2_t1, ax2_approach_t1);
        delay(200);
        // move to home
        moveTopos(ax1_approach_t1, ax1_home, ax2_approach_t1, ax2_home);
      }
    }

 current_position = target_position;
  
    if (target_position == 1) {
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      Serial.println("Payload set to position 1");
    }

    else if (target_position == 2) {
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      Serial.println("Payload set to position 2");
    }

    else if (target_position == 3) {
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      Serial.println("Payload set to position 3");
    }
  }

}