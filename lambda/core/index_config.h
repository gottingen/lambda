//
// Created by 李寅斌 on 2023/4/4.
//

#ifndef LAMBDA_CORE_INDEX_CONFIG_H_
#define LAMBDA_CORE_INDEX_CONFIG_H_

#include <cstddef>
#include <string>

namespace lambda {

enum class EngineType {
  ENGINE_HNSWLIB = 0,
};

#define DEFAULT_M 16  // defines tha maximum number of outgoing connections in the graph
#define DEFAULT_KR_CONSTRUCTION 200  // defines a construction time/accuracy trade-off
#define DEFAULT_KR 50  // the query time accuracy/speed trade-off
#define DEFAULT_HNSW_MAX_ELEMENTS \
  50000  // defines the maximum number of elements that can be stored in the structure
#define DEFAULT_RANDOM_SEED 100  // default random seed

// max_elements defines the maximum number of elements that can be
// stored in the structure(can be increased/shrunk).
// kr_construction defines a construction time/accuracy trade-off.
// M defines tha maximum number of outgoing connections in the graph.
struct HnswParam {
  size_t m = DEFAULT_M;
  size_t kr_construction = DEFAULT_KR_CONSTRUCTION;
  size_t kr = DEFAULT_KR;
  size_t max_elements = DEFAULT_HNSW_MAX_ELEMENTS;
};


#define DEFAULT_BATCH_SIZE 50000
#define DEFAULT_MAX_ELEMENTS 1000

enum class IndexMetricType {
  INDEX_METRIC_INNER_PRODUCT = 0,
  INDEX_METRIC_L2 = 1,
  INDEX_METRIC_COSINE = 2,
  INDEX_METRIC_HAMMING = 3
};

struct IndexConfig {
  std::string index;
  EngineType index_type{EngineType::ENGINE_HNSWLIB};
  bool       is_double_switch{false};
  size_t     max_elements{DEFAULT_MAX_ELEMENTS};
  IndexMetricType metric{IndexMetricType::INDEX_METRIC_L2};
  size_t     dimension;
  HnswParam  hnsw_param;
};

}  // namespace lambda

#endif  // LAMBDA_CORE_INDEX_CONFIG_H_
