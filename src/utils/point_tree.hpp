#ifndef BEESIMULATION_UTILS_2D_TREE_H
#define BEESIMULATION_UTILS_2D_TREE_H

#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

enum Axis { X, Y };

template <typename C> struct Coordinates {
  C x;
  C y;

  double getDistanceTo(const Coordinates<C> &c) const {
    return calculateEuclideanDistance(*this, c);
  }

  static double calculateEuclideanDistance(const Coordinates<C> &point1,
                                           const Coordinates<C> &point2) {
    return std::sqrt(std::pow(point2.x - point1.x, 2) +
                     std::pow(point2.y - point1.y, 2));
  }
};

template <typename C, typename V> struct PointValue {
  Coordinates<C> point;
  std::shared_ptr<V> value;

  PointValue(C x, C y, V value) {
    this->point.x = x;
    this->point.y = y;
    this->value = std::make_unique<V>(value);
  }

  PointValue(Coordinates<C> point, std::shared_ptr<V> value) {
    this->point = point;
    this->value = std::move(value);
  }
};

template <typename C, typename V> struct NearestResult {
  const Coordinates<C> point;
  const V &value;
  const double distance;
  const int visited;
};

template <typename C, typename V> class PointTree {

private:
  struct Node {
    Coordinates<C> point;
    std::shared_ptr<V> value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node(const Coordinates<C> &point, std::shared_ptr<V> value)
        : point(point), value(value), left(nullptr), right(nullptr) {}

    Node(PointValue<C, V> &pv)
        : point(pv.point), value(pv.value), left(nullptr), right(nullptr) {}

    double getDistanceTo(const Coordinates<C> &point) const {
      return this->point.getDistanceTo(point);
    }

    friend std::ostream &operator<<(std::ostream &output, const Node &n) {
      output << "(" << n.point.x << "|" << n.point.y << ") {" << *(n.value)
             << "}";
      return output;
    }
  };

  struct PointValueCompare {
    Axis axis;

    PointValueCompare(Axis axis) : axis(axis) {}

    bool operator()(const PointValue<C, V> &p1,
                    const PointValue<C, V> &p2) const {
      switch (this->axis) {
      case X:
        return p1.point.x < p2.point.x;
        break;
      case Y:
        return p1.point.y < p2.point.y;
        break;
      }

      return false;
    }
  };

  // Per instance variables
  std::unique_ptr<Node> root = nullptr;

  // Per method iteration variables
  // Tree creation
  std::vector<PointValue<C, V>> currentValuesToCreate;

  // Nearest
  Coordinates<C> *currentNearestPoint = nullptr;
  std::shared_ptr<V> currentNearestValue = nullptr;
  double currentSmallestDistance = 0;
  int currentVisited = 0;

  std::unique_ptr<Node> makeTree(std::size_t begin, std::size_t end,
                                 Axis axis) {
    if (begin >= end) {
      return nullptr;
    }

    std::size_t mean = begin + (end - begin) / 2;

    auto it = this->currentValuesToCreate.begin();
    std::nth_element(it + begin, it + mean, it + end, PointValueCompare(axis));

    axis = axis == Axis::X ? Axis::Y : Axis::X;

    std::unique_ptr<Node> meanNode =
        std::make_unique<Node>(this->currentValuesToCreate[mean]);

    meanNode->left = makeTree(begin, mean, axis);
    meanNode->right = makeTree(mean + 1, end, axis);

    return meanNode;
  }

  std::unique_ptr<Node> insertValue(std::unique_ptr<Node> &root,
                                    PointValue<C, V> &value, Axis axis) {
    if (!root) {
      return std::make_unique<Node>(value);
    }

    if (axis == Axis::X) {
      if (value.point.x < root->point.x) {
        root->left = std::move(insertValue(root->left, value, Axis::Y));
      } else {
        root->right = std::move(insertValue(root->right, value, Axis::Y));
      }
    } else {
      if (value.point.y < root->point.y) {
        root->left = std::move(insertValue(root->left, value, Axis::X));
      } else {
        root->right = std::move(insertValue(root->right, value, Axis::X));
      }
    }

    return std::move(root);
  }

  void calculateNearest(std::unique_ptr<Node> &currentNode,
                        const Coordinates<C> &point, Axis axis) {
    if (!currentNode) {
      return;
    }

    this->currentVisited++;

    double distance = currentNode->getDistanceTo(point);

    if (this->currentNearestPoint == nullptr ||
        distance < this->currentSmallestDistance) {
      this->currentSmallestDistance = distance;
      this->currentNearestPoint = &currentNode->point;
      this->currentNearestValue = currentNode->value;
    }

    if (this->currentSmallestDistance == 0) {
      return;
    }

    double difference;
    if (axis == Axis::X) {
      difference = (double)currentNode->point.x - (double)point.x;
    } else {
      difference = (double)currentNode->point.y - (double)point.y;
    }

    axis = axis == Axis::X ? Axis::Y : Axis::X;

    this->calculateNearest(
        difference > 0 ? currentNode->left : currentNode->right, point, axis);

    if (difference * difference >= this->currentSmallestDistance) {
      return;
    }

    this->calculateNearest(
        difference > 0 ? currentNode->right : currentNode->left, point, axis);
  }

  std::size_t calculateHeight(std::unique_ptr<Node> &currentNode) {
    if (!currentNode) {
      return 0;
    }

    std::size_t leftHeight = this->calculateHeight(currentNode->left);
    std::size_t rightHeight = this->calculateHeight(currentNode->right);

    return std::max(leftHeight, rightHeight) + 1;
  }

  std::size_t traverseAndCount(std::unique_ptr<Node> &node) {
    if (!node) {
      return 0;
    }

    return this->traverseAndCount(node->left) +
           this->traverseAndCount(node->right) + 1;
  }

  void collectNodes(std::unique_ptr<Node> &node) {
    if (!node) {
      return;
    }

    PointValue<C, V> newPointValue(node->point, node->value);
    this->currentValuesToCreate.push_back(newPointValue);

    this->collectNodes(node->left);
    this->collectNodes(node->right);
  }

public:
  PointTree() {}

  template <typename iterator>
  PointTree(iterator begin, iterator end) : currentValuesToCreate(begin, end) {
    this->root = this->makeTree(0, this->currentValuesToCreate.size(), Axis::X);
    this->currentValuesToCreate.clear();
  }

  bool empty() const { return this->root == nullptr; }

  std::size_t count() { return this->traverseAndCount(this->root); }

  std::size_t height() { return this->calculateHeight(this->root); }

  void add(PointValue<C, V> value) {
    this->root = this->insertValue(this->root, value, Axis::X);
  }

  template <typename iterator> void addRange(iterator begin, iterator end) {
    for (auto it = begin; it != end; ++it) {
      this->root = this->insertValue(this->root, *it, Axis::X);
    }
  }

  void rebalance() {
    this->currentValuesToCreate.clear();
    this->collectNodes(this->root);
    this->root = this->makeTree(0, this->currentValuesToCreate.size(), Axis::X);
  }

  NearestResult<C, V> *nearest(const Coordinates<C> &point) {
    if (root == nullptr) {
      return nullptr;
    }

    this->currentNearestPoint = nullptr;
    this->currentNearestValue = nullptr;
    this->currentVisited = 0;
    this->currentSmallestDistance = 0;
    this->calculateNearest(this->root, point, Axis::X);
    return new NearestResult<C, V>{
        *this->currentNearestPoint, *(this->currentNearestValue),
        this->currentSmallestDistance, this->currentVisited};
  }

  std::string toDot() {
    std::stringstream result;
    result << "digraph G {" << std::endl;

    // for (auto &node : this->nodes) {
    //   if (node.left != nullptr) {
    //     result << "\"" << node << "\""
    //            << "->"
    //            << "\"" << *(node.left) << "\"" << std::endl;
    //   }

    //   if (node.right != nullptr) {
    //     result << "\"" << node << "\""
    //            << "->"
    //            << "\"" << *(node.right) << "\"" << std::endl;
    //   }
    // }

    result << "}";
    return result.str();
  }
};

#endif