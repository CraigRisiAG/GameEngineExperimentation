


/// \class EditorAtlasPacker
/// \brief Utility class for packing multiple charts into a single atlas texture.
///
/// EditorAtlasPacker provides functionality to arrange 2D chart geometries into an optimized
/// atlas layout, minimizing wasted space through intelligent placement and optional transposition.
///
/// \struct Chart
/// \brief Represents a 2D chart with geometry data and packing metadata.
/// \param vertices Vector of 2D vertices defining the chart geometry
/// \param faces Vector of triangular faces referencing vertex indices
/// \param can_transpose Whether this chart is allowed to be rotated 90 degrees during packing
/// \param final_offset The computed position of this chart in the final atlas
/// \param transposed Whether this chart was rotated during the packing process
///
/// \struct PlottedBitmap
/// \brief Internal structure tracking a chart's placement on the bitmap during packing.
/// \param chart_index Index of the associated Chart in the input vector
/// \param offset Current position being evaluated for placement
/// \param area Total pixel area of the chart's bounding box
/// \param top_heights Height profile of the chart's top edge
/// \param bottom_heights Height profile of the chart's bottom edge
/// \param transposed Whether this bitmap is in rotated orientation
/// \param final_pos Final computed position in the atlas
///
/// \fn static void chart_pack(Vector<Chart> &charts, int &r_width, int &r_height, int p_atlas_max_size = 2048, int p_cell_resolution = 4)
/// \brief Packs multiple charts into a single atlas texture.
/// \param charts Vector of charts to pack; modified with final_offset and transposed values
/// \param r_width Output atlas width in pixels
/// \param r_height Output atlas height in pixels
/// \param p_atlas_max_size Maximum allowed atlas dimension in pixels (default: 2048)
/// \param p_cell_resolution Grid resolution for placement quantization in pixels (default: 4)
#ifndef EDITOR_ATLAS_PACKER_H
#define EDITOR_ATLAS_PACKER_H

#include "core/math/vector2.h"

#include "core/vector.h"
#include "scene/resources/bit_map.h"

class EditorAtlasPacker {
public:
	struct Chart {
		Vector<Vector2> vertices;
		struct Face {
			int vertex[3];
		};
		Vector<Face> faces;
		bool can_transpose;

		Vector2 final_offset;
		bool transposed;
	};

private:
	struct PlottedBitmap {
		int chart_index;
		Vector2i offset;
		int area;
		Vector<int> top_heights;
		Vector<int> bottom_heights;
		bool transposed;

		Vector2 final_pos;

		bool operator<(const PlottedBitmap &p_bm) const {
			return area > p_bm.area;
		}
	};

	static void _plot_triangle(Ref<BitMap> p_bitmap, Vector2i *vertices);

public:
	static void chart_pack(Vector<Chart> &charts, int &r_width, int &r_height, int p_atlas_max_size = 2048, int p_cell_resolution = 4);
};

#endif // EDITOR_ATLAS_PACKER_H
