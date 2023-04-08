

#pragma once

#include <cstdint>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <turbo/synchronization/mutex.h>
#include "lambda/core/index_config.h"


namespace lambda {

struct IDSelector {
  virtual bool contains(size_t id) const = 0;
  virtual ~IDSelector() = default;
};


    // 1.add_with_ids
    // 2.search
    // 3.save
    // 4.load

    // search params
    struct SearchOptions {
        int64_t n = 0;  // query num
        int64_t k = 0;  // top k
        std::vector<float> vecs;  // query vector
        IDSelector *bitmap = nullptr;  // filters
        uint32_t nprobe = 0;  // cluster num, for faiss
        float radius = 0;  // for range search
    };

    class AnnIndex;
    using ann_index_ptr = std::shared_ptr<AnnIndex>;

    class AnnIndex {
    public:
        AnnIndex();

        virtual ~AnnIndex();

        virtual int init(const std::string &model) = 0;

        virtual size_t size() = 0;

        virtual bool support_update() = 0;

        virtual bool support_delete() = 0;

        virtual bool need_model() = 0;

        virtual void add_with_ids(const std::vector<int64_t> &ids, std::vector<float> &vecs) = 0;

        virtual void search(
                SearchOptions &option, std::vector<float> &distances, std::vector<int64_t> &labels) = 0;

        virtual void remove(const std::set<uint64_t> &delete_ids) = 0;

        virtual void update(const std::vector<int64_t> &ids, std::vector<float> &vecs) = 0;

        virtual void clear() = 0;

        virtual int load(const std::string &file, turbo::Mutex &write_lock) = 0;

        virtual int save(const std::string &file) = 0;

        void set_max_elements(size_t max_elements) {
            if (max_elements > index_conf_.max_elements) {
              index_conf_.max_elements = max_elements;
            }
        }

        void set_conf(const IndexConfig &conf) {
            index_conf_ = conf;
        }

        virtual int build_batch_size() {
            return DEFAULT_BATCH_SIZE;
        }

        virtual void range_search(SearchOptions &option, std::vector<std::vector<float>> &distances,
                                  std::vector<std::vector<int64_t>> &labels) {
        }

        virtual void shrink_to_fit() {
        }

        static ann_index_ptr create_ann_index(const IndexConfig &index_conf);
      protected:
        static ann_index_ptr create_single_index(const IndexConfig &index_conf);
    protected:
        IndexConfig index_conf_;
    };


};  // namespace lambda
