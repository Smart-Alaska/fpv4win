#pragma once

extern "C" {
#include "libavutil/frame.h"
}
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QQuickFramebufferObject>
#include <memory>

class YUVData {
public:
    QByteArray Y;
    QByteArray U;
    QByteArray V;
    int yLineSize;
    int uLineSize;
    int vLineSize;
    int height;
};

class TItemRender;
class RealTimeRenderer : public QOpenGLFunctions {
public:
    RealTimeRenderer();
    ~RealTimeRenderer();
    void init();
    void paint();
    void resize(int width, int height);
    void updateTextureInfo(int width, int height, int format);
    void updateTextureData(const std::shared_ptr<AVFrame> &data);
    void clear();

    friend class TItemRender;

protected:
    void initTexture();
    void initShader();
    void initGeometry();

private:
    static constexpr int SHM_MAX_W = 1920;
    static constexpr int SHM_MAX_H = 1080;
    static constexpr int SHM_HEADER = 12;
    static constexpr int SHM_SIZE = SHM_HEADER + SHM_MAX_W * SHM_MAX_H * 3;

    void initShm();
    void writeFrameToShm(const std::shared_ptr<AVFrame> &data);

    QOpenGLShaderProgram mProgram;
    QOpenGLTexture *mTexY = nullptr;
    QOpenGLTexture *mTexU = nullptr;
    QOpenGLTexture *mTexV = nullptr;
    QVector<QVector3D> mVertices;
    QVector<QVector2D> mTexcoords;
    int mModelMatHandle {}, mViewMatHandle {}, mProjectMatHandle {};
    int mVerticesHandle {};
    int mTexCoordHandle {};

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;
    GLint mPixFmt = 0;
    bool mTextureAlloced = false;

    int m_itemWidth = 0;
    int m_itemHeight = 0;
    int mVideoWidth = 0;
    int mVideoHeight = 0;

    bool mNeedClear = false;

    volatile bool inited = false;

    HANDLE mShmHandle = nullptr;
    LPVOID mShmPtr = nullptr;
    int mFrameId = 0;
};
