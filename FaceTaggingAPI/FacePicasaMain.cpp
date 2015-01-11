#include "FaceIncludeHeader.h"

#ifdef FacePicasa


typedef struct _FACEINFO
{
    String ClssifierDir;
    String ClusterAlbumDir;


    String modelPath;
    String FaceEyeDir;
    String imageList;

    String FeaturePath;
    String FeatureList;
    String FaceList;
    String ClusterGroundTruthFaceList;

    String SaveClusterPath;
    String SaveClusterDir;

    int nFaceDetect;
    int detectMode;
    int nEyeFlag;
    int nFeaFlag;
    int nClusterFlag;
    int incrementClusterFlag;
    int nRecommendClusterFlag;
}FACEINFO;

FaceAlbum gFaceAlbum;
int gFaceNum=0;
FACEINFO gFaceInfo;


//#define shunfeng_detect
vector<string> FaceList;


#define  FaceDetect_FFP
#define FaceDetect_picasaRect
#define  debug_test

static int missFaceCount = 0;

void processOneImage(string userDir,string RelativePath)
{
    string imagePath = userDir + "\\" + RelativePath;
    double oneImgTime;
    int pos_start = RelativePath.rfind("\\");
    int pos_end = RelativePath.rfind(".");

    string imageName =RelativePath.substr(pos_start+1,pos_end - pos_start - 1);


    Mat image = imread(imagePath);


	
	
	
    Mat srcImage = image;
    if (image.empty())
    {
        cout<<"load image error :"<<imagePath<<endl;
        return;
    }

    Cexif loadExif;

    FILE *pFImg = fopen(imagePath.c_str(),"rb");
    if (!pFImg)
    {
        //continue;
        return;
    }
    bool isLoad = loadExif.DecodeExif(pFImg);
    EXIFINFO *exif = loadExif.m_exifinfo;
    if (!isLoad )
    {
        cout<<"load exif error!!"<<endl;
       // cout<<loadExif.m_szLastError<<endl;
    }
    else
    {

    }

    int Angle = rotationImageWithOrientation(image,exif->Orientation);
    fclose(pFImg);
   

    cv::Mat imageGray ;

    if (image.channels() == 3)
    {
        cvtColor(image,imageGray,CV_BGR2GRAY);
    }
    else
    {
        imageGray=image.clone();
    }
     
    vector< EyeArray > vec_Eye;
    vector<Rect>face_Rect;

   
    int i,j;

#ifdef FaceDetect_FFP

#ifdef FaceDetect_picasaRect
    int width =srcImage.cols;
    int height = srcImage.rows;

    for (int ii=0;ii<30;++ii)
    {
        char buf[10];
        sprintf(buf,"_%d",ii);
        string imageName_Face = imageName+buf;

        /////////////////picasa 
        ifstream fin;
        string ffpPath = gFaceInfo.FeaturePath+imageName_Face+".face";

        fin.open(ffpPath.c_str());
        if (!fin)
        {
            cout<<"open file :"<<ffpPath<<"  ERROR!!"<<endl;
            break;
        }
        int x1,y1,x2,y2;

        Rect tmpRect;

        fin >> x1;
        fin >> y1;
        fin >> x2;
        fin >> y2;

        tmpRect.x = width *(x1/65535.0);
        tmpRect.y = height *(y1/65535.0);

        tmpRect.width = width *( x2/65535.0) - tmpRect.x;
        tmpRect.height = height *( y2/65535.0) -  tmpRect.y;
        //  cvRectangle(&((IplImage)srcImage),cvPoint(tmpRect.x,tmpRect.y),cvPoint(tmpRect.x+tmpRect.width,tmpRect.y+tmpRect.height),CV_RGB(255,0,0),5,16 );
        // imwrite("D:/1.jpg",srcImage);
        if(Angle==270)
        {

            tmpRect.x= tmpRect.y;
            tmpRect.y=width-width *( x2/65535.0);

            int ttmp=tmpRect.width;
            tmpRect.width =tmpRect.height;
            tmpRect.height=ttmp;

          

        }
        else if (Angle == 90)
        {
             tmpRect.y = tmpRect.x;
             tmpRect.x= height*(1-y2/65535.0);
             int ttmp=tmpRect.width;
             tmpRect.width =tmpRect.height;
             tmpRect.height=ttmp;

          }

        fin.close();
             
        ///根据Picasa框 重新检测人脸
        int roWidth = image.cols;
        int roHeight = image.rows;
        Rect ExpandRect;
        ExpandRect.x = tmpRect.x - tmpRect.width/3 > 0 ? tmpRect.x - tmpRect.width/3:0;
        ExpandRect.y = tmpRect.y - tmpRect.height/3 > 0 ? tmpRect.y - tmpRect.height/3:0;
        ExpandRect.width = ExpandRect.x+tmpRect.width*1.6 <roWidth?tmpRect.width*1.6:roWidth - ExpandRect.x;
        ExpandRect.height = ExpandRect.y+tmpRect.height*1.6  <roHeight?tmpRect.height*1.6:roHeight - ExpandRect.y;

        Mat ExpandMat = image(ExpandRect);

       // imwrite("d:/11.jpg",ExpandMat);
        vector<Rect> fRect =  gFaceAlbum.FaceDetect_Image(ExpandMat);

//         Mat ExpandMat1 = imread("d:/11.jpg");
// 
//         cout<<ExpandMat.step<< "  "<<ExpandMat1.step<<endl;
// 
//         vector<Rect> fRect1 =  gFaceAlbum.FaceDetect_Image(ExpandMat);
// 
//        if (fRect.size() != fRect1.size())
//         {
//             cout<<"error"<<endl;
//         }
//       
        if (fRect.size() == 1)
        {
            //需要放缩回来
            Rect tRect = fRect[0];
            tRect.x += ExpandRect.x;
            tRect.y += ExpandRect.y;
            face_Rect.push_back(tRect);

        }
        else
            face_Rect.push_back(tmpRect);

        if (fRect.size() == 0)
        {
           missFaceCount++;
            
        }
        

    }
  
    

    vec_Eye =  gFaceAlbum.EyeDetect_Image(imageGray,face_Rect);

    


#else
    EyeArray eyePt;

    for (int ii=0;ii<30;++ii)
    {
        char buf[10];
        sprintf(buf,"_%d",ii);
        string imageName_Face = imageName+buf;
        string ffpPath =  gFaceInfo.FeaturePath+imageName_Face+".FFP";

        if( readFaceFFPFromFile(ffpPath,eyePt) != -1)
        {
            vec_Eye.push_back(eyePt);
            string FacePath = FaceDir+imageName+buf+".jpg";
            FaceList.push_back(FacePath);
        }
        else
            break;
    }
#endif  

  /*  if( readFaceFFPFromFile(ffpPath,eyePt) != -1)*/
    
       // vec_Eye.push_back(eyePt);
        for (int i=0;i<vec_Eye.size();++i)
        {
            char buf[10];
            sprintf(buf,"_%d",i);

            saveFaceImgWithEye(image,vec_Eye[i],gFaceInfo.FeaturePath+imageName+buf+".jpg");
            FaceList.push_back(gFaceInfo.FeaturePath+imageName+buf+".jpg");
            //saveFaceImgWithFaceRect(image,vec_Eye[i],face_Rect[i],gFaceInfo.FeaturePath+imageName+buf+".jpg");
            saveEyePtToFile(gFaceInfo.FeaturePath+imageName+buf+".FFP",vec_Eye[i]);
            saveFaceExifToFile(gFaceInfo.FeaturePath+imageName+buf+".exif",vec_Eye[i],exif,RelativePath,width,height);


        }

    //imwrite("d:/1.jpg",image);
   
    //if (gFaceInfo.nFeaFlag)
    {
      
        vector<string>fstr = gFaceAlbum.FeatureExtractor_Image(image,vec_Eye);
        for (int i=0;i<fstr.size();++i)
        {
            char buf[10];
            sprintf(buf,"_%d",i);

            saveFeatureToFile(gFaceInfo.FeaturePath+imageName+buf+".fea",fstr[i]);
            
            Mat NorImg;
            gFaceAlbum.GetColorRectFaceImg(image,vec_Eye[i],NorImg);
            imwrite(gFaceInfo.FeaturePath+imageName+buf+"_color.jpg",NorImg);
            //saveFaceImgWithEye(image,vec_Eye[i],gFaceInfo.FeaturePath+imageName+buf+".jpg");
            //cout<<fstr[i]<<endl;
        }

    }

#endif

}



