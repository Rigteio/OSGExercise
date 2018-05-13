#include "osgexercise.h"

osg::Group *createRotationGizmo(osg::Camera *camera) {
	osg::Geometry *geomZ = new osg::Geometry();
	osg::Vec3Array *verts = new osg::Vec3Array();
	verts->push_back(osg::Vec3(1.025, 0, 0));
	verts->push_back(osg::Vec3(1, 0, -.025));
	verts->push_back(osg::Vec3(.975, 0, 0));
	verts->push_back(osg::Vec3(1, 0, .025));
	osg::Vec3Array *gizmoVertsZ = new osg::Vec3Array();
	for (int i = 0; i < 16; i++) {
		osg::Matrix rotate;
		rotate.makeRotate(osg::PI / 8. * i, osg::Vec3(0, 0, 1));
		for (int j = 0; j < 4; j++)
			gizmoVertsZ->push_back(rotate*verts->at(j));
	}

	geomZ->setVertexArray(gizmoVertsZ);

	unsigned short ring[4 * 4 * 16];
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 4; j++) {
			int offset = i * 16 + j * 4;
			ring[offset] =  i * 4 + j;
			ring[offset+1] = i == 15 ? j : i * 4 + j + 4;
			ring[offset+2] = i == 15 ? 
				             j == 3 ? 0 : j + 1
							 : (j == 3 ? 
							 i*4 + 4 : 
				             i * 4 + j + 5);
			ring[offset+3] = (j == 3 ? 
							 i*4 : 
							 i * 4 + j + 1);
		}
	}

	geomZ->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::QUADS, 4*4*16, ring));

	geomZ->setColorArray(new osg::Vec4Array(1, new osg::Vec4(0, 0, 1, 1)));
	geomZ->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::Geode *geodeZ = new osg::Geode();
	geodeZ->addDrawable(geomZ);
	geodeZ->setName("rz");

	osg::Matrix rotateX, rotateY;
	rotateX.makeRotate(osg::PI / 2, 1, 0, 0);
	rotateY.makeRotate(osg::PI / 2, 0, 1, 0);
	osg::Vec3Array *gizmoVertsY = new osg::Vec3Array();
	osg::Vec3Array *gizmoVertsX = new osg::Vec3Array();
	for (auto v : *gizmoVertsZ) {
		gizmoVertsY->push_back(rotateX*v);
		gizmoVertsX->push_back(rotateY*v);
	}

	osg::Geometry *geomX = new osg::Geometry();
	osg::Geometry *geomY = new osg::Geometry();

	geomX->setVertexArray(gizmoVertsX);
	geomX->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::QUADS, 4*4*16, ring));
	geomX->setColorArray(new osg::Vec4Array(1, new osg::Vec4(1, 0, 0, 1)));
	geomX->setColorBinding(osg::Geometry::BIND_OVERALL);

	geomY->setVertexArray(gizmoVertsY);
	geomY->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::QUADS, 4*4*16, ring));
	geomY->setColorArray(new osg::Vec4Array(1, new osg::Vec4(0, 1, 0, 1)));
	geomY->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::Geode *geodeX = new osg::Geode();
	osg::Geode *geodeY = new osg::Geode();
	geodeX->addDrawable(geomX);
	geodeY->addDrawable(geomY);
	geodeX->setName("rx");
	geodeY->setName("ry");
	osg::Group *group = new osg::Group();
	group->addChild(geodeX);
	group->addChild(geodeY);
	group->addChild(geodeZ);

	osgUtil::SmoothingVisitor smooth;

	group->accept(smooth);

	return group;
}