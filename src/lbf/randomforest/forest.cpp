#include <cassert>
#include <set>
#include "../sampler.h"
#include "forest.h"

namespace lbf {
	namespace randomforest {
		Forest::Forest(int stage, int landmark_index, int num_trees, double radius, int tree_depth){
			_stage = stage;
			_num_trees = num_trees;
			_radius = radius;
			_landmark_index = landmark_index;
			_num_total_leafs = 0;

			_trees.reserve(num_trees);
			for(int n = 0;n < num_trees;n++){
				Tree* tree = new Tree(tree_depth);
				_trees.push_back(tree);
			}
		}
		void Forest::train(std::vector<FeatureLocation> &feature_locations, 
						   cv::Mat_<int> &pixel_differences, 
						   std::vector<cv::Mat_<double>> &target_shapes)
		{
			assert(feature_locations.size() == pixel_differences.rows);
			assert(pixel_differences.cols == target_shapes.size());
			int num_data = pixel_differences.cols;
			assert(num_data > 0);
			for(int tree_index = 0;tree_index < _num_trees;tree_index++){
				// bootstrap
				std::set<int> sampled_indices;
				for(int n = 0;n < num_data;n++){
					int index = sampler::uniform_int(0, num_data - 1);
					sampled_indices.insert(index);
				}
				assert(sampled_indices.size() > 0);
				// build tree
				Tree* tree = _trees[tree_index];
				tree->train(sampled_indices, feature_locations, pixel_differences, target_shapes);
				_num_total_leafs += tree->get_num_leafs();
			}
		}
	}
}