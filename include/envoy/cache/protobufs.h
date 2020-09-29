#pragma once

#include <typeinfo>

#include "common/protobuf/utility.h"

namespace Envoy {
namespace Cache {

template <typename T> class Protobufs {

public:
  Protobufs() = delete;
  ~Protobufs() = delete;
  Protobufs(const Protobufs&) = delete;
  Protobufs& operator=(const Protobufs&) = delete;

  static std::shared_ptr<T> get(T value) {
    static std::vector<std::weak_ptr<T>> cache_;
    static std::mutex cache_lock_;

    std::lock_guard<std::mutex> lock(cache_lock_);

    std::shared_ptr<T> sp_ret;
    std::vector<std::weak_ptr<T>> new_cache;
    for (auto wp : cache_) {
      if (auto sp = wp.lock()) {
        new_cache.push_back(sp);

        if (!sp_ret &&
              Protobuf::util::MessageDifferencer::Equivalent(*sp, value)) {
          sp_ret = sp;
        }
      }
    }  // do not break out of this loop. weak_ptrs need cleaned up

    if (!sp_ret) {
      sp_ret = std::make_shared<T>(value);
      new_cache.push_back(sp_ret);
    }

    cache_ = new_cache;
    return sp_ret;
  }

private:
  static std::vector<std::weak_ptr<T>> cache_;
  static std::mutex cache_lock_;
};

}  // namespace Cache
}  // namespace Envoy
