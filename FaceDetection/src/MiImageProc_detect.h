#ifndef __XIAOMI_IMAGEPROC_DETECT_H__
#define __XIAOMI_IMAGEPROC_DETECT_H__
#include "MiTypes_detect.h"
#include "MiTypes_c.h"

#define RGB565_MASK_RED        0xF800   
#define RGB565_MASK_GREEN      0x07E0   
#define RGB565_MASK_BLUE       0x001F 


#define PRECISION_LAND 16
static int  XmResize_ARGB(unsigned char *pSrc,
    int iWidth,int iHeight,int iWidthStep,
    unsigned char *pDst,
    int lNewWidth,int lNewHeight,int nchannels)
{
    int	i1,j1,i2,j2;
    int   i,j;
    unsigned char   f1,f2,f3,f4,f12,f34;
    int  scale_x, scale_y;
    int  x0,y0;

    //int iwidthStep = iWidth*nchannels;
    int lnewWidthStep = lNewWidth*nchannels;
    
    int argb=0;

    if (lNewWidth)
    {
        scale_x = (iWidth<<PRECISION_LAND)/lNewWidth;
    }
    if (lNewHeight)
    {
        scale_y = (iHeight<<PRECISION_LAND)/lNewHeight;
    }


    for(i = 0; i < lNewHeight; i++)
    {
        for(j = 0; j < lNewWidth; j++)
        {					
            y0 = i*scale_y ;
            x0 = j*scale_x ;

            j1 = x0>>PRECISION_LAND;
            j2 = j1 + 1;
            i1 = y0>>PRECISION_LAND;
            i2 = i1 + 1;
            if ((i2 >=iHeight)||(j2 >=iWidth))
            {
                i2--;
                j2--;
            }
            argb = nchannels == 4?1:0;

            for (;argb<nchannels;++argb)
            {
                f1 = pSrc[i1*iWidthStep+j1*nchannels + argb];
                f2 = pSrc[i1*iWidthStep+j2*nchannels + argb];
                f3 = pSrc[i2*iWidthStep+j1*nchannels + argb];
                f4 = pSrc[i2*iWidthStep+j2*nchannels + argb];


                f12 = ((x0 - (j1<<PRECISION_LAND))*f2 + ((j2<<PRECISION_LAND) - x0)*f1)>>PRECISION_LAND;
                f34 = ((x0 - (j1<<PRECISION_LAND))*f4 + ((j2<<PRECISION_LAND) - x0)*f3)>>PRECISION_LAND;

                pDst[i*lnewWidthStep+j*nchannels+ argb] = ((y0 - (i1<<PRECISION_LAND))*f34 + ((i2<<PRECISION_LAND) - y0)*f12)>>PRECISION_LAND;
            }

        }
    }

    return 0;	
}

static int  XmResize_8T8(unsigned char *pSrc,
    int iWidth,int iHeight,
    unsigned char *pDst,
    int lNewWidth,int lNewHeight)
{
    int	i1,j1,i2,j2;
    int   i,j;
    unsigned char   f1,f2,f3,f4,f12,f34;
    int  scale_x, scale_y;
    int  x0,y0;


    if (lNewWidth)
    {
        scale_x = (iWidth<<PRECISION_LAND)/lNewWidth;
    }
    if (lNewHeight)
    {
        scale_y = (iHeight<<PRECISION_LAND)/lNewHeight;
    }


    for(i = 0; i < lNewHeight; i++)
    {
        for(j = 0; j < lNewWidth; j++)
        {					
            y0 = i*scale_y ;
            x0 = j*scale_x ;

            j1 = x0>>PRECISION_LAND;
            j2 = j1 + 1;
            i1 = y0>>PRECISION_LAND;
            i2 = i1 + 1;
            if ((i2 >=iHeight)||(j2 >=iWidth))
            {
                i2--;
                j2--;
            }

            f1 = pSrc[i1*iWidth+j1];
            f2 = pSrc[i1*iWidth+j2];
            f3 = pSrc[i2*iWidth+j1];
            f4 = pSrc[i2*iWidth+j2];


            f12 = ((x0 - (j1<<PRECISION_LAND))*f2 + ((j2<<PRECISION_LAND) - x0)*f1)>>PRECISION_LAND;
            f34 = ((x0 - (j1<<PRECISION_LAND))*f4 + ((j2<<PRECISION_LAND) - x0)*f3)>>PRECISION_LAND;

            pDst[i*lNewWidth+j] = ((y0 - (i1<<PRECISION_LAND))*f34 + ((i2<<PRECISION_LAND) - y0)*f12)>>PRECISION_LAND;


        }
    }

    return 0;	
}