int ProcessImgList()
{
    ifstream fileIn;
    string listPath =gFaceInfo.imageList;

   
    fileIn.open(listPath.c_str()); //打开文件夹路径下图片列表

    if (!fileIn.good())
    {
        cout<<"open Image List Error!! "<<listPath<<endl;
        return -2;
    }

    char imagePath[1024];

    
    int imgCount=0;

    string userAlbumDir ;
    while (fileIn.getline(imagePath,1024)!=NULL)
    {
        string tmpPath = imagePath;

        int pos1 = tmpPath.rfind("\\");
        string tmpPath1 = tmpPath.substr(0,pos1);
        pos1 = tmpPath1.rfind("\\");
        string uesrDir = tmpPath1.substr(0,pos1);
        if (userAlbumDir != uesrDir)
        {
           
            cout<<"user : "<<uesrDir<<endl;
            string FaceDir = uesrDir+"\\FacePicasa";
            _mkdir(FaceDir.c_str());
            string resultDir = uesrDir+"\\resultPicasa";
            _mkdir(resultDir.c_str());
            
            if (!userAlbumDir.empty())
            {
                
                string FaceListPath = userAlbumDir+"\\FacePicasa.txt";
                ofstream fout;
                fout.open(FaceListPath.c_str(),ios::trunc);
                for (int i=0;i<FaceList.size();++i)
                {
                    fout<<FaceList[i]<<endl;
                }
                fout.close();
            }
            

            userAlbumDir = uesrDir;
            FaceList.clear();
        }
        

        string imageName = tmpPath.substr(pos1+1,tmpPath.size() - 1);

        //string absolutePath = gFaceInfo.ClusterAlbumDir+imagePath;
       // cout<<"image Path : "<<imageName<<endl;

        processOneImage(uesrDir,imageName);

        //cout<<"Image Count:"<<imgCount++<<endl;
    }
    fileIn.close();


    if (!userAlbumDir.empty())
    {
        string FaceListPath = userAlbumDir+"\\FacePicasa.txt";

        ofstream fout;
        fout.open(FaceListPath.c_str(),ios::trunc);
        for (int i=0;i<FaceList.size();++i)
        {
            fout<<FaceList[i]<<endl;
        }
        fout.close();
        FaceList.clear();
    }

    cout<<"missFaceCount : "<<missFaceCount<<endl;
}

