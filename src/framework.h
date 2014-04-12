/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	Here we define all the mathematical classes like Vector3, Matrix44 and some extra useful geometrical functions
*/

#ifndef FRAMEWORK //macros to ensure the code is included once
#define FRAMEWORK

#include <vector>
#include <cmath>
#include <cstdlib>

#ifndef PI
#define PI 3.14159265359
#endif
#define DEG2RAD 0.0174532925

//clamp a value 'x' between 'a' and 'b'
inline double clamp(double x, double a, double b) { return x < a ? a : (x > b ? b : x); }
inline unsigned int clamp(unsigned int x, unsigned int a, unsigned int b) { return x < a ? a : (x > b ? b : x); }
inline unsigned char clamp(unsigned char x, unsigned char a, unsigned char b) { return x < a ? a : (x > b ? b : x); }

class Vector3;

//Color class to store colors in unsigned byte
class Color
{
public:
	union
	{
		struct { unsigned char r;
				 unsigned char g;
				 unsigned char b; };
		unsigned char v[3];
	};
	Color() { r = g = b = 0; }
	Color(double r, double g, double b) { this->r = (unsigned char)r; this->g = (unsigned char)g; this->b = (unsigned char)b; }
	void operator = (const Vector3& v);

	void set(double r, double g, double b) { this->r = (unsigned char)clamp(r,0.0,255.0); this->g = (unsigned char)clamp(g,0.0,255.0); this->b = (unsigned char)clamp(b,0.0,255.0); }
	void random() { r = rand() % 255; g = rand() % 255; b = rand() % 255; }

	Color operator * (double v) { return Color((unsigned char)(r*v), (unsigned char)(g*v), (unsigned char)(b*v)); }
	void operator *= (double v) { r = (unsigned char)(r * v); g = (unsigned char)(g * v); b = (unsigned char)(b * v); }
	Color operator / (double v) { return Color((unsigned char)(r/v), (unsigned char)(g/v), (unsigned char)(b/v)); }
	void operator /= (double v) { r = (unsigned char)(r / v); g = (unsigned char)(g / v); b = (unsigned char)(b / v); }
	Color operator + (const Color& v) { return Color((double)(r+v.r), (double)(g+v.g), (double)(b+v.b) ); }
	void operator += (const Color& v) { r += v.r; g += v.g; b += v.b; }
	Color operator - (const Color& v) { return Color((double)(r-v.r), (double)(g-v.g), (double)(b-v.b)); }
	void operator -= (const Color& v) { r -= v.r; g -= v.g; b -= v.b; }

	//some colors to help
	static const Color WHITE;
	static const Color BLACK;
	static const Color GRAY;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color CYAN;
	static const Color YELLOW;
	static const Color PURPLE;
};

inline Color operator * (const Color& c, double v) { return Color((unsigned char)(c.r*v), (unsigned char)(c.g*v), (unsigned char)(c.b*v)); }
inline Color operator * (double v, const Color& c) { return Color((unsigned char)(c.r*v), (unsigned char)(c.g*v), (unsigned char)(c.b*v)); }
//*********************************

class Vector3
{
public:
	union
	{
		struct { double x,y,z; };
		double v[3];
	};

	Vector3() { x = y = z = 0.0f; }
	Vector3(double x, double y, double z) { this->x = x; this->y = y; this->z = z;	}

	double length();
	double length() const;

	void set(double x, double y, double z) { this->x = x; this->y = y; this->z = z; }

	Vector3& normalize();
	void random(double range);
	void random(Vector3 range);

	double distance(const Vector3& v) const;

	Vector3 cross( const Vector3& v ) const;
	double dot( const Vector3& v ) const;
};

//****************************
//Matrix44 class
class Matrix44
{
	public:

		//This matrix works in
		union { //allows to access the same var using different ways
			struct
			{
				double        _11, _12, _13, _14;
				double        _21, _22, _23, _24;
				double        _31, _32, _33, _34;
				double        _41, _42, _43, _44;
			};
			double M[4][4]; //[row][column]
			double m[16];
		};

		Matrix44();
		Matrix44(const double* v);

		void set(); //multiply with opengl matrix
		void load(); //load in opengl matrix
		void clear();
		void setIdentity();
		void transpose();

