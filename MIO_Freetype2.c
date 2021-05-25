/* example1.c                                                      */
/*                                                                 */
/* This small program shows how to print a rotated string with the */
/* FreeType 2 library.                                             */


#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <wchar.h> 
//#include <freetype2/ft2build.h>
#include <ft2build.h>
#include "MIO_Freetype2.h"
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H

//#define WIDTH   640
//#define HEIGHT  480
#define WIDTH   480
#define HEIGHT  100

#define Font_Fix_Dir                  "./font/"
#define FONT_FIX_NUM  10

/* origin is the upper left corner */
//unsigned char image[HEIGHT][WIDTH];


/* Replace this function with something useful. */

void
draw_bitmap( FT_Bitmap*  bitmap,
             FT_Int      x,
             FT_Int      y,
			 unsigned char *image, int32_t width,int32_t height)
{
  FT_Int  i, j, p, q;
  FT_Int  x_max = x + bitmap->width;
  FT_Int  y_max = y + bitmap->rows;
  int w_leng = ceil(width / 8);
  int tmp_bit = 0;


  /* for simplicity, we assume that `bitmap->pixel_mode' */
  /* is `FT_PIXEL_MODE_GRAY' (i.e., not a bitmap font)   */

  for ( i = x, p = 0; i < x_max; i++, p++ )
  {
    for ( j = y, q = 0; j < y_max; j++, q++ )
    {
      if ( i < 0      || j < 0       ||
           i >= width || j >= height )
        continue;
      tmp_bit = i%8;
      //image[j][i] |= bitmap->buffer[q * bitmap->width + p];
      //image[j*WIDTH + i] |= bitmap->buffer[q * bitmap->width + p];
      image[j * w_leng + i/8] |= ((bitmap->buffer[q * bitmap->width + p] ? 1 : 0) << tmp_bit);
	  //printf("%d-",bitmap->buffer[q * bitmap->width + p]);
    }
  }
}


void
show_image(unsigned char *image, int32_t width,int32_t height)
{
  int  i, j;
  int tmp_bit = 0;
  int tmp;
  int w_leng = ceil(width / 8);

  for ( i = 0; i < height; i++ )
  {
    for ( j = 0; j < width; j++ )
    {
        //putchar( image[i][j] == 0 ? ' '
        //                          : image[i][j] < 128 ? '+'
        //                                              : '*' );
        //putchar( image[i*WIDTH + j] == 0 ? ' ':'*');
        tmp_bit = j%8;
        tmp = ((image[i * w_leng + j/8] >> tmp_bit) & 0x01);
//	  printf("%d-",tmp);
        putchar( tmp == 0 ? ' ':'*');
          //                        : image[i][j] < 128 ? '+'
          //                                            : '*' );
    }
    putchar( '\n' );
  }
}

/**
 * @brief  draw font
 * @param   uint32_t width,��ʾ�������
            uint32_t height,��ʾ����߶�
            uint32_t font_height, ����߶�
            wchar_t  *word,     ��ʾ�����ݣ�ע������
            unsigned char *image,���buf
            uint32_t image_len   ���buf�Ĵ�С
            font ��������
            font_name ����·����ֻ��font�ǹ̶�����������ʱ�Ż����ã������ÿռ���
            space �ּ�� ��λΪ�ٷֱȣ�125��ʾ1.25
            alig ���뷽ʽ Ŀǰֻ֧������룬���ж��룬�Ҷ���
            invalid_width ��Ч���� ��ֵ��Ҫ���ڱ�֤��������׼ȷ�ԣ���ʱ��width���������봦���ģ�������Ч�ĵ㣬���������ֽ�
 * @return   n image���ƺ�ռ�õĴ�С
            -1 ָ��Ϊ��
            -2 buf���Ȳ���
 * @note
 */
