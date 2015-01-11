#include "FaceIncludeHeader.h"

#include "FaceAlbum.h"

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

template<typename T>
int OverLapTwoRect(T rc1,T rc2,float ratio)
{
	int x1 = MAX(rc1.x,rc2.x);
	int y1 = MAX(rc1.y,rc2.y);
	int x2 = MIN(rc1.x+rc1.width-1,rc2.x+rc2.width-1);
	int y2 = MIN(rc1.y+rc1.height-1,rc2.y+rc2.height-1);

	if (x1 >=x2 || y1>=y2)
	{
		return 0;
	}
	else
	{

		int s1 = rc1.width*rc1.height;
		int s2 = rc2.width*rc2.height;
		int s3 = (x2-x1+1)*(y2-y1+1);
		if (s3>=ratio*s2 && s3>=ratio*s1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
		
	}
	
}

void processOneImage_emulateCloud(string imageName)
{
	Mat image = imread(imageName);
	
	gFaceAlbum.RunFaceTaggingFromBuffer( image.data,image.cols,image.rows,image.step,image.channels(),2);
	
	static int kk=0;
	printf("%d finished processs\n",kk++);

}


int init_FaceTagging(string model_path)
{
    
        gFaceInfo.modelPath = model_path;

        
    {
        if (XMFACE_NORMAL != gFaceAlbum.FaceDetect_Init(gFaceInfo.modelPath+"/FaceDetect_model/") )
        {
            cout<<"init detect error!!"<<endl;
            return XMFACE_ERR_INITIALIZE;
        }



    }
  //  if (gFaceInfo.nEyeFlag)
    {
        if (XMFACE_NORMAL != gFaceAlbum.EyeDetect_Init(gFaceInfo.modelPath+"/EyeDetect_model/") )
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
   
#ifdef CORRECT_3D
   {
	   if (XMFACE_NORMAL != gFaceAlbum.Correct3d_Init(gFaceInfo.modelPath+"/3d_model/") )
	   {
		   cout<<"init Feature  error!!"<<endl;
		   return XMFACE_ERR_INITIALIZE;
	   }
   }
#endif



    return 0;
}

int main(int argc,char *argv[])
{
	if( XMFACE_NORMAL != init_FaceTagging(argv[1]))
    {
        system("pause");
        return XMFACE_ERR_INITIALIZE;
    }
	processOneImage_emulateCloud("1.jpg");

	gFaceAlbum.ReleaseFaceTagging();
}