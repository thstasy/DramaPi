#!/bin/bash

SERVO=/sys/class/pwm/pwmchip0/pwm2/duty_cycle

red_on(){
	sudo pinctrl set 17 op dh	
}

red_off(){
	sudo pinctrl set 17 dl
}

green_on(){
	sudo pinctrl set 27 op dh
}

green_off(){
	sudo pinctrl set 27 dl
}

servo_left(){
	echo 1000000 | sudo tee $SERVO > /dev/null
}

servo_center(){
	echo 1500000 | sudo tee $SERVO > /dev/null
}

servo_right(){
	echo 2000000 | sudo tee $SERVO > /devl/null
}

# 開場：假裝正常
red_off
green_on
servo_center
sleep 1

# 開始懷疑人生
green_off
red_on
servo_left
echo "DramaPi: Excuse me?"
sleep 1

# 假裝沒事
red_off
green_on
servo_center
echo "DramaPi: Everything is fine."
sleep 1

# 突然翻臉
green_off
red_on
servo_right
echo "DramaPi: Absolutely not."
sleep 1

# 最後冷靜下來
red_off
green_on
servo_center
echo "DramaPi: Fine. For now."
