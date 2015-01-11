#include "FaceIncludeHeader.h"



#ifdef UserFace

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

string FaceDirName = "newFace";
string gFaceDir;

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

    string FaceDir = userDir+"\\"+FaceDirName+"\\";

    int width = image.cols;
    int height = image.rows;
#ifdef FaceDetect_FFP

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

#else

    {
        face_Rect =  gFaceAlbum.FaceDetect_Image(image);

        //cout<<face_Rect.size()<<endl;
        gFaceNum+=face_Rect.size();

    }

  //  if (gFaceInfo.nEyeFlag)
    {
        vec_Eye =  gFaceAlbum.EyeDetect_Image(imageGray,face_Rect);
    }

#endif

    for (int i=0;i<vec_Eye.size();++i)
    {
        char buf[10];
        sprintf(buf,"_%d",i);

        saveFaceImgWithEye(image,vec_Eye[i],gFaceDir+imageName+buf+".jpg");
       
        FaceList.push_back(gFaceDir+imageName+buf+".jpg");
        //saveFaceImgWithFaceRect(image,face_Rect[i],gFaceInfo.FeaturePath+imageName+buf+".jpg");
        saveEyePtToFile(gFaceDir+imageName+buf+".FFP",vec_Eye[i]);
        saveFaceExifToFile(gFaceDir+imageName+buf+".exif",vec_Eye[i],exif,RelativePath,width,height);
       
    }

    
    {
        vector<string>fstr = gFaceAlbum.FeatureExtractor_Image(image,vec_Eye);
        for (int i=0;i<fstr.size();++i)
        {
            char buf[10];
            sprintf(buf,"_%d",i);

            saveFeatureToFile(gFaceDir+imageName+buf+".fea",fstr[i]);

            //Mat NorImg;
           // gFaceAlbum.GetColorRectFaceImg(image,vec_Eye[i],NorImg);
           // imwrite(gFaceInfo.FeaturePath+imageName+buf+"_color.jpg",NorImg);
        }

    }

    


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
            gFaceDir = uesrDir+"\\"+FaceDirName+"\\";
            _mkdir(gFaceDir.c_str());
            string resultDir = uesrDir+"\\results";
            _mkdir(resultDir.c_str());
            
            if (!userAlbumDir.empty())
            {
                
                string FaceListPath = userAlbumDir+"\\"+FaceDirName+".txt";
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
        string FaceListPath = userAlbumDir+"\\"+FaceDirName+".txt";

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

void init_FaceInfo()
{

     gFaceInfo.ClusterAlbumDir = "D:\\data\\FaceAlbum\\user8\\";


    gFaceInfo.FeaturePath =gFaceInfo.ClusterAlbumDir+FaceDirName+"\\";
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

        if (argc > 3)
        {
            FaceDirName = argv[3];
            cout<<"FaceDirName : "<<FaceDirName<<endl;
        }
        
        
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
int main_zhijun(int argc,char *argv[])
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


#endif