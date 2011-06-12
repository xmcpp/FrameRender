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
    从文件读取数据并构造渲染对象数据
    */
    bool loadObjectFromFile( const std::string & fileName );

    /**
    手动创建对象
    @param vertextCount 顶点数目
    @param indexCount 索引数目
    */
    bool manualCreateObject( long vertexCount , long indexCount );
public:
	void addVertex( const VERTEX_TYPE & vertex );
	void addFaceIndex( long p1 , long p2 , long p3 );

	void setPosition( const Vector3 & pos );

	long getVertexCount();
	long getIndexCount();

	const MATRIX4X4 & getWorldMatrix();

    //用于关联应用层对象与显存中数据的关联
    RenderInput * getBindRenderInput() { return m_inputObj; }
private:
	Vector3 m_position;
    RenderInput * m_inputObj; //硬件层的数据对象
	MATRIX4X4 m_worldMat;
};

#endif //_RENDER_OBJECT_
