#ifndef _CAMERA_H_
#define _CAMERA_H_

class Camera
{
public:
	Camera( float nearClipPlane , float farClipPlane , float fovX );

public:
	void setPosition( const Vector3 & pos );
    Vector3 & getPosition() { return m_pos; }
	void lookAt( const Vector3 & target );
	void setClipPlane( float nearClipPlane , float farClipPlane );
    
    //摄相机的旋转设置,单位弧度
    void yaw( float rad );
    void pitch( float rad );
    void roll( float rad );

    //单位为弧度
	void setFovX( float fov );
	
    //获得摄像机的朝向
    Vector3 getOrientation();

    const MATRIX4X4 & getViewMat();
	const MATRIX4X4 & getProjectMat();

private:
    void createPrjMat(); //用于创建投影矩阵
    void updateProjectDistance();//用于更新投影距离
    void updateTargetPosition();//用于更新摄相机的观察目标点
private:
	MATRIX4X4 m_viewMat;
	MATRIX4X4 m_projectMat;

	Vector3 m_pos;      //摄像机的位置
	Vector3 m_target;   //摄相机的关注目标点
    Vector3 m_orient;   //摄相机的朝向
    
    float m_theta; //相机旋转的方位角
    float m_phi; //相机旋转的仰角

	float m_fovX;   //x方向视野角度，单位为弧度
	float m_near;   //近裁减距离
	float m_far;    //远裁减距离
    float m_prjWidth;   //投影面宽度
    float m_prjHeight;  //投影面高度
    float m_d;          //投影面到视点的距离
};

#endif //_CAMERA_H_
