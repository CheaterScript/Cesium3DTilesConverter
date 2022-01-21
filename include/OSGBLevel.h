#pragma once

#include <OSGBuildState.h>
#include <OSGBPageLodVisitor.h>
#include <QSharedPointer>
#include <QString>
#include <QDir>
#include <QVector>

namespace gzpi {
    class OSGBLevel {
    public:
        using OSGBLevelPtr = QSharedPointer<OSGBLevel>;
        struct OSGBMesh
        {
            QString name;
            osg::Vec3d min;
            osg::Vec3d max;
        };

        OSGBLevel() {}
        OSGBLevel(const QString& name, const QString& path) :
            nodeName(name), nodePath(path) {}
        OSGBLevel(const QString& absoluteLocation) {
            QDir location(absoluteLocation);
            nodeName = location.dirName();
            location.cdUp();
            nodePath = location.absolutePath();
        }

        double              geometricError;
        QString             nodeName;
        QString             nodePath;
        QVector<OSGBLevel>  subNodes;
        osg::Vec3d          bBoxMax;
        osg::Vec3d          bBoxMin;

        QString absoluteLocation() const;

        /// <summary>
        /// ��ȡ Tile_+154_+018_L22_0000320.osgb _L���������
        /// ��ȡʧ�ܣ����� 0
        /// </summary>
        int getLevelNumber();

        bool getAllOSGBLevels(int maxLevel);

        /// <summary>
        /// ��Ƭ��PageLOD�µ������ӽڵ�תB3DM
        /// </summary>
        /// <param name="outLocation"></param>
        /// <returns></returns>
        bool writeB3DM(const QString& outLocation);

        /// <summary>
        /// OSGB�ļ�תB3DM
        /// </summary>
        /// <param name="path"></param>
        /// <param name="tile_box"></param>
        /// <returns></returns>
        bool convertB3DM(QByteArray& b3dmBuffer);

        bool convertGLB(QByteArray& glbBuffer, OSGBMesh& mesh);

    private:
        tinygltf::Material makeColorMaterialFromRGB(double r, double g, double b);
    };
}