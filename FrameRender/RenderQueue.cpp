#include "stdafx.h"
#include "RenderQueue.h"
#include "RenderInput.h"
#include "RenderTarget.h"

RenderQueue::RenderQueue( HWND hwnd , int width , int height , COLOR_TYPE type )
:m_mainHwnd(hwnd),m_enableDrawFrame( false ),m_cullingType(CCW),m_fillType(FILL_SOLIDE)
{
    m_backTarget = new RenderTarget();
    m_frontTarget = new RenderTarget();

    //����ǰ����Ⱦ����
    m_backTarget->createRenderTarget( width , height , type );
    m_frontTarget->createRenderTarget( width , height , type );

}

RenderQueue::~RenderQueue()
{
    delete m_backTarget;
    delete m_frontTarget;

    m_backTarget = NULL;
    m_frontTarget = NULL;
}

void RenderQueue::beginScene()
{
    m_backTarget->reset();
}

void RenderQueue::endScene()
{

}

void RenderQueue::drawRenderInput( RenderInput * input )
{
	m_inputData = input;
    VERTEX_TYPE_PTR vPtr = m_inputData->getVertexData();
    VERTEX_TYPE_TRANSED_PTR vPtrTransed = m_inputData->getTransedVertexData();
    long count = m_inputData->getIndexCount();

    //�ֲ�����ת��������
	transVertexLocalToWorld( &m_worldMat , input );
	
    //��������ת���������
    transVertexWorldToView( &m_viewMat , input );
	
    //���������תͶӰ����
    transVertexViewToPrj( &m_projectMat , input );
    
    //��ü�
    faceCulling();

    //�任���ݱ任���󣬱任����uv����
    transUV( NULL, input );

    //ͶӰ����ת��Ļ����
    processScreenMap();
    
    //դ��
    rasterization();
}

void RenderQueue::transVertexLocalToWorld( MATRIX4X4_PTR mat , RenderInput * input )
{
    VERTEX_TYPE_PTR vPtr = input->getVertexData();
    VERTEX_TYPE_TRANSED_PTR vPtrTransed = input->getTransedVertexData();
    long count = input->getVertexCount();
    for( long i = 0 ; i < count ; i++ )
	{
        MatrixTool::vMulMatrix4X4( mat , &( (vPtr + i)->x ) , &( (vPtrTransed + i)->x ) , 4 );
        
	}

}

//��������ת���������
void RenderQueue::transVertexWorldToView( MATRIX4X4_PTR mat , RenderInput * input )
{
    VERTEX_TYPE_TRANSED_PTR vPtrTransed = input->getTransedVertexData();
    long count = input->getVertexCount();
    for( long i = 0 ; i < count ; i++ )
    {
        MatrixTool::vMulMatrix4X4( mat , &( (vPtrTransed + i)->x ) , &( (vPtrTransed + i)->x ) , 4 );
    }
}

//���������תͶӰ����
void RenderQueue::transVertexViewToPrj( MATRIX4X4_PTR mat , RenderInput * input )
{
    transVertexWorldToView( mat , input );
    
    VERTEX_TYPE_TRANSED_PTR vPtrTransed = input->getTransedVertexData();
    //��һ���������,���ͶӰ���㣬ʹ����ֵ������ͶӰƽ����
    for( long i = 0 ; i < m_inputData->getVertexCount() ; i++ )
    {
        vPtrTransed->x = vPtrTransed->x / vPtrTransed->w;
        vPtrTransed->y = vPtrTransed->y / vPtrTransed->w;
        vPtrTransed++;
    }
}

void RenderQueue::transUV( MATRIX4X4_PTR mat , RenderInput * input )
{
    VERTEX_TYPE_TRANSED_PTR vPtrTransed = input->getTransedVertexData();
    VERTEX_TYPE_PTR vPtr = input->getVertexData();

    //��ʱ���������ɫֵ������ȥ��������uv�ı任
    long count = input->getVertexCount();
    for( long i = 0 ; i < count ; i++ )
    {
        (vPtrTransed + i)->r = (vPtr + i)->r;
        (vPtrTransed + i)->g = (vPtr + i)->g;
        (vPtrTransed + i)->b = (vPtr + i)->b;
        (vPtrTransed + i)->a = (vPtr + i)->a;
    }
}

