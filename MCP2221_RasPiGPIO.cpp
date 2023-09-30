#include "stdafx.h"
#include "MCP2221_RasPiGPIO.h"

#include "common/display_ssd1306_i2c.h"



#if _WIN64
#pragma comment(lib,"mcp2221_dll_um_x64.lib")
#else
#pragma comment(lib,"mcp2221_dll_um_x86.lib")
#endif


MCP2221_RasPiGPIO::MCP2221_RasPiGPIO(mcp2221& handle) :
	m_handle(handle)
{
}

MCP2221_RasPiGPIO::~MCP2221_RasPiGPIO()
{
};

void	MCP2221_RasPiGPIO::InitializeGPIO_Flash()
{
	int				ret;

	// p.59
	unsigned char	pinFunctions[4] = { 0 };	// all gpio
	unsigned char	pinDirections[4] = { 0 };	// all Output
	unsigned char	outputValues[4] = { 0 };	// Keep current output

	ret = Mcp2221_SetGpioSettings(
		m_handle,
		0,	// Write to flash
		pinFunctions,
		pinDirections,
		outputValues);
}

void	MCP2221_RasPiGPIO::InitializeGPIO()
{
	int				ret;

	// p.59
	unsigned char	pinFunctions[4] = { 0 };	// all gpio
	unsigned char	pinDirections[4] = { 0 };	// all Output
	unsigned char	outputValues[4] = { 0xff, 0xff, 0xff, 0xff };	// Keep current output

	ret = Mcp2221_SetGpioSettings(
		m_handle,
		1,	// Read to SRAM
		pinFunctions,
		pinDirections,
		outputValues);
}

int	MCP2221_RasPiGPIO::GetOutputValue(int port)
{
	char	gpioValues[4] = { -1, -1, -1, -1 };
	int		ret;

	ret = Mcp2221_GetGpioValues(m_handle, (unsigned char*)gpioValues);

	return	gpioValues[port];
}

int	MCP2221_RasPiGPIO::SetOutputValue(int port, int value)
{
	unsigned char	gpioValues[4] = { 0xff, 0xff, 0xff, 0xff };
	int		ret;

	gpioValues[port] = value;

	ret = Mcp2221_SetGpioValues(m_handle, gpioValues);

	return	0;
}




MCP2221_RasPiGPIO::SSD1306::SSD1306(mcp2221& handle) :
	m_i2c(handle, 0x3c )
{
	// Set i2c speed
	Mcp2221_SetSpeed(handle, 400000);
}

void MCP2221_RasPiGPIO::SSD1306::DispOn()
{
	Display_SSD1306_i2c	disp(m_i2c, 0);

	disp.Init();
	disp.DispOn();
}

void	MCP2221_RasPiGPIO::SSD1306::DispOff()
{
	Display_SSD1306_i2c	disp(m_i2c, 0);

	disp.Init();
	disp.DispOff();
}


void	MCP2221_RasPiGPIO::SSD1306::DrawImage(CBitmap32& image)
{
	Display_SSD1306_i2c	disp(m_i2c, 0);

	disp.Init();

	int		stride = 0;
	BYTE*	pbScan0 = image.GetImagePtrScan0(stride);
	disp.WriteImageBGRA(0, 0, pbScan0, stride, image.GetSize().cx, image.GetSize().cy);
}


MCP2221_RasPiGPIO::INA226::INA226(mcp2221& handle, double shunt_reg) :
	m_i2c(handle, 0x40 ),
	m_dShuntReg(shunt_reg)
{
	Mcp2221_SetSpeed(handle, 400000);
}

void	MCP2221_RasPiGPIO::INA226::Init()
{
	uint8_t	avg_reg = 0x4;	// 128
	uint8_t	ct_reg = 0x4;	// 1 msec

	uint8_t  w_data0[3] = {
		0x00,
		(uint8_t)(0x40 | (0xe0 & (avg_reg << 1)) | (0x01 & (ct_reg >> 2))),
		(uint8_t)(0x07 | (0xc0 & (ct_reg << 6)) | (0x38 & (ct_reg << 3))) };

	m_i2c.write(w_data0, sizeof(w_data0));


	uint16_t	alert_func = 0x8000;	// ALERT_SHUNT_OVER_VOLT
	uint16_t	alert_value = (uint16_t)(1.0 / GetA_1LSB());

	uint8_t  w_data6[3] = { 0x06, (uint8_t)(0xFF & (alert_func >> 8)), (uint8_t)(0xFF & alert_func) };
	uint8_t  w_data7[3] = { 0x07, (uint8_t)(0xFF & (alert_value >> 8)), (uint8_t)(0xFF & alert_value) };

	m_i2c.write(w_data6, sizeof(w_data6));
	m_i2c.write(w_data7, sizeof(w_data7));
}

double	MCP2221_RasPiGPIO::INA226::GetV()
{
	uint8_t  w_data[1] = { 0x01 };
	uint8_t  r_data[2] = { 0x00, 0x00 };

	// read vbus
	w_data[0] = 0x02;
	m_i2c.write(w_data, sizeof(w_data));
	m_i2c.read(r_data, sizeof(r_data));

	uint16_t	raw_value = (r_data[0] << 8) | r_data[1];
	return	0.00125 * raw_value;
}

double	MCP2221_RasPiGPIO::INA226::GetA()
{
	uint8_t	w_data[1] = { 0x01 };
	uint8_t	r_data[2] = { 0x00, 0x00 };

	// read shunt
	w_data[0] = 0x01;
	m_i2c.write(w_data, sizeof(w_data));
	m_i2c.read(r_data, sizeof(r_data));

	int16_t	raw_value = (r_data[0] << 8) | r_data[1];
	return	GetA_1LSB() * raw_value;
}

double	MCP2221_RasPiGPIO::INA226::GetA_1LSB()
{
	// 1 LSB : 2.5 uV
	return	0.0000025 / m_dShuntReg;
}