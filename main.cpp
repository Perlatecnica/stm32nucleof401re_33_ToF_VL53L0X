/* Copyright (c) 2022 Perlatecnica APS ETS
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/****************************************************
*            RAPID PROTOTYPING WITH NUCLEO          *
* Example Code 33: ToF VL53L0X                      *
* Author: Mauro D'Angelo                            *
* Organization: Perlatecnica APS ETS				*
*****************************************************/

#include "mbed.h"
#include "VL53L0X.h"
#include "DevI2C.h"

#define NEW_SENSOR_CENTRE_ADDRESS		0x54
#define OUT_OF_RANGE 0xFFFF

DigitalOut led(LED1);
UnbufferedSerial pc(USBTX,USBRX);


/* It creates i2C bus management device*/
DevI2C* device_i2c = new DevI2C(I2C_SDA, I2C_SCL);

/* It creates VL53L0X sensor instance*/
VL53L0X *sensor_centre;

/* VL53L0X ShutDown Output pin*/
DigitalOut*  xshutdown_centre = new DigitalOut(D7);

/* VL53L0X configuration parameters*/
OperatingMode operating_mode;
VL53L0X_RangingMeasurementData_t data_sensor_centre;

int status = 0;
uint16_t measure = OUT_OF_RANGE;

int main() {

    /* Init sensor */
	sensor_centre = new VL53L0X(*device_i2c, *xshutdown_centre, NC);
	sensor_centre->VL53L0X_off();

    status = sensor_centre->init_sensor(NEW_SENSOR_CENTRE_ADDRESS);

    if (status) { // Sensor has not been found
        delete sensor_centre;
        delete xshutdown_centre;
        sensor_centre = NULL;
        xshutdown_centre = NULL;
        printf("Sensor is not present\n\r");
    } 
    else { // Sensor has been found
        printf("Sensor is present\n\r");
        /* Set configuration */
        operating_mode = range_continuous_polling; // = range_single_shot_polling;
        /* Start to measure*/
        sensor_centre->start_measurement(operating_mode, NULL);
    }
    /* end Init sensor */

	while(true)
    {
		// Get measurement and print it
        sensor_centre->get_measurement(operating_mode, &data_sensor_centre);
		measure = data_sensor_centre.RangeMilliMeter;
		printf("%d\r\n",measure);
		
        ThisThread::sleep_for(1s); // Takes one measure per second
		led = !led; // Blink led
    }
}
