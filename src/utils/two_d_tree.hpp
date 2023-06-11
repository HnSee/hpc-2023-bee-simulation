#ifndef BEESIMULATION_UTILS_2D_TREE_H
#define BEESIMULATION_UTILS_2D_TREE_H

#include <algorithm>
#include <array>
#include <cmath>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

enum Axis { X, Y };

template <typename C> using Coordinates = std::pair<C, C>;
template <typename C, typename V>
using CoordinatesValue = std::pair<Coordinates<C>, V>;

template <typename C>
void calculateEuclideanDistance(const Coordinates<C> &point1,
                                const Coordinates<C> &point2) {
  return std::sqrt(std::pow(point2.first - point1.first, 2) +
                   std::pow(point2.second - point1.second, 2));
}

template <typename C, typename V> class TwoDTree {

private:
  struct Node {
    Coordinates<C> point;
    V value;
    Node *left;
    Node *right;

    Node(const Coordinates<C> &point, const V &value)
        : point(point), value(value), left(nullptr), right(nullptr) {}

    Node(const CoordinatesValue<C, V> &cv)
        : point(cv.first), value(cv.second), left(nullptr), right(nullptr) {}

    C getDistanceTo(const Coordinates<C> &point) const {
      return calculateEuclideanDistance(this->point, point);
    }

    friend std::ostream &operator<<(std::ostream &output, const Node &n) {
      output << "(" << n.point.first << "|" << n.point.second << ") {"
             << n.value << "}";
      return output;
    }
  };

  struct NodeCompare {
    Axis axis;

    NodeCompare(Axis axis) : axis(axis) {}

    bool operator()(const Node &node1, const Node &node2) const {
      switch (this->axis) {
      case X:
        return node1.point.first < node2.point.first;
        break;
      case Y:
        return node1.point.second < node2.point.second;
        break;
      }

      return false;
    }
  };

  // Per instance variables
  std::vector<Node> nodes;
  Node *root = nullptr;

  // Per method iteration variables
  Node *currentNearestNode = nullptr;
  double currentSmallestDistance = 0;
  int currentVisited = 0;

  Node *makeTree(std::size_t begin, std::size_t end, Axis axis) {
    if (begin >= end) {
      return nullptr;
    }

    std::size_t mean = begin + (end - begin) / 2;

    auto it = this->nodes.begin();
    std::nth_element(it + begin, it + mean, it + end, NodeCompare(axis));

    axis = axis == Axis::X ? Axis::Y : Axis::X;

    this->nodes[mean].left = makeTree(begin, mean, axis);
    this->nodes[mean].right = makeTree(mean + 1, end, axis);

    return &this->nodes[mean];
  }

  void calculateNearest(Node *currentNode, const Coordinates<C> &point,
                        Axis axis) {
    if (currentNode == nullptr) {
      return;
    }

    this->currentVisited++;

    double distance = currentNode->getDistanceTo(point);

    if (this->currentNearestNode == nullptr ||
        distance < this->currentSmallestDistance) {
      this->currentSmallestDistance = distance;
      this->currentNearestNode = currentNode;
    }

    if (this->currentSmallestDistance == 0) {
      return;
    }

    double difference;
    if (axis == Axis::X) {
      difference = (double)currentNode->point.first - (double)point.first;
    } else {
      difference = (double)currentNode->point.second - (double)point.second;
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

  std::size_t calculateHeight(Node *currentNode) {
    if (currentNode == nullptr) {
      return 0;
    }

    std::size_t leftHeight = this->calculateHeight(currentNode->left);
    std::size_t rightHeight = this->calculateHeight(currentNode->right);

    return std::max(leftHeight, rightHeight) + 1;
  }

public:
  TwoDTree() {}

  template <typename iterator>
  TwoDTree(iterator begin, iterator end) : nodes(begin, end) {
    this->root = this->makeTree(0, this->nodes.size(), Axis::X);
  }

  bool empty() const { return this->nodes.empty(); }

  std::size_t size() { return this->nodes.size(); }

  std::size_t height() { return this->calculateHeight(this->root); }

  // int visited() const { return numberOfVisited; }

  // double distance() const { return sqrt(smallestDistance); }

  CoordinatesValue<C, V> *nearest(const Coordinates<C> &point) {
    if (root == nullptr) {
      return nullptr;
    }

    this->currentNearestNode = nullptr;
    this->currentVisited = 0;
    this->currentSmallestDistance = 0;
    this->calculateNearest(this->root, point, Axis::X);
    return std::make_pair(this->currentNearestNode->point,
                          this->currentNearestNode->value);
  }

  std::string toDot() {
    std::stringstream result;
    result << "digraph G {" << std::endl;

    for (auto &node : this->nodes) {
      if (node.left != nullptr) {
        result << "\"" << node << "\""
               << "->"
               << "\"" << *(node.left) << "\"" << std::endl;
      }

      if (node.right != nullptr) {
        result << "\"" << node << "\""
               << "->"
               << "\"" << *(node.right) << "\"" << std::endl;
      }
    }

    result << "}";
    return result.str();
  }
};

#endif