#define MAX_GLYPHS 1024
int DrawFont(int32_t width,int32_t height,int32_t font_height,wchar_t  *word,
    unsigned char *image,uint32_t image_len,En_Font_t font,char *font_name,int space,En_Alignment alig,int32_t invalid_width)
{
  FT_Library    library;
  FT_Face       face;

  FT_GlyphSlot  slot;
  FT_Matrix     matrix;                 /* transformation matrix */
//  FT_Vector     pen;                    /* untransformed origin  */
  FT_Error      error;

  //char*         filename;
  //char*         text;
//  printf("width:%d,invalid_width:%d\n",width,invalid_width);
  double        angle;
  int           target_height;
  uint32_t           n;//, num_chars;
  if((word == NULL) || (image == NULL))
  {
      printf("word is null\n");
      return -1;
  }
  
  uint32_t buf_len = ceil(width / 8.0) * height;
  if(buf_len > image_len)
  {
      printf("buf_len is %d,image_len is %d\n",buf_len,image_len);
      return -2;
  }
  
  memset(image,0,buf_len);
  
  //wchar_t  *chinese_str=L"H9 VideoWall";
  //wchar_t  *chinese_str=(wchar_t *)argv[2];
  //num_chars     = strlen( text );
  angle         = ( 25.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
  target_height = height;

  error = FT_Init_FreeType(&library );              /* initialize library */
//  printf("150 error is %d\n",error);
  /* error handling omitted */
  //���Ϊ�ڲ��̶����壬��ͨ�������ķ�ʽ����������Ļ�ֱ��ʹ���ⲿ·��
  if(font < FONT_FIX_NUM)
  {
    error = FT_New_Face(library,"NotoSansCJKsc-Regular.otf",0,&face );/* create face object */ 
  }
  else
  {
    if(font_name==NULL)
    {
      return -4;
    }
      
    error = FT_New_Face( library,"NotoSansHans-Regular.otf",0,&face );/* create face object */ 
  }

  
  if(error != 0)
  {
        return -3;
  }
  /* error handling omitted */

  /* use 50pt at 100dpi */
  //error = FT_Set_Char_Size( face, 50 * 64, 0,
  //                          100, 0 );                /* set character size */
  error = FT_Set_Pixel_Sizes( face, font_height, 0);                /* set character size */
  /* error handling omitted */

  /* cmap selection omitted;                                        */
  /* for simplicity we assume that the font contains a Unicode cmap */

  slot = face->glyph;

  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xx = matrix.xx;

  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */

//  pen.x = 0 * 64;
//  pen.y = ((target_height - font_height) / 2  + (font_height / 8)) * 64;

  FT_UInt num_glyphs=0;
  FT_UInt glyph_index;
  int pen_x=0,pen_y=((target_height - font_height) / 2  + (font_height / 8)) * 64;;

//  FT_Glyph glyph = glyphs;
  uint32_t len_max = wcslen(word);
  FT_Glyph glyphs[len_max];
  FT_Vector vector[len_max];
  for ( n = 0; n < len_max; n++ )
  {
    glyph_index = FT_Get_Char_Index(face,word[n]);
//    if(glyph_index==0)
//      continue;
    vector[n].x=pen_x;
    vector[n].y=pen_y;
    FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT);
    FT_Get_Glyph(face->glyph,&glyphs[n]);
    //�����ּ��
    int pos = 0;
    if(space>100)
    {
      //���ձ��������ּ��
      pos = slot->advance.x * (space - 100) / 100;
      //���հ����� 1��=1/72Ӣ��  һ����=1/72Ӣ�磨2.54cm�� ����һ����=1/64�� ���1������64,
      //pos = space*64/100;
    }
    pen_x += slot->advance.x+pos;
    pen_y += slot->advance.y;
    num_glyphs++;
  }
  FT_Int start_x=0;

  FT_Int box_width = (width-invalid_width)*64;

  switch(alig)
  {
    case En_CenterAlignment://����
    start_x = (box_width - pen_x)/2;break;
    case En_RightAlignment://�Ҷ���
    start_x = box_width - pen_x;break;
    default:break;
  }
//  pen.x = 0;
//  pen.y = ((target_height - font_height) / 2  + (font_height / 8)) * 64;
  for(n = 0;n < num_glyphs;n++)
  {
    FT_Glyph glyph_temp;
          FT_Vector pen;
          glyph_temp = glyphs[n];
          pen.x = vector[n].x + start_x;
          pen.y = vector[n].y;
    FT_Glyph_To_Bitmap(&glyph_temp,FT_RENDER_MODE_NORMAL,&pen,0);
    FT_BitmapGlyph bit = (FT_BitmapGlyph)glyph_temp;
    draw_bitmap( &bit->bitmap,bit->left,target_height - bit->top,image, width, height);
    FT_Done_Glyph(glyph_temp);
  }
#if 0
  for ( n = 0; n < wcslen(word); n++ )
  {
    /* set transformation */
    //FT_Set_Transform( face, &matrix, &pen );
    FT_Set_Transform( face, 0, &pen );

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, word[n], FT_LOAD_RENDER | FT_RENDER_MODE_MONO);
  //  if ( error )
  //    continue;                 /* ignore errors */

    /* now, draw to our target surface (convert position) */
    draw_bitmap( &slot->bitmap,
                 slot->bitmap_left,
                 target_height - slot->bitmap_top ,image, width, height);

    //�����ּ��
    FT_Pos pos = 0;
    if(space>100)
    {
      pos = slot->advance.x * (space - 100) / 100;
    }
    /* increment pen position */
    pen.x += slot->advance.x + pos;
    pen.y += slot->advance.y;
  }
