# Electronic_Meds_Box
A medicine storage box that has alarms that can remind the elderly to take their daily pills. Built with an arduino Uno

## Introduction:
Forgetfulness is a normal part of aging. As people get older, some people may notice that it takes longer to learn new things and becomes harder to remember everything. The elderly often take multiple medicines a day. Some of these medicines are prescribed over a long term whereas some medicines may be replaced with variants. And many medicines are critical and are required to be taken without fail every day at the right time. For a senior citizen, remembering to take so many medicines daily is no mean feat. The trend of living in nuclear families has been growing for a while as children go live in different states or countries for higher studies or to avail better job opportunities, so the elderly may not have a support system to remind them to take their medicines. As a solution to this problem, I propose the design of the electronic medicine storage and reminder box.

## Objective: 
To build a medicine storage and reminder box with the following features:
1.	It is easy to use with clean, minimalist design to ensure that there is no confusion for the elderly and they are not intimidated by the technology.
2.	Lightweight, hardy and easy to carry around
3.	It has a loud audio as well as visual reminder at the appropriate times to remind the senior citizen to take medicine. 
4.	Which can directly be used by doctors to change medicine timings
5.	Whose content can be easily accessed by medical clinics so that they can provide the right medicine

## Components: 
1.	plastic box (of suitable size) with multiple compartments (for storing the medicine).
2.	Arduino board (for the prototype)
3.	Each compartment will carry a red led
4.	A large push button on the outer surface of the box
5.	A (piezo electric) buzzer
6.	An ESP8266WiFi Module to connect to a network so as t be able to change schedule on the hardware device
7.	An RTC clock
8.	A batter power source


## Working:
The Arduino is on sleep/ power saving mode by default. At the time of an alarm the RTC wakes the Arduino up and plays a loud buzzer tune. Along with this the red LEDs in the compartments of the plastic box where the medicines that need to be consumed are stored light up. Indicating clearly which medicines need to be taken. Once the elderly hears the buzzer, on pressing the large pushbutton once, the buzzer stops playing. But the lights indicating which medicine to take remains switched on. Now the elderly can take their medicine. On pressing the large pushbutton once more, the lights turn off.  The Arduino then goes to sleep until the next alarm.