void rgbToGray( unsigned char *rgbImg,unsigned char *grayImg,int channels,int m_width,int m_height)
{
    if (rgbImg ==NULL || grayImg ==NULL)
    {
        return;
    }
    int R,G,B;
    unsigned char *gray=grayImg;
    unsigned char *rgb=rgbImg;
    int step=channels;
    int i,j;
    int r1,g1,b1;
    if (channels == 1)
    {
        memcpy(grayImg,rgbImg,m_width*m_height*sizeof(unsigned char));
        return;
    }

    else if (channels == 3)
    {
        r1 = 2;
        b1=0;
        g1=1;
    }
    else if (channels == 4)
    {
        r1=1;
        b1=3;
        g1=2;

    }
   if(channels != 2)
   {
       for (j=0;j<m_height*m_width;++j)
       {
           B=*(rgb+b1);
           G=*(rgb+g1);
           R=*(rgb+r1);
           *gray =(unsigned char )( ( 19595  * R + 38469  * G + 7472 * B)>>16 );
           rgb+=step;
           gray++;
       }
   }
   else
   {
       unsigned short *RGB565 = (unsigned short *)rgb; 
       for (j=0;j<m_height*m_width;++j)
       {
           unsigned char chR = ((*(rgb ) & RGB565_MASK_RED) >> 11 ) << 3;
           unsigned char chG = ((*(rgb ) & RGB565_MASK_GREEN) >> 5) << 2;
           unsigned char chB = (*(rgb ) & RGB565_MASK_BLUE) << 3 ;
           *gray = (unsigned char )(chB*0.3 +chR*0.11 +chG*0.59);
           RGB565++;
           gray++;
       }
   }


}

int rgbToYUV( unsigned char *rgbImg,unsigned char *YuvImg,int channels,int m_width,int m_height)
{
    if (rgbImg ==NULL || YuvImg ==NULL)
    {
        return -1;
    }
    int R,G,B;
    unsigned char *Yuv=YuvImg;
    unsigned char *rgb=rgbImg;
    int step=channels;
    int i,j;
    int r1,g1,b1;
    if (channels == 1)
    {
        //memcpy(YuvImg,rgbImg,m_width*m_height*sizeof(unsigned char));
        return -1;
    }

    else if (channels == 3)
    {
        r1 = 2;
        b1=0;
        g1=1;
    }
    else if (channels == 4)
    {
        r1=1;
        b1=3;
        g1=2;

    }
   
    /* ***********公式*************
    Y’ = 0.257*R' + 0.504*G' + 0.098*B' + 16
    Cb' = -0.148*R' - 0.291*G' + 0.439*B' + 128
    Cr' = 0.439*R' - 0.368*G' - 0.071*B' + 128
    */
    if(channels != 2)
    {
        for (j=0;j<m_height*m_width;++j)
        {
            B=*(rgb+b1);
            G=*(rgb+g1);
            R=*(rgb+r1);
//             *Yuv++ =(unsigned char )( ( 19595  * R + 38469  * G + 7472 * B)>>16 );
//             *Yuv++ = (unsigned char )( 128 + ( -9699 * R - 19070  * G + 28770 * B)>>16 );
//             *Yuv++ = (unsigned char )( 128 + ( 28770 * R - 24117  * G + 4653 * B)>>16 );
            *Yuv++ = (unsigned char )(0.299*R+0.587*G+0.114*B);
            *Yuv++ = (unsigned char )(-0.169*R+-0.3310*G+0.5000*B+128);
            *Yuv++ = (unsigned char )(0.5000*R+-0.4190*G-0.0810*B+128);
            rgb+=step;
           
        }
    }
    else
    {
        unsigned short *RGB565 = (unsigned short *)rgb; 
        for (j=0;j<m_height*m_width;++j)
        {
            R = ((*(rgb ) & RGB565_MASK_RED) >> 11 ) << 3;
            G = ((*(rgb ) & RGB565_MASK_GREEN) >> 5) << 2;
            B = (*(rgb ) & RGB565_MASK_BLUE) << 3 ;
            *Yuv++ =(unsigned char )( ( 19595  * R + 38469  * G + 7472 * B)>>16 );
            *Yuv++ = (unsigned char )( 128 + ( -9699 * R - 19070  * G + 28770 * B)>>16 );
            *Yuv++ = (unsigned char )( 128 + ( 28770 * R - 24117  * G + 4653 * B)>>16 );
            RGB565++;
            
        }
    }

    return 0;
}

int YUVImageSkinDetect(unsigned char *YuvImg,unsigned char *maskImg,int m_width,int m_height)
{
    if (YuvImg ==NULL || maskImg ==NULL)
    {
        return -1;
    }
    int size = m_height*m_width;
    int u,v;
   
    for (int i=0;i<size;++i)
    {
        *YuvImg++;
        v = *YuvImg++;
        u= *YuvImg++;

        if (v >= 77 && v <= 127 && u >= 133 && u <= 177)
        //if(u > v)
        {
            *maskImg++ = 1;
           
        }
        else
            *maskImg++ = 0;
        
    }
  
    return 0;
}



