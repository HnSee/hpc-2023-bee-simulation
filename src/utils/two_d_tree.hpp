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

template <typename C, typename V> struct CoordinatesValue {
  Coordinates<C> point;
  std::unique_ptr<V> value;

  CoordinatesValue(C x, C y, V value) {
    this->point.x = x;
    this->point.y = y;
    this->value = std::make_unique<V>(value);
  }
};

template <typename C, typename V> struct NearestResult {
  const Coordinates<C> point;
  const V &value;
  const double distance;
  const int visited;
};

template <typename C, typename V> class TwoDTree {

private:
  struct Node {
    Coordinates<C> point;
    std::unique_ptr<V> value;
    Node *left;
    Node *right;

    Node(const Coordinates<C> &point, std::unique_ptr<V> value)
        : point(point), value(std::move(value)), left(nullptr), right(nullptr) {
    }

    Node(CoordinatesValue<C, V> &cv)
        : point(cv.point), value(std::move(cv.value)), left(nullptr),
          right(nullptr) {}

    double getDistanceTo(const Coordinates<C> &point) const {
      return this->point.getDistanceTo(point);
    }

    friend std::ostream &operator<<(std::ostream &output, const Node &n) {
      output << "(" << n.point.x << "|" << n.point.y << ") {" << *(n.value)
             << "}";
      return output;
    }
  };

  struct NodeCompare {
    Axis axis;

    NodeCompare(Axis axis) : axis(axis) {}

    bool operator()(const Node &node1, const Node &node2) const {
      switch (this->axis) {
      case X:
        return node1.point.x < node2.point.x;
        break;
      case Y:
        return node1.point.y < node2.point.y;
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

  Node *insertValue(Node *root, Node *value, Axis axis) {
    if (root == nullptr) {
      return value;
    }

    if (axis == Axis::X) {
      if (value->point.x < root->point.x) {
        root->left = insertValue(root->left, value, Axis::Y);
      } else {
        root->right = insertValue(root->right, value, Axis::Y);
      }
    } else {
      if (value->point.y < root->point.y) {
        root->left = insertValue(root->left, value, Axis::X);
      } else {
        root->right = insertValue(root->right, value, Axis::X);
      }
    }

    return root;
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

  void add(CoordinatesValue<C, V> value) {
    Node newNode(value);
    this->nodes.push_back(std::move(newNode));
    // this->root = this->makeTree(0, this->nodes.size(), Axis::X);
    this->root = this->insertValue(this->root, &this->nodes.back(), Axis::X);
  }

  template <typename iterator> void addRange(iterator begin, iterator end) {
    // this->root = this->makeTree(0, this->nodes.size(), Axis::X);
    for (auto it = begin; it != end; ++it) {
      Node newNode(*it);
      this->nodes.push_back(std::move(newNode));
      this->root = this->insertValue(this->root, &this->nodes.back(), Axis::X);
    }
  }

  void rebalance() {
    this->root = this->makeTree(0, this->nodes.size(), Axis::X);
  }

  NearestResult<C, V> *nearest(const Coordinates<C> &point) {
    if (root == nullptr) {
      return nullptr;
    }

    this->currentNearestNode = nullptr;
    this->currentVisited = 0;
    this->currentSmallestDistance = 0;
    this->calculateNearest(this->root, point, Axis::X);
    return new NearestResult<C, V>{
        this->currentNearestNode->point, *(this->currentNearestNode->value),
        this->currentSmallestDistance, this->currentVisited};
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