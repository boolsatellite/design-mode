#include <cmath>
#include <csignal>
#include <functional>
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <exception>

using namespace std;

struct Point {
  int x, y;
};

struct Line {
  Point start, end;
};

struct VectorObject {
  virtual std::vector<Line>::iterator begin() = 0;
  virtual std::vector<Line>::iterator end() = 0;
};

struct VectorRectangle : VectorObject {
  VectorRectangle(int x, int y, int width, int height)
      : width_(width), height_(height) {
    lines.emplace_back(Line{Point{x, y}, Point{x + width, y}});
    lines.emplace_back(Line{Point{x + width, y}, Point{x + width, y + height}});
    lines.emplace_back(Line{Point{x, y}, Point{x, y + height}});
    lines.emplace_back(
        Line{Point{x, y + height}, Point{x + width, y + height}});
  }
  std::vector<Line>::iterator begin() override { return lines.begin(); }
  std::vector<Line>::iterator end() override { return lines.end(); }

private:
  int width_;
  int height_;
  std::vector<Line> lines;
};

// 假设我们想在屏幕上画线段，甚至矩形。不幸的是，我们不能做不到，因为绘图的唯一接口是这样的:
class CPaintDC;
void DrawPoints(CPaintDC &dc, std::vector<Point>::iterator start,
                std::vector<Point>::iterator end) {
  for (auto i = start; i != end; ++i)
    // dc.SetPixel(i->x, i->y, 0);
    ;
};

// 我们需要像素点， 但我们只有直线。我们需要一个适配器

vector<shared_ptr<VectorObject>> vectorObjects{
    make_shared<VectorRectangle>(10, 10, 100, 100),
    make_shared<VectorRectangle>(10, 10, 100, 100)};

struct LineToPointAdapter {
  using Points = vector<Point>;
  LineToPointAdapter(Line &line) {
    int left = min(line.start.x, line.end.x);
    int right = max(line.start.x, line.end.x);
    int top = min(line.start.y, line.end.y);
    int bottom = max(line.start.y, line.end.y);
    int dx = right - left;
    int dy = line.end.y - line.start.y;

    // only vertical or horizontal lines
    if (dx == 0) {
      // vertical
      for (int y = top; y <= bottom; ++y) {
        points.emplace_back(Point{left, y});
      }
    } else if (dy == 0) {
      for (int x = left; x <= right; ++x) {
        points.emplace_back(Point{x, top});
      }
    }
  }
  virtual Points::iterator begin() { return points.begin(); }
  virtual Points::iterator end() { return points.end(); }

private:
  Points points;
};


