#include "osgexercise.h"

using namespace osgViewer;

class PickHandler : public osgGA::GUIEventHandler {
public:
	PickHandler() {};
	~PickHandler() {};
	bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) {
		switch (ea.getEventType()) {
		case osgGA::GUIEventAdapter::MOVE: 
			osgViewer::View *view = dynamic_cast<osgViewer::View*>(&aa);
			if (view) pick(view, ea);
		//default: break;
		}
		return false;
	};

	void pick(osgViewer::View *view, const osgGA::GUIEventAdapter& ea) {
		osgUtil::LineSegmentIntersector::Intersections intersections;
		if (view->computeIntersections(ea, intersections)) {
			for (osgUtil::LineSegmentIntersector::Intersections::iterator hitr = intersections.begin();
				hitr != intersections.end(); ++hitr) {
				if (hitr->drawable.valid()) {
					if (hitr->nodePath.empty() || hitr->nodePath.back()->getName().empty()) return;
					std::string name = hitr->nodePath.back()->getName();
					if (name == "tx" || name == "ty" || name == "tz" || name == "rx" || name == "ry" || name == "rz") {

					}
				}
			}
		}
	};
};

osg::Node* loadModel() {
	std::ifstream config("config");
	std::string filePath;
	osg::Node* model = NULL;
	if (!config.is_open()) {
		std::cout << "Could not open config file" << std::endl;
		return NULL;
	}
	while (std::getline(config, filePath)) {
		if (model) break;
		model = osgDB::readNodeFile(filePath);
		if (!model) {
			std::cout << "Could not load '" << filePath << "'" << std::endl;
		}
	}
	return model;
}

int main(int argc, char *argv[]) {

	osg::MatrixTransform* rootnode = new osg::MatrixTransform;
	rootnode->setMatrix(osg::Matrix::rotate(.5, 1, 0, 0));

	osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
	osg::Camera *camera = view->getCamera();

	osg::Node *model = loadModel();
	if (model) rootnode->addChild(model);

	osg::Group *gizmos = new osg::Group();
	gizmos->addChild(createTranslationGizmo(camera));
	gizmos->addChild(createRotationGizmo(camera));
	gizmos->getOrCreateStateSet()->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
	gizmos->getOrCreateStateSet()->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS), osg::StateAttribute::ON);
	rootnode->addChild(gizmos);

	view->addEventHandler(new PickHandler());
	view->setSceneData(rootnode);
	view->setUpViewAcrossAllScreens();
	view->setCameraManipulator(new osgGA::TrackballManipulator());

	osgViewer::CompositeViewer viewer;
	viewer.addView(view);
	viewer.realize();

	return viewer.run();
}