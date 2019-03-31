#include "myersdiff.h"

#include <vector>
#include <map>

using namespace std;

static std::vector<std::vector<int>> calculateV(const QString &fromList, const QString &toList, std::vector<std::vector<Snake>> &visited)
{
    int fromLen = fromList.size();
    int toLen = toList.size();

    std::vector<std::vector<int>> v;
    v.emplace_back(1);
    v[0][0] = 0;

    visited.emplace_back(1);

    for (int i = 0; i < fromLen && i < toLen && fromList[i] == toList[i]; i++)
    {
        v[0][0]++;
        visited[0][0].end.x++;
        visited[0][0].end.y++;
    }

    if (fromLen == toLen && v[0][0] == fromLen)
    {
        return std::move(v);
    }

    for (int d = 1; d <= fromLen + toLen; d++)
    {
        v.emplace_back(d + 1);
        visited.emplace_back(d + 1);
        for (int k = -d; k <= d; k += 2)
        {
            bool fromUp = false;
            if (k == -d || (k != d && v[d - 1][index(k - 1)] < v[d - 1][index(k + 1)]))
            {
                fromUp = true;
            }

            Snake s;

            if (fromUp)
            {
                s.start.x = v[d - 1][index(k + 1)];
                s.start.y = s.start.x - (k + 1);
                s.mid.x = s.start.x;
                s.mid.y = s.mid.x - k;
            }
            else
            {
                s.start.x = v[d - 1][index(k - 1)];
                s.start.y = s.start.x - (k - 1);
                s.mid.x = s.start.x + 1;
                s.mid.y = s.mid.x - k;
            }
            s.end = s.mid;

            while (s.end.x < fromLen && s.end.y < toLen
                   && fromList[s.end.x - 1 + 1] == toList[s.end.y - 1 + 1])
            {
                s.end.x++;
                s.end.y++;
            }
            v[d][index(k)] = s.end.x;
            visited[d][index(k)] = s;

            if (s.end.x == fromLen && s.end.y == toLen)
            {
                return std::move(v);
            }
        }
    }
    assert(false);
    return std::move(v);
}

static std::vector<Snake> calculatePath(const std::vector<std::vector<int>> &v, const QString &fromList, const QString &toList)
{
    std::vector<Snake> result(v.size());

    int fromLen = fromList.size();
    int toLen = toList.size();

    Point last(fromLen, toLen);
    for (int d = static_cast<int>(v.size()) - 1; d > 0; d--)
    {
        Snake s;
        s.end = last;

        int k = last.x - last.y;
        bool fromUp = false;
        if (k == -d || (k != d && v[d - 1][index(k - 1)] < v[d - 1][index(k + 1)]))
        {
            fromUp = true;
        }

        if (fromUp)
        {
            s.start.x = v[d - 1][index(k + 1)];
            s.start.y = s.start.x - (k + 1);
            s.mid.x = s.start.x;
            s.mid.y = s.mid.x - k;
        }
        else
        {
            s.start.x = v[d - 1][index(k - 1)];
            s.start.y = s.start.x - (k - 1);
            s.mid.x = s.start.x + 1;
            s.mid.y = s.mid.x - k;
        }
        last = s.start;
        result[d] = s;
    }

    Snake first;
    first.end.x = v[0][index(0)];
    first.end.y = first.end.x;

    result[0] = first;

    return std::move(result);
}

DiffResult myersDiff(const QString &fromList, const QString &toList)
{
    DiffResult result;
    auto v = calculateV(fromList, toList, result.visited);
    result.answer = calculatePath(v, fromList, toList);
    return result;
}
