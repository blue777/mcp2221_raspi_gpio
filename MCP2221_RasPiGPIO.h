#pragma once

#include "common/ctrl_i2c.h"

//#define MCP2221_LIB
#include <mcp2221_dll_um.h>
#define	MCP2221_USB_VID		0x04D8
#define	MCP2221_USB_PID		0x00DD


class CBitmap32
{
public:
	CBitmap32()
	{
		m_hDC = NULL;
		m_pRgbImg = NULL;
		m_hOldmap = NULL;;
		m_hOldFont = NULL;
		m_size.cx = 0;
		m_size.cy = 0;
	};

	~CBitmap32()
	{
		if (m_hOldFont != NULL)
		{
			HFONT	hFont = (HFONT)::SelectObject(m_hDC, m_hOldFont);
			::DeleteObject(hFont);
		}

		if (m_hOldmap != NULL)
		{
			HBITMAP	hBitmap = (HBITMAP)::SelectObject(m_hDC, m_hOldmap);
			::DeleteObject(hBitmap);
		}

		if (m_hDC != NULL)
		{
			::DeleteDC(m_hDC);
		}

		m_hOldmap = NULL;
		m_pRgbImg = NULL;
		m_hOldFont = NULL;
		m_hDC = NULL;
		m_size.cx = 0;
		m_size.cy = 0;
	}

	BOOL	CreateBitmap(int cx, int cy)
	{
		LPDWORD	pRgbImg = NULL;

		if ((cx <= 0) || (cy == 0))
		{
			cx = 1;
			cy = 1;
		}

		if (m_hDC == NULL)
		{
			m_hDC = ::CreateCompatibleDC(NULL);
			if (m_hDC == NULL)
			{
				return	FALSE;
			}
		}

		if (m_hOldmap != NULL)
		{
			if ((m_size.cx == cx) && (m_size.cy == cy) && (m_pRgbImg != NULL))
			{
				return	TRUE;
			}
		}

		BITMAPINFOHEADER	bmp_info = { 0 };
		bmp_info.biSize = sizeof(BITMAPINFOHEADER);
		bmp_info.biWidth = cx;
		bmp_info.biHeight = cy;
		bmp_info.biPlanes = 1;
		bmp_info.biBitCount = 32;

		HBITMAP	hBitmap = ::CreateDIBSection(m_hDC, (LPBITMAPINFO)&bmp_info, DIB_RGB_COLORS, (void**)&pRgbImg, NULL, NULL);
		if (hBitmap == NULL)
		{
			return	FALSE;
		}

		if (m_hOldmap == NULL)
		{
			m_hOldmap = (HBITMAP)::SelectObject(m_hDC, hBitmap);
		}
		else
		{
			HBITMAP	hOldmap = (HBITMAP)::SelectObject(m_hDC, hBitmap);
			::DeleteObject(hOldmap);
		}

		m_pRgbImg = pRgbImg;
		m_size.cx = cx;
		m_size.cy = cy > 0 ? cy : -cy;

		return	TRUE;
	}

	void	Clear(COLORREF color)
	{
		RECT	tRect = { 0, 0, m_size.cx, m_size.cy };
		::SetBkColor(m_hDC, color);
		::ExtTextOut(m_hDC, 0, 0, ETO_OPAQUE, &tRect, NULL, 0, NULL);
	}

