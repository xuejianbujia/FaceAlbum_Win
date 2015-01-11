


//#define FaceDetect_FFP
//#define  FaceDetect_picasaRect

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



FACEINFO gFaceInfo;

FaceAlbum gFaceAlbum;

int gFaceNum=0;

//#define shunfeng_detect
vector<string> FaceList;

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
   
    //time_t 
   // ImageInfo mImageInfo = gFaceAlbum.RunFaceTaggingFromPath(imagePath,2);
 //   ImageInfo mImageInfo = gFaceAlbum.RunFaceTaggingFromBuffer(image.data,image.cols,image.rows,image.step,image.channels(),2);
  /*  for (int i=0;i<mImageInfo.mFaceNum;++i)
    {
        cout<<mImageInfo.mFaceInfo.size()<<endl;
        cout<<"Eye left : "<<mImageInfo.mFaceInfo[i].Eye_left_x<<endl;
      //  cout<<"Feature  : "<<mImageInfo.mFaceInfo[i].faceFeature<<endl;
    }
   */

 
    string FaceDir = userDir+"\\Face\\";


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
            // cvCircle( &((IplImage)image),cvPoint(tmpRect.x,tmpRect.y),10,CV_RGB(255,0,0),5,16 );

           //  cvCircle( &((IplImage)image),cvPoint(tmpRect.x+tmpRect.width,tmpRect.y+tmpRect.height),10,CV_RGB(0,255,0),5,16 );
          //  imwrite("D:/2.jpg",image);
          }

        fin.close();
        
        
        face_Rect.push_back(tmpRect);


    }
   // imwrite("D:/1.jpg",imageGray);
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
#ifndef FaceDetect_FFP
  /*  if( readFaceFFPFromFile(ffpPath,eyePt) != -1)*/
    
       // vec_Eye.push_back(eyePt);
        for (int i=0;i<vec_Eye.size();++i)
        {
            char buf[10];
            sprintf(buf,"_%d",i);

             saveFaceImgWithEye(image,vec_Eye[i],gFaceInfo.FeaturePath+imageName+buf+".jpg");
      
            saveFaceImgWithFaceRect(image,face_Rect[i],gFaceInfo.FeaturePath+imageName+buf+".jpg");
            saveEyePtToFile(gFaceInfo.FeaturePath+imageName+buf+".FFP",vec_Eye[i]);
            saveFaceExifToFile(gFaceInfo.FeaturePath+imageName+buf+".exif",vec_Eye[i],exif,RelativePath);


        }
#endif
        
    

    //if (gFaceInfo.nFeaFlag)
    {
      
        vector<string>fstr = gFaceAlbum.FeatureExtractor_Image(image,vec_Eye);
        for (int i=0;i<fstr.size();++i)
        {
            char buf[10];
            sprintf(buf,"_%d",i);

            saveFeatureToFile(gFaceInfo.FeaturePath+imageName+buf+".fea",fstr[i]);
            //cout<<fstr[i]<<endl;
        }

    }
#else
  //  if (gFaceInfo.nFaceDetect)
    {
        face_Rect =  gFaceAlbum.FaceDetect_Image(image);

        //cout<<face_Rect.size()<<endl;
        gFaceNum+=face_Rect.size();

    }




  //  if (gFaceInfo.nEyeFlag)
    {
        vec_Eye =  gFaceAlbum.EyeDetect_Image(imageGray,face_Rect);
        for (int i=0;i<vec_Eye.size();++i)
        {
            char buf[10];
            sprintf(buf,"_%d",i);
            string FacePath = FaceDir+imageName+buf+".jpg";
            FaceList.push_back(FacePath);

            saveFaceImgWithEye(image,vec_Eye[i],FacePath);

            saveEyePtToFile(FaceDir+imageName+buf+".FFP",vec_Eye[i]);
            saveFaceExifToFile(FaceDir+imageName+buf+".exif",vec_Eye[i],exif,RelativePath);


        }
    }
  //  if (gFaceInfo.nFeaFlag)
    {
        vector<string>fstr = gFaceAlbum.FeatureExtractor_Image(imageGray,vec_Eye);
        for (int i=0;i<fstr.size();++i)
        {
            char buf[10];
            sprintf(buf,"_%d",i);

            saveFeatureToFile(FaceDir+imageName+buf+".fea",fstr[i]);
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
            string FaceDir = uesrDir+"\\Face";
            _mkdir(FaceDir.c_str());
            string resultDir = uesrDir+"\\results";
            _mkdir(resultDir.c_str());
            
            if (!userAlbumDir.empty())
            {
                
                string FaceListPath = userAlbumDir+"\\Face.txt";
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
        string FaceListPath = userAlbumDir+"\\Face.txt";

        ofstream fout;
        fout.open(FaceListPath.c_str(),ios::trunc);
        for (int i=0;i<FaceList.size();++i)
        {
            fout<<FaceList[i]<<endl;
        }
        fout.close();
        FaceList.clear();
    }
}

//#define FacePicasa




void init_FaceInfo()
{

     gFaceInfo.ClusterAlbumDir = "D:\\data\\FaceAlbum\\user8\\";


    gFaceInfo.FeaturePath =gFaceInfo.ClusterAlbumDir+"Face\\";
    gFaceInfo.imageList=gFaceInfo.ClusterAlbumDir + "oriPhoto.list";
    gFaceInfo.SaveClusterDir =  gFaceInfo.ClusterAlbumDir+"\\results\\";
    gFaceInfo.SaveClusterPath =  gFaceInfo.ClusterAlbumDir+"\\results\\FaceClusters.list";
    gFaceInfo.FaceList = gFaceInfo.ClusterAlbumDir +"Face.list";
    gFaceInfo.ClusterGroundTruthFaceList = gFaceInfo.ClusterAlbumDir+"FaceGroundTruthList.txt";
   
   // gFaceInfo.ClssifierDir = "../../data/";

    gFaceInfo.modelPath ="../../data/models/";

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

  //  if (gFaceInfo.nFaceDetect)
    {
        if (XMFACE_NORMAL != gFaceAlbum.FaceDetect_Init(gFaceInfo.modelPath+"/FaceDetect_model/") )
        {
            cout<<"init detect error!!"<<endl;
            return XMFACE_ERR_INITIALIZE;
        }



    }
  //  if (gFaceInfo.nEyeFlag)
    {
        if (XMFACE_NORMAL != gFaceAlbum.EyeDetect_Init(gFaceInfo.modelPath) )
        {
            
            cout<<"init Eye detect error!!  "<<gFaceInfo.modelPath<<endl;
            return XMFACE_ERR_INITIALIZE;
        }
    }
    
 //  if (gFaceInfo.nFeaFlag)
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
     if (!gFaceInfo.nClusterFlag)
     {
          ProcessImgList();
     }
  

   
   // cout<<"allFace :  "<<gFaceNum<<endl;
    gFaceAlbum.ReleaseFaceTagging();

   
    system("pause");
}