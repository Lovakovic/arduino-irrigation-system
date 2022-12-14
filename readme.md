# Beta branch
## Code in this branch hasn't been tested thoroughly, use at your own risk!
## Check out the stable version on stable branch.

<h4>I'm currently planning on improving the current setup as I've noticed a 
few flaws, the improvements I'm planning on doing are as listed below</h4>

<ul>
<em>Improvements due:</em>
  <li>Switch to a 5V power supply for efficiency and longevity of the board</li>
  <li>Find a better quality and more reliable soil moisture sensor</li>
</ul>

This is an Arduino irrigation project used to irrigate a single large 
potted plant.

It is loosely based on this project: 
https://www.youtube.com/watch?v=V7Kq7MVwJUM&t

Parts needed for this project include:
- **Aruino Uno**
- **12V power supply or a 12V battery** - power supply preferred if 
  left to work for extended periods of time
- **Moisture sensor** - highly advised to use capacitive moisture sensor 
  as ones based on measuring electric conductivity deteriorate rapidly
- **IRFZ44N transistor** 
- **Single pin male to female wire**
- **Single pin male to male wire**
- **Water hose that fits the water pump you chose**

Nice to haves:
- **22 awg wire** - if you'd like to extend the wire connecting the moisture
  sensor to Arduino
- **Barrel DC connector, 1 male, 1 female** - if you'd like to be able to 
  interface your irrigation system with battery/power supply instead of making 
  them insepareble 
