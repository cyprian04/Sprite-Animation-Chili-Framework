#pragma once

#include "Colors.h"
#include <string>
#include "Rect.h"
#include <memory>

class Surface
{
public:
	Surface(const std::string& filename);
	Surface( int width,int height );
	Surface(const Surface&);
	Surface( Surface&& )= default; // ustawiamy na default move ctor i move assign bo gdy mamy zadeklarowane swoje copy ctor i copy assign bez
	Surface() = default; 		  // move ctora i move assigna to kompilator zamiast ich wybierze te copy assign i copy ctor. Jak s¹ default to wybierze je		
	Surface& operator=( const Surface& );
	Surface& operator=( Surface&& )= default;
	void PutPixel( int x,int y,Color c );
	Color GetPixel( int x,int y ) const;
	int GetWidth() const;
	int GetHeight() const;
	RectI GetRect() const;
private:
	std::unique_ptr<Color[]> pPixels;
	int width = 0;
	int height = 0;
};