//��ü�
void RenderQueue::faceCulling()
{
    if( m_cullingType == NONE ) return;

    FACE_TYPE_PTR pCurFace = NULL;
    FACE_TYPE_PTR pFace = m_inputData->getFaceData();
    VERTEX_TYPE_TRANSED_PTR pPtrTransed = m_inputData->getTransedVertexData();
    VERTEX_TYPE_TRANSED_PTR v[3];
    int maxY = 0;

    long count = m_inputData->getFaceCount();
    for( long i = 0 ; i<count ; i++ )
    {
        pCurFace = pFace + i;
                
        v[0] = pPtrTransed + pCurFace->i1;
        v[1] = pPtrTransed + pCurFace->i2;
        v[2] = pPtrTransed + pCurFace->i3;
        
        /*
        //�жϳ�y���ĵ������
        if( v[0]->y < v[1]->y )
        {
            if( v[1]->y < v[2]->y )
                maxY = 2;
            else
                maxY = 1;
        }
        else
        {
            if( v[0]->y < v[2]->y )
                maxY = 2;
        }
        
        pCurFace->state = 0; //���Ȼ�ԭ��״̬������һ�����޳������޷��ָ���

        if( v[(maxY+1)%3]->x - v[(maxY+2)%3]->x > 0  )
        {
            //��ʾ�������˳ʱ����ʾ��,����˳ʱ��ü�
            if( m_cullingType == CW )
                pCurFace->state = 1;
        }
        else
        {
            //��ʾ���������ʱ����ʾ��,������ʱ��ü�
            if( m_cullingType == CCW )
                pCurFace->state = 1;
        }
        */
        //��ʽ(x1-x0)*(y2-y0)-(x2-x0)*(y1-y0) ���>0��ʱ�룬��֮˳ʱ��
        
        pCurFace->state = 0; //���Ȼ�ԭ��״̬������һ�����޳������޷��ָ���

        float val = ( v[1]->x - v[0]->x ) * ( v[2]->y - v[0]->y ) - ( v[2]->x - v[0]->x ) * ( v[1]->y - v[0]->y );
        if( val < 0 )
        {
            //��ʾ�������˳ʱ����ʾ��,����˳ʱ��ü�
            if( m_cullingType == CW )
                pCurFace->state = 1;
        }
        else
        {
            //��ʾ���������ʱ����ʾ��,������ʱ��ü�
            if( m_cullingType == CCW )
                pCurFace->state = 1;
        }
        
    }
}

void RenderQueue::processScreenMap()
{
    VERTEX_TYPE_TRANSED_PTR vPtrTransed = m_inputData->getTransedVertexData();
    int width = m_backTarget->getBufferWidth();
    int height = m_backTarget->getBufferHeight();

    for( long i = 0 ; i < m_inputData->getVertexCount() ; i++ )
    {
        vPtrTransed->x = (int) ( 0.5 * ( width - 1 ) * ( vPtrTransed->x + 1 ) );
        vPtrTransed->y = ( height - 1 ) - (int)( 0.5 * ( height - 1 ) * ( vPtrTransed->y + (float)height / (float)width ) );
        vPtrTransed++;
    }
}

//դ��
void RenderQueue::rasterization()
{
    FACE_TYPE_PTR pCurFace = NULL;
    FACE_TYPE_PTR pFace = m_inputData->getFaceData();
    long count = m_inputData->getFaceCount();

    for( long i = 0 ; i<count ; i++ )
    {
        pCurFace = pFace + i;
        //ȷ������������״̬�������դ�񻯴���
        if( pCurFace->state != 0 )
            continue;

        //���������
        if( m_fillType == FILL_SOLIDE )
        {
            fillTriangle( pCurFace );
        }        

        //���������α߿�
        if( m_enableDrawFrame || m_fillType == FILL_WIREFRAME )
            drawFrame( pCurFace );

    }

    
}

