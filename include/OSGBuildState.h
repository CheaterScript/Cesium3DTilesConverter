#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>
#include <stb_image_write.h>
#include <dxt_img.h>

#include <osg/PagedLOD>
#include <osgDB/ReadFile>
#include <osgDB/ConvertUTF>
#include <osgUtil/Optimizer>
#include <osgUtil/SmoothingVisitor>

namespace gzpi {
    class OSGBuildState
    {
    public:
        tinygltf::Buffer* buffer;
        tinygltf::Model* model;
        osg::Vec3f pointMax;
        osg::Vec3f pointMin;
        int drawArrayFirst;
        int drawArrayCount;

        struct OSGPrimitiveState
        {
            int vertexAccessor;
            int normalAccessor;
            int textcdAccessor;
        };

        bool appendOSGGeometry(const osg::Geometry* geom);
        void expandBBox3d(osg::Vec3f& minPoint, osg::Vec3f& maxPoint, const osg::Vec3f& point);
        void expandBBox2d(osg::Vec2f& minPoint, osg::Vec2f& maxPoint, const osg::Vec2f& point);
        void appendVec2Array(const osg::Vec2Array* v2f, osg::Vec2f& minPoint, osg::Vec2f& maxPoint);
        void appendVec3Array(const osg::Vec3Array* v3f, osg::Vec3f& minPoint, osg::Vec3f& maxPoint);
        bool appendPrimitive(const osg::Geometry* g, const osg::PrimitiveSet* ps, OSGPrimitiveState& pmtState);

        template <typename GLType>
        inline void appendOSGIndex(const osg::MixinVector<GLType>* ps, int componentType)
        {
            unsigned int maxIndex = 0;
            unsigned int minIndex = 1 << 30;
            unsigned int bufferStart = buffer->data.size();

            unsigned int indNum = ps->size();
            for (unsigned m = 0; m < indNum; m++)
            {
                GLType idx = ps->at(m);
                buffer->append(idx);
                if (idx > maxIndex) maxIndex = idx;
                if (idx < minIndex) minIndex = idx;
            }
            buffer->alignment();

            tinygltf::Accessor acc;
            acc.bufferView = model->bufferViews.size();
            acc.type = TINYGLTF_TYPE_SCALAR;
            acc.componentType = componentType;
            acc.count = indNum;
            acc.maxValues = { (double)maxIndex };
            acc.minValues = { (double)minIndex };
            model->accessors.push_back(acc);

            tinygltf::BufferView bfv;
            bfv.buffer = 0;
            bfv.target = TINYGLTF_TARGET_ELEMENT_ARRAY_BUFFER;
            bfv.byteOffset = bufferStart;
            bfv.byteLength = buffer->data.size() - bufferStart;
            model->bufferViews.push_back(bfv);
        }

    };
}
