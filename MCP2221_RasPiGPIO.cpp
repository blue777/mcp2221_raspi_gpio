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
