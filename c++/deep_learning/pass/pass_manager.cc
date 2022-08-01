#include "pass_namager.h"

std::shared_ptr<ScopeGraph> PassManager::BuildScopeGraph(tensorflow::GraphDef *graph_def) {
  scope_graph_ = std::make_shared<ScopeGraph>();
  if (scope_graph_ == nullptr) {
    return nullptr;
  }
  Status ret = scope_graph_->Init();
  if (ret != SUCCESS) {
    return nullptr;
  }
  auto &impl = scope_graph_->impl_;
  impl->BuildScopeGraph(graph_def);
  return scope_graph_;
}

Status PassManager::AddPass(unique_ptr<ScopeBasePass> &pass) {
  graph_passes_.push_back(std::move(pass));

  return SUCCESS;
}

Status PassManager::Run(shared_ptr<ScopeGraph> &graph) {
  bool not_changed = true;

  for (auto &pass : graph_passes_) {
    auto &impl = pass->impl_;
    if (impl == nullptr) {
      continue;
    }

    Status status = impl->Run(graph);
    if (status == SUCCESS) {
      not_changed = false;
    } else if (status != SCOPE_NOT_CHANGED) {
      return status;
    }
  }

  return not_changed ? SCOPE_NOT_CHANGED : SUCCESS;
}
