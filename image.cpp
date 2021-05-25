#include<iostream>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<unistd.h>
#include<cstdlib>
#include<ctime>
#include<sys/time.h>
#include<cstdio>
#include <map>
#include<fstream>
#include<MIO_Freetype2.h>
#include <iconv.h>
#include <wctype.h>
#include <wchar.h>

using namespace std;

#define PATH "../osd/"
#define FILE_bg "../osd/bg.png"


#define COLOR_BOX   0xe6216cb7
#define COLOR_LINE   0xBE2090FF
#define COLOR_FILL  0x80000000
std::string now_str()
{
    struct timeval tpend;
    gettimeofday(&tpend,NULL);

    int secofday = (tpend.tv_sec + 3600 * 8 ) % 86400;
    int hours = secofday / 3600;
    int minutes = (secofday - hours * 3600 ) / 60;
    int seconds = secofday % 60;
    int milliseconds = tpend.tv_usec/1000;
    char buf[40];
    sprintf(buf, "%02d:%02d:%02d.%03d",  hours, minutes, seconds, milliseconds);
    return buf;
}

struct position
{
    int x;
    int y;
    int height;
    int whdth;
};

class TextBox{
private:
    static string s_TextBox_opt;
    static string s_Text_opt;
    static string s_Picture_opt;
    static string s_PictureBox_opt;
    string m_TextBox_opt;
    int repace_str(string &src,const char *str,int desk)
    {
        char temp[20];
        memset(temp,0,sizeof(temp));
        sprintf(temp,"%d",desk);
        src.find(str,0);

        if(src.find(str) == src.npos)
        {
            cout<<"not find "<< str << " in " << src <<endl;
            return -1;
        }
        src.replace(src.find(str),strlen(str),temp);

        return 0;
    }
public:
    string config_TextBox(int box_x1,int box_y1,int box_x2,int box_y2,string str,int flg)
    {
        int box_tx,box_ty;

        box_tx = box_x1+10;
        box_ty = box_y2-15;
        if(flg)
        {
            m_TextBox_opt = s_TextBox_opt;
            repace_str(m_TextBox_opt,"box_x1",box_x1);
            repace_str(m_TextBox_opt,"box_y1",box_y1);
            repace_str(m_TextBox_opt,"box_x2",box_x2);
            repace_str(m_TextBox_opt,"box_y2",box_y2);
        }
        else
        {
            m_TextBox_opt = s_Text_opt;
        }
        repace_str(m_TextBox_opt,"box_tx",box_tx);
        repace_str(m_TextBox_opt,"box_ty",box_ty);
        m_TextBox_opt.replace(m_TextBox_opt.find("STRING",0),6,str);

        return m_TextBox_opt;
    }

    string config_Picture_Text(int box_x1,int box_y1,int box_x2,int box_y2,string str,string pic,int flg)
    {
        int box_tx,box_ty,icon_x,icon_y;

        box_tx = box_x1+61;
        box_ty = box_y2-15;
        icon_x = box_x1+20;
        icon_y = box_y1+15;
          
        if(flg)
        {
            m_TextBox_opt = s_PictureBox_opt;
            repace_str(m_TextBox_opt,"box_x1",box_x1);
            repace_str(m_TextBox_opt,"box_y1",box_y1);
            repace_str(m_TextBox_opt,"box_x2",box_x2);
            repace_str(m_TextBox_opt,"box_y2",box_y2);
        }
        else
        {
            m_TextBox_opt = s_Picture_opt;
        }

        repace_str(m_TextBox_opt,"box_x",icon_x);
        repace_str(m_TextBox_opt,"box_y",icon_y);
        repace_str(m_TextBox_opt,"box_tx",box_tx);
        repace_str(m_TextBox_opt,"box_ty",box_ty);
        m_TextBox_opt.replace(m_TextBox_opt.find("STRING",0),6,str);
        m_TextBox_opt.replace(m_TextBox_opt.find("PICTUR",0),6,pic);

        return m_TextBox_opt;
    }
    

