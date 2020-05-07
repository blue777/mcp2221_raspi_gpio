#ifndef __CTRL_I2C_H_INCLUDED__
#define __CTRL_I2C_H_INCLUDED__

class ctrl_i2c_if
{
public:
	virtual	bool    write(const unsigned char * data, int size) = 0;
	virtual	bool    read(unsigned char * data, int size) = 0;
};

#endif
