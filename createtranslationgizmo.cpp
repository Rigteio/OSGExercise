#include "osgexercise.h"

osg::Group *createTranslationGizmo(osg::Camera *camera) {
	osg::Geometry *geomZ = new osg::Geometry();
	osg::Vec3Array *verts = new osg::Vec3Array();
	verts->push_back(osg::Vec3(0, 0, 1));
	verts->push_back(osg::Vec3(0, .1, .8));
	verts->push_back(osg::Vec3(0, .025, .8));
	verts->push_back(osg::Vec3(0, .025, .6));
	verts->push_back(osg::Vec3(0, .025, .4));
	verts->push_back(osg::Vec3(0, .025, .2));
	verts->push_back(osg::Vec3(0, .025, 0));
	verts->push_back(osg::Vec3(0, 0, 0));
	osg::Vec3Array *gizmoVertsZ = new osg::Vec3Array();
	for (int i = 0; i < 8; i++) {
		if (i == 0 || i == 7) {
			gizmoVertsZ->push_back(verts->at(i));
		}
		else {
			for (int j = 0; j < 8; j++) {
				osg::Matrix rotate;
				rotate.makeRotate(osg::PI / 4. * j, osg::Vec3(0, 0, 1));
				gizmoVertsZ->push_back(rotate*verts->at(i));
			}
		}
	}

	unsigned short tips[3 * 16];
	for (int i = 0; i < 8; i++) {
		tips[i * 3] = 0;
		tips[i * 3 + 2] = i + 1;
		tips[i * 3 + 1] = i == 7 ? 1 : i + 2;
		tips[24 + i * 3] = 49;
		tips[24 + i * 3 + 1] = i + 41;
		tips[24 + i * 3 + 2] = i == 7 ? 41 : i + 42;
	}

	unsigned short shaft[4 * 5 * 8];
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 8; j++) {
			int offset = i * 32 + j * 4;
			shaft[offset] = i * 8 + j + 1;
			shaft[offset + 3] = i * 8 + j + 9;
			shaft[offset + 2] = j == 7 ? i * 8 + 9 : i * 8 + j + 10;
			shaft[offset + 1] = j == 7 ? i * 8 + 1 : i * 8 + j + 2;
		}
	}

	//osg::Geode *geodeZ = new osg::Geode();
	HighlightableGizmo *geodeZ = new HighlightableGizmo();
	geodeZ->addDrawable(geomZ);
	geodeZ->setName("tz");

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

	geomZ->setVertexArray(gizmoVertsZ);
	geomZ->setColorArray(new osg::Vec4Array(1, new osg::Vec4(0, 0, 1, 1)));
	geomZ->setColorBinding(osg::Geometry::BIND_OVERALL);
	/*geomZ->setVertexAttribArray(0, gizmoVertsZ, osg::Array::BIND_PER_VERTEX);
	geomZ->setVertexAttribArray(1, new osg::Vec4Array(1, new osg::Vec4(1, 0, 0, 1)), osg::Array::BIND_OVERALL);*/
	geomZ->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES, 3 * 16, tips));
	geomZ->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::QUADS, 4 * 5 * 8, shaft));

	geomX->setVertexArray(gizmoVertsX);
	geomX->setColorArray(new osg::Vec4Array(1, new osg::Vec4(1, 0, 0, 1)));
	geomX->setColorBinding(osg::Geometry::BIND_OVERALL);
	/*geomX->setVertexAttribArray(0, gizmoVertsX, osg::Array::BIND_PER_VERTEX);
	geomX->setVertexAttribArray(1, new osg::Vec4Array(1, new osg::Vec4(1, 0, 0, 1)), osg::Array::BIND_OVERALL);*/
	geomX->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES, 3 * 16, tips));
	geomX->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::QUADS, 4 * 5 * 8, shaft));
	
	geomY->setVertexArray(gizmoVertsY);
	geomY->setColorArray(new osg::Vec4Array(1, new osg::Vec4(0, 1, 0, 1)));
	geomY->setColorBinding(osg::Geometry::BIND_OVERALL);
	/*geomY->setVertexAttribArray(0, gizmoVertsY, osg::Array::BIND_PER_VERTEX);
	geomY->setVertexAttribArray(1, new osg::Vec4Array(1, new osg::Vec4(1, 0, 0, 1)), osg::Array::BIND_OVERALL);*/
	geomY->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLES, 3 * 16, tips));
	geomY->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::QUADS, 4 * 5 * 8, shaft));

	HighlightableGizmo *geodeX = new HighlightableGizmo();
	HighlightableGizmo *geodeY = new HighlightableGizmo();
	geodeX->addDrawable(geomX);
	geodeY->addDrawable(geomY);
	geodeX->setName("tx");
	geodeY->setName("ty");
	osg::Group *group = new osg::Group();

	osg::ref_ptr<osg::Program> pr = new osg::Program;
	osg::ref_ptr<osg::Shader> vs = new osg::Shader(osg::Shader::VERTEX);
	osg::ref_ptr<osg::Shader> fs = new osg::Shader(osg::Shader::FRAGMENT);
	vs->setShaderSource(std::string(vsrc));
	fs->setShaderSource(std::string(fsrc));
	pr->addShader(fs);
	pr->addShader(vs);

	//geodeX->getOrCreateStateSet()->setAttributeAndModes(pr.get(), osg::StateAttribute::ON);
	//geodeY->getOrCreateStateSet()->setAttributeAndModes(pr.get(), osg::StateAttribute::ON);
	//geodeZ->getOrCreateStateSet()->setAttributeAndModes(pr.get(), osg::StateAttribute::ON);

	osgFX::SpecularHighlights *highlights = new osgFX::SpecularHighlights();
	highlights->setSpecularColor(osg::Vec4(1, 1, 1, 1));
	highlights->setSpecularExponent(1);
	highlights->setLightNumber(0);

	group->addChild(highlights);
	group->addChild(geodeX);
	group->addChild(geodeY);
	group->addChild(geodeZ);

	osgUtil::SmoothingVisitor smooth;
	smooth.setCreaseAngle(1.5);

	group->accept(smooth);

	return group;
}