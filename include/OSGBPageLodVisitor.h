#pragma once

#include <QVector>
#include <QSet>
#include <QMap>

#include <osg/PagedLOD>
#include <osgDB/ReadFile>
#include <osgDB/ConvertUTF>
#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>

namespace gzpi {
	
    class OSGBPageLodVisitor : public osg::NodeVisitor
    {
    public:
        OSGBPageLodVisitor(const QString& path)
            :osg::NodeVisitor(TRAVERSE_ALL_CHILDREN), path(path)
        {}
    
        virtual ~OSGBPageLodVisitor() {}
    
        void apply(osg::Geometry& geometry);
        void apply(osg::PagedLOD& node);
    
    public:
        QString                             path;
        QVector<osg::Geometry*>             geometryArray;
        QSet<osg::Texture*>                 textureArray;
        QMap<osg::Geometry*, osg::Texture*> textureMap;  // ��¼ mesh �� texture �Ĺ�ϵ����ʱ��Ϊһ��ģ�����ֻ��һ��texture
        QVector<QString>                    subNodeNames;
    };
}