XmMat *MiCreateMat(type_int width,type_int height,type_int type)
{
    XmMat *xmMat = new XmMat;
    xmMat->type = type;

    xmMat->step = width*type;

    xmMat->data.ptr = new type_uchar[width*height*type]; 
    xmMat->height = height;
    xmMat->width = width;

    return xmMat;
}

XmMat *MiCreateMat(type_uchar *imgData,type_int width,type_int height,type_int type)
{
    XmMat *xmMat = new XmMat;
    xmMat->type = type;

    xmMat->step = width*type;

    xmMat->data.ptr = imgData; 
    xmMat->height = height;
    xmMat->width = width;

    return xmMat;
}

//不释放mat中data的内存
void MiReleaseMat(XmMat **mat)
{
    if (mat)
    {
        delete (*mat);
        (*mat)=NULL;
    }
}
    
    


void MiReleaseImage(XmImage **img)
{
    if (img==NULL)
    {
        return;
    }
    
    if ( (*img)->data!=NULL)
    {
        delete [] (*img)->data;
        (*img)->data = NULL;
    }
    delete img;
    img=NULL;
}

 void MiIntegralImage_8u64s(const type_uchar*src,type_int srcstep,
                                      type_int *sum,type_int sumstep,
                                      sqsumtype *sqsum,type_int sqsumstep,
                                       XmSize size )
{
    type_int x, y;
    type_int s;
    type_int sq;
    

    srcstep /= sizeof(src[0]);

    memset( sum, 0, (size.width+1)<<2);
    sumstep >>= 2;
    sum += sumstep + 1;
    if( sqsum )
    {
        memset( sqsum, 0, (size.width+1)*sizeof(sqsumtype));
        sqsumstep /= sizeof(sqsumtype );
        sqsum += sqsumstep + 1;
    }

    {
        sum[-1] = 0;
        sqsum[-1] = 0;

        for( x = 0, s = 0, sq = 0; x < size.width;x++ )
        {
            type_int t1,s1,tq1;
            t1= src[x];
            s1=t1+s;		
            sum[x] = s1;s=s1;
            tq1=(t1*t1);

            s1=sq+tq1;sq=s1;
            sqsum[x] = s1;


        }


        for( y = 1; y < size.height; y++ )
        {
            src += srcstep;
            sum += sumstep;
            sqsum += sqsumstep;
//
            sum[-1] = 0;
            sqsum[-1] = 0;

            for( x = 0, s = 0, sq = 0; x < size.width; x++ )
            {
                type_int t1,s1,tq1;


                sqsumtype  tsq1;

                t1= src[x];

                s1=t1+s;s=s1;

                tq1=(t1*t1);

                t1=sum[x - sumstep];
                s1+=t1;
                sum[x] =s1;
                s1=sq+tq1; sq =s1;
                tsq1=sqsum[x - sqsumstep];
                tsq1+=s1;
                sqsum[x] = tsq1;
            }

        }
    }

}
 void MiIntegralImage_8u64s(const type_uchar*src,type_int srcstep,
     type_int *sum,type_int sumstep,
     XmSize size )
 {
     type_int x, y;
     type_int s;
     type_int sq;


     srcstep /= sizeof(src[0]);

     memset( sum, 0, (size.width+1)<<2);
     sumstep >>= 2;
     sum += sumstep + 1;
     {
         sum[-1] = 0;
        
         for( x = 0, s = 0, sq = 0; x < size.width;x++ )
         {
             type_int t1,s1,tq1;
             t1= src[x];
             s1=t1+s;		
             sum[x] = s1;s=s1;
             tq1=(t1*t1);




         }


         for( y = 1; y < size.height; y++ )
         {
             src += srcstep;
             sum += sumstep;
         
             //
             sum[-1] = 0;
         

             for( x = 0, s = 0, sq = 0; x < size.width; x++ )
             {
                 type_int t1,s1,tq1;

                 t1= src[x];

                 s1=t1+s;s=s1;

                 tq1=(t1*t1);

                 t1=sum[x - sumstep];
                 s1+=t1;
                 sum[x] =s1;

             }

         }
     }

 }

void MiIntegral(const XmArr *image,XmArr *sumImage,XmArr *sumSqImage)
{
    XmMat *src = (XmMat *)image;
    XmMat *sum = (XmMat *)sumImage;
    XmMat *sqsum = (XmMat *)sumSqImage;

    if( sum->cols != src->cols + 1 ||
        sum->rows != src->rows + 1 )
        return;
    if (sumSqImage)
    {
        MiIntegralImage_8u64s(src->data.ptr,src->step,(type_int *)sum->data.i,sum->step,
            (sqsumtype *)(sqsum->data.db),sqsum->step,XmSize(src->width,src->height)  
            );
    }
    else
    {
        MiIntegralImage_8u64s(src->data.ptr,src->step,(type_int *)sum->data.i,sum->step,
            XmSize(src->width,src->height)  
            );
    }

}




#endif
