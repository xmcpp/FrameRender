#include "stdafx.h"
#include "Camera.h"
#include "BaseDefine.h"

Camera::Camera( float nearClipPlane , float farClipPlane , float fovX )
:m_near(nearClipPlane),m_far(farClipPlane),m_fovX(fovX),m_theta(0.0f),m_phi(0.0f)
{
    m_prjWidth = 2.0f;              //ͶӰƽ����Ϊ2.0f����֤��ȷ�Χ��-1.0f��+1.0f֮��
    m_prjHeight = 2.0f ;   //����ͶӰƽ��߶ȣ�������1��1��߶�������ͬ
    m_pos = Vector3::ZERO;
    m_target = Vector3::UNIT_Z;
    m_orient = Vector3::UNIT_Z;

    updateProjectDistance();
    createPrjMat();
}

void Camera::setPosition( const Vector3 & pos )
{ 
    m_pos = pos;
    m_orient = m_target - m_pos; //���³���
    m_orient.normalize();
}

void Camera::lookAt( const Vector3 & target )
{ 
    m_target = target;
    m_orient = m_target - m_pos;
    m_orient.normalize();
}

void Camera::yaw( float rad )
{
    //������ת��������
    //MatrixTool::vectorRotateY( m_orient , rad );
    
    m_theta += rad;
    //if( m_theta < 0.0f )
    //    m_theta = 2*FR_PI + m_theta;
    //else if( m_theta > 2*FR_PI )
    //    m_theta = m_theta - 2*FR_PI;

    //����target��
    updateTargetPosition();
}

void Camera::pitch( float rad )
{
    //������ת��������
    //MatrixTool::vectorRotateZ( m_orient , rad );
    m_phi += rad;

    //����target��
    updateTargetPosition();
}

void Camera::roll( float rad )
{

}

Vector3 Camera::getOrientation()
{
    m_orient = m_target - m_pos;
    m_orient.normalize();
    return m_orient;
}

void Camera::setClipPlane( float nearClipPlane , float farClipPlane )
{ 
	m_near = nearClipPlane;
	m_far = farClipPlane;
}

void Camera::setFovX( float fov )
{
    m_fovX = fov;

    updateProjectDistance();
    createPrjMat();
}

void Camera::updateProjectDistance()
{
    m_d = ( 0.5f * m_prjWidth ) / tan( 0.5f * m_fovX ); //�����ӵ㵽ͶӰƽ��ľ���
}

void Camera::updateTargetPosition()
{
    m_target.x = cos( m_phi ) * sin( m_theta );
    m_target.y = sin( m_phi );
    m_target.z = cos(m_phi) * cos( m_theta );
    //m_target.normalize();
    m_target = m_pos + m_target ;
    
}

const MATRIX4X4 & Camera::getViewMat()
{
    Vector3 n = m_target - m_pos;
    n.normalize();
    Vector3 up( 0.0f , 1.0f , 0.0f );
    Vector3 u = up.crossProduct( n );
    Vector3 v = n.crossProduct( u );
    
    u.normalize();
    v.normalize();
    n.normalize();

    MatrixTool::initMatrix4X4( &m_viewMat );
    m_viewMat.a_00 = u.x;
    m_viewMat.a_01 = v.x;
    m_viewMat.a_02 = n.x;
    
    m_viewMat.a_10 = u.y;
    m_viewMat.a_11 = v.y;
    m_viewMat.a_12 = n.y;

    m_viewMat.a_20 = u.z;
    m_viewMat.a_21 = v.z;
    m_viewMat.a_22 = n.z;
    
    //�мɲ���ֱ�ӽ�posֱ�������ڸþ����������֣���Ϊ����ƽ�ƺ���ת�����ֱ������
    //����������ת��ƽ���ˡ�
    //��Ҫ��ƽ�ƾ�������ת������ˣ���Ϊ�����Ч�ʣ����ֶ����¾��󣬲�ʹ�þ������
    m_viewMat.a_30 = -m_pos.x * m_viewMat.a_00 -m_pos.y * m_viewMat.a_10 - m_pos.z * m_viewMat.a_20;
    m_viewMat.a_31 = -m_pos.x * m_viewMat.a_01 -m_pos.y * m_viewMat.a_11 - m_pos.z * m_viewMat.a_21;
    m_viewMat.a_32 = -m_pos.x * m_viewMat.a_02 -m_pos.y * m_viewMat.a_12 - m_pos.z * m_viewMat.a_22;


	return m_viewMat;
}

const MATRIX4X4 & Camera::getProjectMat()
{
	return m_projectMat;
}

void Camera::createPrjMat()
{
    //xp = xw * d / zw
    //yp = yw * d / zw

    MatrixTool::initMatrix4X4( &m_projectMat );
    m_projectMat.a_23 = 1.0f / m_d;
}