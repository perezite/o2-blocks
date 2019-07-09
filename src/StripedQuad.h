#pragma once
#include "Drawable.h"
#include "Transformable.h"

namespace blocks { 
	class StripedQuad : public sb::Drawable, public sb::Transformable {
		sb::Mesh _mesh;
		size_t _numStripes;
		float _alpha;

	protected:
		void createMesh(const sb::Vector2f& extent);

	public:
		StripedQuad(size_t numStripes, float alpha);
		virtual void draw(sb::DrawTarget& target, sb::DrawStates states);
	};
}