    string get_opt()
    {
        return m_TextBox_opt;
    }

};
string TextBox::s_TextBox_opt = " -font NotoSansCJKsc-Regular.otf -fill \"rgb(32,144,255)\" -draw \"rectangle box_x1,box_y1 box_x2,box_y2\" -pointsize 15 -fill \"rgb(255,255,255)\" -draw \"text box_tx,box_ty 'STRING'\" ";
string TextBox::s_Text_opt = " -font NotoSansCJKsc-Regular.otf -pointsize 15 -fill \"rgb(255,255,255)\" -draw \"text box_tx,box_ty 'STRING'\"";
string TextBox::s_Picture_opt = " -compose Atop PICTUR -geometry 32x32+box_x+box_y -composite -font NotoSansCJKsc-Regular.otf -pointsize 15 -fill \"rgb(255,255,255)\" -draw \"text box_tx,box_ty 'STRING'\" ";
string TextBox::s_PictureBox_opt = " -fill \"rgb(32,144,255)\" -draw \"rectangle box_x1,box_y1 box_x2,box_y2\" -compose Atop PICTUR -geometry 32x32+box_x+box_y -composite -font NotoSansCJKsc-Regular.otf -pointsize 15 -fill \"rgb(255,255,255)\" -draw \"text box_tx,box_ty 'STRING'\" ";
class Basic_Interface
{
private:
    /* data */
    std::string back = FILE_bg;
    int category;
    position pos_base = {0,0,480,640};
public:
    const char *get_image(void)
    {
        return this->back.c_str();
    }
    int get_pos_x(){return pos_base.x;}
    int get_pos_y(){return pos_base.y;}
    int get_pos_whigh(){return pos_base.whdth;}
    int get_pos_heigth(){return pos_base.height;}
};

class preset:public Basic_Interface
{
private:
    /* data */
    std::string iamge = "preset";
    int pos;
    position pos_pre = {0,30,50,160};
    position pos_second = {160,30,50,160};

    TextBox b1;

    string screen[10] = 
    {
        "屏幕1",
        "屏幕2",
        "屏幕3",
        "屏幕4",
        "屏幕5",
        "屏幕6",
        "屏幕7",
        "屏幕8",
        "屏幕9",
        "屏幕10",
    };

    string item[4][2] =
    {
        {"场景","../osd/icon_preset.png"},
        {"亮度","../osd/icon_light.png"},
        {"画质","../osd/icon_quality.png"},
        {"高级","../osd/icon_settings.png"},
    };

public:
    void set_preset(int i)
    {
        pos = i;
    }
    void opt(int pos_1,int pos_2)
    {
        const char *temp = "temp.png";
        char cmd[4096];
        char new_cmd[4096];

        for(int i = 0;i < 10;i++)
        {
            int box = 0;
            TextBox m_box;
            if(i == pos_1)
            {
                box = 1;
            }
            m_box.config_TextBox(pos_pre.x,pos_pre.y+pos_pre.height*i,pos_pre.whdth+pos_pre.x,pos_pre.y+pos_pre.height*(i+1),screen[i],box);
            sprintf(cmd,"%s %s",cmd,m_box.get_opt().c_str());
        }

        for(int i = 0;i < 4;i++)
        {
            TextBox m_box;
            int box = 0;
            if(i==pos_2)
                box = 1;
            m_box.config_Picture_Text(pos_second.x,pos_second.y+pos_second.height*i,pos_second.whdth+pos_second.x,pos_second.y+pos_second.height*(i+1),item[i][0],item[i][1],box);
            sprintf(cmd,"%s %s",cmd,m_box.get_opt().c_str());
        }
        memset(new_cmd,0,sizeof(new_cmd));
        sprintf(new_cmd,"convert %s %s %s",get_image(),cmd,"-colorspace sRGB -depth 32 -alpha set -flip osd.bmp");
        cout<<"time is ss"<<now_str()<<endl;
        system(new_cmd);
        cout<<"time is ss"<<now_str()<<endl;

    }
};

