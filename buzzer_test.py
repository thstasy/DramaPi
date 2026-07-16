# import lgpio
# import time

# GPIO = 24

# h = lgpio.gpiochip_open(0)

# # PWM 測試
# lgpio.tx_pwm(h, GPIO, 440, 50)
# time.sleep(2)

# lgpio.tx_pwm(h, GPIO, 880, 50)
# time.sleep(2)

# lgpio.tx_pwm(h, GPIO, 0, 0)

# lgpio.tx_pwm(h, GPIO, 0, 0)

# lgpio.gpiochip_close(h)


import lgpio
import time

GPIO = 19

h = lgpio.gpiochip_open(0)

def note(freq, duration):
    if freq == 0:
        lgpio.tx_pwm(h, GPIO, 440, 0)
    else:
        lgpio.tx_pwm(h, GPIO, freq, 50)
    time.sleep(duration)

# 法式憂鬱風（原創）
melody = [
    (440, 0.4),  # A4
    (392, 0.4),  # G4
    (349, 0.6),  # F4
    (392, 0.4),
    (330, 0.8),  # E4
    (349, 0.4),
    (294, 0.8),  # D4
]

for f, t in melody:
    note(f, t)

lgpio.gpio_write(h, GPIO, 0)
lgpio.gpiochip_close(h)