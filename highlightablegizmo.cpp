#include "osgexercise.h"

HighlightableGizmo::HighlightableGizmo()
	: osg::Geode() {
	highlight = false;
}

void HighlightableGizmo::setHighlight(bool h) {
	highlight = h;
}