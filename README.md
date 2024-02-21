https://www.youtube.com/watch?v=ViCnE7lS93w
https://www.youtube.com/watch?v=ee2lfg4I9_Y
For CS120B's final project, I made Tetris on the AVR ATMega1284 using eight 8-bit SN74HC595 shift registers, two 2088RGB common anode RGB LED 8x8 matrix, a 2-axis analog joystick, with Atmel Studio 7. The project is in C.



It's 3 complexities are:

1) 2-axis analog joystick input with click 

2) LEDs/Shift registers 

3) The Tetris game logic



I talk about using ADMUX to change the ADC channel, needing to control more pins than I have free and the solution (shift registers), and writing a game with no object-oriented abstractions to minimize memory footprint and processing overhead.