int FUTF82WConvert( const char* a_szSrc, wchar_t* a_szDest, int a_nDestSize )
{

 size_t result;
 iconv_t env;
 int size = strlen(a_szSrc)+1 ;
 env = iconv_open("WCHAR_T","UTF-8");
 if (env==(iconv_t)-1)
 {
  printf("iconv_open UTF8->WCHAR_T error %d/n",errno) ;
  return -1;
 }
 result = iconv(env,(char**)&a_szSrc,(size_t*)&size,(char**)&a_szDest,(size_t*)&a_nDestSize);
 if (result==(size_t)-1)
 {
//  printf("iconv UTF8->WCHAR_T error %d/n",errno) ;
  return -1;
 }
 iconv_close(env);
 return (int)result;
}



int read_image_to_map(string image,int length,uint8_t *map)
{
    char cmd[512];
    const char *obj = "desk.bmp";
    uint32_t fileoffset = 0;
    sprintf(cmd,"convert %s %s %s",image.c_str()," -colorspace sRGB -depth 32 -alpha set -flip ",obj);
    system(cmd);
    //cout<< "image.c_str() is " << image.c_str() <<" cmd is "<<cmd<<endl;
    FILE *file = NULL;
    file = fopen(obj,"rb+");
    if(file == NULL)
    {
        cout<<"fopen is error"<<obj<<endl;
        return -1;
    }
    fseek(file,0x0A,SEEK_SET);
    if(fread((uint8_t*)&fileoffset , 1, 4, file) != 4)
    {
        cout<<"fread is error"<<endl;
    }
    fseek(file,fileoffset,SEEK_SET); //设置偏移到像素文件处
    fread(map,1,length,file);
    fclose(file);
    return 0;
}

int write_map_to_image(string image,string desk,int length,uint8_t *map)
{
    char cmd[512];
    const char *obj = "desk.bmp";
    uint32_t fileoffset = 0;
    sprintf(cmd,"convert %s %s %s",image.c_str()," -colorspace sRGB -depth 32 -alpha set -flip ",obj);
    system(cmd);
    FILE *file = NULL;
    file = fopen(obj,"rb+");
    if(file == NULL)
    {
        cout<<"fopen is error"<<obj<<endl;
        return -1;
    }
    fseek(file,0x0A,SEEK_SET);
    if(fread((uint8_t*)&fileoffset , 1, 4, file) != 4)
    {
        cout<<"fread is error"<<endl;
    }
    fseek(file,fileoffset,SEEK_SET); //设置偏移到像素文件处
    fwrite(map,1,length,file);
    fclose(file);

    sprintf(cmd,"convert %s %s %s",obj," -colorspace sRGB -depth 32 -alpha set -flip ",desk.c_str());
    system(cmd);

    return 0;
}

struct s_image
{
    string image;
    int height;
    int weigth;
    int bit_length;
    string path;
    
    uint8_t *bit;
};
struct s_select
{
    int screen;
    int item;
    int other;
    int column;
};

struct ARGB_DATA_TYPE
{
    uint8_t mB;
    uint8_t mG;
    uint8_t mR;
    uint8_t mA;
};

#define PIX_WIDTH  (4)
#define BK_WIDTH (640)
#define BK_HEIGHT (480)
class MaterialManage
{
private:
    uint8_t bitmap[BK_HEIGHT*BK_WIDTH*PIX_WIDTH];
    static int bitmap_dell;
    
public:
    static map<string,s_image> mapImage;
    static int GetBKmap();
    s_select m_select; 
    int DisplayBK()
    {
        //cout<<"time is "<<now_str()<<endl;
        memcpy(bitmap,mapImage["bk"].bit,mapImage["bk"].bit_length);
        //cout<<"time is end"<<now_str()<<endl;
//        printf("bk image addr is %p\n",bitmap);
    }
    // int draw_line(int s_x,int s_y,int e_x,int e_y)
    // {

