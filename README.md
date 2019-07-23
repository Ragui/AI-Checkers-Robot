# AI-Checkers-Robot
This is the source code of an AI Checkers Robotic Arm. 

The project consists of constructing a robotic arm that plays checkers against a person. A computer vision algorithm determines the position of the pieces based on input from a camera. These positions are used by the AI player to decide the best move. The chosen move is then executed by the arm through a set of orders that are provided to the servo motors connected to the arm. The code that controls the servos and runs the AI player calculations and the machine vision algorithm is running on a raspberry pi.
