In this example, i used STM32 NUCLEO-F030R8 MB1136 C-04 development board for reading rising edges of a PWM signal and record input capture mode. I used PA8 pin at Timer1 for reading PWM rising edge times and PA6 at Timer3 for generating PWM signal at 1 kHZ and 25% duty cycle. I connected the PA8 and PA6 pins to each other. I connected the PA8 and PA6 pins to each other. I calculated the frequency by dividing the clock frequency (remaining after the prescaler) by the time elapsed between two rising edges of the PWM signal.

<img width="1473" height="449" alt="input_capture" src="https://github.com/user-attachments/assets/ee4f23c9-4411-4ec8-b4ff-7f5548fc70e6" />

Citation: https://xbowtie.com/egitmen/arifmandal

https://github.com/user-attachments/assets/33e7a02f-9a92-4b8d-8750-d15c7bb6b83d