//#define FacePicasa




void init_FaceInfo()
{

     gFaceInfo.ClusterAlbumDir = "D:\\data\\FaceAlbum\\user5\\";


    gFaceInfo.FeaturePath =gFaceInfo.ClusterAlbumDir+"Face\\";
    gFaceInfo.imageList=gFaceInfo.ClusterAlbumDir + "oriPhoto.list";
    gFaceInfo.SaveClusterDir =  gFaceInfo.ClusterAlbumDir+"\\results\\";
    gFaceInfo.SaveClusterPath =  gFaceInfo.ClusterAlbumDir+"\\results\\FaceClusters.list";
    gFaceInfo.FaceList = gFaceInfo.ClusterAlbumDir +"Face.list";
    gFaceInfo.ClusterGroundTruthFaceList = gFaceInfo.ClusterAlbumDir+"FaceGroundTruthList.txt";
   
   // gFaceInfo.ClssifierDir = "../../data/";

    gFaceInfo.modelPath ="../../data/models";

    gFaceInfo.nEyeFlag = 1;
    gFaceInfo.nFeaFlag =1;

    gFaceInfo.nFaceDetect = 1;

#ifdef FacePicasa
    gFaceInfo.FeaturePath =gFaceInfo.ClusterAlbumDir+"FacePicasa\\";
    gFaceInfo.ClusterGroundTruthFaceList = gFaceInfo.ClusterAlbumDir+"FacePicasa.txt";
#endif
}



int init_FaceTagging(int argc,char *argv[])
{
    

    if (argc == 1)
    {
        init_FaceInfo();

    }
    else
    {
        gFaceInfo.imageList=argv[1];
        gFaceInfo.modelPath = argv[2];

    }


    {
        if (XMFACE_NORMAL != gFaceAlbum.FaceDetect_Init(gFaceInfo.modelPath+"/FaceDetect_model/") )
        {
            cout<<"init detect error!!"<<endl;
            return XMFACE_ERR_INITIALIZE;
        }



    }

    {
        if (XMFACE_NORMAL != gFaceAlbum.EyeDetect_Init(gFaceInfo.modelPath) )
        {
            
            cout<<"init Eye detect error!!  "<<gFaceInfo.modelPath<<endl;
            return XMFACE_ERR_INITIALIZE;
        }
    }
    

   {
       if (XMFACE_NORMAL != gFaceAlbum.FeatureExtractor_Init(gFaceInfo.modelPath+"/feature_model/") )
       {
           cout<<"init Feature  error!!"<<endl;
           return XMFACE_ERR_INITIALIZE;
       }
   }
   



    return 0;
}



//0
//1
//2
//3 是聚类
//4 是增量聚类
//5 是推荐聚类
int main(int argc,char *argv[])
{
   // cout<<argv[1]<<endl;


    if( XMFACE_NORMAL != init_FaceTagging(argc,argv))
    {
        system("pause");
        return XMFACE_ERR_INITIALIZE;
    }

    ProcessImgList();
     
  

   
   // cout<<"allFace :  "<<gFaceNum<<endl;
    gFaceAlbum.ReleaseFaceTagging();

   
    system("pause");
}




#endif