		//get base vectors
		Vector3 rightVector() { return Vector3(m[0],m[1],m[2]); }
		Vector3 topVector() { return Vector3(m[4],m[5],m[6]); }
		Vector3 frontVector() { return Vector3(m[8],m[9],m[10]); }

		bool inverse();
		void setUpAndOrthonormalize(Vector3 up);
		void setFrontAndOrthonormalize(Vector3 front);

		Matrix44 getRotationOnly(); //used when having scale

		//rotate only
		Vector3 rotateVector( const Vector3& v);

		//transform using world coordinates
		void traslate(double x, double y, double z);
		void rotate( double angle_in_rad, const Vector3& axis  );

		//transform using local coordinates
		void traslateLocal(double x, double y, double z);
		void rotateLocal( double angle_in_rad, const Vector3& axis  );

		//create a transformation matrix from scratch
		void setTranslation(double x, double y, double z);
		void setRotation( double angle_in_rad, const Vector3& axis );

		bool getXYZ(double* euler) const;

		Matrix44 operator * (const Matrix44& matrix) const;
};

//Operators, they are our friends
//Matrix44 operator * ( const Matrix44& a, const Matrix44& b );
Vector3 operator * (const Matrix44& matrix, const Vector3& v);
Vector3 operator + (const Vector3& a, const Vector3& b);
Vector3 operator - (const Vector3& a, const Vector3& b);
Vector3 operator * (const Vector3& a, double v);

class Vector2
{
public:
	union
	{
		struct { double x,y; };
		double value[2];
	};

	Vector2() { x = y = 0.0f; }
	Vector2(double x, double y) { this->x = x; this->y = y; }

	double length() { return sqrt(x*x + y*y); }
	double length() const { return sqrt(x*x + y*y); }

	double dot( const Vector2& v );
	double perpdot( const Vector2& v );

	void set(double x, double y) { this->x = x; this->y = y; }

	Vector2& normalize() { *this *= (double)length(); return *this; }

	double distance(const Vector2& v);
	void random(double range);

	void operator *= (double v) { x*=v; y*=v; }
};

Vector2 operator * (const Vector2& a, double v);
Vector2 operator + (const Vector2& a, const Vector2& b);
Vector2 operator - (const Vector2& a, const Vector2& b);

inline double distance(const Vector2& a, const Vector2& b) { return (double)(a-b).length(); }
inline double distance(double x, double y, double x2, double y2) { return sqrtf( (x-x2)*(x-x2) + (y-y2)*(y-y2)); }

class Vector3u
{
public:
	union
	{
		struct { unsigned int x;
				 unsigned int y;
				 unsigned int z; };
		struct { unsigned int r;
				 unsigned int g;
				 unsigned int b; };
		unsigned int v[3];
	};
	Vector3u() { x = y = z = 0; }
	Vector3u(unsigned int x, unsigned int y, unsigned int z) { this->x = x; this->y = y; this->z = z; }

	void operator *= (double v) { x = (unsigned int)(x * v); y = (unsigned int)(y * v); z = (unsigned int)(z * v); }
	Vector3u operator / (double v) { return Vector3u((unsigned int)(r/v), (unsigned int)(g/v), (unsigned int)(b/v)); }
	void operator /= (double v) { x = (unsigned int)(x / v); y = (unsigned int)(y / v); z = (unsigned int)(z / v); }
	Vector3u operator + (const Vector3u& v) { return Vector3u(x+v.x, y+v.y, z+v.z); }
	void operator += (const Vector3u& v) { x += v.x; y += v.y; z += v.z; }
	Vector3u operator - (const Vector3u& v) { return Vector3u(x-v.x, y-v.y, z-v.z); }
	void operator -= (const Vector3u& v) { x -= v.x; y -= v.y; z -= v.z; }
};

inline Vector3u operator * (double v, const Vector3u& c) { return Vector3u((unsigned int)(c.x*v), (unsigned int)(c.y*v), (unsigned int)(c.z*v)); }

double ComputeSignedAngle( Vector2 a, Vector2 b);
Vector3 RayPlaneCollision( const Vector3& plane_pos, const Vector3& plane_normal, const Vector3& ray_origin, const Vector3& ray_dir );

#endif
