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
    
    //���������ת����,��λ����
    void yaw( float rad );
    void pitch( float rad );
    void roll( float rad );

    //��λΪ����
	void setFovX( float fov );
	
    //���������ĳ���
    Vector3 getOrientation();

    const MATRIX4X4 & getViewMat();
	const MATRIX4X4 & getProjectMat();

private:
    void createPrjMat(); //���ڴ���ͶӰ����
    void updateProjectDistance();//���ڸ���ͶӰ����
    void updateTargetPosition();//���ڸ���������Ĺ۲�Ŀ���
private:
	MATRIX4X4 m_viewMat;
	MATRIX4X4 m_projectMat;

	Vector3 m_pos;      //�������λ��
	Vector3 m_target;   //������Ĺ�עĿ���
    Vector3 m_orient;   //������ĳ���
    
    float m_theta; //�����ת�ķ�λ��
    float m_phi; //�����ת������

	float m_fovX;   //x������Ұ�Ƕȣ���λΪ����
	float m_near;   //���ü�����
	float m_far;    //Զ�ü�����
    float m_prjWidth;   //ͶӰ����
    float m_prjHeight;  //ͶӰ��߶�
    float m_d;          //ͶӰ�浽�ӵ�ľ���
};

#endif //_CAMERA_H_