	BOOL SetFont(int nPoint, LPCTSTR pszFaceName)
	{
		if (m_hDC == NULL)
		{
			return	FALSE;
		}

		LOGFONT	tFont;
		tFont.lfHeight = nPoint;
		tFont.lfWidth = 0;
		tFont.lfEscapement = 0;
		tFont.lfOrientation = 0;
		tFont.lfWeight = FW_NORMAL;
		tFont.lfItalic = FALSE;
		tFont.lfUnderline = FALSE;
		tFont.lfStrikeOut = FALSE;
		tFont.lfCharSet = DEFAULT_CHARSET;
		tFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		tFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		tFont.lfQuality = ANTIALIASED_QUALITY;
		tFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

		if (pszFaceName == NULL)
		{
			*tFont.lfFaceName = '\0';
		}
		else
		{
			::lstrcpy(tFont.lfFaceName, pszFaceName);
		}

		HFONT	hFont = ::CreateFontIndirect(&tFont);
		if (hFont == NULL)
		{
			return	FALSE;
		}

		if (m_hOldFont == NULL)
		{
			m_hOldFont = (HFONT)::SelectObject(m_hDC, hFont);
		}
		else
		{
			hFont = (HFONT)::SelectObject(m_hDC, hFont);
			::DeleteObject(hFont);
		}

		return	TRUE;
	}

	__inline	operator HDC() const
	{
		return	m_hDC;
	}

	__inline	SIZE	GetSize()
	{
		return	m_size;
	}

	__inline	LPBYTE	GetImagePtrScan0(int& nSrcStride)
	{
		nSrcStride = -m_size.cx * 4;
		return	(LPBYTE)(m_pRgbImg + (INT_PTR)m_size.cx * (m_size.cy - 1));
	}

protected:
	HDC				m_hDC;
	LPDWORD			m_pRgbImg;
	tagSIZE			m_size;

	HBITMAP			m_hOldmap;
	HFONT			m_hOldFont;
};

class MCP2221_RasPiGPIO
{
public:
	class mcp2221
	{
	public:
		mcp2221(wchar_t * SerialNumber)
		{
			m_hHandle = Mcp2221_OpenBySN(MCP2221_USB_VID, MCP2221_USB_PID, SerialNumber);
		}

		mcp2221()
		{
			m_hHandle = Mcp2221_OpenByIndex(MCP2221_USB_VID, MCP2221_USB_PID, 0);
		}

		~mcp2221()
		{
			Mcp2221_Close(m_hHandle);
		}

		bool	IsValid()
		{
			return	m_hHandle != NULL;
		}

		operator void*() const
		{
			return	m_hHandle;
		}

	protected:
		void*		m_hHandle;
	};

	class ctrl_i2c_mcp2221 : public ctrl_i2c_if
	{
	public:
		ctrl_i2c_mcp2221(mcp2221 &handle, int slave_addr) :
			m_handle(handle)
		{
			Mcp2221_SetSpeed(
				m_handle,
				400000);

			m_nSlaveAddr = slave_addr;
			m_handle = handle;
		}

		virtual	bool    write(const unsigned char * data, int size)
		{
			return	0 == Mcp2221_I2cWrite(
				m_handle,
				size,
				m_nSlaveAddr,
				1,	// 7bit mode
				(unsigned char *)data);
		}

		virtual	bool    read(unsigned char * data, int size)
		{
			return	0 == Mcp2221_I2cRead(
				m_handle,
				size,
				m_nSlaveAddr,
				1,	// 7bit mode
				(unsigned char *)data);
		}

	public:
		mcp2221&	m_handle;
		int			m_nSlaveAddr;
	};



	class SSD1306
	{
	public:
		SSD1306(mcp2221& handle);
	
		void	DispOn();
		void	DispOff();
		void	DrawImage(CBitmap32& image);

	protected:
		ctrl_i2c_mcp2221	m_i2c;
	};

	class INA226
	{
	public:
		INA226(mcp2221& handle, double shunt_reg = 0.005 );

		void	Init();
		double	GetV();
		double	GetA();

	protected:
		double GetA_1LSB();

	protected:
		ctrl_i2c_mcp2221	m_i2c;
		const double		m_dShuntReg;
	};

public:
	MCP2221_RasPiGPIO(mcp2221& handle);
	~MCP2221_RasPiGPIO();
	void	InitializeGPIO_Flash();
	void	InitializeGPIO();
	int		GetOutputValue(int port);
	int		SetOutputValue(int port, int value);

protected:
	mcp2221&	m_handle;
};

