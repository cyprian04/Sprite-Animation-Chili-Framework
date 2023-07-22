#include "Surface.h"
#include "ChiliWin.h"
#include <cassert>
#include <fstream>

Surface::Surface(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	assert(file);

	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32);
	assert(bmInfoHeader.biCompression == BI_RGB);
	const bool is32Bit = bmInfoHeader.biBitCount == 32;

	width = bmInfoHeader.biWidth;
	height = bmInfoHeader.biHeight;

	int yStart;
	int yEnd;
	int dy;

	if (height < 0)
	{
		height = -height;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}
	
		pPixels = std::make_unique<Color[]>(width * height);

		file.seekg(bmFileHeader.bfOffBits);
		//only used if file have 24 bits lines
		const int padding = (4 - (width * 3) % 4) % 4;
		for (int y = yStart; y != yEnd; y+=dy)
		{
			for (int x = 0; x < width; x++)
			{
				PutPixel(x, y, Color(file.get(), file.get(), file.get()));
				if (is32Bit)
				{
					file.seekg(1, std::ios::cur);
				}
			}
			if (!is32Bit)
			{
				file.seekg(padding, std::ios::cur);
			}
		}
}

Surface::Surface( int width,int height )
	:
	width( width ),
	height( height ),
	pPixels( new Color[width*height] )
{
}

Surface::Surface( const Surface& rhs )
	:
	Surface( rhs.width,rhs.height )
{
	const int nPixels = width * height;
	for( int i = 0; i < nPixels; i++ )
	{
		pPixels[i] = rhs.pPixels[i];
	}
}

// nie potrzebujemy ju¿ naszego move ctor i move assign poniewa¿ dla unique_ptr wystarcz¹ te defaultowe od kompilatora

Surface& Surface::operator=( const Surface& rhs )
{
	if (&rhs != this)
	{
		width = rhs.width;
		height = rhs.height;

		pPixels = std::make_unique<Color[]>(width * height);

		const int nPixels = width * height;
		for (int i = 0; i < nPixels; i++)
		{
			pPixels[i] = rhs.pPixels[i];
		}
	}
	return *this;
}


void Surface::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < width );
	assert( y >= 0 );
	assert( y < height );
	pPixels[y * width + x] = c;
}

Color Surface::GetPixel( int x,int y ) const
{
	assert( x >= 0 );
	assert( x < width );
	assert( y >= 0 );
	assert( y < height );
	return pPixels[y * width + x];
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

RectI Surface::GetRect() const
{
	return RectI(0, width, 0, height);
}