//���������ؼ�Ҫ��֤���������㰴��yֵ���������ٰ�xֵ���������Ի����ȷ��v0��v1��v2�ļ���˳��
void RenderQueue::fillTriangle( FACE_TYPE_PTR pFace )
{
    VERTEX_TYPE_TRANSED_PTR pPtrTransed = m_inputData->getTransedVertexData();
    VERTEX_TYPE_TRANSED_PTR v0,v1,v2;
    v0 = pPtrTransed + pFace->i1;
    v1 = pPtrTransed + pFace->i2;
    v2 = pPtrTransed + pFace->i3;
    
    VERTEX_TYPE_TRANSED_PTR tmp; 
    if( v1->y < v0->y )
    {
        //������0�͵�1
        tmp = v0;
        v0 = v1;
        v1 = tmp;
    }

    if( v2->y < v0->y )
    {

        //������0�͵�2
        tmp = v0;
        v0 = v2;
        v2 = tmp;
    }

    if( v2->y < v1->y )
    {
        //������1�͵�2
        tmp = v1;
        v1 = v2;
        v2 = tmp;
    }
    
    //ȷ�������͵�����
    if( F_CMP( v0->y , v1->y ) )
    {
        //����ƽ��������
        fillTopFlatTriangle( v0 , v1 , v2 );
    }
    else if( F_CMP( v1->y , v2->y ) )
    {
        //����ƽ��������
        fillBottomFlatTriangle( v0 , v1 , v2 );
    }
    else
    {
        //����ͨ�����Σ���Ҫ���
        VERTEX_TYPE_TRANSED newV;
        newV.state = 0;
        float deltaY = ( v0->y - v2->y );
        float k = ( v0->x - v2->x ) / deltaY;
        newV.x = v0->x + k * ( v1->y - v0->y );
        newV.y = v1->y;
        
        Vector3 colorK = Vector3::ZERO; //��ɫб��
        colorK.x = ( v0->r - v2->r ) /deltaY;
        colorK.y = ( v0->g - v2->g ) /deltaY;
        colorK.z = ( v0->b - v2->b ) /deltaY;
        newV.r = v0->r + colorK.x * ( v1->r - v0->r );
        newV.g = v0->g + colorK.x * ( v1->g - v0->g );
        newV.b = v0->b + colorK.x * ( v1->b - v0->b );

        fillBottomFlatTriangle( v0 , v1 , &newV );
        fillTopFlatTriangle( &newV , v1 , v2 );
        
    }

}

//���ƽ��������
void RenderQueue::fillTopFlatTriangle( VERTEX_TYPE_TRANSED_PTR v0 , VERTEX_TYPE_TRANSED_PTR v1 ,VERTEX_TYPE_TRANSED_PTR v2 )
{
    int width = m_backTarget->getBufferWidth();
    int height = m_backTarget->getBufferHeight();

    float deltaY = v0->y - v2->y ;
    int yStart = ceil( v0->y );
    int yEnd = ceil( v2->y );
    
    if( v1->x < v0->x )
    {
        //��������б��
        VERTEX_TYPE_TRANSED_PTR tmp = v0; v0 = v1; v1 = tmp;
    }

    float leftK = 0.0f , rightK = 0.0f;
    
    Vector3 leftColorK = Vector3::ZERO; //����ɫб��
    Vector3 rightColorK = Vector3::ZERO; //����ɫб��
    
    //������ɫб��
    leftColorK.x = ( v0->r - v2->r ) /deltaY;
    leftColorK.y = ( v0->g - v2->g ) /deltaY;
    leftColorK.z = ( v0->b - v2->b ) /deltaY;

    rightColorK.x = ( v1->r - v2->r ) /deltaY;
    rightColorK.y = ( v1->g - v2->g ) /deltaY;
    rightColorK.z = ( v1->b - v2->b ) /deltaY;

    //��������б�ʵĵ���
    leftK = ( v0->x - v2->x ) / deltaY;
    rightK = ( v1->x - v2->x ) / deltaY;
    
    Vector3 colorL( v0->r , v0->g , v0->b ); //ɨ������˵���ɫ
    Vector3 colorR( v1->r , v1->g , v1->b ); //ɨ�����Ҷ˵���ɫ


    float xl = v0->x + ( yStart - v0->y ) * leftK ;
    float xr = v1->x + ( yStart - v0->y ) * rightK ;
    int l,r;
    Vector3 cl , cr;
    VERTEX_TYPE_TRANSED tv0 , tv1;
    for( int y=yStart ; y<yEnd ; y++ )
    {
        if( y > height - 1 ) break; //����ü�������

        if( y >= 0 )         
        {
            l = ceil(xl);
            r = ceil(xr) - 1;
            cl = colorL;
            cr = colorR;

            if( r >= l )
            {
                if(!( ( l < 0 && r < 0 ) || ( l > width -1 && r > width -1  ) ) )
                {
                    if( l < 0 )
                    {
                        cl = cl + ( colorR - colorL ) * abs( (float)l / (float)( r - l ) );
                        l = 0;
                    }
                    else if( r > width - 1 )
                    {
                        cr = cl + ( colorR - colorL ) * ( (float)( width - 1 - l ) / (float)( r - l ) ) ;
                        r = width - 1;
                    }
                    tv0.sy = tv1.sy = y;
                    tv0.sx = l;
                    tv1.sx = r;

                    tv0.r = cl.x;
                    tv0.g = cl.y;
                    tv0.b = cl.z;

                    tv1.r = cr.x;
                    tv1.g = cr.y;
                    tv1.b = cr.z;
                    drawScanLine( &tv0 , &tv1 );
                }
            }
        }
        
        xl += leftK ;
        xr += rightK ;

        colorL += leftColorK;
        colorR += rightColorK;
    }
}

