#pragma once

class Color
{
public:
	/* Members */
	float r, g, b;
	
	/* Constructors */
	Color(void);
	Color(float red, float green, float blue);
	~Color(void);

	/* Methods */
	void setEqual(Color dest);
	
	/* Operators */
	const Color operator+(const Color &c1)	{
		Color result = Color(c1.r + this->r, c1.g + this->g, c1.b + this->b);
		if (result.r > 1) result.r = 1;
		if (result.g > 1) result.g = 1;
		if (result.b > 1) result.b = 1;
		return result;
	}

	const Color operator+=(const Color &c1) {
		this->r+=c1.r; 
		this->g+=c1.g; 
		this->b+=c1.b;
		
		if(this->r > 1) this->r = 1;
		if(this->g > 1) this->g = 1;
		if(this->b > 1) this->b = 1;

		return *this;
	}
	const Color operator-(const Color &c1)	{
		return Color(this->r - c1.r, this->g - c1.g, this->b - c1.b);
	}

	const Color operator-=(const Color &c1) {
		this->r-=c1.r; 
		this->g-=c1.g; 
		this->b-=c1.b;
		return *this;
	}
	const Color operator*(const Color &c1)	{
		return Color(c1.r * this->r, c1.g * this->g, c1.b * this->b);
	}

	const Color operator*=(const Color &c1) {
		this->r*=c1.r; 
		this->g*=c1.g; 
		this->b*=c1.b;
		return *this;
	}

	const Color operator*(const double &num)	{
		return Color(num * this->r, num * this->g, num* this->b);
	}

	const Color operator*=(const double &num) {
		this->r*=num; 
		this->g*=num; 
		this->b*=num;
		return *this;
	}

	const Color operator/(const Color &c1)	{
		return Color(this->r / c1.r, this->g / c1.g, this->b / c1.b);
	}

	const Color operator/=(const Color &c1) {
		this->r/=c1.r; 
		this->g/=c1.g; 
		this->b/=c1.b;
		return *this;
	}
	
	const bool operator>(const double &num) {
		return (this->r > num || this->g > num || this->b > num);
	}

};