    // };
    int draw_rectangle(int x,int y,int weigth,int height,int line,unsigned int fill)
    {
        if(x > (BK_WIDTH) || (x+weigth) < 0 || y > BK_HEIGHT || y+height < 0)
        {
            cout<<" rectangle is out bk "<< "x = "<<x<<"y"<<y<<endl;
        }
        for(int i = 0;i < height;i++)
        {
            if((i + y) < BK_HEIGHT)
            {
                int start_desk = x * PIX_WIDTH + (i+y)*BK_WIDTH * PIX_WIDTH;
                int start_src = i * weigth * PIX_WIDTH;
                int len = ((x+weigth) > BK_WIDTH ? BK_WIDTH-x:weigth)*PIX_WIDTH;
                //cout<<"rectangle x is "<<x<<" y "<<y<<" i "<<i<<" desk start pos is "<<start_desk<<" src start is "<<start_src<<" len is "<<len<<endl;
                for(int pos = 0;pos < len;)
                {
                    bitmap[start_desk + pos + 3] = fill>>24;
                    bitmap[start_desk + pos + 2] = fill>>16;
                    bitmap[start_desk + pos + 1] = fill>>8;
                    bitmap[start_desk + pos + 0] = fill;
                    pos+=4;
//                    cout<<"rectangle x is "<<x<<" y "<<y<<" i "<<i<<" pos is "<<pos <<" desk start pos is "<<start_desk<<" src start is "<<start_src<<" len is "<<len<<endl;
                }
            }
        }
        printf("image addr is %p\n",bitmap);
    };
    int add_font(int x,int y,int weigth,int height,int font_size,unsigned int font,unsigned int fill,string str,int is_box,int line,unsigned int fill_line)
    {
        this->clear(x,y,weigth,height);
        wchar_t word[512];
        memset(word,0,sizeof(word));
        FUTF82WConvert(str.c_str(),word,512);
        weigth = (weigth%8)?(weigth/8 + 1)*8:weigth;
        uint8_t  *data = (unsigned char *)malloc(weigth*height/8);
        DrawFont(weigth,height,font_size,word,data,weigth*height,En_SourceHanSans_CH,NULL,0,En_CenterAlignment,0);
        if(x > (BK_WIDTH) || (x+weigth) < 0 || y > BK_HEIGHT || y+height < 0)
        {
            cout<<" rectangle is out bk "<< "x = "<<x<<"y"<<y<<endl;
        }
        for(int i = 0;i < height;i++)
        {
            if((i + y) < BK_HEIGHT)
            {
                int start_desk = x * PIX_WIDTH + (i+y)*BK_WIDTH * PIX_WIDTH;
                int start_src = i * ((weigth % 8)?(weigth / 8 + 1):(weigth / 8));
                int len = ((x+weigth) > BK_WIDTH ? BK_WIDTH-x:weigth)*PIX_WIDTH;
                for(int pos = 0;pos < len;)
                {
                    uint8_t p = data[start_src + (pos / 32)];
                    uint8_t p1 = pos / 4 % 8;
                    p = (p >> p1) & 0x01;
                    if(p)
                    {
                        bitmap[start_desk + pos + 3] = font>>24;
                        bitmap[start_desk + pos + 2] = font>>16;
                        bitmap[start_desk + pos + 1] = font>>8;
                        bitmap[start_desk + pos + 0] = font;
                    }
                    else if(is_box)
                    {
                        bitmap[start_desk + pos + 3] = fill>>24;
                        bitmap[start_desk + pos + 2] = fill>>16;
                        bitmap[start_desk + pos + 1] = fill>>8;
                        bitmap[start_desk + pos + 0] = fill;
                    }
                    
                    if(line)
                    {
                        //if((i < line) || ((height - i) < line) || (pos < (line * 4)) || ((len - pos) > (line * 4)))
                        if((i < line) || ((height - i) <= (line)) || (pos < (line * 4)) || ((len - pos) <= (line * 4)))
                        {
                            bitmap[start_desk + pos + 3] = fill_line>>24;
                            bitmap[start_desk + pos + 2] = fill_line>>16;
                            bitmap[start_desk + pos + 1] = fill_line>>8;
                            bitmap[start_desk + pos + 0] = fill_line;
                        }
                    }

                    pos+=4;
                }
            }
        }
        free(data);
    }
    int clear(int x,int y,int weigth,int height)
    {
        if(x > (BK_WIDTH) || (x+weigth) < 0 || y > BK_HEIGHT || y+height < 0)
        {
            cout<<" rectangle is out bk "<< "x = "<<x<<"y"<<y<<endl;
        }
        for(int i = 0;i < height;i++)
        {
            if((i + y) < BK_HEIGHT)
            {
                int start_desk = x * PIX_WIDTH + (i+y)*BK_WIDTH * PIX_WIDTH;
                int len = ((x+weigth) > BK_WIDTH ? BK_WIDTH-x:weigth)*PIX_WIDTH;
                memcpy(bitmap+start_desk,mapImage["bk"].bit+start_desk,len);
                //cout<<"start is "<<start_desk<<" len is "<<len<<endl;
            }
        }
    }
    int add_pic(string desk,int x,int y)
    {
//        cout<<"desk "<<desk<<" x "<<x<<" y "<<y<<endl;
        if(mapImage.count(desk) == 0)
        {
            cout<< desk <<" is not exist"<<endl;
        }
        if(x > (BK_WIDTH) || (x+mapImage[desk].weigth) < 0 || y > BK_HEIGHT || y+mapImage[desk].height < 0)
        {
            cout<< desk <<" out bk "<< "x = "<<x<<"y"<<y<<endl;
        }
        //clear(x,y,mapImage[desk].weigth,mapImage[desk].height);
        for(int i = 0;i < mapImage[desk].height;i++)
        {
            if((i + y) < BK_HEIGHT)
            {
                int start_desk = x * PIX_WIDTH + (i+y)*BK_WIDTH * PIX_WIDTH;
                int start_src = i * mapImage[desk].weigth * PIX_WIDTH;
                int len = ((x+mapImage[desk].weigth) > BK_WIDTH ? BK_WIDTH-x:mapImage[desk].weigth)*PIX_WIDTH;
 //               cout<<"x is "<<x<<" y "<<y<<" i "<<i<<" desk start pos is "<<start_desk<<" src start is "<<start_src<<" len is "<<len<<endl;              
                for(int pos = 0;pos < len ;)
                {
                    //*(bitmap+start_desk + pos ) += *(mapImage[desk].bit+start_src + pos);
                    ARGB_DATA_TYPE *a = (ARGB_DATA_TYPE *)(bitmap+start_desk + pos);
                    ARGB_DATA_TYPE *b = (ARGB_DATA_TYPE *)(mapImage[desk].bit+start_src + pos);
                    if(a->mA != 0)
                    {
                        a->mR = (((uint32_t)(a->mA) * a->mR) + (uint32_t)(255 - a->mA) * b->mR) / 255;
                        a->mG = (((uint32_t)(a->mA) * a->mG) + (uint32_t)(255 - a->mA) * b->mG) / 255;
                        a->mB = (((uint32_t)(a->mA) * a->mB) + (uint32_t)(255 - a->mA) * b->mB) / 255;
                        //a->mA = b->mA;
                    }
                    pos += 4;
                }
                
                
                //memcpy(bitmap+start_desk,mapImage[desk].bit+start_src,len);
            }
        }
    };
    
