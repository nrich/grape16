#ifndef __MATH_POINT2_H__
#define __MATH_POINT2_H__

#include "Common/Shared.h"

#include <string>
#include <sstream>

namespace Math {
    template <class T>struct Point2 {
        private:
            union {
                T vec[2];

                struct {
                    T x;
                    T y;
                };
            };

        public:
            Point2<T>() {
                x = 0;
                y = 0;
            }

            Point2<T>(T _x, T _y) {
                x = _x;
                y = _y;
            }

            Point2<T>(const T v[2]) {
                x = v[0];
                y = v[1];
            }

            //Accessors
            T X() const {
                return x;
            }

            T Y() const {
                return y;
            }

            //maths functions

            //Add
            Point2<T> operator+(const Point2<T> &add) const {
                return Point2<T>((x+add.x), (y+add.y));
            }

            //Subtract
            Point2<T> operator-(const Point2<T> &sub) const {
                return Point2<T>((x-sub.x), (y-sub.y));
            }

            T length() const {
                return sqrt(x*x + y*y);
            }

            T distance(const Point2<T> &p) const {
                T xd = p.x - x;
                T yd = p.y - y;

                return xd * xd + yd * yd;
            }

            Point2<T> operator*(T mult) const {
                return Point2<T>(x*mult, y*mult);
            }


            Point2<T> operator!() const {
                return Point2<T>(-x, -y);
            }

            Point2<T> operator/(T div) const {
                return Point2<T>(x/div, y/div);
            }

            // TODO non const operator
            // Potentially dangerous
            const T operator[](int index) const {
                return vec[index];
            }

            T operator()(int index) const {
                return vec[index];
            }

            Point2<T> operator-() const {
                return Point2<T>(-x, -y);
            }

            //Comparison
            bool operator==(const Point2<T> &V) const {
                if (x != V.x)
                    return false;
                if (y != V.y)
                    return false;

                return true;
            }

            bool operator!=(const Point2<T> &V) const {
                if (x != V.x)
                    return true;
                if (y != V.y)
                    return true;

                return false;
            }

            bool operator>(const Point2<T> &V) const {
                if (x <= V.x)
                    return false;
                if (y <= V.y)
                    return false;

                return true;
            }

            bool operator<(const Point2<T> &V) const {
                if (x >= V.x)
                    return false;
                if (y >= V.y)
                    return false;

                return true;
            }

            bool operator>=(const Point2<T> &V) const {
                if (x < V.x)
                    return false;
                if (y < V.y)
                    return false;

                return true;
            }

            bool operator<=(const Point2<T> &V) const {
                if (x > V.x)
                    return false;
                if (y > V.y)
                    return false;

                return true;
            }

            //Dot Product
            const T operator%(const Point2<T> &v) const  {
                return (x*v.x + y*v.y);
            }

            Point2<T> operator~() const {
                T len = length();

                if (len) {
                    len = 1 / len;

                    T lx = x * len;
                    T ly = y * len;

                    return Point2<T>(lx, ly);
                } else {
                    return (*this);
                }
            }

            void copy(vec2f out) const{
                out[0] = x;
                out[1] = y;
            }

            static Point2<T> BuildMin(const Point2<T> &a, const Point2<T> &b) {
                T x = a.x < b.x ? a.x : b.x;
                T y = a.y < b.y ? a.y : b.y;

                return Point2<T>(x, y);
            }

            static Point2<T> BuildMax(const Point2<T> &a, const Point2<T> &b) {
                T x = a.x > b.x ? a.x : b.x;
                T y = a.y > b.y ? a.y : b.y;

                return Point2<T>(x, y);
            }

            Point2<T> withX(T X) const {
                return Point2<T>(X, y);
            }

            Point2<T> withY(T Y) const {
                return Point2<T>(x, Y);
            }

            bool isEmpty() const {
                if (x != 0)
                    return false;
                if (y != 0)
                    return false;

                return true;
            }

            std::string toString() const {
                std::ostringstream s;

                s << "(" << x << "," << y << ")";

                return s.str();
            }
    };
};

typedef Math::Point2<float> Vec2f;
typedef Math::Point2<double> Vec2d;

#endif //__MATH_POINT2_H__

