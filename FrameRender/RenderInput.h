#ifndef _RENDERINPUT_H_
#define _RENDERINPUT_H_

/*
该模块相当于显存的内容

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
	VERTEX_TYPE_PTR m_vertexData; //记录变换前顶点数据
	VERTEX_TYPE_TRANSED_PTR m_vertexTransed; //记录已经变换之后的顶点
    FACE_TYPE_PTR m_faceData; //记录面信息
	
	long m_vbIndex; //顶点索引
	long m_ibIndex; //index索引
    long m_fIndex; //面插入索引
};

#endif //_RENDERINPUT_H_

