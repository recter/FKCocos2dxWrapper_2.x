﻿//--------------------------------------------------------------------
#include "../Include/FKCW_3D_GLMath.h"
//--------------------------------------------------------------------
FKCW_3D_Matrix4 calculateViewMatrixInverse(const FKCW_3D_Vector4&_eyePos,const FKCW_3D_Vector4&_center,const FKCW_3D_Vector4&_up)
{//up no need to be unit vector
    assert(_eyePos.w()==1);
    assert(_center.w()==1);
    assert(_up.w()==0);
    FKCW_3D_Vector4 forward,side,up;
    forward=_center-_eyePos;
    up=_up;
    forward=normalize(forward);
    /* Side = forward x up */
    side=cross(forward, up);
    side=normalize(side);
    /* Recompute up as: up = side x forward */
    up=cross(side, forward);
    //calculate back (use back as Z axis)
    FKCW_3D_Vector4 back=FKCW_3D_Vector4(0,0,0,0)-forward;
    //the matrix transform point from camera space to world space is
    float m[16]={
        side.x(),side.y(),side.z(),0,//col1
        up.x(),up.y(),up.z(),0,
        back.x(),back.y(),back.z(),0,
        _eyePos.x(),_eyePos.y(),_eyePos.z(),1
    };
    FKCW_3D_Matrix4 viewMatInverse(m);
    return viewMatInverse;
}
//--------------------------------------------------------------------
FKCW_3D_Matrix4 calculateViewMatrix(const FKCW_3D_Vector4&eyePos,const FKCW_3D_Vector4&center,const FKCW_3D_Vector4&up)
{//up no need to be unit vector
    assert(eyePos.w()==1);
    assert(center.w()==1);
    assert(up.w()==0);
    FKCW_3D_Matrix4 viewMatInverse=calculateViewMatrixInverse(eyePos, center, up);
    //inverse viewMatInverse to get viewMat
    //notice that the left up 3x3 sub matrix of viewMatInverse is orthogonal matrix,
    //so we have easy way to inverse it, see:
    //http://user.qzone.qq.com/350479720/blog/1329478296
    //calculate the fourth col of view mat
    const float *mInv=viewMatInverse.getArray();
    float x=-(mInv[12]*mInv[0]+mInv[13]*mInv[1]+mInv[14]*mInv[2]);
    float y=-(mInv[12]*mInv[4]+mInv[13]*mInv[5]+mInv[14]*mInv[6]);
    float z=-(mInv[12]*mInv[8]+mInv[13]*mInv[9]+mInv[14]*mInv[10]);
    //view mat
    float m[16]={
        mInv[0],mInv[4],mInv[8],0,//col 1
        mInv[1],mInv[5],mInv[9],0,//col 2
        mInv[2],mInv[6],mInv[10],0,//col 3
        x,y,z,1//col 4
    };
    FKCW_3D_Matrix4 viewMat(m);
    return viewMat;
}
//--------------------------------------------------------------------
FKCW_3D_Matrix4 calculateRotationMatrix(const FKCW_3D_Vector4&axis,float cosA,float sinA){
    //axis must be a unit vector
    assert(axis.w()==0);
    assert(getLength2(axis)==1);
    //
    float nx=axis.x();
    float ny=axis.y();
    float nz=axis.z();
    
    float _cosA=1-cosA;
    float nx_cosA=nx*_cosA;
    float nz_cosA=nz*_cosA;
    float nxnx_cosA=nx*nx_cosA;
    float nxny_cosA=ny*nx_cosA;
    float nxnz_cosA=nz*nx_cosA;
    float nyny_cosA=ny*ny*_cosA;
    float nynz_cosA=ny*nz_cosA;
    float nznz_cosA=nz*nz_cosA;
    float nxsinA=nx*sinA;
    float nzsinA=nz*sinA;
    float nysinA=ny*sinA;
    float rotMat[16]={
        nxnx_cosA+cosA,nxny_cosA+nzsinA,nxnz_cosA-nysinA,0,//col 1
        nxny_cosA-nzsinA,nyny_cosA+cosA,nynz_cosA+nxsinA,0,//col 2
        nxnz_cosA+nysinA,nynz_cosA-nxsinA,nznz_cosA+cosA,0,//col 3
        0,0,0,1//col 4
    };
    return FKCW_3D_Matrix4(rotMat);
}
//--------------------------------------------------------------------
FKCW_3D_Matrix4 calculateTranslateMatrix(const FKCW_3D_Vector4&translate){
    assert(translate.w()==0);
    //1 0 0 dx
    //0 1 0 dy
    //0 0 1 dz
    //0 0 0  1
    float dx=translate.x();
    float dy=translate.y();
    float dz=translate.z();
    float TMat[16]={
        1,0,0,0,//col 1
        0,1,0,0,
        0,0,1,0,
        dx,dy,dz,1
    };
    return FKCW_3D_Matrix4(TMat);
}
//--------------------------------------------------------------------
FKCW_3D_Matrix4 calculateScaleMatrix(float kx,float ky,float kz){
    //1 0 0 0
    //0 1 0 0
    //0 0 1 0
    //0 0 0 1
    float SMat[16]={
        kx,0,0,0,//col 1
        0,ky,0,0,
        0,0,kz,0,
        0,0,0,1
    };
    return FKCW_3D_Matrix4(SMat);
}
//--------------------------------------------------------------------
FKCW_3D_Matrix4 calculatePerspectiveProjectionMatrix(float fovy,float aspect,float zNear,float zFar)
//if calculation failed, return zero matrix
{
    double sine, cotangent, deltaZ;
    double radians = (fovy/2) * M_PI/180;
    deltaZ = zFar - zNear;
    sine = sinf(static_cast<float>(radians));
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
        return zeroMat();
    }
    cotangent = cosf(static_cast<float>(radians)) / sine;

	float col1_0 = static_cast<float>(cotangent / aspect);
	float col2_1 = static_cast<float>(cotangent);
	float col3_2 = static_cast<float>(-(zFar + zNear) / deltaZ);
	float col4_3 = static_cast<float>(-2 * zNear * zFar / deltaZ);

    float m[16]={
        col1_0,0,0,0,//col 1
        0,col2_1,0,0,//col 2
        0,0,col3_2,-1,//col 3
        0,0, col4_3,0};//col 4

    return FKCW_3D_Matrix4(m);
}
//--------------------------------------------------------------------
FKCW_3D_Matrix4 calculateOrthoProjectionMatrix(float left,float right,float bottom,float top,float _near,float _far)
{
	//"near" and "far" have been defined in windows, so we use other var name 
    float tx=-((right+left)/(right-left));
    float ty=-((top+bottom)/(top-bottom));
	float tz=-((_far+_near)/(_far-_near));

	float col1_0 = static_cast<float>(2.0/(right-left));
	float col2_1 = static_cast<float>(2.0/(top-bottom));
	float col3_2 = static_cast<float>((-2.0)/(_far-_near));

    float m[16]={
        col1_0,0,0,0,//col 1
        0,col2_1,0,0,//col 2
        0,0,col3_2,0,//col 3
        tx,ty,tz,1//col 4
    };
    return FKCW_3D_Matrix4(m);
}
//--------------------------------------------------------------------
FKCW_3D_Vector4 convertFromViewportSpaceToWorldSpace(const FKCW_3D_Vector4&winPos,
                                                 const FKCW_3D_Matrix4&projMat,
                                                 const FKCW_3D_Matrix4&viewMat,
                                                 const float viewport[4])
{
    float viewportX=viewport[0];
    float viewportY=viewport[1];
    float viewportW=viewport[2];
    float viewportH=viewport[3];
    
    FKCW_3D_Vector4 t_winPos=winPos;
    // Map x and y from window coordinates
    t_winPos.setx((t_winPos.x()-viewportX)/viewportW);
    t_winPos.sety((t_winPos.y()-viewportY)/viewportH);
    // Map to range -1 to 1
    t_winPos.setx(t_winPos.x()*2-1);
    t_winPos.sety(t_winPos.y()*2-1);
    t_winPos.setz(t_winPos.z()*2-1);
    FKCW_3D_Matrix4 projMatInverse=inverse(projMat);
    FKCW_3D_Matrix4 viewMatInverse=inverse(viewMat);
    FKCW_3D_Vector4 eyeSpacePos=projMatInverse*t_winPos;
    FKCW_3D_Vector4 worldPos=viewMatInverse*eyeSpacePos;
    if (worldPos.w() == 0.0){
        assert(false);
    }
    worldPos.setx(worldPos.x()/worldPos.w());
    worldPos.sety(worldPos.y()/worldPos.w());
    worldPos.setz(worldPos.z()/worldPos.w());
    worldPos.setw(1);
    return worldPos;
}
//--------------------------------------------------------------------
FKCW_3D_Vector4 getIntersectPointOfLine3dIntersectWithPlane3d(const FKCW_3D_Vector4&start,const FKCW_3D_Vector4&dir,
                                  const FKCW_3D_Vector4&p,const FKCW_3D_Vector4&norm)
