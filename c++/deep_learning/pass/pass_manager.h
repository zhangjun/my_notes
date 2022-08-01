#pragma once

#include <memory>
#include <vector>

enum Status {
  SUCCESS = 1,
  SCOPE_NOT_CHANGED
};

class ScopeGraph;
class ScopeBasePass;
class tensorflow::GraphDef;

class PassManager {
 public:
  PassManager(): scope_graph_(nullptr) {}
  PassManager(const PassManager &pass_manager) = delete;
  PassManager &operator=(const PassManager &pass_manager) = delete;
  ~PassManager() {}

  std::shared_ptr<ScopeGraph> BuildScopeGraph(tensorflow::GraphDef *graph_def);

  Status AddPass(std::unique_ptr<ScopeBasePass> &pass);
  Status Run(std::shared_ptr<ScopeGraph> &graph);

  std::shared_ptr<ScopeGraph> scope_graph_;

 private:
  std::vector<std::unique_ptr<ScopeBasePass>> graph_passes_;
};