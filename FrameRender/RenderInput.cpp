#include "stdafx.h"
#include "RenderInput.h"

RenderInput::RenderInput()
:m_ibIndex(0),m_vbIndex(0),m_fIndex(0),m_vertexData(NULL) , m_vertexTransed(NULL),m_faceData(NULL)
{

}

void RenderInput::createVertexBuff( long vertexCount )
{
	m_vertexData = new VERTEX_TYPE[vertexCount];
	m_vertexTransed = new VERTEX_TYPE_TRANSED[vertexCount];
}

void RenderInput::createIndexBuff( long indexCount )
{
	m_faceData = new FACE_TYPE[indexCount / 3];
    m_ibIndex = indexCount;
}

void RenderInput::addVertex( const VERTEX_TYPE & point )
{
	memcpy( m_vertexData + ( m_vbIndex++ ) ,  &point , sizeof(VERTEX_TYPE) );
}

void RenderInput::addIndex( long p1 , long p2 , long p3 )
{
	FACE_TYPE_PTR type = m_faceData + ( m_fIndex++ ) ; //计算当前的面结构地址
    type->i1 = p1;
    type->i2 = p2;
    type->i3 = p3;
    type->state = 0;
    
}

void RenderInput::clearData()
{
	m_vbIndex = 0;
	m_ibIndex = 0;
    m_fIndex = 0;
	delete []m_vertexData;
	delete []m_vertexTransed;
    delete []m_faceData;
}