from gpiozero import Button
from time import sleep

button = Button(25)

print("Waiting for button...")

while True:
    if button.is_pressed:
        print("Pressed!")
    else:
        print("Released")

    sleep(0.2)
