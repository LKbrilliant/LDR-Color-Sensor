# LDR-RGB-Color-Sensor
Cheap but reliable color sensor using a microcontroller (Arduino), an RGB LED (WS2812B), a LDR and a 10k resistor. The sensor can pick up colors from a surface and detected color is shown on the PC by using 'processing' application. (Not suitable for glossy surfaces)

# Building the sensor:
When constructing the sensor, consider these things.
The LDR should only be exposed to the reflected light from the surface which was emitted by the RGB LED. 
Light from the RGB LED should not directly goes to the LDR and light from the surrounding needs to be blocked.

# calibration:
The sensor needs to be properly calibrated before using. To calibrate the sensor, we need to have 'BLACK' color and 'WHITE' colors of the material which we are hoping to detect colors from. For example, if the sensor is going to be used to detect colors from a paper, when calibrating the sensor, it needs to be calibrated using black and white color papers. If the sensor was calibrated to one material and tries to detect color from other material, often results may not be accurate.

# Basic Idea:
LDR can be used to measure the intensity of light. The resistance between the two pins of an LDR will change depending on the intensity of the light. In this case the an RGB LED is used for illuminating the target surface with red, green and blue colors while reflected light intensity was measured by an LDR.

The targeted surface is illuminated only by the RGB LED. Depending on the surface texture and the color the reflected intensity was reduced. Since a white surface will reflect almost every wavelength, intensity readings from a white surface is the highest reading among any other color on that particular surface. Likewise, reflected light intensity is lowest on a black colored surface.

During the calibration process the microcontroller will store the light intensity readings from the LDR for red, green and blue colors which is reflected from white and black color. These values are the highest and lowest values for that surface. Any readings which will be taken after the calibration should be in between these values.

When an RGB color reading was obtained from a surface the program will compare that values with the white and black values which was stored during the calibration and decides the color.
