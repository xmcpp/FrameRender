#include "stdafx.h"
#include "RenderObject.h"
#include "RenderInput.h"

RenderObject::RenderObject()
:m_inputObj(NULL)
{
	MatrixTool::initMatrix4X4( &m_worldMat );
    m_inputObj = new RenderInput();
}

RenderObject::~RenderObject()
{
    if( m_inputObj )
    {
        delete m_inputObj;
        m_inputObj = NULL;
    }
}

bool RenderObject::manualCreateObject( long vertexCount , long indexCount )
{
    m_inputObj->createVertexBuff( vertexCount );
    m_inputObj->createIndexBuff( indexCount );

    return true;
}

void RenderObject::addVertex( const VERTEX_TYPE & vertex )
{
	m_inputObj->addVertex( vertex );
}

void RenderObject::addFaceIndex( long p1 , long p2 , long p3 )
{
	m_inputObj->addIndex( p1 , p2 , p3 );
}

void RenderObject::setPosition( const Vector3 & pos )
{
	m_position = pos;
	
	m_worldMat.a_30 = m_position.x;
	m_worldMat.a_31 = m_position.y;
	m_worldMat.a_32 = m_position.z;
}

long RenderObject::getVertexCount()
{
	return m_inputObj->getVertexCount();
}

long RenderObject::getIndexCount()
{
	return m_inputObj->getIndexCount();
}

const MATRIX4X4 & RenderObject::getWorldMatrix()
{
	return m_worldMat;
}

bool RenderObject::loadObjectFromFile( const std::string & fileName )
{
    return true;
}