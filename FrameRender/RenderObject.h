#ifndef _RENDER_OBJECT_
#define _RENDER_OBJECT_

class RenderInput;
class RenderObject
{
public:
	RenderObject();
	~RenderObject();
public:
    /**
    ���ļ���ȡ���ݲ�������Ⱦ��������
    */
    bool loadObjectFromFile( const std::string & fileName );

    /**
    �ֶ���������
    @param vertextCount ������Ŀ
    @param indexCount ������Ŀ
    */
    bool manualCreateObject( long vertexCount , long indexCount );
public:
	void addVertex( const VERTEX_TYPE & vertex );
	void addFaceIndex( long p1 , long p2 , long p3 );

	void setPosition( const Vector3 & pos );

	long getVertexCount();
	long getIndexCount();

	const MATRIX4X4 & getWorldMatrix();

    //���ڹ���Ӧ�ò�������Դ������ݵĹ���
    RenderInput * getBindRenderInput() { return m_inputObj; }
private:
	Vector3 m_position;
    RenderInput * m_inputObj; //Ӳ��������ݶ���
	MATRIX4X4 m_worldMat;
};

#endif //_RENDER_OBJECT_
