#pragma once

#include <Windows.h>

#include <osgViewer/Viewer>
#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osgGA/TrackballManipulator>
#include <osg/PrimitiveSet>
#include <osgUtil/SmoothingVisitor>
#include <osg/Depth>
#include <osg/ref_ptr>
#include <osgViewer/CompositeViewer>
#include <osgFX/SpecularHighlights>

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

static const GLchar *vsrc =
"#version 450\n"
"in vec4 position;"
"in vec4 color;"
"out vec4 Color;"
"uniform mat4 projectionMatrix;"
"void main() {"
"Color = color;"
"gl_Position = projectionMatrix*position;"
"}";

static const GLchar *fsrc =
"#version 450\n"
"in vec4 Color;"
"out vec4 screenColor;"
"uniform bool highlighted;"
"void main(){"
"if (highlighted) screenColor = mix(Color, vec4(1), .5f);"
"else screenColor = Color;"
"}";

class HighlightableGizmo : public osg::Geode {
	bool highlight;
public:
	HighlightableGizmo();
	void setHighlight(bool);
};

struct ProjectionMatrixCallback : public osg::Uniform::Callback {
	osg::Camera *camera;
	ProjectionMatrixCallback(osg::Camera*);
	virtual void operator()(osg::Uniform*, osg::NodeVisitor*);
};

osg::Group *createTranslationGizmo(osg::Camera*);
osg::Group *createRotationGizmo(osg::Camera*);