#endif
//   for(int i=0;i<buf_len;i++)
//  	printf("%d-",image[i]);

//  show_image(image,width,height);

  FT_Done_Face    ( face );
  FT_Done_FreeType( library );

//  show_image(image,width,height);

  return 0;
}
//�����ַ��Ƿ������������������
int JudgeStr(int32_t width,int32_t height,int32_t font_height,wchar_t  *word,
    En_Font_t font,char *font_name,int space,int32_t invalid_width)
{
  FT_Library    library;
  FT_Face       face;

  FT_GlyphSlot  slot;
  FT_Error      error;

  int           target_height;
  uint32_t           n;//, num_chars;
  if(word == NULL)
  {
      printf("word is null\n");
      return -1;
  }
  
  target_height = height;

  error = FT_Init_FreeType( &library );              /* initialize library */
  /* error handling omitted */
  //���Ϊ�ڲ��̶����壬��ͨ�������ķ�ʽ����������Ļ�ֱ��ʹ���ⲿ·��
  if(font < FONT_FIX_NUM)
  {
    //char name[512]={0};
    //sprintf(name,"%s%s",Font_Fix_Dir,gFontName[font]);
    error = FT_New_Face( library,"NotoSansCJKsc-Regular.otf",0,&face );/* create face object */ 
  }
  else
  {
    if(font_name==NULL)
    {
      printf("font name is null\n");
      return -4;
    }
      
    error = FT_New_Face( library,font_name,0,&face );/* create face object */ 
  }

  if(error != 0)
  {
    printf("FT_New_Face is error\n");
        return -3;
  }

  error = FT_Set_Pixel_Sizes( face, font_height, 0);                /* set character size */

  slot = face->glyph;


  FT_UInt glyph_index;
  int pen_x=0,pen_y=((target_height - font_height) / 2  + (font_height / 8)) * 64;;

  uint32_t len_max = wcslen(word);
  FT_Glyph glyphs[len_max];
  for ( n = 0; n < len_max; n++ )
  {
    glyph_index = FT_Get_Char_Index(face,word[n]);
    FT_Load_Glyph(face,glyph_index,FT_LOAD_DEFAULT);
    FT_Get_Glyph(face->glyph,&glyphs[n]);
    //�����ּ��
    int pos = 0;
    if(space>100)
    {
      //���ձ��������ּ��
      pos = slot->advance.x * (space - 100) / 100;
      //���հ����� 1��=1/72Ӣ��  һ����=1/72Ӣ�磨2.54cm�� ����һ����=1/64�� ���1������64,
      //pos = space*64/100;
    }
    pen_x += slot->advance.x+pos;
    pen_y += slot->advance.y;
  }
//  pen_x -= pos;	//��ȥ���һ�μ�����ּ��
  FT_Int box_width = (width-invalid_width)*64;


  FT_Done_Face    ( face );
  FT_Done_FreeType( library );
  if(pen_x <= box_width)
	return 0;
  else
	return -1;
}

