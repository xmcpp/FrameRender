#ifndef _RENDERINPUT_H_
#define _RENDERINPUT_H_

/*
��ģ���൱���Դ������

*/

class RenderInput
{
public:
	RenderInput();
public:
	void createVertexBuff( long vertexCount );
	void createIndexBuff( long indexCount );
	void addVertex( const VERTEX_TYPE & vertex );
	void addIndex( long p1 , long p2 , long p3 );
	
	void clearData();

	VERTEX_TYPE_PTR getVertexData(){ return m_vertexData; }
	VERTEX_TYPE_TRANSED_PTR getTransedVertexData(){ return m_vertexTransed; }
    FACE_TYPE_PTR getFaceData(){ return m_faceData; }
    long getFaceCount(){ return m_ibIndex / 3; }

	long getVertexCount(){ return m_vbIndex; }
	long getIndexCount(){ return m_ibIndex; }
private:
	VERTEX_TYPE_PTR m_vertexData; //��¼�任ǰ��������
	VERTEX_TYPE_TRANSED_PTR m_vertexTransed; //��¼�Ѿ��任֮��Ķ���
    FACE_TYPE_PTR m_faceData; //��¼����Ϣ
	
	long m_vbIndex; //��������
	long m_ibIndex; //index����
    long m_fIndex; //���������
};

#endif //_RENDERINPUT_H_