//dir not need to be normalized
//norm not need to be normalized
{
    //line3d:
    //               x-startx=dirx*t
    //               y-starty=diry*t
    //               z-startz=dirz*t
    //plan3d:
    //              normx*(x-px)+normy*(y-py)+normz*(z-pz)=0
    //solve:
    //              t=-B/A
    //              where A=normx*dirx+normy*diry+normz*dirz
    //                           B=normx*(startx-px)+normy*(starty-py)+normz*(startz-pz)
    float A=norm.x()*dir.x()+norm.y()*dir.y()+norm.z()*dir.z();
    float B=norm.x()*(start.x()-p.x())+norm.y()*(start.y()-p.y())+norm.z()*(start.z()-p.z());
    float t=-B/A;
    float x=dir.x()*t+start.x();
    float y=dir.y()*t+start.y();
    float z=dir.z()*t+start.z();
    return FKCW_3D_Vector4(x,y,z,1);
}
//--------------------------------------------------------------------
bool isLine3dIntersectWithTriangle3d(const FKCW_3D_Vector4&start,const FKCW_3D_Vector4&dir,
                                     const FKCW_3D_Vector4&p0,const FKCW_3D_Vector4&p1,const FKCW_3D_Vector4&p2)
{

    FKCW_3D_Vector4 triNormNE=cross(p1-p0, p2-p1);//normNE is not normalized normal
    FKCW_3D_Vector4 intersectPointOfLine3dAndTri3d=getIntersectPointOfLine3dIntersectWithPlane3d(start, dir, p0, triNormNE);
    FKCW_3D_Vector4&p=intersectPointOfLine3dAndTri3d;
    //if p is in triangle p0p1p2
    FKCW_3D_Vector4 crossP0P1_P0P=cross(p1-p0, p-p0);
    FKCW_3D_Vector4 crossP1P2_P1P=cross(p2-p1, p-p1);
    FKCW_3D_Vector4 crossP2P0_P2P=cross(p0-p2, p-p2);
    float dot_crossP0P1_P0P_triNormNE=dot(crossP0P1_P0P, triNormNE);
    float dot_crossP1P2_P1P_triNormNE=dot(crossP1P2_P1P, triNormNE);
    float dot_crossP2P0_P2P_triNormNE=dot(crossP2P0_P2P, triNormNE);
    
    if(dot_crossP0P1_P0P_triNormNE>0&&dot_crossP1P2_P1P_triNormNE>0&&dot_crossP2P0_P2P_triNormNE>0){
        return true;
    }else{
        return false;
    }
}
//--------------------------------------------------------------------