#ifdef MIO_FREETYPE2_TEST

int
main( int     argc,
      char**  argv )
{
  FT_Library    library;
  FT_Face       face;

  FT_GlyphSlot  slot;
  FT_Matrix     matrix;                 /* transformation matrix */
  FT_Vector     pen;                    /* untransformed origin  */
  FT_Error      error;

  char*         filename;
  char*         text;

  double        angle;
  int           target_height;
  int           n, num_chars;
  unsigned char *image = NULL;
  int buf_len = ceil(WIDTH / 8) * HEIGHT;
  image = (unsigned char *)malloc(buf_len);
  memset(image,0,buf_len);
  if ( argc != 3 )
  {
    fprintf ( stderr, "usage: %s font sample-text\n", argv[0] );
    exit( 1 );
  }

  filename      = argv[1];                           /* first argument     */
  text          = argv[2];                           /* second argument    */
  wchar_t  *chinese_str=L"H9 VideoWall";
  //wchar_t  *chinese_str=(wchar_t *)argv[2];
  num_chars     = strlen( text );
  angle         = ( 25.0 / 360 ) * 3.14159 * 2;      /* use 25 degrees     */
  target_height = HEIGHT;

  error = FT_Init_FreeType( &library );              /* initialize library */
  /* error handling omitted */

  error = FT_New_Face( library, filename, 0, &face );/* create face object */
  /* error handling omitted */

  /* use 50pt at 100dpi */
  //error = FT_Set_Char_Size( face, 50 * 64, 0,
  //                          100, 0 );                /* set character size */
  error = FT_Set_Pixel_Sizes( face, 80, 0);                /* set character size */
  /* error handling omitted */

  /* cmap selection omitted;                                        */
  /* for simplicity we assume that the font contains a Unicode cmap */

  slot = face->glyph;

  /* set up matrix */
  matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
  matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
  matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
  matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

  /* the pen position in 26.6 cartesian space coordinates; */
  /* start at (300,200) relative to the upper left corner  */
  //pen.x = 300 * 64;
  //pen.y = ( target_height - 200 ) * 64;
  pen.x = 0 * 64;
  pen.y = ( target_height - 90) * 64;

  for ( n = 0; n < wcslen(chinese_str); n++ )
  {
    /* set transformation */
    //FT_Set_Transform( face, &matrix, &pen );
    FT_Set_Transform( face, 0, &pen );

    /* load glyph image into the slot (erase previous one) */
    error = FT_Load_Char( face, chinese_str[n], FT_LOAD_RENDER | FT_RENDER_MODE_MONO);
    if ( error )
      continue;                 /* ignore errors */

    /* now, draw to our target surface (convert position) */
    draw_bitmap( &slot->bitmap,
                 slot->bitmap_left,
                 target_height - slot->bitmap_top ,image);

    /* increment pen position */
    pen.x += slot->advance.x;
    pen.y += slot->advance.y;
  }
//   for(int i=0;i<buf_len;i++)
 //  	printf("%d-",image[i]);

  show_image(image);

  FT_Done_Face    ( face );
  FT_Done_FreeType( library );

  return 0;
}
#endif
/* EOF */
