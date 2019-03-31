#ifndef MYERSDIFF_H
#define MYERSDIFF_H

#include <QStringList>

#include <vector>

struct Point
{
    Point() : x(0), y(0) {}
    Point(int xIn, int yIn) : x(xIn), y(yIn) {}
    int x;
    int y;
};

inline bool operator ==(const Point &lhs, const Point &rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}
inline bool operator !=(const Point &lhs, const Point &rhs)
{
    return !(lhs == rhs);
}

struct Snake
{
    Snake() : start(), mid(), end() {}
    Point start;
    Point mid;
    Point end;
};

struct DiffResult
{
    std::vector<std::vector<Snake>> visited;
    std::vector<Snake> answer;
};

inline int index(int k)
{
    if (k >= 0)
    {
        return k;
    }
    else
    {
        return -k - 1;
    }
}

DiffResult myersDiff(const QString &fromList, const QString &toList);

#endif // MYERSDIFF_H
