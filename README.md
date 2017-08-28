# Solar Timer

The goal of this device is to measure the time it takes for Earth to rotate so that the light
received at elevation `X` reaches the same position, but at elevation `X - L`, `L being the length
of a wall or something. The experience is similar to the one proposed
[in this page][radius-of-earth].

How it works is that we place the device at the top of a high structure and wait for the first rays
of light of the day. We then activate the clock through the push button and lower the device
quickly, facing the sun (with a rope or something).

The device, when the push button was pressed, activated a clock and memorized the light level at
that precise time. It then waits for a light level drop so that we have the chance to start the
lowering operation and then waits for the light level to get back to the level it was. When it does,
it stops the timer and displays the result on the segmented LED matrices.

## Status

I only have the first draft of a schematic for now. I haven't even tried it in a breadboard yet,
still waiting for parts. Source code and assembly instructions will come after the first prototype
has been built.

[radius-of-earth]: http://www.solipsys.co.uk/new/TheRadiusOfTheEarth.html
