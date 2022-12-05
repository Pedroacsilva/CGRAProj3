class CGRAimage{
    int widthStep;
    static bool pminitialized;
public:
    int width; // columns
    int height; // rows
    unsigned char * data; // RGBRGBRGB...
    CGRAimage();
    CGRAimage(CGRAimage &);
    ~CGRAimage();
  
    bool loadPPM(const char * fname);
    void savePBM(const char * fname);
    bool loadBMP(const char * fname);
};
