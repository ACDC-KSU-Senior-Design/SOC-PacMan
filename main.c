#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include <linux/i2c-dev.h>
#include "hwlib.h"
#include "ADXL345.h"
#include "VGA.h"

#define HW_REGS_BASE ( 0xFC000000 )
#define HW_OCRAM_BASE ( 0xC8000000 )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )
#define FPGA_CHAR_BASE      0xC9000000

#define PHYSMEM_32(addr) (*((unsigned int *)(virtual_base + (addr & HW_REGS_MASK))))
#define PHYSMEM_16(addr) (*((unsigned short *)(virtual_base + (addr & HW_REGS_MASK))))

bool ADXL345_REG_WRITE(int file, uint8_t address, uint8_t value);
bool ADXL345_REG_READ(int file, uint8_t address,uint8_t *value);
bool ADXL345_REG_MULTI_READ(int file, uint8_t readaddr,uint8_t readdata[], uint8_t len);

typedef struct {
	int X;
	int Y;
	int R;
}Circle;

typedef struct{
	int X1;
	int Y1;
	int X2;
	int Y2;
}Line;

// Test program for use with the DE1-SoC University Computer
// 

int main(int argc,char ** argv) {
	
    void *virtual_base;
    int fd;
  	
   if( ( fd = open( "/dev/mem", ( O_RDWR | O_SYNC ) ) ) == -1 ) {
	    printf( "ERROR: could not open \"/dev/mem\"...\n" );
		return( 1 );
	}
    
	virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_REGS_BASE );
	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}
	
    // Set framebuffer addr to beginning of the SRAM
    PHYSMEM_32(0xff203024) = 0xc8000000;  	// Pixel BackBuffer register
    PHYSMEM_32(0xff203020) = 0xc8000000;	// Pixel Buffer register
    
    // Unmap registers region, map onchip ram region
    if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}
    virtual_base = mmap( NULL, HW_REGS_SPAN, ( PROT_READ | PROT_WRITE ), MAP_SHARED, fd, HW_OCRAM_BASE );
	if( virtual_base == MAP_FAILED ) {
		printf( "ERROR: mmap() failed...\n" );
		close( fd );
		return( 1 );
	}
    
	int file;
	const char *filename = "/dev/i2c-0";
	const int mg_per_digi = 4;
	uint8_t id;t
	uint16_t szXYZ[3];
	int cnt=0, max_cnt=0;
		
	if ((file = open(filename, O_RDWR)) < 0) {	// open bus
  	  /* ERROR HANDLING: you can check errno to see what went wrong */
	    perror("Failed to open the i2c bus of gsensor");
  	  exit(1);
	}
	
	// init	 
	// gsensor i2c address: 101_0011
	int addr = 0b01010011; 
	if (ioctl(file, I2C_SLAVE, addr) < 0) {
  	  printf("Failed to acquire bus access and/or talk to slave.\n");
	    /* ERROR HANDLING; you can check errno to see what went wrong */
  	  exit(1);
	}	
	
	// configure accelerometer as +-2g and start measure
    bool bSuccess = ADXL345_Init(file);
    if (bSuccess){
        // dump chip id
        bSuccess = ADXL345_IdRead(file, &id);
        if (bSuccess)
            printf("id=%02Xh\r\n", id);
    }   
	
	VGA_clear(virtual_base);

	Circle currCircle = {240,240,25};
	Circle prevCircle = currCircle;
	
	Line currLine = {currCircle.X, currCircle.Y, 240,240};
	Line prevLine = currLine;
	
	while(bSuccess && (max_cnt == 0 || cnt < max_cnt)){
        if (ADXL345_IsDataReady(file)){
            bSuccess = ADXL345_XYZ_Read(file, szXYZ);
            if (bSuccess){
	              cnt++;
				  
				int16_t xg = szXYZ[0] * mg_per_digi;
				int16_t yg = szXYZ[1] * mg_per_digi;
				int16_t zg = szXYZ[2] * mg_per_digi;
                //printf("[%d]X=%d mg, Y=%d mg, Z=%d mg\r\n", cnt, xg, yg, zg);
                // show raw data, 

				//Calculate the next position depending on the sensors
				currCircle.X += xg / 100;	//X axis is good so just add the value
				currCircle.Y -= yg / 100;	//Y axis is inverted so invert the answer

				currLine.X1 = currCircle.X;
				currLine.Y1 = currCircle.Y;
				//End calculate position from sensors
				
				//Prevent the circle from going out of bounds
				if(currCircle.X < currCircle.R)
					currCircle.X = currCircle.R;
				else if(currCircle.X > SCREEN_WIDTH - currCircle.R)
					currCircle.X = SCREEN_WIDTH - currCircle.R;
				
				if(currCircle.Y < currCircle.R)
					currCircle.Y = currCircle.R;
				else if(currCircle.Y > SCREEN_HEIGHT - currCircle.R)
					currCircle.Y = SCREEN_HEIGHT - currCircle.R;
				//End prevent circle from out of bounds

				if(currCircle.X != prevCircle.X || currCircle.Y != prevCircle.Y){
					VGA_circle(prevCircle.X, prevCircle.Y, prevCircle.R, BLACK, virtual_base);
					VGA_line(prevLine.X1, prevLine.Y1, prevLine.X2, prevLine.Y2, BLACK, virtual_base);
					
					VGA_circle(currCircle.X,  currCircle.Y,  currCircle.R,  LIGHT_BLUE, virtual_base);
					VGA_line(currLine.X1, currLine.Y1, currLine.X2, currLine.Y2, LIGHT_BLUE, virtual_base);
					
					
					prevCircle = currCircle;
					prevLine = currLine;
				}
				
                usleep(1000*33);// 1/30 * 1000 = 30FPS
            }
        }
    }
	
	
	
	if (!bSuccess)
        printf("Failed to access accelerometer\r\n");
	if (file)
		close(file);
	printf("gsensor, bye!\r\n");
	
	if( munmap( virtual_base, HW_REGS_SPAN ) != 0 ) {
		printf( "ERROR: munmap() failed...\n" );
		close( fd );
		return( 1 );
	}

	close( fd );

	return( 0 );

}

bool ADXL345_REG_WRITE(int file, uint8_t address, uint8_t value){
	bool bSuccess = false;
	uint8_t szValue[2];
	
	// write to define register
	szValue[0] = address;
	szValue[1] = value;
	if (write(file, &szValue, sizeof(szValue)) == sizeof(szValue)){
			bSuccess = true;
	}
		
	
	return bSuccess;		
}

bool ADXL345_REG_READ(int file, uint8_t address,uint8_t *value){
	bool bSuccess = false;
	uint8_t Value;
	
	// write to define register
	if (write(file, &address, sizeof(address)) == sizeof(address)){
	
		// read back value
		if (read(file, &Value, sizeof(Value)) == sizeof(Value)){
			*value = Value;
			bSuccess = true;
		}
	}
		
	
	return bSuccess;	
}

bool ADXL345_REG_MULTI_READ(int file, uint8_t readaddr,uint8_t readdata[], uint8_t len){
	bool bSuccess = false;

	// write to define register
	if (write(file, &readaddr, sizeof(readaddr)) == sizeof(readaddr)){
		// read back value
		if (read(file, readdata, len) == len){
			bSuccess = true;
		}
	}
	
		
	return bSuccess;
}