//���ƽ��������
void RenderQueue::fillBottomFlatTriangle( VERTEX_TYPE_TRANSED_PTR v0 , VERTEX_TYPE_TRANSED_PTR v1 ,VERTEX_TYPE_TRANSED_PTR v2 )
{
    int width = m_backTarget->getBufferWidth();
    int height = m_backTarget->getBufferHeight();

    float deltaY = v0->y - v1->y ;
    int yStart = ceil( v0->y );
    int yEnd = ceil( v1->y );

    if( v1->x < v2->x )
    {
        //��������б��
        VERTEX_TYPE_TRANSED_PTR tmp = v1; v1 = v2; v2 = tmp;
    }
    
    float leftK = 0.0f , rightK = 0.0f; //����б��

    Vector3 leftColorK = Vector3::ZERO; //����ɫб��
    Vector3 rightColorK = Vector3::ZERO; //����ɫб��

    //��������б�ʵĵ���
    leftK = ( v0->x - v2->x ) / deltaY;
    rightK = ( v0->x - v1->x ) / deltaY;
    
    //������ɫб��
    leftColorK.x = ( v0->r - v2->r ) /deltaY;
    leftColorK.y = ( v0->g - v2->g ) /deltaY;
    leftColorK.z = ( v0->b - v2->b ) /deltaY;
    
    rightColorK.x = ( v0->r - v1->r ) /deltaY;
    rightColorK.y = ( v0->g - v1->g ) /deltaY;
    rightColorK.z = ( v0->b - v1->b ) /deltaY;

    float xl = v0->x + ( yStart - v0->y ) * leftK ;
    float xr = xl;
    Vector3 colorL( v0->r , v0->g , v0->b ); //ɨ������˵���ɫ
    Vector3 colorR = colorL; //ɨ�����Ҷ˵���ɫ

    int l,r;
    Vector3 cl , cr;

    VERTEX_TYPE_TRANSED tv0 , tv1;
    for( int y=yStart ; y<yEnd ; y++ )
    {
        if( y > height - 1 ) break; //����ü�������

        if( y >= 0 )         
        {
            l = ceil(xl);
            r = ceil(xr) - 1;
            cl = colorL;
            cr = colorR;

            if( r >= l )
            {
                //����ü���Ϊ���¼������������
                //1. ��������������㶼С��0����ڿ������ȫ����
                //2.�����ߵĶ���С��0�����ұߵĴ��ڿ����ü�
                //3.��������㶼��0�Ϳ��֮�䣬�򲻲ü�
                if(!( ( l < 0 && r < 0 ) || ( l > width -1 && r > width -1  ) ) )
                {
                    if( l < 0 )
                    {
                        cl = cl + ( colorR - colorL ) * abs( (float)l / (float)( r - l ) );
                        l = 0;

                    }
                    else if( r > width - 1 )
                    {
                        cr = cl + ( colorR - colorL ) * ( (float)( width - 1 - l ) / (float)( r - l ) ) ;
                        r = width - 1;
                    }
                    tv0.sy = tv1.sy = y;
                    tv0.sx = l;
                    tv1.sx = r;
                    tv0.r = cl.x;
                    tv0.g = cl.y;
                    tv0.b = cl.z;

                    tv1.r = cr.x;
                    tv1.g = cr.y;
                    tv1.b = cr.z;
                    drawScanLine( &tv0 , &tv1 );
                }
                
            }
        }
        xl += leftK ;
        xr += rightK ;

        colorL += leftColorK;
        colorR += rightColorK;
        
    }
}

