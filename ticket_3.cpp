#include <iostream>
#include <vector>
#include <utility>

class EdgeList {
  int n_vertex_{};
  int n_edges_{};
  std::vector<std::pair<int, int>> edges_;

 public:
  EdgeList() {
    std::cin >> n_vertex_ >> n_edges_;
    edges_.resize(n_edges_);
    for (auto &[a, b] : edges_) {
      std::cin >> a >> b;
      a--;
      b--;
    }
  }
};

class AdjMatrix {
  int n_vertex_{};
  int n_edges_{};
  std::vector<std::vector<int>> adj_matrix_;
 public:
  AdjMatrix() {
    std::cin >> n_vertex_ >> n_edges_;
    adj_matrix_.resize(n_vertex_, std::vector<int>(n_vertex_, 0));
    for (int i = 0; i < n_edges_; ++i) {
      int a, b;
      std::cin >> a >> b;
      a--;
      b--;
      adj_matrix_[a][b] = 1;
      adj_matrix_[b][a] = 1;
    }
  }
};

class AdjList {
  int n_vertex_{};
  int n_edges_{};
  std::vector<std::vector<int>> adj_list_;
 public:
  AdjList() {
    std::cin >> n_vertex_ >> n_edges_;
    adj_list_.resize(n_vertex_);
    for (int i = 0; i < n_edges_; ++i) {
      int a, b;
      std::cin >> a >> b;
      a--;
      b--;
      adj_list_[a].push_back(b);
      adj_list_[b].push_back(a);
    }
  }
};
