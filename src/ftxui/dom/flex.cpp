// Copyright 2020 Arthur Sonzogni. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.

#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"

namespace ftxui {

class Flex : public Node {
 public:
  Flex() {}
  Flex(Element child) : Node(unpack(std::move(child))) {}
  ~Flex() override {}
  void ComputeRequirement() override {
    requirement_.min.x = 0;
    requirement_.min.y = 0;
    if (!children.empty()) {
      children[0]->ComputeRequirement();
      requirement_ = children[0]->requirement();
    }
    requirement_.flex.x = 1;
    requirement_.flex.y = 1;
  }

  void SetBox(Box box) override {
    if (children.empty())
      return;
    children[0]->SetBox(box);
  }
};

class NotFlex : public Flex {
 public:
  NotFlex() {}
  NotFlex(Element child) : Flex(std::move(child)) {}
  ~NotFlex() override {}
  void ComputeRequirement() override {
    requirement_.min.x = 0;
    requirement_.min.y = 0;
    if (!children.empty()) {
      children[0]->ComputeRequirement();
      requirement_ = children[0]->requirement();
    }
    requirement_.flex.x = 0;
    requirement_.flex.y = 0;
  }
};

/// @brief An element that will take expand proportionnally to the space left in
/// a container.
/// @ingroup dom
Element filler() {
  return std::make_shared<Flex>();
}

/// @brief Make a child element to expand proportionnally to the space left in a
/// container.
///
/// #### Examples:
///
/// ~~~cpp
///   hbox({
///     text(L"left") | border ,
///     text(L"middle") | border | flex,
///     text(L"right") | border,
///   });
/// ~~~
///
/// #### Output:
///
/// ~~~bash
/// ┌────┐┌─────────────────────────────────────────────────────────┐┌─────┐
/// │left││middle                                                   ││right│
/// └────┘└─────────────────────────────────────────────────────────┘└─────┘
/// ~~~
Element flex(Element input) {
  return std::make_shared<Flex>(std::move(input));
}

/// @brief Make a child element not to be flexible anymore.
Element notflex(Element child) {
  return std::make_shared<NotFlex>(std::move(child));
}

}  // namespace ftxui