//����ɨ����
void RenderQueue::drawScanLine( VERTEX_TYPE_TRANSED_PTR v0 , VERTEX_TYPE_TRANSED_PTR v1 )
{
    
    float rk = ( v0->r - v1->r ) / ( v0->sx - v1->sx );
    float gk = ( v0->g - v1->g ) / ( v0->sx - v1->sx );
    float bk = ( v0->b - v1->b ) / ( v0->sx - v1->sx );

    int pitch = 0 , tmp = 0 ;
    float r = v0->r , g = v0->g , b = v0->b;
    unsigned char * pData = m_backTarget->lockBuffer( pitch );
    COLOR_TYPE type = m_backTarget->getBufferColorType();
    ColorValue color( 1.0f , 1.0f , 1.0f );
    for( int x = v0->sx ; x <= v1->sx ; x++ )
    {
        color.setRGB( r , g , b );
        tmp = v0->sy * pitch + x*type;
        *((int*)(pData + tmp)) = color.getRGBA();

        r += rk; g += gk ; b += bk;
    }
    
}

void RenderQueue::drawFrame( FACE_TYPE_PTR pFace )
{
    VERTEX_TYPE_TRANSED_PTR pPtrTransed = m_inputData->getTransedVertexData();
    

    drawDDALine( ( pPtrTransed + pFace->i1 )->x , ( pPtrTransed + pFace->i1 )->y , ( pPtrTransed + pFace->i2 )->x , ( pPtrTransed + pFace->i2 )->y ,  ColorValue( 1.0f , 1.0f , 1.0f ) );
    drawDDALine( ( pPtrTransed + pFace->i2 )->x , ( pPtrTransed + pFace->i2 )->y , ( pPtrTransed + pFace->i3 )->x , ( pPtrTransed + pFace->i3 )->y ,  ColorValue( 1.0f , 1.0f , 1.0f ) );
    drawDDALine( ( pPtrTransed + pFace->i3 )->x , ( pPtrTransed + pFace->i3 )->y , ( pPtrTransed + pFace->i1 )->x , ( pPtrTransed + pFace->i1 )->y ,  ColorValue( 1.0f , 1.0f , 1.0f ) );
}

void RenderQueue::drawDDALine( float x1 , float y1 , float x2 , float y2 , ColorValue & val )
{
    int width = m_backTarget->getBufferWidth();
    int height = m_backTarget->getBufferHeight();
    COLOR_TYPE type = m_backTarget->getBufferColorType();
    int pitch = 0;
    unsigned char * pData = m_backTarget->lockBuffer( pitch );

    float  length , dx , dy ,x , y;
    
    if ( abs( x2 - x1 ) >= abs( y2 - y1 ) )
        length = abs( x2 - x1 );
    else 
        length = abs( y2 - y1 );
    dx = ((x2 - x1) / length);
    dy = ((y2 - y1) / length);

    x = x1 + 0.5f ;
    y = y1 + 0.5f ;
    int  i = 1 ;
    while ( i  <=  length )
    {
        //ִ�����زü�
        if( x >= 0 && x < width && y >= 0 && y < height )
        {
            int tmp = int(y)*pitch + int(x)*type;
            *((int*)(pData + tmp)) = val.getRGBA();
        }
        x = x + dx;
        y = y + dy;
        i ++ ;
     }
}

void RenderQueue::present()
{
    //���󻺳������ύ
    HDC hdc;
    hdc = GetDC(m_mainHwnd);

    int width , height , pitch;
    COLOR_TYPE type;
    
    width = m_backTarget->getBufferWidth();
    height = m_backTarget->getBufferHeight();
    type = m_backTarget->getBufferColorType();

    unsigned char * pData = m_backTarget->lockBuffer( pitch );
    for( int i = 0 ; i < width ; i++ )
    {
        for( int j = 0 ; j < height ; j++ )
        {
            int tmp = j*pitch + i*type;
            //��Ϊ��ɫRGBA���д洢������R�ڸ�λ��a�ڵ�λ������ȡֵʱҪע�ⷴ����
            SetPixel( hdc , i , j , RGB(pData[tmp+3] , pData[tmp+2] , pData[tmp+1]) );
        }
    }
    ReleaseDC( m_mainHwnd , hdc );

    //����������
    RenderTarget * tmp = m_backTarget;
    m_backTarget = m_frontTarget;
    m_frontTarget = tmp;

    Sleep(10);
}

void RenderQueue::setPixel( int x , int y , const ColorValue & val )
{
    int width , height;
    width = m_backTarget->getBufferWidth();
    height = m_backTarget->getBufferHeight();

    //ִ�����زü�
    if( x < 0 || x >= width )
        return;
    if( y < 0 || y >= height )
        return;


}