    int SavebitTodata(string desk,string file_name)
    {
        ofstream ofs;
        ofs.open(desk.c_str(),ios::out|ios::binary);
        ofs.write((const char *)bitmap,1228800);
        ofs.close();

        write_map_to_image("bg.png",file_name,1228800,(uint8_t *)bitmap);
    }
};

int MaterialManage::bitmap_dell = 0;
map<string,s_image> MaterialManage::mapImage = {};
s_image gImage[] =
{
    {"bk",          640,480,    0,"../osd/bg.png",NULL},
    {"ligth",       32,32,      0,"../osd/icon_light.png",NULL},
    {"preset",      32,32,      0,"../osd/icon_preset.png",NULL},
    {"quality",     32,32,      0,"../osd/icon_quality.png",NULL},
    {"settings",    32,32,      0,"../osd/icon_settings.png",NULL},
};
int MaterialManage::GetBKmap()
{
    if(MaterialManage::bitmap_dell)
        return 0;
    for(int i = 0;i < sizeof(gImage)/sizeof(s_image);i++)
    {    
        gImage[i].bit_length = gImage[i].height*gImage[i].weigth*4;
        gImage[i].bit = (uint8_t *)malloc(gImage[i].bit_length);
        read_image_to_map(gImage[i].path,gImage[i].bit_length,gImage[i].bit);
        MaterialManage::mapImage.insert(pair<string,s_image>(gImage[i].image,gImage[i]));
    }
//    cout<<"map size is "<<MaterialManage::mapImage.size()<<endl;
    MaterialManage::bitmap_dell = 1;
    return 0;
}
MaterialManage image;
int main(int argc,char **argv)
{
    int oc;
    int p1 = 0,p2 = 0;
    while((oc = getopt(argc,argv,"f:s:")) != -1)
    {
        switch(oc)
        {
            case 'f':
            p1 = atoi(optarg);
            cout<<"f is "<<optarg<<endl;
            break;
            case 's':
            p2 = atoi(optarg);
            cout<<"s is "<<optarg<<endl;
            break;
        }
    }
    image.GetBKmap();
    image.DisplayBK();
#if 0
    cout<<"add pic is star "<<now_str()<<endl;
    image.add_font(160,30,160,50,22,0xffffffff,0xe6216cb7,"亮度",0);
    image.add_pic(MaterialManage::mapImage["ligth"].image,160,30);
    image.add_font(160,80,160,50,22,0xffffffff,0xe6216cb7,"场景",0);
    image.add_pic(MaterialManage::mapImage["preset"].image,160,90);
    image.add_font(160,130,160,50,22,0xffffffff,0xe6216cb7,"画质",0);
    image.add_pic(MaterialManage::mapImage["quality"].image,160,150);
    image.add_font(160,180,160,50,22,0xffffffff,0xe6216cb7,"高级",0);
    image.add_pic(MaterialManage::mapImage["settings"].image,160,240);

//    image.draw_rectangle(0,0,10,10,1,0xffffffff);

    image.add_font(0,30,160,50,22,0xffffffff,0xe6216cb7,"屏幕1",1);
    image.add_font(0,80,160,50,22,0xffffffff,0xe6216cb7,"屏幕2",1);
    image.add_font(0,130,160,50,22,0xffffffff,0xe6216cb7,"屏幕3",1);
    image.add_font(0,180,160,50,22,0xffffffff,0xe6216cb7,"屏幕4",0);
    image.add_font(0,230,160,50,22,0xffffffff,0xe6216cb7,"屏幕5",0);
    image.add_font(0,280,160,50,22,0xffffffff,0xe6216cb7,"屏幕6",0);
    image.add_font(0,330,160,50,22,0xffffffff,0xe6216cb7,"屏幕7",0);
    image.add_font(0,380,160,50,22,0xffffffff,0xe6216cb7,"屏幕8",0);
    image.add_font(0,430,160,50,22,0xffffffff,0xe6216cb7,"屏幕9",1);
 #endif   
    string screen_dis[10]
    {
        "屏幕0",
        "屏幕1",
        "屏幕2",
        "屏幕3",
        "屏幕4",
        "屏幕5",
        "屏幕6",
        "屏幕7",
        "屏幕8",
        "屏幕9",
    };
    string ioc[4][2] =
    {
        {"ligth","亮度"},
        {"preset","场景"},
        {"quality","画质"},
        {"settings","高级"},
    };
    image.m_select.screen = 0;
    image.m_select.item = 0;
    image.m_select.other = 0;
    int pos_f = 0;
    int pos = 0;
    for(int i = 0;i < 10;i++)
    {
        pos = 50 * i + 30;
        if(image.m_select.screen == i)
            image.add_font(0,pos,160,50,22,0xffffffff,0xe6216cb7,screen_dis[i].c_str(),1,0,0xbf2090ff);
        else
            image.add_font(0,pos,160,50,22,0xffffffff,0xe6216cb7,screen_dis[i].c_str(),0,0,0xbf2090ff);

        printf("pos is %d\n",pos);
    }
    for(int i = 0;i < 4;i++)
    {
        pos_f = 50 * i + 30;
        printf("pos_f is %d\n",pos_f);
        if(image.m_select.item == i)
            image.add_font(160,pos_f,160,50,22,0xffffffff,0xe6216cb7,ioc[i][1],1,0,0xbf2090ff);
        else
            image.add_font(160,pos_f,160,50,22,0xffffffff,0xe6216cb7,ioc[i][1],0,0,0xbf2090ff);
        image.add_pic(MaterialManage::mapImage[ioc[i][0]].image,180,pos_f+9);
        
    }
    image.SavebitTodata("osd","new.png");
//    int a = 0,b = 0;
    image.m_select.screen = 0;
    image.m_select.item = 0;
    image.m_select.other = 0;
    image.m_select.column = 0;
    char op;
    while(1)
    {
        s_select s_temp = image.m_select;
        cout<<"plead input a:Left  d:Right  w:Upper  s:Lower "<<endl;
        cin>>op;
        switch (op)
        {
        case 'a':
            /* code */
            if(image.m_select.column > 0)
                s_temp.column--;
            break;
        case 'd':
            if(image.m_select.column < 3)
                s_temp.column++;
            /* code */
            break;
        case 'w':
            switch (image.m_select.column)
            {
            case 0:
                if(s_temp.screen>0)
                    s_temp.screen--;
                break;
            case 1:
                if(s_temp.item>0)
                    s_temp.item--;
                break;
            default:
                break;
            }
            /* code */
            break;
        case 's':
            /* code */
            switch (image.m_select.column)
            {
            case 0:
                if(s_temp.screen<9)
                    s_temp.screen++;
                break;
            case 1:
                if(s_temp.item<4)
                    s_temp.item++;
                break;
            }
        }
        //cout<<"a is "<<a<<" b is "<<b<<endl;
        

        if (s_temp.screen != image.m_select.screen && image.m_select.screen < 10)
        {
            pos = 50 * image.m_select.screen + 30;
            image.clear(0,pos,160,50);
            image.add_font(0,pos,160,50,22,0xffffffff,COLOR_BOX,screen_dis[image.m_select.screen].c_str(),0,0,COLOR_LINE);
            image.m_select.screen = s_temp.screen;
            pos = 50 * image.m_select.screen + 30;
            image.clear(0,pos,160,50);
            if(s_temp.column == 0)
            {
                printf("dis box fill and pos is %d\n",pos);
                image.add_font(0,pos,160,50,22,0xffffffff,COLOR_BOX,screen_dis[image.m_select.screen].c_str(),1,0,COLOR_LINE);
            }
            else
            {
                printf("dis box fill\n");
                image.add_font(0,pos,160,50,22,0xffffffff,COLOR_BOX,screen_dis[image.m_select.screen].c_str(),0,2,COLOR_LINE);
            }
        }
        if (s_temp.item != image.m_select.item)
        {
            if(image.m_select.item < 4)
            {
                pos_f = 50 * image.m_select.item + 30;
                printf("pos_f old is %d\n",pos_f);
                image.clear(160,pos_f,160,50);
                image.add_font(160,pos_f,160,50,22,0xffffffff,COLOR_BOX,ioc[image.m_select.item][1],0,0,COLOR_LINE);
                image.add_pic(MaterialManage::mapImage[ioc[image.m_select.item][0]].image,180,pos_f+9);
            }

            image.m_select.item = s_temp.item;

            if(image.m_select.item < 4)
            {
                pos_f = 50 * image.m_select.item + 30;
                printf("pos_f is new %d\n",pos_f);
                image.clear(160,pos_f,160,50);
                image.add_font(160,pos_f,160,50,22,0xffffffff,COLOR_BOX,ioc[image.m_select.item][1],1,0,COLOR_LINE);
                image.add_pic(MaterialManage::mapImage[ioc[image.m_select.item][0]].image,180,pos_f+9);
            }
        }
        image.SavebitTodata("osd","new.png");
        system("display new.png");
    }
    
//    image.draw_rectangle(0,30,160,50,1,0xE6216CB7);     //ARGB
//    image.draw_rectangle(0,0,160,50,1,0xE6216CB7);     //ARGB
//    cout<<"add pic is end  "<<now_str()<<endl;
//    image.DisplayBK();
    image.SavebitTodata("osd","new.png");
/*
    preset p;
    cout<<"time is "<<now_str()<<endl;
    p.opt(p1,p2);
    cout<<"time is "<<now_str()<<endl;

    system("display osd.bmp");
*/
    return 0;
}