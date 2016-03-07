#!/bin/bash
figlet -t FAJA
echo -e "Arya JEMO & Alexandre FOURGS - 2016 \n"


echo -e " ------------------------------------- \n \n \n"
echo -e " 1- Launch servo kernel \n	Enter your password : \n"

sudo kernel/servod
sudo echo P1-11=50 > /dev/servoblaster
sudo echo P1-12=50 > /dev/servoblaster

echo -e " Launch of servo controller \n"
./joystick
