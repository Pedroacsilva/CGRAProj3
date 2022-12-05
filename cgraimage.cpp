#include <fcntl.h>
#include <string.h>
#ifdef __APPLE__
extern "C"{
#include <netpbm/pam.h>
}
#else
extern "C"{
#include <pam.h>
}
#endif
#include <math.h>
#include "cgraimage.h"

bool CGRAimage::pminitialized=false;

CGRAimage::CGRAimage(){
    widthStep=width=height=0;
    data=NULL;

}

CGRAimage::CGRAimage(CGRAimage & orig){
    widthStep=orig.widthStep;
    width=orig.width;
    height=orig.width;
    if(data!=NULL)
	delete []data;
    int size=widthStep*height;
    data=new unsigned char[size];
    memcpy(data,orig.data,size);
}

CGRAimage::~CGRAimage(){
    if(data!=NULL)
	delete []data;
}

bool CGRAimage::loadPPM(const char * fname){
    FILE * file;
    struct pam inpam;
    tuple * tuplerow;
    unsigned int row;
    int aux;
    
    if(!pminitialized)
	pm_init("cgra",0);
    
  file=fopen(fname,"r");
  if(file==NULL)
      return false;
 
#ifdef PAM_STRUCT_SIZE 
  pnm_readpaminit(file, &inpam,  PAM_STRUCT_SIZE(tuple_type));
#else
  pnm_readpaminit(file, &inpam,  sizeof(struct pam));
#endif
   
  printf("Reading image %s ...\n",fname );
  printf("width=%d,height=%d,depth=%d\n",
	 inpam.width,inpam.height,inpam.depth);
   
  /* allocating image*/
  
  width=inpam.width;
  height=inpam.height;
  widthStep=width*inpam.depth;
 
  // data=(unsigned char *)malloc(widthStep*height);
  data = new unsigned char [widthStep*height];
	
  tuplerow = pnm_allocpamrow(&inpam);
   
  for (row = 0; row < inpam.height; row++) {
    unsigned int column;
    pnm_readpamrow(&inpam, tuplerow);
    for (column = 0; column < inpam.width; ++column) {
      unsigned int plane;
      for (plane = 0; plane < inpam.depth; ++plane) {
	data[widthStep*row+column*inpam.depth+plane]= tuplerow[column][plane];
      }
    }
  } 
     
  pnm_freepamrow(tuplerow);
  fclose(file);
  return true;
}

void CGRAimage::savePBM(const char * fname){
    FILE * file;
    struct pam outpam;
    tuple * tuplerow;
    unsigned int row;
    int aux;
    
    file=fopen(fname,"w");
    outpam.file=file;
    outpam.size=sizeof(struct pam);
    outpam.len=sizeof(struct pam);
    outpam.format=RPGM_FORMAT;
    outpam.plainformat=0;
    outpam.height=height;
    outpam.width=width;
    outpam.depth=1;
    outpam.maxval=255;
    strcpy(outpam.tuple_type,PAM_PGM_TUPLETYPE);
    /*  outpam.allocation_depth=0;
	outpam.comment_p="PM/DEEC";
    */
    pnm_writepaminit( &outpam );
    
    printf("Writing image\n");

    tuplerow = pnm_allocpamrow(&outpam);
    
    for (row = 0; row < outpam.height; row++) {
	unsigned int column;
	for (column = 0; column < outpam.width; ++column) {
	    unsigned int plane;
	    for (plane = 0; plane < outpam.depth; ++plane) {
		tuplerow[column][plane]=data[widthStep*row+column];
	    }
	}
	pnm_writepamrow(&outpam, tuplerow); 
    } 
    
    pnm_freepamrow(tuplerow);
    fclose(file);
}

bool CGRAimage::loadBMP(const char * fname){
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP starts with a 54-bytes header
    unsigned int dataPos;     // Position  where the actual data begins
    unsigned int imageSize;   // = width*height*3

    FILE * file = fopen(fname,"rb");
    if (!file){printf("Image could not be opened\n"); return false;}
    
    if ( fread(header, 1, 54, file)!=54 ){ // not 54 bytes : problem
	printf("Not a correct BMP file\n");
	return false;
    }
    
    if ( header[0]!='B' || header[1]!='M' ){
	printf("Not a correct BMP file\n");
	return false;
    }
    
    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    width      = *(int*)&(header[0x12]);
    height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize==0)    imageSize=width*height*3; 
    if (dataPos==0)      dataPos=54; 
    widthStep=width*3;
    
    data = new unsigned char [imageSize];
	
	
    // Read the actual data from the file into the buffer
    fread(data,1,imageSize,file);
    
    //Everything is in memory now, the file can be closed
    fclose(file);
    return true;
}
